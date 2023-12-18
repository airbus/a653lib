/*
 * Copyright (c) 2022-2023 Airbus Defence and Space
 *
 * This file is part of a653_4_linux.
 *
 * a653_4_linux is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with a653_4_linux; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file      apex_i_queuing.c
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     a653 queuing port
 * @details    
 */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
//--------------------

#define APEX_QUEUING_INTERN

#include "apexInit.h"
#include "apexType.h"
#include "apexError.h"
#include "apexQueuing.h"
#include "apexProcess.h"

#include "apex_i_shm_if.h"


/*internal types and data */

/* typedef struct { */
/*   int   size; */
/*   char  *data; */
/* } fifo_msg_header_t; */

/* this data is shared !!!! */
/* typedef struct { */
/*   char                    QUEUING_PORT_NAME[34]; */
/*   unsigned short          PortId; */
/*   int                     init_done; */
/*   int                     max_elem; */
/*   int                     max_size; */
/*   int                     cur_elem; */
/*   int                     tx_next; */
/*   int                     rx_next; */
/*   sem_t                   sem_lock; */
/*   int                     magic_number; */
/*   fifo_msg_header_t       msg[]; */
/* } t_queuing_port_shm_data; */

extern apex_shm_info_t *shm_ptr;
extern int own_partition_idx;

/* local data */
typedef struct {
  int                          max_port_id;
  int                          max_port_num;
  int                          max_port_data_size;
  t_queuing_port_shm_data    **Ports;
  QUEUING_PORT_ID_TYPE        *PortsHash;
} queuing_port_data_t;


/* be very carefull!!! pointer to shared memory can be different in each process!!! */
static apex_queuing_port_config_t *apex_qp_config = NULL;
static queuing_port_data_t *qp_ptr = NULL;



/* internal buffer handling */


static int createFifo (t_queuing_port_shm_data *fifo_ptr, 
                       unsigned int max_elem, 
                       unsigned int max_elem_size){
 
  int           idx          = 0;

  fifo_ptr->max_elem    = max_elem;
  fifo_ptr->max_size    = max_elem_size;
  fifo_ptr->cur_elem    = 0;
  fifo_ptr->tx_next     = 0;
  fifo_ptr->rx_next     = 0;
  
  sem_init(&(fifo_ptr->sem_lock),0,1);

  fifo_ptr->magic_number = 0xAA55AA55;
  //  fifo_ptr->msg = &fifo_ptr->msg + sizeof(fifo_msg_header_t) * max_elem;

  /* buid dynamic array */
  for (idx = 0; idx < max_elem; idx++){
    int d_idx = 0;

    fifo_ptr->msg[idx].size = 0;
    
    for (d_idx = 0; d_idx < max_elem_size; d_idx++){
      fifo_ptr->msg[idx].data[d_idx] = d_idx;
    }
    
  }  
  return 0;
}

static int putFifo (t_queuing_port_shm_data *fifo_ptr, void *src_ptr, int size){

  int ret_val = 0; 
  
  sem_wait(&(fifo_ptr->sem_lock)); 

  if (fifo_ptr->cur_elem == fifo_ptr->max_elem){
    // Buffer overflow
    ret_val = 1; 
  } else if (fifo_ptr->max_size < size) {
    // size to big 
    ret_val = 2; 
  } else {
    /* set current size */
    fifo_ptr->msg[fifo_ptr->tx_next].size = size;
    /* copy data */
    memcpy(fifo_ptr->msg[fifo_ptr->tx_next].data,   /* dest */ 
           src_ptr,                                 /* src  */
           fifo_ptr->msg[fifo_ptr->tx_next].size);  /* size */
    /* update index */
    if (fifo_ptr->tx_next == (fifo_ptr->max_elem - 1)) {
      fifo_ptr->tx_next = 0;
    } else {
      fifo_ptr->tx_next += 1;
    }
    /* update counter */
    fifo_ptr->cur_elem++;   
  }

  sem_post(&(fifo_ptr->sem_lock)); 

  
  printDebug(3,"%s : return %d\n",__func__,ret_val);  
  return(ret_val);
}

static int getFifo (t_queuing_port_shm_data *fifo_ptr, void *dest_ptr, int *size){

  int ret_val = 0; 

  sem_wait(&(fifo_ptr->sem_lock)); 

  if (fifo_ptr->cur_elem == 0){
    // Buffer underrun .. no data
    *size = 0;
    ret_val = 1;    
  } else {
    /* copy data */
    memcpy(dest_ptr, 
           fifo_ptr->msg[fifo_ptr->rx_next].data,          
           fifo_ptr->msg[fifo_ptr->rx_next].size);

    /* get size */
    *size = fifo_ptr->msg[fifo_ptr->rx_next].size;
    fifo_ptr->msg[fifo_ptr->rx_next].size = 0;

    /* update index */
    if (fifo_ptr->rx_next == (fifo_ptr->max_elem - 1)) {
      fifo_ptr->rx_next = 0;
    } else {
      fifo_ptr->rx_next += 1;
    }
    /* update counter */
    fifo_ptr->cur_elem--;
  }

  sem_post(&(fifo_ptr->sem_lock)); 

  return(ret_val);
}

/* external interface */



/* todo init for QUEUING_PORT data !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/


int apex_init_queuing_ports(int max_port_num, apex_queuing_port_config_t *config){
  int ret_val = 0;
  int idx = 0;
  int highest_queuing_port_id = 0;

  printDebug(3,"%s number of ports %d\n",__func__,max_port_num);
  
  for(idx=0; idx <= QUEUING_PORT_ID_MAX; idx++){

    if (config[idx].PortId == 0){
      break;
    }
    if (highest_queuing_port_id < config[idx].PortId)
      highest_queuing_port_id = config[idx].PortId;   
  }
  
  if(highest_queuing_port_id > QUEUING_PORT_ID_MAX){
    printDebug(3," queuing port id range error !!!\n\n");
    exit(1);
  }
  
  apex_qp_config = (apex_queuing_port_config_t *) malloc(sizeof(apex_queuing_port_config_t) * max_port_num);
  memcpy(apex_qp_config,config,sizeof(apex_queuing_port_config_t) * max_port_num);

  qp_ptr = (queuing_port_data_t*) malloc(sizeof(queuing_port_data_t) * max_port_num);
  if (qp_ptr == NULL){
    printDebug(3,"can not get shared memory for queuing ports!!!\n\n");
    exit(1);
  }
  memset(qp_ptr,0,sizeof(queuing_port_data_t) * max_port_num);

  qp_ptr->PortsHash = (QUEUING_PORT_ID_TYPE*) malloc(sizeof(QUEUING_PORT_ID_TYPE) * highest_queuing_port_id);
  if (qp_ptr->PortsHash == NULL){
    printDebug(3,"can not get shared memory for queuing ports!!!\n\n");
    exit(1);
  }
  memset(qp_ptr->PortsHash,0,sizeof(QUEUING_PORT_ID_TYPE) * max_port_num);

  qp_ptr->Ports = (t_queuing_port_shm_data **) malloc(sizeof(t_queuing_port_shm_data *) * max_port_num);
    if (qp_ptr->Ports == NULL){
    printDebug(3,"can not get shared memory fpr queuings ports!!!\n\n");
    exit(1);
  }
  memset(qp_ptr->Ports,0,sizeof(t_queuing_port_shm_data *) * max_port_num);

  qp_ptr->max_port_id        = highest_queuing_port_id;
  qp_ptr->max_port_num       = max_port_num;
  qp_ptr->max_port_data_size = MAX_Q_PORT_SIZE;
  
  for(idx=0; idx < max_port_num; idx++){
    qp_ptr->Ports[idx] = &shm_ptr->q_port_data[idx];
  }
  
  return ret_val;
}


/* external interface */

void create_queuing_port_ip (QUEUING_PORT_NAME_TYPE  QUEUING_PORT_NAME,
			     MESSAGE_SIZE_TYPE       MAX_MESSAGE_SIZE, 
			     MESSAGE_RANGE_TYPE      MAX_NB_MESSAGE,
			     PORT_DIRECTION_TYPE     PORT_DIRECTION,
			     QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE,
			     QUEUING_PORT_ID_TYPE   *QUEUING_PORT_ID,
			     RETURN_CODE_TYPE       *RETURN_CODE){
    
  int idx   = 0;
  int found = 0;
    
  *RETURN_CODE = INVALID_CONFIG;

  while ((!found) && (apex_qp_config[idx].PortId != 0)) {

    if ((strncmp(apex_qp_config[idx].name_str,QUEUING_PORT_NAME,25)) == 0) {

      found = 1;

      /* todo handle direction !!!!!!!!!!!!!!!!!!!!*/
      if (own_partition_idx == apex_qp_config[idx].DstPartitionIdx){
	
	/* save link to instance*/
	qp_ptr->PortsHash[apex_qp_config[idx].PortId] = idx;
	
	/* set return values */
        *QUEUING_PORT_ID = apex_qp_config[idx].PortId;
        *RETURN_CODE     = NO_ERROR;

	break;
	
      } else {
	
        /* int space_needed = 0; */
        
        /* if (MAX_MESSAGE_SIZE & 0x00000003) { */
        /*   MAX_MESSAGE_SIZE = (MAX_MESSAGE_SIZE & 0xfffffffC) +4; */
        /* } */

	
        /* space_needed = sizeof(t_queuing_port_shm_data) + sizeFifo (MAX_NB_MESSAGE, */
        /*                                                               MAX_MESSAGE_SIZE); */
        
	if (MAX_MESSAGE_SIZE > qp_ptr->max_port_data_size) {
	  *RETURN_CODE = NOT_AVAILABLE;
	} else {
	  /* update internel struct */
	  qp_ptr->Ports[idx]->PortId = apex_qp_config[idx].PortId;
	  strcpy(qp_ptr->Ports[idx]->QUEUING_PORT_NAME,
		 apex_qp_config[idx].name_str);

	  createFifo (qp_ptr->Ports[idx],
		      MAX_NB_MESSAGE,
		      MAX_MESSAGE_SIZE);
	  
	  qp_ptr->Ports[idx]->PortId = apex_qp_config[idx].PortId;
	  qp_ptr->Ports[idx]->init_done = 1;  
	  qp_ptr->PortsHash[apex_qp_config[idx].PortId] = idx;
	  
	  /* set return values */
	  *QUEUING_PORT_ID = apex_qp_config[idx].PortId;
	  *RETURN_CODE     = NO_ERROR;

	  break;
	}
      }
    }
    idx++;
  }

  if (*RETURN_CODE != NO_ERROR){
    printDebug(3,"CREATE_QUEUING_PORT error: %d\n",*RETURN_CODE);
  }
}

void send_queuing_message_ip (QUEUING_PORT_ID_TYPE   QUEUING_PORT_ID,
			      MESSAGE_ADDR_TYPE      MESSAGE_ADDR,
			      MESSAGE_SIZE_TYPE      LENGTH,
			      SYSTEM_TIME_TYPE       TIME_OUT,
			      RETURN_CODE_TYPE     * RETURN_CODE){

  int idx = qp_ptr->PortsHash[QUEUING_PORT_ID];

  *RETURN_CODE = NO_ACTION;

  if (idx < qp_ptr->max_port_num){

    if (qp_ptr->Ports[idx]->init_done == 0) {
      *RETURN_CODE = NOT_AVAILABLE;
    } else {
      /*       if ((LENGTH > qp_ptr->Ports[idx]->Queuing_Port_Status.MAX_MESSAGE_SIZE) ||  */
      /*           (qp_ptr->Ports[idx]->Queuing_Port_Status.PORT_DIRECTION != SOURCE)) { */
      if (LENGTH > qp_ptr->Ports[idx]->max_size){
        *RETURN_CODE = INVALID_CONFIG;
      } else {

        if( 0 == putFifo (qp_ptr->Ports[idx],
                          MESSAGE_ADDR, /* src  */
                          LENGTH)){
          *RETURN_CODE = NO_ERROR;          
        } else {
          *RETURN_CODE = NO_ACTION;
        }
      }
    }
  }  
}

void receive_queuing_message_ip (QUEUING_PORT_ID_TYPE QUEUING_PORT_ID,
				 SYSTEM_TIME_TYPE     TIME_OUT,
				 MESSAGE_ADDR_TYPE    MESSAGE_ADDR,
				 MESSAGE_SIZE_TYPE  * LENGTH,
				 RETURN_CODE_TYPE   * RETURN_CODE){

  int idx = qp_ptr->PortsHash[QUEUING_PORT_ID];

  *RETURN_CODE = NO_ACTION;

  if (idx < qp_ptr->max_port_num){

    if (qp_ptr->Ports[idx]->init_done == 0) {
      *RETURN_CODE = NOT_AVAILABLE;
    } else {
      /*       if (qp_ptr->Ports[idx]->Queuing_Port_Status.PORT_DIRECTION != DESTINATION) { */
      /*         *RETURN_CODE = INVALID_CONFIG; */
      /*       } else { */
      
      if (0 == getFifo (qp_ptr->Ports[idx], 
                        MESSAGE_ADDR, /* dest */ 
                        (int *)LENGTH)){
        *RETURN_CODE = NO_ERROR;
      } else {
        *LENGTH      = 0;
        *RETURN_CODE = NO_ACTION;
      }
      
      /*   } */
    }
  }
}
/****************************************************************************************************************/


void CREATE_QUEUING_PORT (QUEUING_PORT_NAME_TYPE  QUEUING_PORT_NAME,
			  MESSAGE_SIZE_TYPE       MAX_MESSAGE_SIZE, 
			  MESSAGE_RANGE_TYPE      MAX_NB_MESSAGE,
			  PORT_DIRECTION_TYPE     PORT_DIRECTION,
			  QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE,
			  QUEUING_PORT_ID_TYPE  * QUEUING_PORT_ID,
			  RETURN_CODE_TYPE      * RETURN_CODE){

  QUEUING_PORT_ID_TYPE l_port_id;
  RETURN_CODE_TYPE  l_return_code;

  GET_QUEUING_PORT_ID (QUEUING_PORT_NAME,
                       &l_port_id,
                       &l_return_code);

  if (l_return_code != INVALID_CONFIG){
  
    /* if (l_port_id < 2000){ */
    
      create_queuing_port_ip (QUEUING_PORT_NAME,
			      MAX_MESSAGE_SIZE, 
			      MAX_NB_MESSAGE,
			      PORT_DIRECTION,
			      QUEUING_DISCIPLINE,
			      QUEUING_PORT_ID,
			      RETURN_CODE);
    
    /* } else if (l_port_id < 3000) { */
    
    /*   create_queuing_port_pp (QUEUING_PORT_NAME, */
    /* 			      MAX_MESSAGE_SIZE,  */
    /* 			      MAX_NB_MESSAGE, */
    /* 			      PORT_DIRECTION, */
    /* 			      QUEUING_DISCIPLINE, */
    /* 			      QUEUING_PORT_ID, */
    /* 			      RETURN_CODE); */
    /*   *QUEUING_PORT_ID = l_port_id; */
      
    /* } else { */
    /*   printDebug(3,"error: wrong port range\n\n"); */
    /* } */
  
  } else {
    printDebug(3,"%s error: invalid prot config\n\n",__func__);
  }
  
}

void SEND_QUEUING_MESSAGE (QUEUING_PORT_ID_TYPE   QUEUING_PORT_ID,
                           MESSAGE_ADDR_TYPE      MESSAGE_ADDR,
                           MESSAGE_SIZE_TYPE      LENGTH,
                           SYSTEM_TIME_TYPE       TIME_OUT,
                           RETURN_CODE_TYPE     * RETURN_CODE){
  
  /* if (QUEUING_PORT_ID < 2000){ */
    
    send_queuing_message_ip (QUEUING_PORT_ID,
			     MESSAGE_ADDR,
			     LENGTH,
			     TIME_OUT,
			     RETURN_CODE);
    
  /* } else if (QUEUING_PORT_ID < 3000) { */

  /*   send_queuing_message_pp (QUEUING_PORT_ID, */
  /* 			     MESSAGE_ADDR, */
  /* 			     LENGTH, */
  /* 			     TIME_OUT, */
  /* 			     RETURN_CODE); */
      
  /* } else { */
  /*   printDebug(3,"error: wrong port range\n\n"); */
  /* }   */
}

void RECEIVE_QUEUING_MESSAGE (QUEUING_PORT_ID_TYPE QUEUING_PORT_ID,
                              SYSTEM_TIME_TYPE     TIME_OUT,
                              MESSAGE_ADDR_TYPE    MESSAGE_ADDR,
                              MESSAGE_SIZE_TYPE  * LENGTH,
                              RETURN_CODE_TYPE   * RETURN_CODE){
  
  /* if (QUEUING_PORT_ID < 2000){ */
    
    receive_queuing_message_ip(QUEUING_PORT_ID,
                               TIME_OUT,
                               MESSAGE_ADDR,
                               LENGTH,
 			       RETURN_CODE);
			       
  /* } else if (QUEUING_PORT_ID < 3000) { */
      
  /*   receive_queuing_message_pp(QUEUING_PORT_ID, */
  /*                              TIME_OUT, */
  /*                              MESSAGE_ADDR, */
  /*                              LENGTH, */
  /* 			       RETURN_CODE); */
  /* } else { */
  /*   printDebug(3,"error: wrong port range\n\n"); */
  /* }    */ 
}



void GET_QUEUING_PORT_ID (QUEUING_PORT_NAME_TYPE   QUEUING_PORT_NAME,
                          QUEUING_PORT_ID_TYPE   * QUEUING_PORT_ID,
                          RETURN_CODE_TYPE       * RETURN_CODE){
  int idx   = 0;
  int found = 0;
  
  *QUEUING_PORT_ID = 0;    
  *RETURN_CODE     = INVALID_CONFIG;
  
  while ((!found) && (apex_qp_config[idx].PortId != 0)) {

    if ((strncmp(apex_qp_config[idx].name_str,QUEUING_PORT_NAME,25)) == 0) {

      found = 1;
	
      /* set return values */
      *QUEUING_PORT_ID = apex_qp_config[idx].PortId;
      *RETURN_CODE     = NO_ERROR;

      break;
    }	
    idx++;
  }
}

void GET_QUEUING_PORT_STATUS (QUEUING_PORT_ID_TYPE       QUEUING_PORT_ID,
                              QUEUING_PORT_STATUS_TYPE * QUEUING_PORT_STATUS,
                              RETURN_CODE_TYPE         * RETURN_CODE){
  *RETURN_CODE = NO_ACTION;
}


void CLEAR_QUEUING_PORT (
                         /*in */ QUEUING_PORT_ID_TYPE QUEUING_PORT_ID,
                         /*out*/ RETURN_CODE_TYPE *RETURN_CODE ){
  *RETURN_CODE = NO_ACTION;
}

