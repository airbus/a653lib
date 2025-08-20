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


/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>

#define A653_QUEUING_INTERN

#include "a653Init.h"
#include "a653Type.h"
#include "a653Error.h"
#include "a653Process.h"
#include "a653Partition.h"

#define A653_PROCESS_INTERN

#include "a653_i_process.h"
#include "a653_i_shm_if.h"
#include "a653_i_time_lib.h"

//extern entry_point_t model_main;

#define PRCS_START_ID 1

extern a653_shm_info_t *shm_ptr;
extern int own_partition_idx;
extern int64_t time_slice;

int number_of_processes = 0;
int prcs_id_next = PRCS_START_ID;

prcs_info_t *prcs_info;
static int *prcsHash;

static void prcs_main(void);


int a653_prcs_init(void);

/********************************************************************************************************************************/

/*
 * initialize of all a653 prozess data for this partition
 */

int a653_prcs_init(void){

  int ret_val = 0;

  prcs_info = (prcs_info_t *) malloc (MAX_PRCS * sizeof(prcs_info_t));
  memset(prcs_info, 0, MAX_PRCS * sizeof(prcs_info_t));
  prcsHash = (int *) malloc(sizeof(int) *(PRCS_START_ID + MAX_PRCS));
  
  if ((prcs_info != NULL) &&
      (prcsHash != NULL)){
    number_of_processes = 0;
    prcs_id_next = PRCS_START_ID;    
  } else {
    ret_val = 1;
  }
 
  return ret_val;
}

int  a653_sync_prcs(void){
  int idx   = 0;
#ifdef S_TRACE
  int ltIdx = 0;
#endif
  pthread_t pt_self = pthread_self();

  for (idx = 0; idx < number_of_processes; idx++) {
    
    if (prcs_info[idx].t_ctx == pt_self){
      //     printDebug(1,"%s lock prcs %d\n",__func__,idx);
#ifdef S_TRACE
      if (shm_ptr->trace_info.tIdx < MAX_TRACE_ENTRIES - 1){
	ltIdx = shm_ptr->trace_info.tIdx++;
      } else {
	ltIdx = shm_ptr->trace_info.tIdx;
	shm_ptr->trace_info.tIdx = 0;
      }
      shm_ptr->trace_info.entry[ltIdx].time = getTime();
      shm_ptr->trace_info.entry[ltIdx].code = T_STOP;
      shm_ptr->trace_info.entry[ltIdx].pid = own_partition_idx;
      shm_ptr->trace_info.entry[ltIdx].tid = idx;
#endif
      pthread_mutex_lock(&prcs_info[idx].t_lock);
      break;
    }
  }
  return (idx);
}

void a653_act_prcs(void){
 
  int idx   = 0;
#ifdef S_TRACE
  int ltIdx = 0;
#endif
  int64_t diff;
  struct timespec t1 = getTime();
  
  for (idx = 0; idx < number_of_processes; idx++) {
    /* periodic process ?? */
    if (prcs_info[idx].timerPeriod != -1){
      
      diff = my_time_diff(&prcs_info[idx].nextActivation,&t1);
    
      if (diff < time_slice) {
	prcs_info[idx].nextActivation = getTime();
	my_time_next(&prcs_info[idx].nextActivation,prcs_info[idx].timerPeriod);
	//	printDebug(1,"%s unlock prcs %d\n",__func__,idx);
#ifdef S_TRACE
	if (shm_ptr->trace_info.tIdx < MAX_TRACE_ENTRIES - 1){
	  ltIdx = shm_ptr->trace_info.tIdx++;
	} else {
	  ltIdx = shm_ptr->trace_info.tIdx;
	  shm_ptr->trace_info.tIdx = 0;
	}
	shm_ptr->trace_info.entry[ltIdx].time = getTime();
	shm_ptr->trace_info.entry[ltIdx].code = T_START;
	shm_ptr->trace_info.entry[ltIdx].pid = own_partition_idx;
	shm_ptr->trace_info.entry[ltIdx].tid = idx;
#endif     
	if (pthread_mutex_unlock(&prcs_info[idx].t_lock) != 0){
	  printDebug(1,"%s unlock prcs error %d\n",__func__,idx);
	}
      }
    }
  }
}

static void prcs_main(void){
  int idx   = 0;
  
  pthread_t pt_self = pthread_self();

  for (idx = 0; idx < number_of_processes; idx++) {
    
    if (prcs_info[idx].t_ctx == pt_self){
      
      pthread_mutex_lock(&prcs_info[idx].t_lock);
      my_time_next(&prcs_info[idx].nextActivation,prcs_info[idx].timerPeriod);
      /* call custom  process function now */
      printDebug(1,"%s call custom process function %s now %d\n",__func__, prcs_info[idx].name,idx);
      (*(prcs_info[idx].prcs_main_func))();
      break;
    }
  }
}


void GET_PROCESS_ID (PROCESS_NAME_TYPE PROCESS_NAME,
                     PROCESS_ID_TYPE  *PROCESS_ID,
                     RETURN_CODE_TYPE *RETURN_CODE){

  int idx   = 0;
  int found = 0;

  *RETURN_CODE = INVALID_CONFIG;

  while ((!found) && (prcs_info[idx].id != 0)) {
    if ((strncmp(prcs_info[idx].name,PROCESS_NAME,25)) == 0) {   
 
      found = 1;

      /* set return values */
      *PROCESS_ID  = prcs_info[idx].id;
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
      *PROCESS_ID  = prcs_info[idx].id;
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

  while ((number_of_processes < MAX_PRCS && idx < MAX_PRCS)) {
    if (prcs_info[idx].id == 0) {
      number_of_processes++;
      prcs_info[idx].id = prcs_id_next++;
      strncpy(prcs_info[idx].name,ATTRIBUTES->NAME,25);
      
      prcs_info[idx].timerPeriod = (ATTRIBUTES->PERIOD); /* in nsec */
      
      prcs_info[idx].priority = (ATTRIBUTES->BASE_PRIORITY);
      //      prcs_info[idx].timerCnt    = 0;

      prcsHash[prcs_info[idx].id] = idx;
      prcs_info[idx].prcs_main_func = ATTRIBUTES->ENTRY_POINT;

                     
      /* set return values */
      *PROCESS_ID  = prcs_info[idx].id;
      *RETURN_CODE = NO_ERROR;

      printDebug(3,"create process: partition index: %d name: %s id: %d index: %d\n",
		 own_partition_idx,
		 prcs_info[idx].name,
		 prcs_info[idx].id,
		 idx);

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
    pthread_setschedprio(prcs_info[idx].t_ctx, prcs_info[idx].priority);
  } else {
    /* aperiodic process */
    printDebug(1,"start aperiodic process : partition %d : %s\n",
	       own_partition_idx,
	       prcs_info[idx].name);

    pthread_create(&prcs_info[idx].t_ctx,
		   &prcs_info[idx].t_attr, 
		   (__start_routine) prcs_info[idx].prcs_main_func,
		   NULL); /* no arg*/
    pthread_setschedprio(prcs_info[idx].t_ctx, prcs_info[idx].priority);
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

void INITIALIZE_PROCESS_CORE_AFFINITY (
       /*in */ PROCESS_ID_TYPE          PROCESS_ID,
       /*in */ PROCESSOR_CORE_ID_TYPE   PROCESSOR_CORE_ID,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void GET_MY_PROCESSOR_CORE_ID (
       /*out*/ PROCESSOR_CORE_ID_TYPE   *PROCESSOR_CORE_ID,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void GET_MY_INDEX (
       /*out*/ PROCESS_INDEX_TYPE       *PROCESS_INDEX,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}


