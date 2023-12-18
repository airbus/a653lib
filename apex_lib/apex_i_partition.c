/**
 * @file      apex_i_partition.c
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

#include "apexInit.h"
#include "apexType.h"
#include "apexError.h"
#include "apexProcess.h"
#include "apexPartition.h"

#include "apex_i_process.h"
#include "apex_i_shm_if.h"
#include "apex_i_sync.h"

extern apex_shm_info_t *shm_ptr;

extern apex_global_config_t global_config;
extern apex_process_config_t APEX_PROCESS_CONFIG[];
extern apex_sampling_port_config_t APEX_SP_CONFIG[];
extern apex_queuing_port_config_t APEX_QP_CONFIG[];

PARTITION_STATUS_TYPE pertition_status = {0,0,1,0,IDLE,NORMAL_START,1};

int own_partition_idx;

/* do not call main schaduler !!!!!! */
int apex_init_partition(void){
  
  int ret_val = 0;
  int idx,idx_i,idx_o = 0;
  int numOfPrcs = 0;
  int numOfSPorts = 0;
  int numOfQPorts = 0;
  int found = 0;
  pid_t ownPid = getpid();

  apex_process_config_t *apex_process_config;
  
  apex_shm_init();
 
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
    if (APEX_PROCESS_CONFIG[idx++].PartitionIdx == own_partition_idx){
      numOfPrcs++;
    }
  } while (APEX_PROCESS_CONFIG[idx].PrcsId != 0);
  
  apex_process_config = malloc(sizeof(apex_process_config_t) * numOfPrcs);

  idx_i = idx_o = 0;
  do {
    if (APEX_PROCESS_CONFIG[idx_i].PartitionIdx == own_partition_idx){
      apex_process_config[idx_o] =  APEX_PROCESS_CONFIG[idx_i];
      idx_o++;
    }
    idx_i++;
  } while (APEX_PROCESS_CONFIG[idx_i].PrcsId != 0);
      
  if ( apex_prcs_init(numOfPrcs,apex_process_config) ){ ret_val = 1; }

  /* init all sampling ports */
  idx = 0;
  while (APEX_SP_CONFIG[idx++].PortId != 0){
    numOfSPorts++;
  }
  
  if (apex_init_sampling_ports(numOfSPorts,APEX_SP_CONFIG)){ ret_val = 1; }

  /* init al queuing ports */
  idx = 0;
  while (APEX_QP_CONFIG[idx++].PortId != 0){
    numOfQPorts++;
  }
  
  if (apex_init_queuing_ports(numOfSPorts,APEX_QP_CONFIG)){ ret_val = 1; }
  
  
  return ret_val;
}

void apex_act_partition(void){
  if (shm_ptr->partition_info[own_partition_idx].go){
    shm_ptr->partition_info[own_partition_idx].go = 0;
    apex_act_prcs();
  }
}


extern void GET_PARTITION_STATUS (
/*out*/ PARTITION_STATUS_TYPE *PARTITION_STATUS,
/*out*/ RETURN_CODE_TYPE *RETURN_CODE ){
  
  if(shm_ptr->partition_info == NULL){
    apex_init_partition();
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
  *RETURN_CODE      = NO_ERROR;
}

