// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: PARTITION


#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "arinc653_part1_apex_partition_wasm32.h"
#include "wasm32_struct_getset.h" /* auto-generated header */
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

  void *dl_struct_getset = env;

  // FIXME: pointer is 32bit, but could be 64bit ..
  PARTITION_STATUS_TYPE* STATUS_guest = (PARTITION_STATUS_TYPE*)&wasm_baseaddr[args[0].of.i32];

  SET__PARTITION_STATUS_TYPE__PERIOD__FNC_PTR_DCL(SET__PARTITION_STATUS_TYPE__PERIOD)
  = (SET__PARTITION_STATUS_TYPE__PERIOD__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PARTITION_STATUS_TYPE__PERIOD");
  if (! SET__PARTITION_STATUS_TYPE__PERIOD) {
    fprintf(stderr, "ERR: SET__PARTITION_STATUS_TYPE__PERIOD not present!\n");
    exit(-1);
  }

  SET__PARTITION_STATUS_TYPE__DURATION__FNC_PTR_DCL(SET__PARTITION_STATUS_TYPE__DURATION)
  = (SET__PARTITION_STATUS_TYPE__DURATION__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PARTITION_STATUS_TYPE__DURATION");
  if (! SET__PARTITION_STATUS_TYPE__DURATION) {
    fprintf(stderr, "ERR: SET__PARTITION_STATUS_TYPE__DURATION not present!\n");
    exit(-1);
  }

  SET__PARTITION_STATUS_TYPE__IDENTIFIER__FNC_PTR_DCL(SET__PARTITION_STATUS_TYPE__IDENTIFIER)
  = (SET__PARTITION_STATUS_TYPE__IDENTIFIER__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PARTITION_STATUS_TYPE__IDENTIFIER");
  if (! SET__PARTITION_STATUS_TYPE__IDENTIFIER) {
    fprintf(stderr, "ERR: SET__PARTITION_STATUS_TYPE__IDENTIFIER not present!\n");
    exit(-1);
  }

  SET__PARTITION_STATUS_TYPE__LOCK_LEVEL__FNC_PTR_DCL(SET__PARTITION_STATUS_TYPE__LOCK_LEVEL)
  = (SET__PARTITION_STATUS_TYPE__LOCK_LEVEL__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PARTITION_STATUS_TYPE__LOCK_LEVEL");
  if (! SET__PARTITION_STATUS_TYPE__LOCK_LEVEL) {
    fprintf(stderr, "ERR: SET__PARTITION_STATUS_TYPE__LOCK_LEVEL not present!\n");
    exit(-1);
  }

  SET__PARTITION_STATUS_TYPE__OPERATING_MODE__FNC_PTR_DCL(SET__PARTITION_STATUS_TYPE__OPERATING_MODE)
  = (SET__PARTITION_STATUS_TYPE__OPERATING_MODE__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PARTITION_STATUS_TYPE__OPERATING_MODE");
  if (! SET__PARTITION_STATUS_TYPE__OPERATING_MODE) {
    fprintf(stderr, "ERR: SET__PARTITION_STATUS_TYPE__OPERATING_MODE not present!\n");
    exit(-1);
  }

  SET__PARTITION_STATUS_TYPE__START_CONDITION__FNC_PTR_DCL(SET__PARTITION_STATUS_TYPE__START_CONDITION)
  = (SET__PARTITION_STATUS_TYPE__START_CONDITION__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PARTITION_STATUS_TYPE__START_CONDITION");
  if (! SET__PARTITION_STATUS_TYPE__START_CONDITION) {
    fprintf(stderr, "ERR: SET__PARTITION_STATUS_TYPE__START_CONDITION not present!\n");
    exit(-1);
  }

  SET__PARTITION_STATUS_TYPE__NUM_ASSIGNED_CORES__FNC_PTR_DCL(SET__PARTITION_STATUS_TYPE__NUM_ASSIGNED_CORES)
  = (SET__PARTITION_STATUS_TYPE__NUM_ASSIGNED_CORES__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PARTITION_STATUS_TYPE__NUM_ASSIGNED_CORES");
  if (! SET__PARTITION_STATUS_TYPE__NUM_ASSIGNED_CORES) {
    fprintf(stderr, "ERR: SET__PARTITION_STATUS_TYPE__NUM_ASSIGNED_CORES not present!\n");
    exit(-1);
  }

  SET__PARTITION_STATUS_TYPE__PERIOD(STATUS_guest, STATUS__host_64bit.PERIOD);
  SET__PARTITION_STATUS_TYPE__DURATION(STATUS_guest, STATUS__host_64bit.DURATION);
  SET__PARTITION_STATUS_TYPE__IDENTIFIER(STATUS_guest, STATUS__host_64bit.IDENTIFIER);
  SET__PARTITION_STATUS_TYPE__LOCK_LEVEL(STATUS_guest, STATUS__host_64bit.LOCK_LEVEL);
  SET__PARTITION_STATUS_TYPE__OPERATING_MODE(STATUS_guest, STATUS__host_64bit.OPERATING_MODE);
  SET__PARTITION_STATUS_TYPE__START_CONDITION(STATUS_guest, STATUS__host_64bit.START_CONDITION);
  SET__PARTITION_STATUS_TYPE__NUM_ASSIGNED_CORES(STATUS_guest, STATUS__host_64bit.NUM_ASSIGNED_CORES);

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
