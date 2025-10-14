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
 * @file      a653_i_time.c
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Wed 11 Dec 2024 04:07:47 AM EST
 * @brief     a653 timer handling
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



#include "a653Type.h"
#include "a653Init.h"
#include "a653Error.h"
#include "a653Process.h"

SYSTEM_TIME_TYPE bootTime;

void initTime(void){
  struct timespec tp;
  
  if (clock_gettime(CLOCK_REALTIME, &tp) == 0){
      bootTime = (SYSTEM_TIME_TYPE)tp.tv_sec * (SYSTEM_TIME_TYPE)1000000000 + (SYSTEM_TIME_TYPE)tp.tv_nsec;
  }
}


void TIMED_WAIT (SYSTEM_TIME_TYPE DELAY_TIME, /* */
                 RETURN_CODE_TYPE * RETURN_CODE){

  /*  int usleep(useconds_t usec); */
  usleep(DELAY_TIME / 1000);
}

void PERIODIC_WAIT (RETURN_CODE_TYPE * RETURN_CODE){

  a653_sync_prcs();

}

void GET_TIME (SYSTEM_TIME_TYPE * SYSTEM_TIME,
               RETURN_CODE_TYPE * RETURN_CODE){

  struct timespec tp;
  
  if (0 == clock_gettime(CLOCK_REALTIME, &tp)){
   /* printDebug(5,"GET_TIME >%d:%d< \n",tp.tv_sec,tp.tv_nsec); */
    *SYSTEM_TIME = (SYSTEM_TIME_TYPE)tp.tv_sec * (SYSTEM_TIME_TYPE)1000000000 + (SYSTEM_TIME_TYPE)tp.tv_nsec - bootTime;
    *RETURN_CODE = NO_ERROR;
  } else {
    *SYSTEM_TIME = 0;
    *RETURN_CODE = NOT_AVAILABLE;
  }
}

void REPLENISH (SYSTEM_TIME_TYPE   BUDGET_TIME, 
                RETURN_CODE_TYPE * RETURN_CODE){
  printDebug(1,"not implemented yet!!!\n\n");
}


