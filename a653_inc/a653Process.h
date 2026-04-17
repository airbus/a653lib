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
 * @file      a653Process.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     a653 process
 * @details
 */

#ifndef A653_PROCESS_H
#define A653_PROCESS_H

#include "a653Type.h"

/* defines */


typedef NAME_TYPE        PROCESS_NAME_TYPE;  /* process name type  */
typedef A653_INTEGER     PROCESS_INDEX_TYPE;
typedef A653_INTEGER     PRIORITY_TYPE;      /* priority type      */

/* process state type */
typedef enum PROCESS_STATE_VALUE_TYPE {
  DORMANT = 0,
  READY   = 1,
  RUNNING = 2,
  WAITING = 3
} PROCESS_STATE_TYPE;

/* deadline response type   */
typedef enum DEADLINE_VALUE_TYPE {
  SOFT   = 0,
  HARD   = 1 
} DEADLINE_TYPE;

/* process attribute type    */
typedef struct {
  SYSTEM_TIME_TYPE      PERIOD;               /* process period            */
  SYSTEM_TIME_TYPE      TIME_CAPACITY;        /* time capacity             */
  SYSTEM_ADDRESS_TYPE   ENTRY_POINT;          /* starting address          */
  STACK_SIZE_TYPE       STACK_SIZE;           /* stack size                */
  PRIORITY_TYPE         BASE_PRIORITY;        /* process base priority     */
  DEADLINE_TYPE         DEADLINE;             /* deadline type (soft/hard) */
  PROCESS_NAME_TYPE     NAME;                 /* process name              */
} PROCESS_ATTRIBUTE_TYPE;

/* process status type      */
typedef struct {
  SYSTEM_TIME_TYPE       DEADLINE_TIME;       /* time to next deadline    */
  PRIORITY_TYPE          CURRENT_PRIORITY;    /* process current priority */
  PROCESS_STATE_TYPE     PROCESS_STATE;       /* process state            */
  PROCESS_ATTRIBUTE_TYPE ATTRIBUTES;          /* process attributes       */
} PROCESS_STATUS_TYPE;


/* function declarations */

int a653_prcs_init(void);

int a653_sync_prcs(void);


extern void GET_PROCESS_ID (PROCESS_NAME_TYPE PROCESS_NAME,
                            PROCESS_ID_TYPE * PROCESS_ID,
                            RETURN_CODE_TYPE * RETURN_CODE);
extern void GET_MY_ID (PROCESS_ID_TYPE * PROCESS_ID,
		       RETURN_CODE_TYPE * RETURN_CODE);
extern void GET_PROCESS_STATUS (PROCESS_ID_TYPE PROCESS_ID,
				PROCESS_STATUS_TYPE * PROCESS_STATUS,
				RETURN_CODE_TYPE * RETURN_CODE);
extern void CREATE_PROCESS (PROCESS_ATTRIBUTE_TYPE * ATTRIBUTES,
			    PROCESS_ID_TYPE * PROCESS_ID,
			    RETURN_CODE_TYPE * RETURN_CODE);
extern void SET_PRIORITY (PROCESS_ID_TYPE PROCESS_ID,
			  PRIORITY_TYPE PRIORITY,
			  RETURN_CODE_TYPE * RETURN_CODE);
extern void SUSPEND_SELF (SYSTEM_TIME_TYPE TIME_OUT,
                          RETURN_CODE_TYPE * RETURN_CODE);
extern void SUSPEND (PROCESS_ID_TYPE PROCESS_ID,
		     RETURN_CODE_TYPE * RETURN_CODE);
extern void RESUME (PROCESS_ID_TYPE PROCESS_ID,
		    RETURN_CODE_TYPE * RETURN_CODE);
extern void STOP_SELF  (void);
extern void STOP (PROCESS_ID_TYPE PROCESS_ID,
		  RETURN_CODE_TYPE * RETURN_CODE);
extern void START (PROCESS_ID_TYPE PROCESS_ID,
		   RETURN_CODE_TYPE * RETURN_CODE);
extern void DELAYED_START (PROCESS_ID_TYPE PROCESS_ID,
		           SYSTEM_TIME_TYPE  DELAY_TIME,
		           RETURN_CODE_TYPE * RETURN_CODE);
extern void LOCK_PREEMPTION (LOCK_LEVEL_TYPE * LOCK_LEVEL,
			     RETURN_CODE_TYPE * RETURN_CODE);
extern void UNLOCK_PREEMPTION (LOCK_LEVEL_TYPE * LOCK_LEVEL,
			       RETURN_CODE_TYPE * RETURN_CODE);
extern void a653TimeMonitorProcGet (PROCESS_ID_TYPE PROCESS_ID,
                                    SYSTEM_TIME_TYPE * procTime,
                                    SYSTEM_TIME_TYPE *  refTime,
                                    SYSTEM_TIME_TYPE *  DLMinTime,
                                    SYSTEM_TIME_TYPE *  DLMaxTime,
                                    RETURN_CODE_TYPE *  RETURN_CODE);
extern PROCESS_ID_TYPE procIdFromTaskIdGet (int taskId);
extern int taskIdFromProcIdGet (PROCESS_ID_TYPE procId);

#endif /* A653_PROCESS_H */
