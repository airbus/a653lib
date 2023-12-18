/**
 * @file      apex_i_time.c
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     apex timer handling
 * @details
 */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
//--------------------
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>



#include "apexType.h"
#include "apexError.h"
#include "apexProcess.h"


void TIMED_WAIT (SYSTEM_TIME_TYPE DELAY_TIME, 
                 RETURN_CODE_TYPE * RETURN_CODE){
  usleep(40000);
}

void PERIODIC_WAIT (RETURN_CODE_TYPE * RETURN_CODE){

  apex_sync_prcs();

}

void GET_TIME (SYSTEM_TIME_TYPE * SYSTEM_TIME,
               RETURN_CODE_TYPE * RETURN_CODE){
}

void REPLENISH (SYSTEM_TIME_TYPE   BUDGET_TIME, 
                RETURN_CODE_TYPE * RETURN_CODE){
}


