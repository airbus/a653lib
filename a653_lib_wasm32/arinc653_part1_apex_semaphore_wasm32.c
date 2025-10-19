// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: SEMAPHORE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "arinc653_part1_apex_semaphore_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_inc/a653Semaphore.h"
#include "a653_i_common_wasm32.h"


#if 0
void CREATE_SEMAPHORE
( /*in */ SEMAPHORE_NAME_TYPE SEMAPHORE_NAME,
  /*in */ SEMAPHORE_VALUE_TYPE CURRENT_VALUE,
  /*in */ SEMAPHORE_VALUE_TYPE MAXIMUM_VALUE,
  /*in */ QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE,
  /*out*/ SEMAPHORE_ID_TYPE *SEMAPHORE_ID,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
#endif
const char* WASM32_SIGNATURE__CREATE_SEMAPHORE = "(iiiiii)";
wasm_trap_t* WASM32_CREATE_SEMAPHORE(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  SEMAPHORE_VALUE_TYPE CURRENT_VALUE;
  CURRENT_VALUE = (SEMAPHORE_VALUE_TYPE)camw32_get__SEMAPHORE_VALUE_TYPE((uint8_t*)&args[1].of.i32);
  SEMAPHORE_VALUE_TYPE MAXIMUM_VALUE;
  MAXIMUM_VALUE = (SEMAPHORE_VALUE_TYPE)camw32_get__SEMAPHORE_VALUE_TYPE((uint8_t*)&args[2].of.i32);
  QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE;
  QUEUING_DISCIPLINE = (QUEUING_DISCIPLINE_TYPE)camw32_get__QUEUING_DISCIPLINE_TYPE((uint8_t*)&args[3].of.i32);
  SEMAPHORE_ID_TYPE SEMAPHORE_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  CREATE_SEMAPHORE(
    (char*)&wasm_baseaddr[args[0].of.i32], // FIXME: address args[].of.i32 could be LE/BE swapped
    CURRENT_VALUE,
    MAXIMUM_VALUE,
    QUEUING_DISCIPLINE,
    &SEMAPHORE_ID,
    &RETURN_CODE
  );

  // TODO: could still be an issue, with using the args[].of.i32 directly due to LE/BE
  camw32_set__SEMAPHORE_ID_TYPE(&wasm_baseaddr[args[4].of.i32], (int32_t)SEMAPHORE_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[args[5].of.i32], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
void WAIT_SEMAPHORE
( /*in */ SEMAPHORE_ID_TYPE SEMAPHORE_ID,
  /*in */ SYSTEM_TIME_TYPE TIME_OUT,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
#endif
const char* WASM32_SIGNATURE__WAIT_SEMAPHORE = "(iIi)";
wasm_trap_t* WASM32_WAIT_SEMAPHORE(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  SEMAPHORE_ID_TYPE SEMAPHORE_ID;
  SEMAPHORE_ID = (SEMAPHORE_ID_TYPE)camw32_get__SEMAPHORE_ID_TYPE((uint8_t*)&args[0].of.i32);
  SYSTEM_TIME_TYPE TIME_OUT;
  TIME_OUT = (SYSTEM_TIME_TYPE)camw32_get__SYSTEM_TIME_TYPE((uint8_t*)&args[1].of.i64);
  RETURN_CODE_TYPE RETURN_CODE;

  WAIT_SEMAPHORE(
    SEMAPHORE_ID,
    TIME_OUT,
    &RETURN_CODE
  );

  // TODO: could still be an issue, with using the args[].of.i32 directly due to LE/BE
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[args[2].of.i32], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
void SIGNAL_SEMAPHORE
( /*in */ SEMAPHORE_ID_TYPE SEMAPHORE_ID,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
#endif
const char* WASM32_SIGNATURE__SIGNAL_SEMAPHORE = "(ii)";
wasm_trap_t* WASM32_SIGNAL_SEMAPHORE(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);


  SEMAPHORE_ID_TYPE SEMAPHORE_ID;
  SEMAPHORE_ID = (SEMAPHORE_ID_TYPE)camw32_get__SEMAPHORE_ID_TYPE((uint8_t*)&args[0].of.i32);
  RETURN_CODE_TYPE RETURN_CODE;

  SIGNAL_SEMAPHORE(
    SEMAPHORE_ID,
    &RETURN_CODE
  );

  // TODO: could still be an issue, with using the args[].of.i32 directly due to LE/BE
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[args[1].of.i32], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
void GET_SEMAPHORE_ID
( /*in */ SEMAPHORE_NAME_TYPE SEMAPHORE_NAME,
  /*out*/ SEMAPHORE_ID_TYPE *SEMAPHORE_ID,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
#endif
const char* WASM32_SIGNATURE__GET_SEMAPHORE_ID = "(iii)";
wasm_trap_t* WASM32_GET_SEMAPHORE_ID(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);


  SEMAPHORE_ID_TYPE SEMAPHORE_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_SEMAPHORE_ID(
    (char*)&wasm_baseaddr[args[0].of.i32], // FIXME: address args[].of.i32 could be LE/BE swapped
    &SEMAPHORE_ID,
    &RETURN_CODE
  );

  // TODO: could still be an issue, with using the args[].of.i32 directly due to LE/BE
  camw32_set__SEMAPHORE_ID_TYPE(&wasm_baseaddr[args[1].of.i32], (int32_t)SEMAPHORE_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[args[2].of.i32], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
typedef struct {
  SEMAPHORE_VALUE_TYPE CURRENT_VALUE;
  SEMAPHORE_VALUE_TYPE MAXIMUM_VALUE;
  WAITING_RANGE_TYPE WAITING_PROCESSES;
} SEMAPHORE_STATUS_TYPE;
}
void GET_SEMAPHORE_STATUS
( /*in */ SEMAPHORE_ID_TYPE SEMAPHORE_ID,
  /*out*/ SEMAPHORE_STATUS_TYPE *SEMAPHORE_STATUS,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
#endif
const char* WASM32_SIGNATURE__GET_SEMAPHORE_STATUS = "(iii)";
wasm_trap_t* WASM32_GET_SEMAPHORE_STATUS(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);


  SEMAPHORE_ID_TYPE SEMAPHORE_ID;
  SEMAPHORE_ID = (SEMAPHORE_ID_TYPE)camw32_get__SEMAPHORE_ID_TYPE((uint8_t*)&args[0].of.i32);
  SEMAPHORE_STATUS_TYPE SEMAPHORE_STATUS;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_SEMAPHORE_STATUS(
    SEMAPHORE_ID,
    &SEMAPHORE_STATUS,
    &RETURN_CODE
  );

  // TODO: could still be an issue, with using the args[].of.i32 directly due to LE/BE
  uint8_t* SEMAPHORE_STATUS_guest = (uint8_t*)&wasm_baseaddr[args[1].of.i32];
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[args[2].of.i32], (int32_t)RETURN_CODE);

  camw32_set__SEMAPHORE_STATUS_TYPE__CURRENT_VALUE(SEMAPHORE_STATUS_guest, SEMAPHORE_STATUS.CURRENT_VALUE);
  camw32_set__SEMAPHORE_STATUS_TYPE__MAXIMUM_VALUE(SEMAPHORE_STATUS_guest, SEMAPHORE_STATUS.MAXIMUM_VALUE);
  camw32_set__SEMAPHORE_STATUS_TYPE__WAITING_PROCESSES(SEMAPHORE_STATUS_guest, SEMAPHORE_STATUS.WAITING_PROCESSES);

  return NULL;
}
