// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: TIME

#include "arinc653_part1_apex_time_wasm32.h"
#include "../a653_inc/a653Time.h"


/*
  SYSTEM_TIME_TYPE DELAY_TIME,
  RETURN_CODE_TYPE * RETURN_CODE
 */
const char* WASM32_SIGNATURE__TIMED_WAIT = "(Ii)";
wasm_trap_t* WASM32_TIMED_WAIT(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  TIMED_WAIT(
    (SYSTEM_TIME_TYPE)args[0].of.i64,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );
  return NULL;
}


/*
 * extern void PERIODIC_WAIT (RETURN_CODE_TYPE * RETURN_CODE);
 */
const char* WASM32_SIGNATURE__PERIODIC_WAIT = "(i)";
wasm_trap_t* WASM32_PERIODIC_WAIT(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  PERIODIC_WAIT(
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[0].of.i32]
  );
  return NULL;
}


/*
 extern void GET_TIME (SYSTEM_TIME_TYPE * SYSTEM_TIME,
 RETURN_CODE_TYPE * RETURN_CODE);
 */
const char* WASM32_SIGNATURE__GET_TIME = "(ii)";
wasm_trap_t* WASM32_GET_TIME(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  GET_TIME(
    (SYSTEM_TIME_TYPE*)&wasm_baseaddr[args[0].of.i32],
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );
  return NULL;
}


#if 0
void REPLENISH (SYSTEM_TIME_TYPE   BUDGET_TIME,
                RETURN_CODE_TYPE * RETURN_CODE)
#endif
const char* WASM32_SIGNATURE__REPLENISH = "(Ii)";
wasm_trap_t* WASM32_REPLENISH(void* env,
                             wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
                             wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  REPLENISH(
    (SYSTEM_TIME_TYPE)args[0].of.i64,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );
  return NULL;
}
