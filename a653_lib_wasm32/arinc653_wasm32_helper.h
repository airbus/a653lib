// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#ifndef ARINC653_WASM32_HELPER

#ifdef __WAMR__
#include <stdint.h>
#include <wasm_export.h>
#include "a653_wamr.h"

#define GET_ARG_i32( X ) (*(uint32_t*)&args[X])
#define GET_ARG_i64( X ) (*(uint64_t*)&args[X])

#else // WASMTIME
#include <stdio.h>
#include <wasmtime.h>
#include "a653_wasmtime.h"

#define GET_ARG_i32( X ) args_and_results[X].i32
#define GET_ARG_i64( X ) args_and_results[X].i64
#endif


#ifdef __WAMR__

#define WASM32_HOST_FUNC_HEADER(NAME) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint64_t *args);

#define WASM32_HOST_FUNCTION(NAME, WASM_BASEADDR, CONTENT) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint64_t *args) \
{ \
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env); \
  uint8_t* (WASM_BASEADDR) = wasm_runtime_addr_app_to_native(module_inst, 0); \
  \
  CONTENT \
  \
}

#else // WASMTIME

#define WASM32_HOST_FUNC_HEADER(NAME) \
wasm_trap_t* WASM32_##NAME(void* env, \
  wasmtime_caller_t *caller, \
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);

#define WASM32_HOST_FUNCTION(NAME, WASM_BASEADDR, CONTENT) \
wasm_trap_t* WASM32_##NAME(void* env, \
  wasmtime_caller_t *caller, \
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results) \
{ \
  wasmtime_extern_t ext; \
  const char *m = "memory"; \
  if ( ! wasmtime_caller_export_get(caller, m, strlen(m), &ext)) { \
    fprintf(stderr, "ERR: 'memory' export not found!\n"); \
    return NULL; \
  } \
  \
  if (ext.kind != WASM_EXTERN_MEMORY) { \
    fprintf(stderr, "ERR: export 'memory' is not a memory!\n"); \
    return NULL; \
  } \
  \
  wasmtime_context_t *context = wasmtime_caller_context(caller); \
  uint8_t* (WASM_BASEADDR) = wasmtime_memory_data(context, &ext.of.memory); \
  \
  CONTENT \
  \
  return NULL; \
}

#endif

#endif /* #ifndef ARINC653_WASM32_HELPER */
