// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: SEMAPHORE

#ifndef ARINC653_PART1_APEX_SEMAPHORE_WASM32
#define ARINC653_PART1_APEX_SEMAPHORE_WASM32

#include "a653_i_common_wasm32.h"

/* APEX (ARINC 653 Part 1): SEMAPHORE */
#define WASM32_SIGNATURE__CREATE_SEMAPHORE                  "(iiiiii)"
#define WASM32_SIGNATURE__WAIT_SEMAPHORE                    "(iIi)"
#define WASM32_SIGNATURE__SIGNAL_SEMAPHORE                  "(ii)"
#define WASM32_SIGNATURE__GET_SEMAPHORE_ID                  "(iii)"
#define WASM32_SIGNATURE__GET_SEMAPHORE_STATUS              "(iii)"

wasm_trap_t* WASM32_CREATE_SEMAPHORE(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_WAIT_SEMAPHORE(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_SIGNAL_SEMAPHORE(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_GET_SEMAPHORE_ID(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_GET_SEMAPHORE_STATUS(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);

#endif /* #ifndef ARINC653_PART1_APEX_SEMAPHORE_WASM32 */
