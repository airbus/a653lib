// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: QUEUING PORT

#ifndef ARINC653_PART1_APEX_QUEUING_PORT_WASM32
#define ARINC653_PART1_APEX_QUEUING_PORT_WASM32

#include "a653_i_common_wasm32.h"

/* APEX (ARINC 653 Part 1): QUEUING PORT */
#define WASM32_SIGNATURE__CREATE_QUEUING_PORT               "(iiiiiii)"
#define WASM32_SIGNATURE__SEND_QUEUING_MESSAGE              "(iiiIi)"
#define WASM32_SIGNATURE__RECEIVE_QUEUING_MESSAGE           "(iIiii)"
#define WASM32_SIGNATURE__GET_QUEUING_PORT_ID               "(iii)"
#define WASM32_SIGNATURE__GET_QUEUING_PORT_STATUS           "(iii)"
#define WASM32_SIGNATURE__CLEAR_QUEUING_PORT                "(ii)"

wasm_trap_t* WASM32_CREATE_QUEUING_PORT(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_SEND_QUEUING_MESSAGE(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_RECEIVE_QUEUING_MESSAGE(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_GET_QUEUING_PORT_ID(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_GET_QUEUING_PORT_STATUS(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_CLEAR_QUEUING_PORT(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);

#endif /* #ifndef ARINC653_PART1_APEX_QUEUING_PORT_WASM32 */
