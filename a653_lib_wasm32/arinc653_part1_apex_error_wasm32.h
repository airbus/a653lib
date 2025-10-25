// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: ERROR

#ifndef ARINC653_PART1_APEX_ERROR_WASM32
#define ARINC653_PART1_APEX_ERROR_WASM32

#include "a653_i_common_wasm32.h"

/* APEX (ARINC 653 Part 1): ERROR */
#define WASM32_SIGNATURE__REPORT_APPLICATION_MESSAGE        "(iii)"
#define WASM32_SIGNATURE__CREATE_ERROR_HANDLER              "(iii)"
#define WASM32_SIGNATURE__GET_ERROR_STATUS                  "(ii)"
#define WASM32_SIGNATURE__RAISE_APPLICATION_ERROR           "(iiii)"
#define WASM32_SIGNATURE__CONFIGURE_ERROR_HANDLER           "(iii)"

wasm_trap_t* WASM32_REPORT_APPLICATION_MESSAGE(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_CREATE_ERROR_HANDLER(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_GET_ERROR_STATUS(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_RAISE_APPLICATION_ERROR(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_CONFIGURE_ERROR_HANDLER(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);

#endif /* #ifndef ARINC653_PART1_APEX_ERROR_WASM32 */
