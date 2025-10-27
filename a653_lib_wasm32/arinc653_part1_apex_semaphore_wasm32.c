// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: SEMAPHORE

#include <endian.h>
#include "arinc653_part1_apex_semaphore_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_inc/a653Semaphore.h"


#if 0
void CREATE_SEMAPHORE
( /*in */ SEMAPHORE_NAME_TYPE SEMAPHORE_NAME,
  /*in */ SEMAPHORE_VALUE_TYPE CURRENT_VALUE,
  /*in */ SEMAPHORE_VALUE_TYPE MAXIMUM_VALUE,
  /*in */ QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE,
  /*out*/ SEMAPHORE_ID_TYPE *SEMAPHORE_ID,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
#endif
wasm_trap_t* WASM32_CREATE_SEMAPHORE(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  SEMAPHORE_VALUE_TYPE CURRENT_VALUE;
  CURRENT_VALUE = (SEMAPHORE_VALUE_TYPE)le32toh(args_and_results[1].i32);
  SEMAPHORE_VALUE_TYPE MAXIMUM_VALUE;
  MAXIMUM_VALUE = (SEMAPHORE_VALUE_TYPE)le32toh(args_and_results[2].i32);
  QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE;
  QUEUING_DISCIPLINE = (QUEUING_DISCIPLINE_TYPE)le32toh(args_and_results[3].i32);
  SEMAPHORE_ID_TYPE SEMAPHORE_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  CREATE_SEMAPHORE(
    (char*)&wasm_baseaddr[le32toh(args_and_results[0].i32)], // FIXME: only safe as long as char[]
    CURRENT_VALUE,
    MAXIMUM_VALUE,
    QUEUING_DISCIPLINE,
    &SEMAPHORE_ID,
    &RETURN_CODE
  );

  camw32_set__SEMAPHORE_ID_TYPE(&wasm_baseaddr[le32toh(args_and_results[4].i32)], (int32_t)SEMAPHORE_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[5].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
void WAIT_SEMAPHORE
( /*in */ SEMAPHORE_ID_TYPE SEMAPHORE_ID,
  /*in */ SYSTEM_TIME_TYPE TIME_OUT,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
#endif
wasm_trap_t* WASM32_WAIT_SEMAPHORE(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  SEMAPHORE_ID_TYPE SEMAPHORE_ID;
  SEMAPHORE_ID = (SEMAPHORE_ID_TYPE)le32toh(args_and_results[0].i32);
  SYSTEM_TIME_TYPE TIME_OUT;
  TIME_OUT = (SYSTEM_TIME_TYPE)le64toh(args_and_results[1].i64);
  RETURN_CODE_TYPE RETURN_CODE;

  WAIT_SEMAPHORE(
    SEMAPHORE_ID,
    TIME_OUT,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
void SIGNAL_SEMAPHORE
( /*in */ SEMAPHORE_ID_TYPE SEMAPHORE_ID,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
#endif
wasm_trap_t* WASM32_SIGNAL_SEMAPHORE(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  SEMAPHORE_ID_TYPE SEMAPHORE_ID;
  SEMAPHORE_ID = (SEMAPHORE_ID_TYPE)le32toh(args_and_results[0].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  SIGNAL_SEMAPHORE(
    SEMAPHORE_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
void GET_SEMAPHORE_ID
( /*in */ SEMAPHORE_NAME_TYPE SEMAPHORE_NAME,
  /*out*/ SEMAPHORE_ID_TYPE *SEMAPHORE_ID,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
#endif
wasm_trap_t* WASM32_GET_SEMAPHORE_ID(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  SEMAPHORE_ID_TYPE SEMAPHORE_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_SEMAPHORE_ID(
    (char*)&wasm_baseaddr[le32toh(args_and_results[0].i32)], // FIXME: only safe as long as char[]
    &SEMAPHORE_ID,
    &RETURN_CODE
  );

  camw32_set__SEMAPHORE_ID_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)SEMAPHORE_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);

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
wasm_trap_t* WASM32_GET_SEMAPHORE_STATUS(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  SEMAPHORE_ID_TYPE SEMAPHORE_ID;
  SEMAPHORE_ID = (SEMAPHORE_ID_TYPE)le32toh(args_and_results[0].i32);
  SEMAPHORE_STATUS_TYPE SEMAPHORE_STATUS;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_SEMAPHORE_STATUS(
    SEMAPHORE_ID,
    &SEMAPHORE_STATUS,
    &RETURN_CODE
  );

  uint8_t* SEMAPHORE_STATUS_guest = (uint8_t*)&wasm_baseaddr[le32toh(args_and_results[1].i32)];
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);

  camw32_set__SEMAPHORE_STATUS_TYPE__CURRENT_VALUE(SEMAPHORE_STATUS_guest, SEMAPHORE_STATUS.CURRENT_VALUE);
  camw32_set__SEMAPHORE_STATUS_TYPE__MAXIMUM_VALUE(SEMAPHORE_STATUS_guest, SEMAPHORE_STATUS.MAXIMUM_VALUE);
  camw32_set__SEMAPHORE_STATUS_TYPE__WAITING_PROCESSES(SEMAPHORE_STATUS_guest, SEMAPHORE_STATUS.WAITING_PROCESSES);

  return NULL;
}
