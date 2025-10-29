// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#ifndef A653_I_COMMON_WASM32
#define A653_I_COMMON_WASM32

#ifdef __WAMR__
#include <wasm_export.h>
#else
#include <wasmtime.h>
#endif
#include "../a653_inc/a653Init.h"

typedef struct {
#ifndef __WAMR__ // FIXME: remove
  wasm_engine_t* engine;
  wasmtime_sharedmemory_t* shm_memory;
  wasmtime_module_t* module;
#endif

  //  wasmtime_table_t shared_table; // used for the function indexes (due to pthread_create())
  uint32_t ENTRY_POINT[MAX_PRCS];
  uint32_t ENTRY_POINT_ERROR_HANDLER;
} wasm_processes_t;

#ifndef __WAMR__
void print_wasmtime_error(wasmtime_error_t* error);
uint8_t* get_linear_memory(wasmtime_caller_t* caller);
#endif
int exec_wasm_guest_func(int start_default, uint32_t idx);

#endif
