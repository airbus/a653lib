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
 * @file      a653_i_partition.c
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     a653 partition
 * @details    
 */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "a653Init.h"
#include "a653Type.h"
#include "a653Error.h"
#include "a653Process.h"
#include "a653Partition.h"

#include "a653_i_process.h"
#include "a653_i_shm_if.h"
#include "a653_i_sync.h"

extern a653_shm_info_t *shm_ptr;

extern a653_global_config_t global_config;
extern a653_process_config_t A653_PROCESS_CONFIG[];
extern a653_sampling_port_config_t A653_SP_CONFIG[];
extern a653_queuing_port_config_t A653_QP_CONFIG[];

PARTITION_STATUS_TYPE pertition_status = {0,0,1,0,IDLE,NORMAL_START,1};

int own_partition_idx;

/* do not call main schaduler !!!!!! */
int a653_init_partition(void){
  
  int ret_val = 0;
  int idx,idx_i,idx_o = 0;
  int numOfPrcs = 0;
  int numOfSPorts = 0;
  int numOfQPorts = 0;
  int found = 0;
  pid_t ownPid = getpid();

  a653_process_config_t *a653_process_config;
  
  a653_shm_init();
 
  while(found ==0){
    for (idx = 0;idx < global_config.partition_number; idx++){
      if (shm_ptr->partition_info[idx].pid == ownPid){
	own_partition_idx = idx;
	found = 1;
	printDebug(3,"%s %d fount index: %d\n",
		   shm_ptr->partition_info[idx].name,
		   shm_ptr->partition_info[idx].pid,
		   own_partition_idx);
      }  
    }
  }

  /* init all local processes */
  idx = 0;
  do {
    if (A653_PROCESS_CONFIG[idx++].PartitionIdx == own_partition_idx){
      numOfPrcs++;
    }
  } while (A653_PROCESS_CONFIG[idx].PrcsId != 0);
  
  a653_process_config = malloc(sizeof(a653_process_config_t) * numOfPrcs);

  idx_i = idx_o = 0;
  do {
    if (A653_PROCESS_CONFIG[idx_i].PartitionIdx == own_partition_idx){
      a653_process_config[idx_o] =  A653_PROCESS_CONFIG[idx_i];
      idx_o++;
    }
    idx_i++;
  } while (A653_PROCESS_CONFIG[idx_i].PrcsId != 0);
      
  if ( a653_prcs_init(numOfPrcs,a653_process_config) ){ ret_val = 1; }

  /* init all sampling ports */
  idx = 0;
  while (A653_SP_CONFIG[idx++].PortId != 0){
    numOfSPorts++;
  }
  
  if (a653_init_sampling_ports(numOfSPorts,A653_SP_CONFIG)){ ret_val = 1; }

  /* init al queuing ports */
  idx = 0;
  while (A653_QP_CONFIG[idx++].PortId != 0){
    numOfQPorts++;
  }
  
  if (a653_init_queuing_ports(numOfSPorts,A653_QP_CONFIG)){ ret_val = 1; }
  
  
  return ret_val;
}

void a653_act_partition(void){
  if (shm_ptr->partition_info[own_partition_idx].go){
    shm_ptr->partition_info[own_partition_idx].go = 0;
    a653_act_prcs();
  }
}


extern void GET_PARTITION_STATUS (
/*out*/ PARTITION_STATUS_TYPE *PARTITION_STATUS,
/*out*/ RETURN_CODE_TYPE *RETURN_CODE ){

  if(shm_ptr->partition_info == NULL){
    a653_init_partition();
  }
  
  if(shm_ptr->partition_info[own_partition_idx].init == 1){
    pertition_status.OPERATING_MODE = COLD_START;
  }
  
  *PARTITION_STATUS = pertition_status;
  *RETURN_CODE      = NO_ERROR;
  
}

extern void SET_PARTITION_MODE (
/*in */ OPERATING_MODE_TYPE OPERATING_MODE,
/*out*/ RETURN_CODE_TYPE *RETURN_CODE ){

  if (pertition_status.OPERATING_MODE != IDLE &&
      OPERATING_MODE == NORMAL){
    shm_ptr->partition_info[own_partition_idx].init = 0;
  }
  
  pertition_status.OPERATING_MODE = OPERATING_MODE;
  
  printDebug(1,"a653 start partition (%d)\n",getpid());
  while (1){
    a653_act_partition();
    usleep(50);	 
  }
  
  *RETURN_CODE      = NO_ERROR;
}

