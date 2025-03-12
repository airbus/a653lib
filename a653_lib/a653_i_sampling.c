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
 * @file      a653_i_sampling.c
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

#define A653_SAMPLING_INTERN

#include "a653Init.h"
#include "a653Type.h"
#include "a653Error.h"
#include "a653Sampling.h"

#include "a653_i_shm_if.h"


#define SP_START_ID 1

typedef unsigned int offset_t;


extern a653_shm_info_t *shm_ptr;
extern int own_partition_idx;
extern a653_channel_config_t channel_config[];


int sp_id_next = SP_START_ID;

/* typedef struct{ */
/*   VALIDITY_TYPE            LAST_MSG_VALIDITY;  /\* message validity *\/ */
/*   MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE;   /\* port size *\/ */
/*   int                      init_done; */
/*   char                     data[MAX_S_PORT_SIZE];   */
/* } t_sampling_port_shm_data; */


typedef struct {
  unsigned short              PortId;
  unsigned short              ChannelIdx;
  unsigned short              Dir;
  unsigned short              MaxMsgSize;
  char                        SAMPLING_PORT_NAME[34];
  t_sampling_port_shm_data   *Port;
} sampling_port_data_t;

static SAMPLING_PORT_ID_TYPE  *PortsHash = NULL;
static sampling_port_data_t   sp_data[MAX_S_PORT];

/* typedef struct { */
/*   unsigned short ChannelId; */
/*   unsigned short Dir; */
/*   char           name_str[32]; */
/* } a653_sampling_port_config_t; */

int a653_init_sampling_ports(a653_sampling_port_config_t *config){
  int ret_val = 0;
  int p_idx = 0;
  int c_idx = 0;
  int found = 0;
 
  for (p_idx = 0; p_idx < MAX_S_PORT; p_idx++){
 
    c_idx = 0;
    while (channel_config[c_idx].ChannelId != 0){
      if (channel_config[c_idx].ChannelId == config[p_idx].ChannelId){

	if (shm_ptr->channel_info[c_idx].Id != config[p_idx].ChannelId){
	  ret_val = -1;
	} else {
	sp_data[p_idx].ChannelIdx = c_idx;
	sp_data[p_idx].PortId = sp_id_next++;
	sp_data[p_idx].Dir = config[p_idx].Dir;
	sp_data[p_idx].MaxMsgSize = channel_config[c_idx].maxMsgSize;
	sp_data[p_idx].Port = (t_sampling_port_shm_data *)&shm_ptr->channel_info[c_idx].data.sp_d;
	sp_data[p_idx].Port->LAST_MSG_VALIDITY = INVALID;
	
	strcpy(sp_data[p_idx].SAMPLING_PORT_NAME, config[p_idx].name_str);
	found++;

	 printDebug(2,"%s PortId %02d; ChannelIdx %04d; Dir %d; MaxMsgSize %04d; addr 0x%08x; name %s\n",
		    __func__,
		    sp_data[p_idx].PortId,
		    sp_data[p_idx].ChannelIdx,
		    sp_data[p_idx].Dir,
		    sp_data[p_idx].MaxMsgSize,
		    sp_data[p_idx].Port,
		    sp_data[p_idx].SAMPLING_PORT_NAME);
	}
      }
      c_idx++;
    }
   
  }
  
  PortsHash = (SAMPLING_PORT_ID_TYPE *) malloc(sizeof(SAMPLING_PORT_ID_TYPE) * (SP_START_ID + MAX_S_PORT));
  
  printDebug(1,"%s number of ports %d\n",__func__,found);
					      
  return ret_val;
}


void CREATE_SAMPLING_PORT (SAMPLING_PORT_NAME_TYPE  SAMPLING_PORT_NAME, 
			   MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE, 
			   PORT_DIRECTION_TYPE      PORT_DIRECTION, 
			   SYSTEM_TIME_TYPE         REFRESH_PERIOD, 
			   SAMPLING_PORT_ID_TYPE   *SAMPLING_PORT_ID, 
			   RETURN_CODE_TYPE        *RETURN_CODE){
  int p_idx   = 0;
  int length  = 0;
  int found   = 0;

  *RETURN_CODE = INVALID_CONFIG;

  while ((!found) && (sp_data[p_idx].PortId != 0)){

    length = strnlen(sp_data[p_idx].SAMPLING_PORT_NAME, 32);

    if ((strncmp(sp_data[p_idx].SAMPLING_PORT_NAME,SAMPLING_PORT_NAME,length)) == 0) {

      found = 1;

      if ((sp_data[p_idx].MaxMsgSize >= MAX_MESSAGE_SIZE) &&
	  (sp_data[p_idx].Dir == PORT_DIRECTION)){

	sp_data[p_idx].Port->LAST_MSG_VALIDITY = INVALID;
	sp_data[p_idx].Port->LAST_SIZE = 0;
	
	/*
	 PORT_DIRECTION
	 REFRESH_PERIOD
	 */
	/* save link to instance*/
	PortsHash[sp_data[p_idx].PortId] = p_idx;
        /* set return values */
        *SAMPLING_PORT_ID = sp_data[p_idx].PortId;
        *RETURN_CODE      = NO_ERROR;          
      } else {
	*RETURN_CODE      = INVALID_PARAM;
      }
      
    }    
    p_idx++;
  }

  if (*RETURN_CODE != NO_ERROR){
    printDebug(1,"%s error: %d name %s length %d max_msg_size %d\n",__func__,*RETURN_CODE,SAMPLING_PORT_NAME,length,MAX_MESSAGE_SIZE);
  } else {
    printDebug(1,"%s ok: %d name %s length %d max_msg_size %d\n",__func__,*RETURN_CODE,SAMPLING_PORT_NAME,length,MAX_MESSAGE_SIZE);
  }
}


void WRITE_SAMPLING_MESSAGE(SAMPLING_PORT_ID_TYPE   SAMPLING_PORT_ID, 
			    MESSAGE_ADDR_TYPE       MESSAGE_ADDR,
			    MESSAGE_SIZE_TYPE       LENGTH, 
			    RETURN_CODE_TYPE      * RETURN_CODE){
  
  int p_idx = PortsHash[SAMPLING_PORT_ID];

  // printDebug(3,"%s sp_id %d idx: %d\n",__func__,SAMPLING_PORT_ID,p_idx);

  if (p_idx < MAX_S_PORT){

    if ((sp_data[p_idx].Port->init_done == 0) &&
	(sp_data[p_idx].Dir != SOURCE) &&
	(sp_data[p_idx].MaxMsgSize <= LENGTH)) {
      
      *RETURN_CODE = INVALID_CONFIG;
      
      } else {
      
      memcpy(sp_data[p_idx].Port->data,     /* dest */
	     MESSAGE_ADDR,                /* src  */
	     LENGTH);
	       
      sp_data[p_idx].Port->LAST_MSG_VALIDITY = VALID;
      sp_data[p_idx].Port->LAST_SIZE = LENGTH;
	
      *RETURN_CODE = NO_ERROR;
    }
  }
  
  if (*RETURN_CODE != NO_ERROR){
    printDebug(3,"%s error: %d\n",__func__,*RETURN_CODE);
  }
}

void READ_SAMPLING_MESSAGE (SAMPLING_PORT_ID_TYPE    SAMPLING_PORT_ID, 
			    MESSAGE_ADDR_TYPE        MESSAGE_ADDR, 
			    MESSAGE_SIZE_TYPE      * LENGTH, 
			    VALIDITY_TYPE          * VALIDITY, 
			    RETURN_CODE_TYPE       * RETURN_CODE){
  
  int p_idx = PortsHash[SAMPLING_PORT_ID];

  if (p_idx < MAX_S_PORT){

    if ((sp_data[p_idx].Port->init_done == 0) &&
	(sp_data[p_idx].Dir != DESTINATION)) {
      
      *RETURN_CODE = INVALID_CONFIG;
      
    } else {
       
      if ( sp_data[p_idx].Port->LAST_MSG_VALIDITY == INVALID){
	/* no data available */
	*VALIDITY    = INVALID;
	*RETURN_CODE = NO_ACTION;
      } else {
	/* return last data */
	*LENGTH   = sp_data[p_idx].Port->LAST_SIZE;
	*VALIDITY = VALID;
        
	memcpy(MESSAGE_ADDR,                        /* dest */ 
	       sp_data[p_idx].Port->data,           /* src  */             
	       *LENGTH);

	//	sp_data[p_idx].Port->LAST_MSG_VALIDITY = INVALID; todo bn add subvision

	*RETURN_CODE = NO_ERROR;
      }
    }
  }
  if (*RETURN_CODE != NO_ERROR &&
      *RETURN_CODE != NO_ACTION){
    printDebug(3,"%s error: %d\n",__func__,*RETURN_CODE);
  }
}



void INIT_SAMPLING_PORT (RETURN_CODE_TYPE        *RETURN_CODE){
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

  int p_idx   = 0;
  int length  = 0;

  *RETURN_CODE = INVALID_CONFIG;

  while (sp_data[p_idx].PortId != 0){

    length = strnlen(sp_data[p_idx].SAMPLING_PORT_NAME, 32);

    if ((strncmp(sp_data[p_idx].SAMPLING_PORT_NAME,SAMPLING_PORT_NAME,length)) == 0) {
      
      /* set return values */
      *SAMPLING_PORT_ID = sp_data[p_idx].PortId;

      if ((*SAMPLING_PORT_ID < 0) ||
	  (*SAMPLING_PORT_ID > MAX_S_PORT)){
	*SAMPLING_PORT_ID = 0;
	*RETURN_CODE = INVALID_CONFIG;
      } else {
	*SAMPLING_PORT_ID = sp_data[p_idx].PortId;
	*RETURN_CODE = NO_ERROR;	
      }
      break;
    }    
    p_idx++;
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


