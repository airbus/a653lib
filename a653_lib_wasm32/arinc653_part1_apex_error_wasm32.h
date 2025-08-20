// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: ERROR

#ifndef ARINC653_PART1_APEX_ERROR_WASM32
#define ARINC653_PART1_APEX_ERROR_WASM32

#include "a653_i_common_wasm32.h"

wasm_trap_t* WASM32_REPORT_APPLICATION_MESSAGE(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults);
wasm_trap_t* WASM32_CREATE_ERROR_HANDLER(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults);
wasm_trap_t* WASM32_GET_ERROR_STATUS(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults);
wasm_trap_t* WASM32_RAISE_APPLICATION_ERROR(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults);
wasm_trap_t* WASM32_CONFIGURE_ERROR_HANDLER(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults);

#endif /* #ifndef ARINC653_PART1_APEX_ERROR_WASM32 */
