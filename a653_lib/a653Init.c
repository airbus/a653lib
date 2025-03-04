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
 * @file      a653Init.c
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     do a653 interface initialization
 * @details 
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
//--------------------
#include "a653_i_time_lib.h"

/*
 *
 */
static int debugLevel = 0;


void setDebug(int level){
  debugLevel = level;
}

void printDebug(int level, const char *format, ... ){
  va_list arglist;
  struct timespec ts;

  if (level <= debugLevel){

    ts = getTime(); 
    
    //    printf("pid: %d <%lu.%09lu>: ",getpid(),ts.tv_sec,ts.tv_nsec);
    printf("%lu.%09lu:%d: ",ts.tv_sec,ts.tv_nsec,getpid());
    
    va_start( arglist, format );
    vprintf(format,arglist);
    va_end (arglist);
  }
}
