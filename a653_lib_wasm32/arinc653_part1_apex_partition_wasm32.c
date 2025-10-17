// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: PARTITION


#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "arinc653_part1_apex_partition_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_inc/a653Partition.h"

#if 0
typedef struct {
  SYSTEM_TIME_TYPE PERIOD;
  SYSTEM_TIME_TYPE DURATION;
  PARTITION_ID_TYPE IDENTIFIER;
  LOCK_LEVEL_TYPE LOCK_LEVEL;
  OPERATING_MODE_TYPE OPERATING_MODE;
  START_CONDITION_TYPE START_CONDITION;
  NUM_CORES_TYPE NUM_ASSIGNED_CORES;
} PARTITION_STATUS_TYPE;

WASM_IMPORT_MODULE("arinc653")
extern void GET_PARTITION_STATUS (
/* out */ PARTITION_STATUS_TYPE *PARTITION_STATUS,
/* out */ RETURN_CODE_TYPE *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__GET_PARTITION_STATUS = "(ii)";
wasm_trap_t* WASM32_GET_PARTITION_STATUS(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  PARTITION_STATUS_TYPE STATUS__host_64bit;
  GET_PARTITION_STATUS(
    &STATUS__host_64bit,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );

  // FIXME: pointer is 32bit, but could be 64bit ..
  uint8_t* STATUS_guest = (uint8_t*)&wasm_baseaddr[args[0].of.i32];
  camw32_set__PARTITION_STATUS_TYPE__PERIOD(STATUS_guest, STATUS__host_64bit.PERIOD);
  camw32_set__PARTITION_STATUS_TYPE__DURATION(STATUS_guest, STATUS__host_64bit.DURATION);
  camw32_set__PARTITION_STATUS_TYPE__IDENTIFIER(STATUS_guest, STATUS__host_64bit.IDENTIFIER);
  camw32_set__PARTITION_STATUS_TYPE__LOCK_LEVEL(STATUS_guest, STATUS__host_64bit.LOCK_LEVEL);
  camw32_set__PARTITION_STATUS_TYPE__OPERATING_MODE(STATUS_guest, STATUS__host_64bit.OPERATING_MODE);
  camw32_set__PARTITION_STATUS_TYPE__START_CONDITION(STATUS_guest, STATUS__host_64bit.START_CONDITION);
  camw32_set__PARTITION_STATUS_TYPE__NUM_ASSIGNED_CORES(STATUS_guest, STATUS__host_64bit.NUM_ASSIGNED_CORES);

  return NULL;
}


#if 0
extern void SET_PARTITION_MODE (
/* in  */ OPERATING_MODE_TYPE OPERATING_MODE,
/* out */ RETURN_CODE_TYPE *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__SET_PARTITION_MODE = "(ii)";
wasm_trap_t* WASM32_SET_PARTITION_MODE(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);
 
  SET_PARTITION_MODE(
    (OPERATING_MODE_TYPE)args[0].of.i32,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );
  return NULL;
}
