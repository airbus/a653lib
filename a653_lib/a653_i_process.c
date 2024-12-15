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
 * @file      a653_i_process.c
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     a653 process 
 * @details    
 */

#define DEBUG


/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#ifdef DEBUG
#include <sys/time.h>
//--------------------
#endif

#define A653_QUEUING_INTERN

#include "a653Init.h"
#include "a653Type.h"
#include "a653Error.h"
#include "a653Process.h"
#include "a653Partition.h"

#define A653_PROCESS_INTERN

#include "a653_i_process.h"


//extern entry_point_t model_main;


extern int own_partition_idx;

int number_of_processes = 0;

static a653_process_config_t *a653_process_config;
static prcs_info_t *prcs_info; 
static int *prcsHash;

static void prcs_main(void);


int a653_prcs_init(int num_of_prcs, a653_process_config_t *config);

/********************************************************************************************************************************/

/*
 * initialize of all a653 prozess data for this partition
 */

int a653_prcs_init(int num_of_prcs, a653_process_config_t *config){

  int ret_val = 0;

  prcs_info = (prcs_info_t *) malloc(sizeof(prcs_info_t) * num_of_prcs);
  prcsHash = (int *) malloc(sizeof(int) * num_of_prcs);
  
  if ((config != NULL) &&
      (prcs_info != NULL) &&
      (prcsHash != NULL)){
    
    number_of_processes = num_of_prcs;
    a653_process_config = config;
        
  } else {
    ret_val = 1;
  }
  
  return ret_val;
}

int  a653_sync_prcs(void){
  int idx   = 0;
  
  pthread_t pt_self = pthread_self();

  for (idx = 0; idx < number_of_processes; idx++) {
    
    if (prcs_info[idx].t_ctx == pt_self){
      
      pthread_mutex_lock(&prcs_info[idx].t_lock);
      break;
    }
  }
  return (idx);
}

void a653_act_prcs(void){
 
  int idx   = 0;
  
  for (idx = 0; idx < number_of_processes; idx++) {
    pthread_mutex_unlock(&prcs_info[idx].t_lock);
    //    break;
  }
  
}

static void prcs_main(void){
  int idx   = 0;
  
  pthread_t pt_self = pthread_self();

  for (idx = 0; idx < number_of_processes; idx++) {
    
    if (prcs_info[idx].t_ctx == pt_self){
      
      pthread_mutex_lock(&prcs_info[idx].t_lock);
      break;
    }
  }

  (*(prcs_info[idx].prcs_main_func))();
}


void GET_PROCESS_ID (PROCESS_NAME_TYPE PROCESS_NAME,
                     PROCESS_ID_TYPE  *PROCESS_ID,
                     RETURN_CODE_TYPE *RETURN_CODE){

  int idx   = 0;
  int found = 0;

  *RETURN_CODE = INVALID_CONFIG;

  while ((!found) && (a653_process_config[idx].PrcsId != 0)) {
    if ((strncmp(a653_process_config[idx].name_str,PROCESS_NAME,25)) == 0) {   
 
      found = 1;

      /* set return values */
      *PROCESS_ID  = a653_process_config[idx].PrcsId;
      *RETURN_CODE = NO_ERROR;
    }
    idx++;
  }
}

void GET_MY_ID (PROCESS_ID_TYPE  *PROCESS_ID,
                RETURN_CODE_TYPE *RETURN_CODE){

  int idx           = 0;
  int found         = 0;
  pthread_t pt_self = pthread_self();

  *RETURN_CODE = INVALID_CONFIG;

  while ((!found) && idx < number_of_processes) {
    
    if (prcs_info[idx].t_ctx == pt_self){

      found = 1;

      /* set return values */
      *PROCESS_ID  = a653_process_config[idx].PrcsId;
      *RETURN_CODE = NO_ERROR;
    }
    idx++;
  }
}

void GET_PROCESS_STATUS (PROCESS_ID_TYPE      PROCESS_ID,
                         PROCESS_STATUS_TYPE *PROCESS_STATUS,
                         RETURN_CODE_TYPE    *RETURN_CODE){
  *RETURN_CODE = NO_ACTION;
}

void CREATE_PROCESS (PROCESS_ATTRIBUTE_TYPE *ATTRIBUTES,
                     PROCESS_ID_TYPE        *PROCESS_ID,
                     RETURN_CODE_TYPE       *RETURN_CODE){

  int idx           = 0;

  *RETURN_CODE = INVALID_CONFIG;

  while (a653_process_config[idx].PrcsId != 0) {
    if ((strncmp(a653_process_config[idx].name_str,ATTRIBUTES->NAME,25)) == 0) {   
      /* valid entry found */
      
      strncpy(prcs_info[idx].name,ATTRIBUTES->NAME,25);
      
      prcs_info[idx].timerPeriod = (ATTRIBUTES->PERIOD); /* in nsec */
      prcs_info[idx].priority = (ATTRIBUTES->BASE_PRIORITY);
      //      prcs_info[idx].timerCnt    = 0;

      prcsHash[a653_process_config[idx].PrcsId] = idx;
      prcs_info[idx].prcs_main_func = ATTRIBUTES->ENTRY_POINT;

                     
      /* set return values */
      *PROCESS_ID  = a653_process_config[idx].PrcsId;
      *RETURN_CODE = NO_ERROR;

      break;
    }
    idx++;
  }
}

void SET_PRIORITY (PROCESS_ID_TYPE PROCESS_ID,
                   PRIORITY_TYPE PRIORITY,
                   RETURN_CODE_TYPE * RETURN_CODE){
  *RETURN_CODE = NO_ACTION;
}

void SUSPEND_SELF (SYSTEM_TIME_TYPE TIME_OUT,
                   RETURN_CODE_TYPE * RETURN_CODE){
  *RETURN_CODE = NO_ACTION;
}

void SUSPEND (PROCESS_ID_TYPE PROCESS_ID,
              RETURN_CODE_TYPE * RETURN_CODE){
  *RETURN_CODE = NO_ACTION;
}

void RESUME (PROCESS_ID_TYPE PROCESS_ID,
             RETURN_CODE_TYPE * RETURN_CODE){
  *RETURN_CODE = NO_ACTION;
}

void STOP_SELF  (void){
}

void STOP (PROCESS_ID_TYPE PROCESS_ID,
           RETURN_CODE_TYPE * RETURN_CODE){
  *RETURN_CODE = NO_ACTION;
}

void START (PROCESS_ID_TYPE PROCESS_ID,
            RETURN_CODE_TYPE * RETURN_CODE){

  int idx = 0;
  *RETURN_CODE = INVALID_CONFIG;

  idx = prcsHash[PROCESS_ID];

  pthread_attr_init(&prcs_info[idx].t_attr);

  if(prcs_info[idx].timerPeriod > 0){
    /* periodic process */
    printDebug(1,"start periodic process : partition %d : %s\n",
	       own_partition_idx,
	       prcs_info[idx].name);
     
    pthread_mutex_init(&prcs_info[idx].t_lock,NULL);
    pthread_mutex_lock(&prcs_info[idx].t_lock);

    pthread_create(&prcs_info[idx].t_ctx,
		   &prcs_info[idx].t_attr, 
		   (__start_routine) prcs_main,
		   NULL); /* no arg*/
    pthread_setschedprio(&prcs_info[idx].t_ctx, prcs_info[idx].priority);
  } else {
    /* aperiodic process */
    printDebug(1,"start aperiodic process : partition %d : %s\n",
	       own_partition_idx,
	       prcs_info[idx].name);

    pthread_create(&prcs_info[idx].t_ctx,
		   &prcs_info[idx].t_attr, 
		   (__start_routine) prcs_info[idx].prcs_main_func,
		   NULL); /* no arg*/
    pthread_setschedprio(&prcs_info[idx].t_ctx, prcs_info[idx].priority);
  }
  /*
  prcs_info[idx].timer = ptimer_start(prcs_info[idx].timerPeriod,
				      a653_act_prcs,
				      TIMER_PERIODIC,
				      &prcs_info[idx]);
  */
  *RETURN_CODE = NO_ERROR;
}

void DELAYED_START (PROCESS_ID_TYPE PROCESS_ID,
                    SYSTEM_TIME_TYPE  DELAY_TIME,
                    RETURN_CODE_TYPE * RETURN_CODE){
  *RETURN_CODE = NO_ACTION;
}

void LOCK_PREEMPTION (LOCK_LEVEL_TYPE * LOCK_LEVEL,
                      RETURN_CODE_TYPE * RETURN_CODE){
  *RETURN_CODE = NO_ACTION;
}

void UNLOCK_PREEMPTION (LOCK_LEVEL_TYPE * LOCK_LEVEL,
                        RETURN_CODE_TYPE * RETURN_CODE){
  *RETURN_CODE = NO_ACTION;
}

void a653TimeMonitorProcGet (PROCESS_ID_TYPE PROCESS_ID,
                             SYSTEM_TIME_TYPE * procTime,
                             SYSTEM_TIME_TYPE *  refTime,
                             SYSTEM_TIME_TYPE *  DLMinTime,
                             SYSTEM_TIME_TYPE *  DLMaxTime,
                             RETURN_CODE_TYPE *  RETURN_CODE){
  *RETURN_CODE = NO_ACTION;
}

PROCESS_ID_TYPE procIdFromTaskIdGet (int taskId){
  PROCESS_ID_TYPE ret_val = 0;

  return(ret_val);
}

int taskIdFromProcIdGet (PROCESS_ID_TYPE procId){
  int ret_val = 0;

  return(ret_val);
}


