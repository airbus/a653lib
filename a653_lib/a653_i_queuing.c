/*
 * Copyright (c) 2022-2023 Airbus Defence and Space
 *
 * This file is part of liba653.
 *
 * liba653 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * liba653 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with liba653; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file      a653_i_queuing.c
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

#define A653_QUEUING_INTERN

#include "a653Init.h"
#include "a653Type.h"
#include "a653Error.h"
#include "a653Queuing.h"
#include "a653Process.h"

#include "a653_i_shm_if.h"

#define QP_START_ID 1

extern a653_shm_info_t *shm_ptr;
extern int own_partition_idx;
extern a653_channel_config_t channel_config[];

int qp_id_next = QP_START_ID;

/* local data */

/* typedef struct { */
/*   int   size; */
/*   char  data[MAX_Q_PORT_SIZE]; */
/* } fifo_msg_header_t; */


/* typedef struct { */
/*   int                     init_done; */
/*   int                     max_elem; */
/*   int                     max_size; */
/*   int                     cur_elem; */
/*   int                     tx_next; */
/*   int                     rx_next; */
/*   sem_t                   sem_lock; */
/*   int                     magic_number; */
/*   fifo_msg_header_t       msg[MAX_Q_PORT_ENTRIES]; */
/* } t_queuing_port_shm_data; */


typedef struct {
  unsigned short              PortId;
  unsigned short              ChannelIdx;
  unsigned short              Dir;
  char                        QUEUING_PORT_NAME[34];
  t_queuing_port_shm_data    *Port;
} queuing_port_data_t;

static QUEUING_PORT_ID_TYPE *PortsHash = NULL;
static queuing_port_data_t qp_data[MAX_Q_PORT];

/* internal buffer handling */

static int createFifo (t_queuing_port_shm_data *fifo_ptr, 
		   unsigned int max_elem, 
		   unsigned int max_elem_size){
 
  int idx     = 0;
  int ret_val = 0; 

  if ((max_elem <= MAX_Q_PORT_ENTRIES) &&
      (max_elem_size <= MAX_Q_PORT_SIZE)){
  
    fifo_ptr->max_elem    = max_elem;
    fifo_ptr->max_size    = max_elem_size;
    fifo_ptr->cur_elem    = 0;
    fifo_ptr->tx_next     = 0;
    fifo_ptr->rx_next     = 0;
  
    sem_init(&(fifo_ptr->sem_lock),0,1);

    fifo_ptr->magic_number = 0xAA55AA55;

    /* buid dynamic array */
    for (idx = 0; idx < max_elem; idx++){
      int d_idx = 0;
      
      fifo_ptr->msg[idx].size = 0;
    
      for (d_idx = 0; d_idx < max_elem_size; d_idx++){
      fifo_ptr->msg[idx].data[d_idx] = d_idx;
      }
      
    }
  } else {    
    ret_val = -1;
  }
  printDebug(1,"createFifo : max_elem %d max_elem_size %d\n",max_elem,max_elem_size);
  return ret_val;
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


int a653_init_queuing_ports(a653_queuing_port_config_t *config){
  int ret_val = 0;
  int p_idx = 0;
  int c_idx = 0;
  int found = 0;
  
  for (p_idx = 0; p_idx < MAX_Q_PORT; p_idx++){

    c_idx = 0;
    while (channel_config[c_idx].ChannelId != 0){
      if (channel_config[c_idx].ChannelId == config[p_idx].ChannelId){

	if (shm_ptr->channel_info[c_idx].Id != config[p_idx].ChannelId){
	  ret_val = -1;
	} else {
	qp_data[p_idx].ChannelIdx = c_idx;
	qp_data[p_idx].PortId     = qp_id_next++;
	qp_data[p_idx].Dir        = config[p_idx].Dir;
	qp_data[p_idx].Port       = (t_queuing_port_shm_data *)&shm_ptr->channel_info[c_idx].data.qp_d;
	
	strcpy(qp_data[p_idx].QUEUING_PORT_NAME, config[p_idx].name_str);
	found++;
	}
      }
      c_idx++;
    }      
  }
  
  PortsHash = (QUEUING_PORT_ID_TYPE *) malloc(sizeof(QUEUING_PORT_ID_TYPE) * (QP_START_ID + MAX_Q_PORT));

  printDebug(3,"%s number of ports %d\n",__func__,found);
  
  return ret_val;
}


/* external interface */

void CREATE_QUEUING_PORT (QUEUING_PORT_NAME_TYPE  QUEUING_PORT_NAME,
			  MESSAGE_SIZE_TYPE       MAX_MESSAGE_SIZE, 
			  MESSAGE_RANGE_TYPE      MAX_NB_MESSAGE,
			  PORT_DIRECTION_TYPE     PORT_DIRECTION,
			  QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE,
			  QUEUING_PORT_ID_TYPE   *QUEUING_PORT_ID,
			  RETURN_CODE_TYPE       *RETURN_CODE){
    
  int p_idx   = 0;
  int found = 0;
    
  *RETURN_CODE = INVALID_CONFIG;

  while ((!found) && (qp_data[p_idx].PortId != 0)) {

    if ((strncmp(qp_data[p_idx].QUEUING_PORT_NAME,QUEUING_PORT_NAME,25)) == 0) {
      
      found = 1;

      /* todo handle direction !!!!!!!!!!!!!!!!!!!!*/
      if (qp_data[p_idx].Dir == SOURCE){
	/* TX */
	/* save link to instance*/
	PortsHash[qp_data[p_idx].PortId] = p_idx;
	/* set return values */
        *QUEUING_PORT_ID = qp_data[p_idx].PortId;
        *RETURN_CODE     = NO_ERROR;

	break;
	
      } else {
	/* RX */
        
	if (shm_ptr->channel_info[p_idx].maxMsgSize <= MAX_MESSAGE_SIZE){
	  *RETURN_CODE = NOT_AVAILABLE;
	} else {
	  /* update internel struct */

	  if (createFifo (qp_data[p_idx].Port,MAX_NB_MESSAGE,MAX_MESSAGE_SIZE) == 0){	  
	    qp_data[p_idx].Port->init_done = 1;  
	    /* save link to instance*/
	    PortsHash[qp_data[p_idx].PortId] = p_idx;
	    
	    /* set return values */
	    *QUEUING_PORT_ID = qp_data[p_idx].PortId;
	    *RETURN_CODE     = NO_ERROR;
	  } 
	  break;
	}
      }
    }
    p_idx++;
  }
  
  if (*RETURN_CODE != NO_ERROR){
    printDebug(3,"%s error: %d\n",__func__,*RETURN_CODE);
  }
}

void SEND_QUEUING_MESSAGE (QUEUING_PORT_ID_TYPE   QUEUING_PORT_ID,
			   MESSAGE_ADDR_TYPE      MESSAGE_ADDR,
			   MESSAGE_SIZE_TYPE      LENGTH,
			   SYSTEM_TIME_TYPE       TIME_OUT,
			   RETURN_CODE_TYPE     * RETURN_CODE){

  int p_idx = PortsHash[QUEUING_PORT_ID];

  *RETURN_CODE = NO_ACTION;

  if (p_idx < MAX_Q_PORT){

    if ((qp_data[p_idx].Port->init_done == 0) &&
	(qp_data[p_idx].Dir != SOURCE) &&
	(shm_ptr->channel_info[p_idx].maxMsgSize <= LENGTH)){
      *RETURN_CODE = INVALID_CONFIG;
    } else {
				
      if( 0 == putFifo (qp_data[p_idx].Port,
			MESSAGE_ADDR, /* src  */
			LENGTH)){
	*RETURN_CODE = NO_ERROR;
      }
    }  
  }
  
  if (*RETURN_CODE != NO_ERROR){
    printDebug(3,"%s error: %d\n",__func__,*RETURN_CODE);
  }
}

void RECEIVE_QUEUING_MESSAGE (QUEUING_PORT_ID_TYPE QUEUING_PORT_ID,
			      SYSTEM_TIME_TYPE     TIME_OUT,
			      MESSAGE_ADDR_TYPE    MESSAGE_ADDR,
			      MESSAGE_SIZE_TYPE  * LENGTH,
			      RETURN_CODE_TYPE   * RETURN_CODE){

  int p_idx = PortsHash[QUEUING_PORT_ID];

  *RETURN_CODE = NO_ACTION;

  if (p_idx < MAX_Q_PORT){

    if (( qp_data[p_idx].Port->init_done == 0) &&
	(qp_data[p_idx].Dir != DESTINATION)) {
      
      *RETURN_CODE = INVALID_CONFIG;
      
    } else {
  
      if (0 == getFifo (qp_data[p_idx].Port,
                        MESSAGE_ADDR, /* dest */
                        (int *)LENGTH)){
        *RETURN_CODE = NO_ERROR;
      } else {
        *LENGTH      = 0;
        *RETURN_CODE = NO_ACTION;
      }
    }
  }
  
  if (*RETURN_CODE != NO_ERROR){
    printDebug(3,"%s error: %d\n",__func__,*RETURN_CODE);
  }
}
 
void GET_QUEUING_PORT_ID (QUEUING_PORT_NAME_TYPE   QUEUING_PORT_NAME,
                          QUEUING_PORT_ID_TYPE   * QUEUING_PORT_ID,
                          RETURN_CODE_TYPE       * RETURN_CODE){
  int p_idx   = 0;
  
  *QUEUING_PORT_ID = 0;    
  *RETURN_CODE     = INVALID_CONFIG;
  
  while ( qp_data[p_idx].PortId != 0) {

    if ((strncmp(qp_data[p_idx].QUEUING_PORT_NAME,QUEUING_PORT_NAME,25)) == 0) {

      /* set return values */
      *QUEUING_PORT_ID = qp_data[p_idx].PortId;
      *RETURN_CODE     = NO_ERROR;

      break;
    }	
    p_idx++;
  }
    
  if (*RETURN_CODE != NO_ERROR){
    printDebug(3,"%s error: %d\n",__func__,*RETURN_CODE);
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

