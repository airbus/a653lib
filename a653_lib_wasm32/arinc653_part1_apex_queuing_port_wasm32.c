// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: QUEUING PORT

#include <endian.h>
#include "arinc653_part1_apex_queuing_port_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_inc/a653Queuing.h"


#if 0
extern void CREATE_QUEUING_PORT (
  /*in */ QUEUING_PORT_NAME_TYPE   QUEUING_PORT_NAME,
  /*in */ MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE,
  /*in */ MESSAGE_RANGE_TYPE       MAX_NB_MESSAGE,
  /*in */ PORT_DIRECTION_TYPE      PORT_DIRECTION,
  /*in */ QUEUING_DISCIPLINE_TYPE  QUEUING_DISCIPLINE,
  /*out*/ QUEUING_PORT_ID_TYPE     *QUEUING_PORT_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );
#endif
wasm_trap_t* WASM32_CREATE_QUEUING_PORT(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  MESSAGE_SIZE_TYPE MAX_MESSAGE_SIZE;
  MAX_MESSAGE_SIZE = (MESSAGE_SIZE_TYPE)le32toh(args_and_results[1].i32);
  MESSAGE_RANGE_TYPE MAX_NB_MESSAGE;
  MAX_NB_MESSAGE = (MESSAGE_RANGE_TYPE)le32toh(args_and_results[2].i32);
  PORT_DIRECTION_TYPE PORT_DIRECTION;
  PORT_DIRECTION = (PORT_DIRECTION_TYPE)le32toh(args_and_results[3].i32);
  QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE;
  QUEUING_DISCIPLINE = (QUEUING_DISCIPLINE_TYPE)le32toh(args_and_results[4].i32);
  QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  CREATE_QUEUING_PORT(
    (char*)&wasm_baseaddr[le32toh(args_and_results[0].i32)], // FIXME: only safe as long as char[]
    MAX_MESSAGE_SIZE,
    MAX_NB_MESSAGE,
    PORT_DIRECTION,
    QUEUING_DISCIPLINE,
    &QUEUING_PORT_ID,
    &RETURN_CODE
  );

  camw32_set__QUEUING_PORT_ID_TYPE(&wasm_baseaddr[le32toh(args_and_results[5].i32)], (int32_t)QUEUING_PORT_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[6].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void SEND_QUEUING_MESSAGE (
  /*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
  /*in */ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,       /* by reference */
  /*in */ MESSAGE_SIZE_TYPE        LENGTH,
  /*in */ SYSTEM_TIME_TYPE         TIME_OUT,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE);
#endif
wasm_trap_t* WASM32_SEND_QUEUING_MESSAGE(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
  QUEUING_PORT_ID = (QUEUING_PORT_ID_TYPE)le32toh(args_and_results[0].i32);
  int32_t MESSAGE_ADDR; /* is a pointer / address into Wasm linear memory */
  MESSAGE_ADDR = le32toh(args_and_results[1].i32);
  MESSAGE_SIZE_TYPE LENGTH;
  LENGTH = (MESSAGE_SIZE_TYPE)le32toh(args_and_results[2].i32);
  SYSTEM_TIME_TYPE TIME_OUT;
  TIME_OUT = (SYSTEM_TIME_TYPE)le64toh(args_and_results[3].i64);
  RETURN_CODE_TYPE RETURN_CODE;

  SEND_QUEUING_MESSAGE(
    QUEUING_PORT_ID,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[MESSAGE_ADDR], // FIXME: only safe as long as char[]
    LENGTH,
    TIME_OUT,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[4].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void RECEIVE_QUEUING_MESSAGE (
  /*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
  /*in */ SYSTEM_TIME_TYPE         TIME_OUT,
  /*in */ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
    /* The message address is passed IN, although */
    /* the respective message is passed OUT       */
  /*out*/ MESSAGE_SIZE_TYPE        *LENGTH,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );
#endif
wasm_trap_t* WASM32_RECEIVE_QUEUING_MESSAGE(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
  QUEUING_PORT_ID = (QUEUING_PORT_ID_TYPE)le32toh(args_and_results[0].i32);
  SYSTEM_TIME_TYPE TIME_OUT;
  TIME_OUT = (SYSTEM_TIME_TYPE)le64toh(args_and_results[1].i64);
  int32_t MESSAGE_ADDR; /* is a pointer / address into Wasm linear memory */
  MESSAGE_ADDR = le32toh(args_and_results[2].i32);
  MESSAGE_SIZE_TYPE LENGTH;
  RETURN_CODE_TYPE RETURN_CODE;

  RECEIVE_QUEUING_MESSAGE(
    QUEUING_PORT_ID,
    TIME_OUT,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[MESSAGE_ADDR], // FIXME: only safe as long as char[]
    &LENGTH,
    &RETURN_CODE
  );

  camw32_set__MESSAGE_SIZE_TYPE(&wasm_baseaddr[le32toh(args_and_results[3].i32)], (int32_t)LENGTH);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[4].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void GET_QUEUING_PORT_ID (
  /*in */ QUEUING_PORT_NAME_TYPE   QUEUING_PORT_NAME,
  /*out*/ QUEUING_PORT_ID_TYPE     *QUEUING_PORT_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );
#endif
wasm_trap_t* WASM32_GET_QUEUING_PORT_ID(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_QUEUING_PORT_ID(
    (char*)&wasm_baseaddr[le32toh(args_and_results[0].i32)], // FIXME: only safe as long as char[]
    &QUEUING_PORT_ID,
    &RETURN_CODE
  );

  camw32_set__QUEUING_PORT_ID_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)QUEUING_PORT_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void GET_QUEUING_PORT_STATUS (
  /*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
  /*out*/ QUEUING_PORT_STATUS_TYPE *QUEUING_PORT_STATUS,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );
#endif
wasm_trap_t* WASM32_GET_QUEUING_PORT_STATUS(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
  QUEUING_PORT_ID = (QUEUING_PORT_ID_TYPE)le32toh(args_and_results[0].i32);
  QUEUING_PORT_STATUS_TYPE QUEUING_PORT_STATUS;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_QUEUING_PORT_STATUS(
    QUEUING_PORT_ID,
    &QUEUING_PORT_STATUS,
    &RETURN_CODE
  );

  uint8_t* QUEUING_PORT_STATUS_guest = (uint8_t*)&wasm_baseaddr[le32toh(args_and_results[1].i32)];
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);

#if 0
typedef struct {
  MESSAGE_RANGE_TYPE     NB_MESSAGE;        /* number of messages */			A653_INTEGER
  MESSAGE_RANGE_TYPE     MAX_NB_MESSAGE;    /* maximum number of messages */ 	A653_INTEGER
  MESSAGE_SIZE_TYPE      MAX_MESSAGE_SIZE;  /* max message size */ 				A653_INTEGER
  PORT_DIRECTION_TYPE    PORT_DIRECTION;    /* port direction */				ENUM
  WAITING_RANGE_TYPE     WAITING_PROCESSES; /* max number of processes */		A653_INTEGER
} QUEUING_PORT_STATUS_TYPE;
#endif

  camw32_set__QUEUING_PORT_STATUS_TYPE__NB_MESSAGE(QUEUING_PORT_STATUS_guest, QUEUING_PORT_STATUS.NB_MESSAGE);
  camw32_set__QUEUING_PORT_STATUS_TYPE__MAX_NB_MESSAGE(QUEUING_PORT_STATUS_guest, QUEUING_PORT_STATUS.MAX_NB_MESSAGE);
  camw32_set__QUEUING_PORT_STATUS_TYPE__MAX_MESSAGE_SIZE(QUEUING_PORT_STATUS_guest, QUEUING_PORT_STATUS.MAX_MESSAGE_SIZE);
  camw32_set__QUEUING_PORT_STATUS_TYPE__PORT_DIRECTION(QUEUING_PORT_STATUS_guest, QUEUING_PORT_STATUS.PORT_DIRECTION);
  camw32_set__QUEUING_PORT_STATUS_TYPE__WAITING_PROCESSES(QUEUING_PORT_STATUS_guest, QUEUING_PORT_STATUS.WAITING_PROCESSES);

  return NULL;
}


#if 0
extern void CLEAR_QUEUING_PORT (
  /*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );
#endif
wasm_trap_t* WASM32_CLEAR_QUEUING_PORT(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
  QUEUING_PORT_ID = (QUEUING_PORT_ID_TYPE)le32toh(args_and_results[0].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  CLEAR_QUEUING_PORT(
    QUEUING_PORT_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  return NULL;
}
