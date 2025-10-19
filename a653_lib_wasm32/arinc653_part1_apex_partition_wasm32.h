// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: PARTITION

#ifndef ARINC653_PART1_APEX_PARTITION_WASM32
#define ARINC653_PART1_APEX_PARTITION_WASM32

#include "a653_i_common_wasm32.h"

/* APEX (ARINC 653 Part 1): PARTITION */
extern const char* WASM32_SIGNATURE__GET_PARTITION_STATUS;
extern const char* WASM32_SIGNATURE__SET_PARTITION_MODE;

wasm_trap_t* WASM32_GET_PARTITION_STATUS(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults);
wasm_trap_t* WASM32_SET_PARTITION_MODE(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults);

#endif /* #ifndef ARINC653_PART1_APEX_PARTITION_WASM32 */
