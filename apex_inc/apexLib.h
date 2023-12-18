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
