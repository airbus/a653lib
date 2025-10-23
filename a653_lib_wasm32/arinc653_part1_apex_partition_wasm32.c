// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: PARTITION


#include <endian.h>
#include "arinc653_part1_apex_partition_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_inc/a653Partition.h"


#if 0
extern void GET_PARTITION_STATUS (
  /* out */ PARTITION_STATUS_TYPE *PARTITION_STATUS,
  /* out */ RETURN_CODE_TYPE *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__GET_PARTITION_STATUS = "(ii)";
wasm_trap_t* WASM32_GET_PARTITION_STATUS(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  PARTITION_STATUS_TYPE STATUS;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_PARTITION_STATUS(
    &STATUS,
    &RETURN_CODE
  );

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
#endif

  uint8_t* STATUS_guest = &wasm_baseaddr[le32toh(args_and_results[0].i32)];
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  camw32_set__PARTITION_STATUS_TYPE__PERIOD(STATUS_guest, STATUS.PERIOD);
  camw32_set__PARTITION_STATUS_TYPE__DURATION(STATUS_guest, STATUS.DURATION);
  camw32_set__PARTITION_STATUS_TYPE__IDENTIFIER(STATUS_guest, STATUS.IDENTIFIER);
  camw32_set__PARTITION_STATUS_TYPE__LOCK_LEVEL(STATUS_guest, STATUS.LOCK_LEVEL);
  camw32_set__PARTITION_STATUS_TYPE__OPERATING_MODE(STATUS_guest, STATUS.OPERATING_MODE);
  camw32_set__PARTITION_STATUS_TYPE__START_CONDITION(STATUS_guest, STATUS.START_CONDITION);
  camw32_set__PARTITION_STATUS_TYPE__NUM_ASSIGNED_CORES(STATUS_guest, STATUS.NUM_ASSIGNED_CORES);

  return NULL;
}


#if 0
extern void SET_PARTITION_MODE (
/* in  */ OPERATING_MODE_TYPE OPERATING_MODE,
/* out */ RETURN_CODE_TYPE *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__SET_PARTITION_MODE = "(ii)";
wasm_trap_t* WASM32_SET_PARTITION_MODE(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  OPERATING_MODE_TYPE OPERATING_MODE;
  OPERATING_MODE = (OPERATING_MODE_TYPE)le32toh(args_and_results[0].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  SET_PARTITION_MODE(
    OPERATING_MODE,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  return NULL;
}
