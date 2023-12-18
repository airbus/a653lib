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


