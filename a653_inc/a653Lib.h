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
 * @file      a653Lib.h
 * @copyright Airbus Defence and Space
 * @author    Nicolaus Baer
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     do a653 interface initialization
 * @details 
 */

#ifndef __A653_LIB_H
#define __A653_LIB_H

/* a653 includes */
#include <a653Type.h>
  //#include <a653Blackboard.h>
  //#include <a653MemoryBlock.h>
  //#include <a653Buffer.h>
#include <a653Error.h>
  //#include <a653Event.h>
#include <a653Partition.h>
#include <a653Process.h>
#include <a653Queuing.h>
#include <a653Sampling.h>
#include <a653Semaphore.h>
#include <a653Time.h>

#ifndef __wasm__ /* Do not expose non ARINC653 functions into WebAssembly */
#include <a653Init.h>
#endif /* #ifndef __wasm__ */

/* defines */

/* externs */

/* function declarations */

#ifndef __wasm__ /* Do not expose non ARINC653 functions into WebAssembly */
extern int a653LibInit ();
extern int a653MinimalLibInit ();
#endif /* #ifndef __wasm__ */


#endif /* __A653_LIB_H */
