// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: TIME

#include <endian.h>
#include "arinc653_part1_apex_time_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_inc/a653Time.h"

#ifdef __WAMR__
#define GET_ARG_i32( X ) arg##X
#define GET_ARG_i64( X ) arg##X
#else // WASMTIME
#include "a653_i_common_wasm32.h"

#define GET_ARG_i32( X ) args_and_results[X].i32
#define GET_ARG_i64( X ) args_and_results[X].i64
#endif


#ifdef __WAMR__
void WASM32_TIMED_WAIT(
  wasm_exec_env_t exec_env,
  uint64_t arg0, uint32_t arg1)
{
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env);
  uint8_t* wasm_baseaddr = wasm_runtime_addr_app_to_native(module_inst, 0);
#else // WASMTIME
wasm_trap_t* WASM32_TIMED_WAIT(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);
#endif

  SYSTEM_TIME_TYPE DELAY_TIME;
  DELAY_TIME = (SYSTEM_TIME_TYPE)le64toh(GET_ARG_i64(0));
  RETURN_CODE_TYPE RETURN_CODE;

  TIMED_WAIT(
    DELAY_TIME,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);

#ifndef __WAMR__
  return NULL;
#endif
}


#ifdef __WAMR__
void WASM32_PERIODIC_WAIT(
  wasm_exec_env_t exec_env,
  uint32_t arg0)
{
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env);
  uint8_t* wasm_baseaddr = wasm_runtime_addr_app_to_native(module_inst, 0);
#else // WASMTIME
wasm_trap_t* WASM32_PERIODIC_WAIT(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);
#endif

  RETURN_CODE_TYPE RETURN_CODE;

  PERIODIC_WAIT(
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(0))], (int32_t)RETURN_CODE);

#ifndef __WAMR__
  return NULL;
#endif
}


#ifdef __WAMR__
void WASM32_GET_TIME(
  wasm_exec_env_t exec_env,
  uint32_t arg0, uint32_t arg1)
{
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env);
  uint8_t* wasm_baseaddr = wasm_runtime_addr_app_to_native(module_inst, 0);
#else // WASMTIME
wasm_trap_t* WASM32_GET_TIME(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);
#endif

  SYSTEM_TIME_TYPE SYSTEM_TIME;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_TIME(
    &SYSTEM_TIME,
    &RETURN_CODE
  );

  camw32_set__SYSTEM_TIME_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(0))], (int64_t)SYSTEM_TIME);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);

#ifndef __WAMR__
  return NULL;
#endif
}


#ifdef __WAMR__
void WASM32_REPLENISH(
  wasm_exec_env_t exec_env,
  uint64_t arg0, uint32_t arg1)
{
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env);
  uint8_t* wasm_baseaddr = wasm_runtime_addr_app_to_native(module_inst, 0);
#else // WASMTIME
wasm_trap_t* WASM32_REPLENISH(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);
#endif

  SYSTEM_TIME_TYPE BUDGET_TIME;
  BUDGET_TIME = (SYSTEM_TIME_TYPE)le64toh(GET_ARG_i64(0));
  RETURN_CODE_TYPE RETURN_CODE;

  REPLENISH(
    BUDGET_TIME,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);

#ifndef __WAMR__
  return NULL;
#endif
}
