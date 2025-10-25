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
 * @file      a653Queuing.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     a653 queuing port
 * @details
 */

#ifndef A653_QUEUING_H
#define A653_QUEUING_H

#include "a653Type.h"

#define QUEUING_PORT_ID_MAX 5000

/* type definitions */

typedef A653_INTEGER QUEUING_PORT_ID_TYPE;   /* queuing port ident type */
typedef NAME_TYPE    QUEUING_PORT_NAME_TYPE;  /* name type */

/* queuing port status type */
typedef
   struct {
      MESSAGE_RANGE_TYPE      NB_MESSAGE;        /* number of messages */
      MESSAGE_RANGE_TYPE      MAX_NB_MESSAGE;    /* maximum number of messages */ 
      MESSAGE_SIZE_TYPE       MAX_MESSAGE_SIZE;  /* max message size */ 
      PORT_DIRECTION_TYPE     PORT_DIRECTION;    /* port direction */
      WAITING_RANGE_TYPE      WAITING_PROCESSES; /* max number of processes */
   } QUEUING_PORT_STATUS_TYPE;

/* Below is not even used anywhere */
#ifndef __wasm__ /* Do not expose non ARINC653 functions into WebAssembly */
/*-----------------------------------------------*/
/*  queuing port access function pointer types   */
/*-----------------------------------------------*/

typedef void (* Q_PORT_STATUS_FUNCPTR) (void *const pPortId,
                                        void *const pStatus,
                                        RETURN_CODE_TYPE *pRetCode);
typedef void (* Q_PORT_SEND_FUNCPTR) (void *const pPortId,
                                      const char *const msg,
                                      MESSAGE_SIZE_TYPE msgSize,
                                      SYSTEM_TIME_TYPE timeout,
                                      RETURN_CODE_TYPE * pRetCode);
typedef void (* Q_PORT_RECEIVE_FUNCPTR) (void *const pPortId,
                                         SYSTEM_TIME_TYPE timeout,
                                         char *const msg,
                                         MESSAGE_SIZE_TYPE *const pMsgSize,
                                         RETURN_CODE_TYPE *pRetCode);

/*------------------------------------------*/
/*  queuing port access function pointers   */
/*------------------------------------------*/

typedef struct q_port_funcs_t {
  Q_PORT_STATUS_FUNCPTR       funcStatus;
  Q_PORT_SEND_FUNCPTR         funcSend;
  Q_PORT_RECEIVE_FUNCPTR      funcReceive;
} Q_PORT_FUNCS_TYPE;

/* function declarations */
#endif /* #ifndef __wasm__ */

WASM_IMPORT_MODULE("arinc653")
extern void CREATE_QUEUING_PORT (
       /*in */ QUEUING_PORT_NAME_TYPE   QUEUING_PORT_NAME,
       /*in */ MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE,
       /*in */ MESSAGE_RANGE_TYPE       MAX_NB_MESSAGE,
       /*in */ PORT_DIRECTION_TYPE      PORT_DIRECTION,
       /*in */ QUEUING_DISCIPLINE_TYPE  QUEUING_DISCIPLINE,
       /*out*/ QUEUING_PORT_ID_TYPE     *QUEUING_PORT_ID,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void SEND_QUEUING_MESSAGE (
       /*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
       /*in */ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,       /* by reference */
       /*in */ MESSAGE_SIZE_TYPE        LENGTH,
       /*in */ SYSTEM_TIME_TYPE         TIME_OUT,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void RECEIVE_QUEUING_MESSAGE (
       /*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
       /*in */ SYSTEM_TIME_TYPE         TIME_OUT,
       /*in */ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
               /* The message address is passed IN, although */
               /* the respective message is passed OUT       */
       /*out*/ MESSAGE_SIZE_TYPE        *LENGTH,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void GET_QUEUING_PORT_ID (
       /*in */ QUEUING_PORT_NAME_TYPE   QUEUING_PORT_NAME,
       /*out*/ QUEUING_PORT_ID_TYPE     *QUEUING_PORT_ID,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void GET_QUEUING_PORT_STATUS (
       /*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
       /*out*/ QUEUING_PORT_STATUS_TYPE *QUEUING_PORT_STATUS,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void CLEAR_QUEUING_PORT (
       /*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );


#ifndef __wasm__ /* Do not expose non ARINC653 functions into WebAssembly */
void create_queuing_port_pp (QUEUING_PORT_NAME_TYPE  QUEUING_PORT_NAME,
			     MESSAGE_SIZE_TYPE       MAX_MESSAGE_SIZE, 
			     MESSAGE_RANGE_TYPE      MAX_NB_MESSAGE,
			     PORT_DIRECTION_TYPE     PORT_DIRECTION,
			     QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE,
			     QUEUING_PORT_ID_TYPE   *QUEUING_PORT_ID,
			     RETURN_CODE_TYPE       *RETURN_CODE);

void send_queuing_message_pp (QUEUING_PORT_ID_TYPE   QUEUING_PORT_ID,
			      MESSAGE_ADDR_TYPE      MESSAGE_ADDR,
			      MESSAGE_SIZE_TYPE      LENGTH,
			      SYSTEM_TIME_TYPE       TIME_OUT,
			      RETURN_CODE_TYPE     * RETURN_CODE);

void receive_queuing_message_pp (QUEUING_PORT_ID_TYPE QUEUING_PORT_ID,
				 SYSTEM_TIME_TYPE     TIME_OUT,
				 MESSAGE_ADDR_TYPE    MESSAGE_ADDR,
				 MESSAGE_SIZE_TYPE  * LENGTH,
				 RETURN_CODE_TYPE   * RETURN_CODE);
#endif /* #ifndef __wasm__ */

#endif /* A653_QUEUING_H */
