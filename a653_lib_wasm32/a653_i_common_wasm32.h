// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#ifndef A653_I_COMMON_WASM32
#define A653_I_COMMON_WASM32

#include <wasmtime.h>
#include "../a653_inc/a653Lib.h"

#define MAX_FUNC_ARGS 10
typedef struct {
  unsigned count;
  wasm_valkind_t kind[MAX_FUNC_ARGS];
} wasm_parms_t;

typedef struct {
  wasm_engine_t* engine;
  wasmtime_sharedmemory_t* shm_memory;
  wasmtime_module_t* module;

  //  wasmtime_table_t shared_table; // used for the function indexes (due to pthread_create())
  uint32_t ENTRY_POINT[MAX_PRCS];
  uint32_t ENTRY_POINT_ERROR_HANDLER;
} wasm_processes_t;

int get_exported_memory(wasmtime_caller_t* caller, wasmtime_memory_t* out_mem);

#endif
