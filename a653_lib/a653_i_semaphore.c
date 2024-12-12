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
 * @file      a653_i_semaphore.c
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Wed 11 Dec 2024 08:03:18 AM EST
 * @brief     a653 semaphore
 * @details    
 */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "a653Init.h"
#include "a653Type.h"
#include "a653Error.h"
#include "a653Semaphore.h"
#include "a653Partition.h"

/*
  The CREATE_SEMAPHORE service request is used to create a semaphore of a specified current 
  and maximum value. The maximum value parameter is the maximum value that the semaphore can 
  be signaled to (e.g., number of resources managed by the semaphore). The current value is the 
  semaphores’ starting value after creation. For example, if the semaphore was used to manage 
  access to five resources, and at the time of creation three resources were available, the 
  semaphore would be created with a maximum value of five and a current value of three. The 
  QUEUING_DISCIPLINE input parameter indicates the process queuing policy (FIFO or priority 
  order) associated with the semaphore.
 */
void CREATE_SEMAPHORE
( /*in */ SEMAPHORE_NAME_TYPE SEMAPHORE_NAME,
  /*in */ SEMAPHORE_VALUE_TYPE CURRENT_VALUE,
  /*in */ SEMAPHORE_VALUE_TYPE MAXIMUM_VALUE,
  /*in */ QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE,
  /*out*/ SEMAPHORE_ID_TYPE *SEMAPHORE_ID,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
{
}

/*
 The WAIT_SEMAPHORE service request is used to attempt to acquire (i.e., decrements) one count 
 of a semaphore’s value (e.g., request for one resource managed by the semaphore). The service 
 moves the current process from the running state to the waiting state if the current value 
 of the specified semaphore is zero and if the specified time-out is not zero. If the current 
 value of the specified semaphore is positive, the process goes on executing and the 
 semaphore’s current value is decremented.
 */
void WAIT_SEMAPHORE
( /*in */ SEMAPHORE_ID_TYPE SEMAPHORE_ID,
  /*in */ SYSTEM_TIME_TYPE TIME_OUT,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
{
}
/*
  If the semaphore’s value is not equal to its maximum, the SIGNAL_SEMAPHORE service request 
  increments the current value of the specified semaphore (e.g., the current process completed 
  its usage of one resource managed by the semaphore). If processes are waiting on that 
  semaphore, the first process of the queue is moved from the waiting state to the ready state 
  and a scheduling takes place. 
*/
void SIGNAL_SEMAPHORE
( /*in */ SEMAPHORE_ID_TYPE SEMAPHORE_ID,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
{
}
/*
  The GET_SEMAPHORE_ID service request returns the semaphore identifier that corresponds to a 
  semaphore name. 
*/
void GET_SEMAPHORE_ID
( /*in */ SEMAPHORE_NAME_TYPE SEMAPHORE_NAME,
  /*out*/ SEMAPHORE_ID_TYPE *SEMAPHORE_ID,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
{
}

/*
  The GET_SEMAPHORE_STATUS service request returns the status of the specified semaphore. 
*/
void GET_SEMAPHORE_STATUS
( /*in */ SEMAPHORE_ID_TYPE SEMAPHORE_ID,
  /*out*/ SEMAPHORE_STATUS_TYPE *SEMAPHORE_STATUS,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
{
}
