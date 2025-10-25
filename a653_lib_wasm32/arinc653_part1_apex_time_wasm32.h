// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: TIME

#ifndef ARINC653_PART1_APEX_TIME_WASM32
#define ARINC653_PART1_APEX_TIME_WASM32

#ifdef __WAMR__
#include <wasm_export.h>
#else // WASMTIME
#include <wasmtime.h>
#endif

/* APEX (ARINC 653 Part 1): TIME */
#define WASM32_SIGNATURE__TIMED_WAIT            "(Ii)"
#define WASM32_SIGNATURE__PERIODIC_WAIT         "(i)"
#define WASM32_SIGNATURE__GET_TIME              "(ii)"
#define WASM32_SIGNATURE__REPLENISH             "(Ii)"

#ifdef __WAMR__
void WASM32_TIMED_WAIT(
  wasm_exec_env_t exec_env,
  uint64_t arg0, uint32_t arg1);
#else // WASMTIME
wasm_trap_t* WASM32_TIMED_WAIT(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
#endif
#ifdef __WAMR__
void WASM32_PERIODIC_WAIT(
  wasm_exec_env_t exec_env,
  uint32_t arg0);
#else // WASMTIME
wasm_trap_t* WASM32_PERIODIC_WAIT(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
#endif
#ifdef __WAMR__
void WASM32_GET_TIME(
  wasm_exec_env_t exec_env,
  uint32_t arg0, uint32_t arg1);
#else // WASMTIME
wasm_trap_t* WASM32_GET_TIME(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
#endif
#ifdef __WAMR__
void WASM32_REPLENISH(
  wasm_exec_env_t exec_env,
  uint64_t arg0, uint32_t arg1);
#else // WASMTIME
wasm_trap_t* WASM32_REPLENISH(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
#endif

#endif /* #ifndef ARINC653_PART1_APEX_TIME_WASM32 */
