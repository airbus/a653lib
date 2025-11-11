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
 * @file      a653Sampling.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     a653 sampling port
 * @details
 */

#ifndef A653_SAMPLING_H
#define A653_SAMPLING_H

#include "a653Type.h"
#include "a653Error.h"

/* constant definitions */ 

#define SAMPLING_PORT_ID_MAX 5000

/* type definitions */ 

typedef NAME_TYPE SAMPLING_PORT_NAME_TYPE; /* port name type */ 

typedef A653_INTEGER SAMPLING_PORT_ID_TYPE; /* sampling port ident type */ 

typedef  enum { INVALID = 0, VALID = 1 } VALIDITY_TYPE;

/* sampling port status type */
typedef
   struct {
      SYSTEM_TIME_TYPE         REFRESH_PERIOD;    /* port size */
      MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE;  /* refresh period */
      PORT_DIRECTION_TYPE      PORT_DIRECTION;    /* port direction */
      VALIDITY_TYPE            LAST_MSG_VALIDITY; /* message validity */
   } SAMPLING_PORT_STATUS_TYPE;


typedef  
   enum { 
      EMPTY_PORT       = 0, 
      CONSUMED_MESSAGE = 1, 
      NEW_MESSAGE      = 2 
   } UPDATED_TYPE;

typedef enum { STALE = 0, FRESH = 1 } AGE_TYPE; 

/* sampling port status type */
typedef
   struct { 
      SYSTEM_TIME_TYPE 		REFRESH_PERIOD;   /* refresh period */
      SYSTEM_TIME_TYPE 		TIME_STAMP;       /* when message was written */
      MESSAGE_SIZE_TYPE 	MAX_MESSAGE_SIZE; /* port max byte size */
      PORT_DIRECTION_TYPE 	PORT_DIRECTION;   /* port direction */
      AGE_TYPE 			MESSAGE_AGE;      /* message age - is msg is older than ports refresh period */
	UPDATED_TYPE		UPDATED;          /* empty, consumed, or new message */
   } SAMPLING_PORT_CURRENT_STATUS_TYPE;

#ifndef __wasm__ /* Do not expose non ARINC653 functions into WebAssembly */

/*-----------------------------------------------*/
/*  sampling port access function pointer types  */
/*-----------------------------------------------*/

typedef void (* SAMPLING_PORT_CURRENT_STATUS_FUNCPTR)
(void *const pPortId, void *const pStatus, RETURN_CODE_TYPE * pRetCode);

typedef void (* SAMPLING_PORT_STATUS_FUNCPTR)
(void *const pPortId, void *const pStatus, RETURN_CODE_TYPE *pRetCode);

typedef void (* SAMPLING_PORT_SEND_FUNCPTR)
(void *const pPortId, const char *const msg, MESSAGE_SIZE_TYPE msgSize,
 RETURN_CODE_TYPE * pRetCode);

typedef void (* SAMPLING_PORT_RECEIVE_FUNCPTR)
(void *const pPortId, char *const msg, MESSAGE_SIZE_TYPE *const pMsgSize,
 VALIDITY_TYPE *palidity, RETURN_CODE_TYPE *pRetCode);

/*------------------------------------------*/
/*  sampling port access function pointers  */
/*------------------------------------------*/

typedef struct sample_port_funcs_s {
  SAMPLING_PORT_STATUS_FUNCPTR                funcStatus;
  SAMPLING_PORT_CURRENT_STATUS_FUNCPTR        funcCurrentStatus;
  SAMPLING_PORT_SEND_FUNCPTR                  funcSend;
  SAMPLING_PORT_RECEIVE_FUNCPTR               funcReceive;
} SAMPLING_PORT_FUNCS_TYPE;


/* function declarations */

void INIT_SAMPLING_PORT (RETURN_CODE_TYPE        *RETURN_CODE);
#endif /* #ifndef __wasm__ */

WASM_IMPORT_MODULE("arinc653")
extern void CREATE_SAMPLING_PORT (
       /*in */ SAMPLING_PORT_NAME_TYPE    SAMPLING_PORT_NAME,
       /*in */ MESSAGE_SIZE_TYPE          MAX_MESSAGE_SIZE,
       /*in */ PORT_DIRECTION_TYPE        PORT_DIRECTION,
       /*in */ SYSTEM_TIME_TYPE           REFRESH_PERIOD,
       /*out*/ SAMPLING_PORT_ID_TYPE      *SAMPLING_PORT_ID,
       /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void WRITE_SAMPLING_MESSAGE (
       /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
       /*in */ MESSAGE_ADDR_TYPE          MESSAGE_ADDR,     /* by reference */
       /*in */ MESSAGE_SIZE_TYPE          LENGTH,
       /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void READ_SAMPLING_MESSAGE (
       /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
       /*in */ MESSAGE_ADDR_TYPE          MESSAGE_ADDR,
               /* The message address is passed IN, although */
               /* the respective message is passed OUT       */
       /*out*/ MESSAGE_SIZE_TYPE          *LENGTH,
       /*out*/ VALIDITY_TYPE              *VALIDITY,
       /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void GET_SAMPLING_PORT_ID (
       /*in */ SAMPLING_PORT_NAME_TYPE    SAMPLING_PORT_NAME,
       /*out*/ SAMPLING_PORT_ID_TYPE      *SAMPLING_PORT_ID,
       /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void GET_SAMPLING_PORT_STATUS (
       /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
       /*out*/ SAMPLING_PORT_STATUS_TYPE  *SAMPLING_PORT_STATUS,
       /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void READ_UPDATED_SAMPLING_MESSAGE (
       /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
       /*in */ MESSAGE_ADDR_TYPE          MESSAGE_ADDR,
               /* the message address is passed IN, */
               /* although the respective message   */
               /* is passed OUT                     */
       /*out*/ MESSAGE_SIZE_TYPE          *LENGTH,
       /*out*/ UPDATED_TYPE               *UPDATED,
       /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void GET_SAMPLING_PORT_CURRENT_STATUS (
       /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
       /*out*/ SAMPLING_PORT_CURRENT_STATUS_TYPE
                                          *SAMPLING_PORT_CURRENT_STATUS,
       /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void READ_SAMPLING_MESSAGE_CONDITIONAL (
       /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
       /*in */ SYSTEM_TIME_TYPE           REF_TIME_STAMP,
       /*in */ MESSAGE_ADDR_TYPE          MESSAGE_ADDR,
               /* the message address is passed IN, */
               /* although the respective message   */
               /* is passed OUT                     */
       /*out*/ MESSAGE_SIZE_TYPE          *LENGTH,
       /*out*/ SYSTEM_TIME_TYPE           *TIME_STAMP,
       /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );


#ifndef __wasm__ /* Do not expose non ARINC653 functions into WebAssembly */
void create_sampling_port_pp (SAMPLING_PORT_NAME_TYPE   SAMPLING_PORT_NAME, 
			      MESSAGE_SIZE_TYPE         MAX_MESSAGE_SIZE, 
			      PORT_DIRECTION_TYPE       PORT_DIRECTION, 
			      SYSTEM_TIME_TYPE          REFRESH_PERIOD, 
			      SAMPLING_PORT_ID_TYPE   * SAMPLING_PORT_ID, 
			      RETURN_CODE_TYPE        * RETURN_CODE);

void write_sampling_message_pp(SAMPLING_PORT_ID_TYPE   SAMPLING_PORT_ID, 
			       MESSAGE_ADDR_TYPE       MESSAGE_ADDR,
			       MESSAGE_SIZE_TYPE       LENGTH, 
			       RETURN_CODE_TYPE      * RETURN_CODE);

void read_sampling_message_pp(SAMPLING_PORT_ID_TYPE       SAMPLING_PORT_ID, 
			      MESSAGE_ADDR_TYPE        MESSAGE_ADDR, 
			      MESSAGE_SIZE_TYPE      * LENGTH, 
			      VALIDITY_TYPE          * VALIDITY, 
			      RETURN_CODE_TYPE       * RETURN_CODE);
#endif /* #ifndef __wasm__ */

#endif /* A653_SAMPLING_H */
