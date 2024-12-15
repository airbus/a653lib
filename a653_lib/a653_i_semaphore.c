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

/*
The semaphores defined in this standard are counting semaphores, and are commonly used to
provide controlled access to partition resources. A process waits on a semaphore to gain access to
the partition resource, and then signals the semaphore when it is finished. A semaphore’s value in
this example indicates the number of currently available partition resources.

The CREATE_SEMAPHORE operation creates a semaphore for use by any of the process in the
partition. At creation, the semaphore’s initial value, maximum value, and queuing discipline are
defined.
Processes waiting on a semaphore are queued in FIFO or priority order. In the case of priority
order, for the same priority, processes are also queued in FIFO order (with the oldest waiting
process being at the front of the FIFO).

The WAIT_SEMAPHORE operation decrements the semaphore’s value if the semaphore is not
already at its minimum value of zero. If the value is already zero, the calling process may optionally
be queued until either the semaphore is signaled or until a specified (as part of the service call)
duration of real-time expires.

The SIGNAL_SEMAPHORE operation increments the semaphore’s value. If there are processes
waiting on the semaphore, the queuing discipline algorithm, FIFO or priority order, will be applied to
determine which queue process will receive the signal. Signaling a semaphore where processes
are waiting increments and decrements the semaphores value in one request. The end result is
there are still no available resources, and the semaphore value remains zero.
Rescheduling of processes will occur when a process attempts to wait on a zero value semaphore,
and when a semaphore is signaled that has processes queued on it.
When a process is removed from the queue, either by the semaphore being signaled or by the
expiration of the specified time-out period, the process will be moved into the ready state (unless
another process has suspended it).
*/

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>

#include "a653Init.h"
#include "a653Type.h"
#include "a653Error.h"
#include "a653Semaphore.h"
#include "a653Partition.h"

#define MAX_SEM_NAME_LEN 35

typedef struct {
  int next_free;
  struct {
    int max_value;
    int cur_value;
    int waiting_processes; /*number of processes waiting */
    sem_t sem;
    char name[MAX_SEM_NAME_LEN];
  } imp [MAX_SEM_NUM];
} sem_info_t;

static sem_info_t sem_info;


void init_semaphore(void){
  int index = 0;

  sem_info.next_free = 0;
  for (index = 0; index < MAX_SEM_NUM; index++){
    sem_info.imp[index].name[0] = 0;
    
  }
}

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
 int value;
  
  if (sem_info.next_free < MAX_SEM_NUM){

    sem_destroy(&sem_info.imp[sem_info.next_free].sem);
    
    if (0 == sem_init(&sem_info.imp[sem_info.next_free].sem,
		      0, /* value 0, then the semaphore is shared between the threads of a process */
		      CURRENT_VALUE)){      
      /* we got semaphore */
      sem_info.imp[sem_info.next_free].cur_value = CURRENT_VALUE;
      sem_info.imp[sem_info.next_free].max_value = MAXIMUM_VALUE;
      strncpy(sem_info.imp[sem_info.next_free].name,SEMAPHORE_NAME,MAX_SEM_NAME_LEN);
      *SEMAPHORE_ID = sem_info.next_free;
      *RETURN_CODE = NO_ERROR;
      sem_info.next_free++;
    } else {
      *RETURN_CODE = NOT_AVAILABLE;
    }
  }
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
  int value;
  
  if (SEMAPHORE_ID >= 0 || SEMAPHORE_ID < MAX_SEM_NUM){
    /*   
    sem_getvalue(&sem_info.imp[SEMAPHORE_ID].sem, &value);
    printDebug(3,"sem wait value2 %d\n",value);
    */  
    sem_wait(&sem_info.imp[SEMAPHORE_ID].sem);
    *RETURN_CODE = NO_ERROR;
  } else {
    *RETURN_CODE = NOT_AVAILABLE;
  }
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
   if (SEMAPHORE_ID >= 0 || SEMAPHORE_ID < MAX_SEM_NUM){
    sem_post(&sem_info.imp[SEMAPHORE_ID].sem);
    *RETURN_CODE = NO_ERROR;
  } else {
    *RETURN_CODE = NOT_AVAILABLE;
  } 
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
  int index = 0;
  
  *RETURN_CODE = NOT_AVAILABLE;
  
  for (index = 0; index < MAX_SEM_NUM; index++){   
    if ((strncmp(sem_info.imp[index].name,
		 SEMAPHORE_NAME,
		 MAX_SEM_NAME_LEN)) == 0) {
      *SEMAPHORE_ID = index;
      *RETURN_CODE = NO_ERROR;
    }
  }
}

/*
  The GET_SEMAPHORE_STATUS service request returns the status of the specified semaphore. 
*/
void GET_SEMAPHORE_STATUS
( /*in */ SEMAPHORE_ID_TYPE SEMAPHORE_ID,
  /*out*/ SEMAPHORE_STATUS_TYPE *SEMAPHORE_STATUS,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
{
  int value;
  
  if (SEMAPHORE_ID >= 0 || SEMAPHORE_ID < MAX_SEM_NUM){
    
    sem_getvalue(&sem_info.imp[SEMAPHORE_ID].sem, &value);
    SEMAPHORE_STATUS->CURRENT_VALUE = value;
    SEMAPHORE_STATUS->MAXIMUM_VALUE = sem_info.imp[SEMAPHORE_ID].max_value;
    *RETURN_CODE = NO_ERROR;
    
  } else {
    *RETURN_CODE = NOT_AVAILABLE;
  } 
}

