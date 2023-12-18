/**
 * @file      apex_i_process.c
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

#define APEX_QUEUING_INTERN

#include "apexInit.h"
#include "apexType.h"
#include "apexError.h"
#include "apexProcess.h"
#include "apexPartition.h"

#define APEX_PROCESS_INTERN

#include "apex_i_process.h"


//extern entry_point_t model_main;




int number_of_processes = 0;

static apex_process_config_t *apex_process_config;
static prcs_info_t *prcs_info; 
static int *prcsHash;

static void prcs_main(void);


int apex_prcs_init(int num_of_prcs, apex_process_config_t *config);

/********************************************************************************************************************************/

/*
 * initialize of all apex prozess data for this partition
 */

int apex_prcs_init(int num_of_prcs, apex_process_config_t *config){

  int ret_val = 0;

  prcs_info = (prcs_info_t *) malloc(sizeof(prcs_info_t) * num_of_prcs);
  prcsHash = (int *) malloc(sizeof(int) * num_of_prcs);
  
  if ((config != NULL) &&
      (prcs_info != NULL) &&
      (prcsHash != NULL)){
    
    number_of_processes = num_of_prcs;
    apex_process_config = config;
        
  } else {
    ret_val = 1;
  }
  
  return ret_val;
}

int  apex_sync_prcs(void){
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

void apex_act_prcs(void){
 
  int idx   = 0;
  
  for (idx = 0; idx < number_of_processes; idx++) {
    pthread_mutex_unlock(&prcs_info[idx].t_lock);
    break;
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

  while ((!found) && (apex_process_config[idx].PrcsId != 0)) {
    if ((strncmp(apex_process_config[idx].name_str,PROCESS_NAME,25)) == 0) {   
 
      found = 1;

      /* set return values */
      *PROCESS_ID  = apex_process_config[idx].PrcsId;
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
      *PROCESS_ID  = apex_process_config[idx].PrcsId;
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

  while (apex_process_config[idx].PrcsId != 0) {
    if ((strncmp(apex_process_config[idx].name_str,ATTRIBUTES->NAME,25)) == 0) {   
      /* valid entry found */
      
      strncpy(prcs_info[idx].name,ATTRIBUTES->NAME,25);
      
      prcs_info[idx].timerPeriod = (ATTRIBUTES->PERIOD); /* in nsec */
      //      prcs_info[idx].timerCnt    = 0;

      prcsHash[apex_process_config[idx].PrcsId] = idx;
      prcs_info[idx].prcs_main_func = ATTRIBUTES->ENTRY_POINT;

                     
      /* set return values */
      *PROCESS_ID  = apex_process_config[idx].PrcsId;
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
  
  pthread_mutex_init(&prcs_info[idx].t_lock,NULL);
  pthread_mutex_lock(&prcs_info[idx].t_lock);
  
  pthread_create(&prcs_info[idx].t_ctx,
		 NULL, /* attr NULL - default value*/
		 (__start_routine) prcs_main,
		 NULL); /* no arg*/

  /*
  prcs_info[idx].timer = ptimer_start(prcs_info[idx].timerPeriod,
				      apex_act_prcs,
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

void apexTimeMonitorProcGet (PROCESS_ID_TYPE PROCESS_ID,
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


