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
 * @file      apex_i_sampling.c
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     a653 sampling port
 * @details    
 */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
//--------------------

#define APEX_SAMPLING_INTERN

#include "apexInit.h"
#include "apexType.h"
#include "apexError.h"
#include "apexSampling.h"

#include "apex_i_shm_if.h"

typedef unsigned int offset_t;


/* typedef struct{ */
/*   char                     SAMPLING_PORT_NAME[34]; */
/*   unsigned short           PortId; */
/*   VALIDITY_TYPE            LAST_MSG_VALIDITY;  /\* message validity *\/ */
/*   MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE;   /\* port size *\/ */
/*   int                      init_done; */
/*   char                     *data;   */
/* } t_sampling_port_shm_data; */

extern apex_shm_info_t *shm_ptr;
extern int own_partition_idx;


typedef struct {
  int                         max_port_id;
  int                         max_port_num;
  int                         max_port_data_size;
  t_sampling_port_shm_data  **Ports;
  SAMPLING_PORT_ID_TYPE      *PortsHash;
} sampling_port_data_t;

/* be very carefull!!! pointer to shared memory can be different in each process!!! */
static sampling_port_data_t *sp_ptr = NULL;
static apex_sampling_port_config_t *apex_sp_config = NULL;



int apex_init_sampling_ports(int max_port_num, apex_sampling_port_config_t *config){
  int ret_val = 0;
  int idx = 0;
  int highest_sampling_port_id = 0;
  
  for(idx=0; idx <= SAMPLING_PORT_ID_MAX; idx++){
    if (config[idx].PortId == 0){
      break;
    }
    if (highest_sampling_port_id < config[idx].PortId)
      highest_sampling_port_id = config[idx].PortId;   
  }
  
  if(highest_sampling_port_id > SAMPLING_PORT_ID_MAX){
    printDebug(3," sampling port id range error !!!\n\n");
    ret_val = 1; // exit(1);
  } else {
  
    apex_sp_config = (apex_sampling_port_config_t *) malloc(sizeof(apex_sampling_port_config_t) * max_port_num);
    memcpy(apex_sp_config,config,sizeof(apex_sampling_port_config_t) * max_port_num);

    sp_ptr = (sampling_port_data_t*) malloc(sizeof(sampling_port_data_t) * max_port_num);
    if (sp_ptr == NULL){
      printDebug(3,"can not get shared memory for sampling ports!!!\n\n");
      ret_val = 1; // exit(1);
    } else {
      memset(sp_ptr,0,sizeof(sampling_port_data_t) * max_port_num);

      sp_ptr->PortsHash = (SAMPLING_PORT_ID_TYPE *) malloc(sizeof(SAMPLING_PORT_ID_TYPE) * highest_sampling_port_id);
      if (sp_ptr->PortsHash == NULL){
	printDebug(3,"can not get shared memory for sampling ports!!!\n\n");
	ret_val = 1; // exit(1);
      } else {
	memset(sp_ptr->PortsHash,0,sizeof(SAMPLING_PORT_ID_TYPE) * max_port_num);

	sp_ptr->Ports = (t_sampling_port_shm_data **) malloc(sizeof(t_sampling_port_shm_data *) * max_port_num);
	if (sp_ptr->Ports == NULL){
	  printDebug(3,"can not get shared memory fpr sammpling ports!!!\n\n");
	  ret_val = 1; // exit(1);
	} else {
	  memset(sp_ptr->Ports,0,sizeof(t_sampling_port_shm_data *) * max_port_num);

	  sp_ptr->max_port_id        = highest_sampling_port_id;
	  sp_ptr->max_port_num       = max_port_num;
	  sp_ptr->max_port_data_size = MAX_S_PORT_SIZE;
	  
	  for(idx=0; idx < max_port_num; idx++){
	    sp_ptr->Ports[idx] = &shm_ptr->s_port_data[idx];
	  }
	  printDebug(3,"%s number of ports %d\n",__func__,max_port_num);
	}
      }
    }
  }
  
  return ret_val;
}


static void create_sampling_port_ip (SAMPLING_PORT_NAME_TYPE  SAMPLING_PORT_NAME, 
				     MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE, 
				     PORT_DIRECTION_TYPE      PORT_DIRECTION, 
				     SYSTEM_TIME_TYPE         REFRESH_PERIOD, 
				     SAMPLING_PORT_ID_TYPE   *SAMPLING_PORT_ID, 
				     RETURN_CODE_TYPE        *RETURN_CODE){
  int idx   = 0;
  int found = 0;

  *RETURN_CODE = INVALID_CONFIG;

  while ((!found) && (apex_sp_config[idx].PortId != 0)){

    if ((strncmp(apex_sp_config[idx].name_str,SAMPLING_PORT_NAME,25)) == 0) {

      found = 1;

      if (sp_ptr->max_port_data_size >= MAX_MESSAGE_SIZE){

	strcpy(sp_ptr->Ports[idx]->SAMPLING_PORT_NAME,
	       apex_sp_config[idx].name_str);
	
        sp_ptr->Ports[idx]->LAST_MSG_VALIDITY = INVALID;
        sp_ptr->Ports[idx]->MAX_MESSAGE_SIZE  = MAX_MESSAGE_SIZE;
	/*
	 PORT_DIRECTION
	 REFRESH_PERIOD
	 */
	sp_ptr->Ports[idx]->PortId = apex_sp_config[idx].PortId;
	/* save link to instance*/
	sp_ptr->PortsHash[apex_sp_config[idx].PortId] = idx;
	
        /* set return values */
        *SAMPLING_PORT_ID = apex_sp_config[idx].PortId;
        *RETURN_CODE      = NO_ERROR;          
      }
    }    
    idx++;
  }
  
  if (*RETURN_CODE != NO_ERROR){
    printDebug(3,"CREATE_SAMPLING_PORT error: %d\n",*RETURN_CODE);
  }
}


static void write_sampling_message_ip (SAMPLING_PORT_ID_TYPE   SAMPLING_PORT_ID, 
				       MESSAGE_ADDR_TYPE       MESSAGE_ADDR,
				       MESSAGE_SIZE_TYPE       LENGTH, 
				       RETURN_CODE_TYPE      * RETURN_CODE){
  
  int idx = sp_ptr->PortsHash[SAMPLING_PORT_ID];

  //  printDebug(3,"%s sp_id %d idx: %d\n",__func__,SAMPLING_PORT_ID,idx);

  if (idx < sp_ptr->max_port_num){

    if (sp_ptr->Ports[idx]->data == NULL) {
      *RETURN_CODE = NOT_AVAILABLE;
    } else {
      if ((LENGTH > sp_ptr->Ports[idx]->MAX_MESSAGE_SIZE)){ // || 
	//          (sp_ptr->Ports[idx]->Sampling_Port_Status.PORT_DIRECTION != SOURCE)) {
	*RETURN_CODE = INVALID_CONFIG;
      } else {
	memcpy(sp_ptr->Ports[idx]->data,             /* dest */
	       MESSAGE_ADDR,                         /* src  */
	       LENGTH);
	sp_ptr->Ports[idx]->LAST_MSG_VALIDITY = VALID;

	//	printDebug(3,"%s LAST_MSG_VALIDITY %d (%p)\n",__func__,sp_ptr->Ports[idx]->LAST_MSG_VALIDITY, &sp_ptr->Ports[idx]->LAST_MSG_VALIDITY);
	
	*RETURN_CODE = NO_ERROR;
      }
    }
  }
  if (*RETURN_CODE != NO_ERROR){
    printDebug(3,"%s error: %d\n",__func__,*RETURN_CODE);
  }
}

static void read_sampling_message_ip (SAMPLING_PORT_ID_TYPE    SAMPLING_PORT_ID, 
				      MESSAGE_ADDR_TYPE        MESSAGE_ADDR, 
				      MESSAGE_SIZE_TYPE      * LENGTH, 
				      VALIDITY_TYPE          * VALIDITY, 
				      RETURN_CODE_TYPE       * RETURN_CODE){
  
  int idx = sp_ptr->PortsHash[SAMPLING_PORT_ID];

  //  printDebug(3,"%s sp_id %d idx: %d\n",__func__,SAMPLING_PORT_ID,idx);
  
  if (idx < sp_ptr->max_port_num){

    if (sp_ptr->Ports[idx]->data == NULL) {
      *RETURN_CODE = NOT_AVAILABLE;
    } else {
      //      printDebug(3,"%s LAST_MSG_VALIDITY %d (%p)\n",__func__,sp_ptr->Ports[idx]->LAST_MSG_VALIDITY, &sp_ptr->Ports[idx]->LAST_MSG_VALIDITY);
      
      /*       if (sp_ptr->Ports[idx]->Sampling_Port_Status.PORT_DIRECTION != DESTINATION) { */
      /*         *RETURN_CODE = INVALID_CONFIG; */
      /*       } else { */
      if (sp_ptr->Ports[idx]->LAST_MSG_VALIDITY == INVALID){
	/* no data available */
	*VALIDITY    = INVALID;
	*RETURN_CODE = NO_ACTION;
      } else {
	/* return last data */
	*LENGTH   = sp_ptr->Ports[idx]->MAX_MESSAGE_SIZE;
	*VALIDITY = VALID;
        
	memcpy(MESSAGE_ADDR,                             /* dest */ 
	       (sp_ptr->Ports[idx]->data),               /* src  */             
	       *LENGTH);

	sp_ptr->Ports[idx]->LAST_MSG_VALIDITY = INVALID;

	*RETURN_CODE = NO_ERROR;
      }
    }
    /*     } */
  }
  if (*RETURN_CODE != NO_ERROR){
    printDebug(3,"%s error: %d\n",__func__,*RETURN_CODE);
  }
}



void INIT_SAMPLING_PORT (RETURN_CODE_TYPE        *RETURN_CODE){
}

void CREATE_SAMPLING_PORT (SAMPLING_PORT_NAME_TYPE   SAMPLING_PORT_NAME, 
			   MESSAGE_SIZE_TYPE         MAX_MESSAGE_SIZE, 
			   PORT_DIRECTION_TYPE       PORT_DIRECTION, 
			   SYSTEM_TIME_TYPE          REFRESH_PERIOD, 
			   SAMPLING_PORT_ID_TYPE   * SAMPLING_PORT_ID, 
			   RETURN_CODE_TYPE        * RETURN_CODE){
  
  SAMPLING_PORT_ID_TYPE l_port_id; 
  RETURN_CODE_TYPE  l_return_code;

  GET_SAMPLING_PORT_ID (SAMPLING_PORT_NAME,
			&l_port_id, 
			&l_return_code);

  if (l_return_code != INVALID_CONFIG){

    /* if (l_port_id < 2000){  */
    
      create_sampling_port_ip (SAMPLING_PORT_NAME,
			       MAX_MESSAGE_SIZE,
			       PORT_DIRECTION,
			       REFRESH_PERIOD,
			       SAMPLING_PORT_ID,
			       RETURN_CODE);
    /* } else { */
    /*   create_sampling_port_pp (SAMPLING_PORT_NAME,  */
    /* 			       MAX_MESSAGE_SIZE,  */
    /* 			       PORT_DIRECTION,  */
    /* 			       REFRESH_PERIOD,  */
    /* 			       SAMPLING_PORT_ID,  */
    /* 			       RETURN_CODE);          */
    /* } */
      
  } else {
    printDebug(3,"%s error: %d\n",__func__,*RETURN_CODE);
  } 
}

void WRITE_SAMPLING_MESSAGE (SAMPLING_PORT_ID_TYPE   SAMPLING_PORT_ID, 
			     MESSAGE_ADDR_TYPE       MESSAGE_ADDR,
			     MESSAGE_SIZE_TYPE       LENGTH, 
			     RETURN_CODE_TYPE      * RETURN_CODE){

  *RETURN_CODE = NO_ACTION;

  if ((SAMPLING_PORT_ID < 0) ||
      (SAMPLING_PORT_ID > sp_ptr->max_port_id)){

    *RETURN_CODE = INVALID_CONFIG;
    
  } else {

  
    /* if (SAMPLING_PORT_ID < 2000){  */
      write_sampling_message_ip(SAMPLING_PORT_ID,
				MESSAGE_ADDR,
				LENGTH,
				RETURN_CODE);
    /* } else { */
    /*   write_sampling_message_pp(SAMPLING_PORT_ID,  */
    /* 				MESSAGE_ADDR, */
    /* 				LENGTH,  */
    /* 				RETURN_CODE); */
    /* } */
  }
  if (*RETURN_CODE > NO_ACTION){
    printDebug(3,"%s error: %d\n",__func__,*RETURN_CODE);
  }
}

void READ_SAMPLING_MESSAGE (SAMPLING_PORT_ID_TYPE    SAMPLING_PORT_ID, 
			    MESSAGE_ADDR_TYPE        MESSAGE_ADDR, 
			    MESSAGE_SIZE_TYPE      * LENGTH, 
			    VALIDITY_TYPE          * VALIDITY, 
			    RETURN_CODE_TYPE       * RETURN_CODE){

  *RETURN_CODE = NO_ACTION;

  if ((SAMPLING_PORT_ID < 0) ||
      (SAMPLING_PORT_ID > sp_ptr->max_port_id)){

    *RETURN_CODE = INVALID_CONFIG;
    
  } else {
    
    /* if (SAMPLING_PORT_ID < 2000){  */
      read_sampling_message_ip(SAMPLING_PORT_ID,
			       MESSAGE_ADDR,
			       LENGTH,
			       VALIDITY,
			       RETURN_CODE);
    /* } else { */
    /*   read_sampling_message_pp(SAMPLING_PORT_ID,  */
    /* 			       MESSAGE_ADDR,  */
    /* 			       LENGTH,  */
    /* 			       VALIDITY,  */
    /* 			       RETURN_CODE); */
    /* } */
  }
  if (*RETURN_CODE > NO_ACTION){
    printDebug(3,"%s error: %d\n",__func__,*RETURN_CODE);
  }
}

void READ_UPDATED_SAMPLING_MESSAGE (SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID,
                                    MESSAGE_ADDR_TYPE       MESSAGE_ADDR,
                                    MESSAGE_SIZE_TYPE *     LENGTH,
                                    UPDATED_TYPE *          UPDATED,
                                    RETURN_CODE_TYPE *      RETURN_CODE){
}

void READ_SAMPLING_MESSAGE_CONDITIONAL (SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID,
                                        SYSTEM_TIME_TYPE        REF_TIME_STAMP,
                                        MESSAGE_ADDR_TYPE       MESSAGE_ADDR,
                                        MESSAGE_SIZE_TYPE *     LENGTH,
                                        SYSTEM_TIME_TYPE *      TIME_STAMP,
                                        RETURN_CODE_TYPE *      RETURN_CODE){
}

void GET_SAMPLING_PORT_ID (SAMPLING_PORT_NAME_TYPE   SAMPLING_PORT_NAME,
                           SAMPLING_PORT_ID_TYPE   * SAMPLING_PORT_ID, 
                           RETURN_CODE_TYPE        * RETURN_CODE){

  int idx   = 0;

  *RETURN_CODE = INVALID_CONFIG;

  while (apex_sp_config[idx].PortId != 0){

    if ((strncmp(apex_sp_config[idx].name_str,SAMPLING_PORT_NAME,25)) == 0) {
      
      /* set return values */
      *SAMPLING_PORT_ID = apex_sp_config[idx].PortId;

      if ((*SAMPLING_PORT_ID < 0) ||
	  (*SAMPLING_PORT_ID > sp_ptr->max_port_id)){
	*SAMPLING_PORT_ID = 0;
	*RETURN_CODE = INVALID_CONFIG;
      } else {
	*SAMPLING_PORT_ID = apex_sp_config[idx].PortId;
	*RETURN_CODE = NO_ERROR;	
      }
      break;
    }    
    idx++;
  }
  
  if (*RETURN_CODE > NO_ACTION){
    printDebug(3,"%s error: %d\n",__func__,*RETURN_CODE);
  }
  
}

void GET_SAMPLING_PORT_STATUS (SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID, 
                               SAMPLING_PORT_STATUS_TYPE * SAMPLING_PORT_STATUS, 
                               RETURN_CODE_TYPE    * RETURN_CODE){
}

void GET_SAMPLING_PORT_CURRENT_STATUS (SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID,
                                       SAMPLING_PORT_CURRENT_STATUS_TYPE * SAMPLING_PORT_CURRENT_STATUS,
                                       RETURN_CODE_TYPE *      RETURN_CODE){
}


