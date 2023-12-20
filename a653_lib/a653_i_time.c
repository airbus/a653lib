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
 * @date      Tue Oct 24 15:53:18 CEST 2023
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
#include "a653Error.h"
#include "a653Process.h"


void TIMED_WAIT (SYSTEM_TIME_TYPE DELAY_TIME, 
                 RETURN_CODE_TYPE * RETURN_CODE){
  usleep(40000);
}

void PERIODIC_WAIT (RETURN_CODE_TYPE * RETURN_CODE){

  a653_sync_prcs();

}

void GET_TIME (SYSTEM_TIME_TYPE * SYSTEM_TIME,
               RETURN_CODE_TYPE * RETURN_CODE){
}

void REPLENISH (SYSTEM_TIME_TYPE   BUDGET_TIME, 
                RETURN_CODE_TYPE * RETURN_CODE){
}


