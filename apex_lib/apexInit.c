/**
 * @file      apexInit.c
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     do apex interface initialization
 * @details 
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
//--------------------
#include "apex_i_time_lib.h"

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
  static char buf[256];

  if (level < debugLevel){

    ts = getTime(); 
    
    printf("pid: %d <%lu.%08lu>: ",getpid(),ts.tv_sec,ts.tv_nsec);
    
    va_start( arglist, format );
    vprintf(format,arglist);
    va_end (arglist);
  }
}
