// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: PROCESS

#ifndef ARINC653_PART1_APEX_PROCESS_WASM32
#define ARINC653_PART1_APEX_PROCESS_WASM32

#include "a653_i_common_wasm32.h"

/* APEX (ARINC 653 Part 1): PROCESS */
#define WASM32_SIGNATURE__GET_PROCESS_ID                    "(iii)"
#define WASM32_SIGNATURE__GET_PROCESS_STATUS                "(iii)"
#define WASM32_SIGNATURE__CREATE_PROCESS                    "(iii)"
#define WASM32_SIGNATURE__SET_PRIORITY                      "(iii)"
#define WASM32_SIGNATURE__SUSPEND_SELF                      "(Ii)"
#define WASM32_SIGNATURE__SUSPEND                           "(ii)"
#define WASM32_SIGNATURE__RESUME                            "(ii)"
#define WASM32_SIGNATURE__STOP_SELF                         "()"
#define WASM32_SIGNATURE__STOP                              "(ii)"
#define WASM32_SIGNATURE__START                             "(ii)"
#define WASM32_SIGNATURE__DELAYED_START                     "(iIi)"
#define WASM32_SIGNATURE__LOCK_PREEMPTION                   "(ii)"
#define WASM32_SIGNATURE__UNLOCK_PREEMPTION                 "(ii)"
#define WASM32_SIGNATURE__GET_MY_ID                         "(ii)"
#define WASM32_SIGNATURE__INITIALIZE_PROCESS_CORE_AFFINITY  "(iii)"
#define WASM32_SIGNATURE__GET_MY_PROCESSOR_CORE_ID          "(ii)"
#define WASM32_SIGNATURE__GET_MY_INDEX                      "(ii)"

wasm_trap_t* WASM32_GET_PROCESS_ID(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_GET_MY_ID(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_GET_PROCESS_STATUS(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_CREATE_PROCESS(void *env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_SET_PRIORITY(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_SUSPEND_SELF(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_SUSPEND(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_RESUME(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_STOP_SELF(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_STOP(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_START(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_DELAYED_START(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_LOCK_PREEMPTION(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_UNLOCK_PREEMPTION(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_INITIALIZE_PROCESS_CORE_AFFINITY(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_GET_MY_PROCESSOR_CORE_ID(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_GET_MY_INDEX(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);

#endif /* #ifndef ARINC653_PART1_APEX_PROCESS_WASM32 */
