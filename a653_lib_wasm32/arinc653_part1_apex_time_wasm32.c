// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: TIME

#include <endian.h>
#include "arinc653_part1_apex_time_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_inc/a653Time.h"


#if 0
extern void TIMED_WAIT (
  /*in */ SYSTEM_TIME_TYPE         DELAY_TIME,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__TIMED_WAIT = "(Ii)";
wasm_trap_t* WASM32_TIMED_WAIT(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  SYSTEM_TIME_TYPE DELAY_TIME;
  DELAY_TIME = (SYSTEM_TIME_TYPE)le64toh(args_and_results[0].i64);
  RETURN_CODE_TYPE RETURN_CODE;

  TIMED_WAIT(
    DELAY_TIME,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void PERIODIC_WAIT (
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__PERIODIC_WAIT = "(i)";
wasm_trap_t* WASM32_PERIODIC_WAIT(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  RETURN_CODE_TYPE RETURN_CODE;

  PERIODIC_WAIT(
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[0].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void GET_TIME (
  /*out*/ SYSTEM_TIME_TYPE         *SYSTEM_TIME, /* 64bit - 1 nanosecond LSB */
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__GET_TIME = "(ii)";
wasm_trap_t* WASM32_GET_TIME(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  SYSTEM_TIME_TYPE SYSTEM_TIME;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_TIME(
    &SYSTEM_TIME,
    &RETURN_CODE
  );

  camw32_set__SYSTEM_TIME_TYPE(&wasm_baseaddr[le32toh(args_and_results[0].i32)], (int64_t)SYSTEM_TIME);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void REPLENISH (
  /*in */ SYSTEM_TIME_TYPE         BUDGET_TIME,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE);
#endif
const char* WASM32_SIGNATURE__REPLENISH = "(Ii)";
wasm_trap_t* WASM32_REPLENISH(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  SYSTEM_TIME_TYPE BUDGET_TIME;
  BUDGET_TIME = (SYSTEM_TIME_TYPE)le64toh(args_and_results[0].i64);
  RETURN_CODE_TYPE RETURN_CODE;

  REPLENISH(
    BUDGET_TIME,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  return NULL;
}
