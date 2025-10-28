// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#ifndef ARINC653_WASM32_HELPER

#ifdef __WAMR__
#include <wasm_export.h>

#define GET_ARG_i32( X ) arg##X
#define GET_ARG_i64( X ) arg##X
#else // WASMTIME
#include <wasmtime.h>
#include "a653_i_common_wasm32.h"

#define GET_ARG_i32( X ) args_and_results[X].i32
#define GET_ARG_i64( X ) args_and_results[X].i64
#endif


#ifdef __WAMR__
#define WASM_HOST_FUNC_HEADER(NAME) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1);

#define WASM_HOST_FUNCTION(NAME, WASM_BASEADDR, CONTENT) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1) \
{ \
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env); \
  uint8_t* (WASM_BASEADDR) = wasm_runtime_addr_app_to_native(module_inst, 0); \
\
  CONTENT \
\
}
#else // WASMTIME
#define WASM_HOST_FUNC_HEADER(NAME) \
wasm_trap_t* WASM32_##NAME(void* env, \
  wasmtime_caller_t *caller, \
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);

#define WASM_HOST_FUNCTION(NAME, WASM_BASEADDR, CONTENT) \
wasm_trap_t* WASM32_##NAME(void* env, \
  wasmtime_caller_t *caller, \
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results) \
{ \
  uint8_t* (WASM_BASEADDR) = get_linear_memory(caller); \
\
  CONTENT \
\
  return NULL; \
}
#endif

#endif /* #ifndef ARINC653_WASM32_HELPER */
