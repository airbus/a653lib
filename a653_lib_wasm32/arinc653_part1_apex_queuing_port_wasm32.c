// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: QUEUING PORT

#include "arinc653_part1_apex_queuing_port_wasm32.h"
#include "../a653_inc/a653Queuing.h"

/*
 *  QUEUING_PORT_NAME_TYPE  QUEUING_PORT_NAME,
 *  MESSAGE_SIZE_TYPE       MAX_MESSAGE_SIZE,
 *  MESSAGE_RANGE_TYPE      MAX_NB_MESSAGE,
 *  PORT_DIRECTION_TYPE     PORT_DIRECTION,
 *  QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE,
 *  QUEUING_PORT_ID_TYPE  * QUEUING_PORT_ID,
 *  RETURN_CODE_TYPE      * RETURN_CODE
 */
const char* WASM32_SIGNATURE__CREATE_QUEUING_PORT = "(iiiiiii)";
wasm_trap_t* WASM32_CREATE_QUEUING_PORT(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  CREATE_QUEUING_PORT(
    (char*)&wasm_baseaddr[args[0].of.i32],
    args[1].of.i32,
    args[2].of.i32,
    args[3].of.i32,
    args[4].of.i32,
    (QUEUING_PORT_ID_TYPE*)&wasm_baseaddr[args[5].of.i32],
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[6].of.i32]
  );
  return NULL;
}


/*
 *  QUEUING_PORT_ID_TYPE   QUEUING_PORT_ID,
 *  MESSAGE_ADDR_TYPE      MESSAGE_ADDR,
 *  MESSAGE_SIZE_TYPE      LENGTH,
 *  SYSTEM_TIME_TYPE       TIME_OUT,
 *  RETURN_CODE_TYPE     * RETURN_CODE
 */
const char* WASM32_SIGNATURE__SEND_QUEUING_MESSAGE = "(iiiIi)";
wasm_trap_t* WASM32_SEND_QUEUING_MESSAGE(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  SEND_QUEUING_MESSAGE(
    (QUEUING_PORT_ID_TYPE)args[0].of.i32,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[args[1].of.i32],
    (MESSAGE_SIZE_TYPE)args[2].of.i32,
    (SYSTEM_TIME_TYPE)args[3].of.i64,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[4].of.i32]
  );
  return NULL;
}


/*
 * void RECEIVE_QUEUING_MESSAGE (QUEUING_PORT_ID_TYPE QUEUING_PORT_ID,
 *			      SYSTEM_TIME_TYPE     TIME_OUT,
 *			      MESSAGE_ADDR_TYPE    MESSAGE_ADDR,
 *			      MESSAGE_SIZE_TYPE  * LENGTH,
 *			      RETURN_CODE_TYPE   * RETURN_CODE);
 */
const char* WASM32_SIGNATURE__RECEIVE_QUEUING_MESSAGE = "(iIiii)";
wasm_trap_t* WASM32_RECEIVE_QUEUING_MESSAGE(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  RECEIVE_QUEUING_MESSAGE(
    (QUEUING_PORT_ID_TYPE)args[0].of.i32,
    (SYSTEM_TIME_TYPE)args[1].of.i64,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[args[2].of.i32],
    (MESSAGE_SIZE_TYPE*)&wasm_baseaddr[args[3].of.i32],
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[4].of.i32]
  );
  return NULL;
}


/*
 *  QUEUING_PORT_NAME_TYPE   QUEUING_PORT_NAME,
 *  QUEUING_PORT_ID_TYPE   * QUEUING_PORT_ID,
 *  RETURN_CODE_TYPE       * RETURN_CODE
 */
const char* WASM32_SIGNATURE__GET_QUEUING_PORT_ID = "(iii)";
wasm_trap_t* WASM32_GET_QUEUING_PORT_ID(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  GET_QUEUING_PORT_ID(
    (char*)&wasm_baseaddr[args[0].of.i32],
    (QUEUING_PORT_ID_TYPE*)&wasm_baseaddr[args[1].of.i32],
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[2].of.i32]
  );
  return NULL;
}


#if 0
extern void GET_QUEUING_PORT_STATUS (QUEUING_PORT_ID_TYPE       QUEUING_PORT_ID,
                                     QUEUING_PORT_STATUS_TYPE * QUEUING_PORT_STATUS,
                                     RETURN_CODE_TYPE         * RETURN_CODE);

typedef struct {
  MESSAGE_RANGE_TYPE     NB_MESSAGE;        /* number of messages */			A653_INTEGER
  MESSAGE_RANGE_TYPE     MAX_NB_MESSAGE;    /* maximum number of messages */ 	A653_INTEGER
  MESSAGE_SIZE_TYPE      MAX_MESSAGE_SIZE;  /* max message size */ 				A653_INTEGER
  PORT_DIRECTION_TYPE    PORT_DIRECTION;    /* port direction */				ENUM
  WAITING_RANGE_TYPE     WAITING_PROCESSES; /* max number of processes */		A653_INTEGER
} QUEUING_PORT_STATUS_TYPE;
#endif
const char* WASM32_SIGNATURE__GET_QUEUING_PORT_STATUS = "(iii)";
wasm_trap_t* WASM32_GET_QUEUING_PORT_STATUS(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  GET_QUEUING_PORT_STATUS(
    (QUEUING_PORT_ID_TYPE)args[0].of.i32,
    (QUEUING_PORT_STATUS_TYPE*)&wasm_baseaddr[args[1].of.i32],
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[2].of.i32]
  );
  return NULL;
}


#if 0
extern void CLEAR_QUEUING_PORT (QUEUING_PORT_ID_TYPE QUEUING_PORT_ID, /* in */
                                RETURN_CODE_TYPE *RETURN_CODE ); /* out */
#endif
const char* WASM32_SIGNATURE__CLEAR_QUEUING_PORT = "(ii)";
wasm_trap_t* WASM32_CLEAR_QUEUING_PORT(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  CLEAR_QUEUING_PORT(
    (QUEUING_PORT_ID_TYPE)args[0].of.i32,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );
  return NULL;
}
