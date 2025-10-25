// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: PARTITION

#ifndef ARINC653_PART1_APEX_PARTITION_WASM32
#define ARINC653_PART1_APEX_PARTITION_WASM32

#ifdef __WAMR__
#include <wasm_export.h>
#else // WASMTIME
#include <wasmtime.h>
#endif

/* APEX (ARINC 653 Part 1): PARTITION */
#define WASM32_SIGNATURE__GET_PARTITION_STATUS              "(ii)"
#define WASM32_SIGNATURE__SET_PARTITION_MODE                "(ii)"

#ifdef __WAMR__
void WASM32_GET_PARTITION_STATUS(
  wasm_exec_env_t exec_env,
  uint32_t arg0, uint32_t arg1);
#else // WASMTIME
wasm_trap_t* WASM32_GET_PARTITION_STATUS(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
#endif
#ifdef __WAMR__
void WASM32_SET_PARTITION_MODE(
  wasm_exec_env_t exec_env,
  uint32_t arg0, uint32_t arg1);
#else // WASMTIME
wasm_trap_t* WASM32_SET_PARTITION_MODE(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
#endif

#endif /* #ifndef ARINC653_PART1_APEX_PARTITION_WASM32 */
