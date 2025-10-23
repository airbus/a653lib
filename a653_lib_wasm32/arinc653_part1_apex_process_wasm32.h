// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: PROCESS

#ifndef ARINC653_PART1_APEX_PROCESS_WASM32
#define ARINC653_PART1_APEX_PROCESS_WASM32

#include "a653_i_common_wasm32.h"

/* APEX (ARINC 653 Part 1): PROCESS */
extern const char* WASM32_SIGNATURE__CREATE_PROCESS;
extern const char* WASM32_SIGNATURE__SET_PRIORITY;
extern const char* WASM32_SIGNATURE__SUSPEND_SELF;
extern const char* WASM32_SIGNATURE__SUSPEND;
extern const char* WASM32_SIGNATURE__RESUME;
extern const char* WASM32_SIGNATURE__STOP_SELF;
extern const char* WASM32_SIGNATURE__STOP;
extern const char* WASM32_SIGNATURE__START;
extern const char* WASM32_SIGNATURE__DELAYED_START;
extern const char* WASM32_SIGNATURE__LOCK_PREEMPTION;
extern const char* WASM32_SIGNATURE__UNLOCK_PREEMPTION;
extern const char* WASM32_SIGNATURE__GET_MY_ID;
extern const char* WASM32_SIGNATURE__GET_PROCESS_ID;
extern const char* WASM32_SIGNATURE__GET_PROCESS_STATUS;
extern const char* WASM32_SIGNATURE__INITIALIZE_PROCESS_CORE_AFFINITY;
extern const char* WASM32_SIGNATURE__GET_MY_PROCESSOR_CORE_ID;
extern const char* WASM32_SIGNATURE__GET_MY_INDEX;

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
