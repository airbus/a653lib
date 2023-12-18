/**
 * @file      apexLib.h
 * @copyright Airbus Defence and Space
 * @author    Nicolaus Baer
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     do apex interface initialization
 * @details 
 */

#ifndef __APEX_LIB_H
#define __APEX_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//--------------------
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#include <sys/shm.h>

/* apex includes */
#include <apexType.h>
  //#include <apexBlackboard.h>
  //#include <apexMemoryBlock.h>
  //#include <apexBuffer.h>
#include <apexError.h>
  //#include <apexEvent.h>
#include <apexPartition.h>
#include <apexProcess.h>
#include <apexQueuing.h>
#include <apexSampling.h>
//#include <apexSemaphore.h>
#include <apexTime.h>


/* defines */

/* externs */

/* function declarations */

extern int apexLibInit ();
extern int apexMinimalLibInit ();


#endif /* __APEX_LIB_H */
