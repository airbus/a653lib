// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: SAMPLING PORT

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "arinc653_part1_apex_sampling_port_wasm32.h"
#include "wasm32_struct_getset.h" /* auto-generated header */
#include "../a653_inc/a653Sampling.h"


/*
 * SAMPLING_PORT_NAME_TYPE   SAMPLING_PORT_NAME,
 * MESSAGE_SIZE_TYPE         MAX_MESSAGE_SIZE,
 * PORT_DIRECTION_TYPE       PORT_DIRECTION,
 * SYSTEM_TIME_TYPE          REFRESH_PERIOD,
 * SAMPLING_PORT_ID_TYPE   * SAMPLING_PORT_ID,
 * RETURN_CODE_TYPE        * RETURN_CODE
 */
const char* WASM32_SIGNATURE__CREATE_SAMPLING_PORT = "(iiiIii)";
wasm_trap_t* WASM32_CREATE_SAMPLING_PORT(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  CREATE_SAMPLING_PORT(
    (char*)&wasm_baseaddr[args[0].of.i32],
    (MESSAGE_SIZE_TYPE)args[1].of.i32,
    (PORT_DIRECTION_TYPE)args[2].of.i32,
    (SYSTEM_TIME_TYPE)args[3].of.i64,
    (SAMPLING_PORT_ID_TYPE*)&wasm_baseaddr[args[4].of.i32],
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[5].of.i32]
  );
  return NULL;
}


/*
 * SAMPLING_PORT_ID_TYPE   SAMPLING_PORT_ID,
 * MESSAGE_ADDR_TYPE       MESSAGE_ADDR,
 * MESSAGE_SIZE_TYPE       LENGTH,
 * RETURN_CODE_TYPE      * RETURN_CODE);
 */
const char* WASM32_SIGNATURE__WRITE_SAMPLING_MESSAGE = "(iiii)";
wasm_trap_t* WASM32_WRITE_SAMPLING_MESSAGE(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  WRITE_SAMPLING_MESSAGE(
    (SAMPLING_PORT_ID_TYPE)args[0].of.i32,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[args[1].of.i32],
    (MESSAGE_SIZE_TYPE)args[2].of.i32,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[3].of.i32]
  );
  return NULL;
}


/*
 v *oid READ_SAMPLING_MESSAGE (SAMPLING_PORT_ID_TYPE    SAMPLING_PORT_ID,
 MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
MESSAGE_SIZE_TYPE      * LENGTH,
VALIDITY_TYPE          * VALIDITY,
RETURN_CODE_TYPE       * RETURN_CODE);
*/
const char* WASM32_SIGNATURE__READ_SAMPLING_MESSAGE = "(iiiii)";
wasm_trap_t* WASM32_READ_SAMPLING_MESSAGE(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  READ_SAMPLING_MESSAGE(
    (SAMPLING_PORT_ID_TYPE)args[0].of.i32,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[args[1].of.i32],
    (MESSAGE_SIZE_TYPE*)&wasm_baseaddr[args[2].of.i32],
    (VALIDITY_TYPE*)&wasm_baseaddr[args[3].of.i32],
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[4].of.i32]
  );
  return NULL;
}


/*
 * SAMPLING_PORT_NAME_TYPE   SAMPLING_PORT_NAME,
 * SAMPLING_PORT_ID_TYPE   * SAMPLING_PORT_ID,
 * RETURN_CODE_TYPE        * RETURN_CODE
 */
const char* WASM32_SIGNATURE__GET_SAMPLING_PORT_ID = "(iii)";
wasm_trap_t* WASM32_GET_SAMPLING_PORT_ID(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  GET_SAMPLING_PORT_ID(
    (char*)&wasm_baseaddr[args[0].of.i32],
    (SAMPLING_PORT_ID_TYPE*)&wasm_baseaddr[args[1].of.i32],
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[2].of.i32]
  );
  return NULL;
}


#if 0
extern void GET_SAMPLING_PORT_STATUS (
  /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
  /*out*/ SAMPLING_PORT_STATUS_TYPE  *SAMPLING_PORT_STATUS,
  /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__GET_SAMPLING_PORT_STATUS = "(iii)";
wasm_trap_t* WASM32_GET_SAMPLING_PORT_STATUS(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  SAMPLING_PORT_STATUS_TYPE SAMPLING_PORT_STATUS__host_64bit;
  GET_SAMPLING_PORT_STATUS(
    (SAMPLING_PORT_ID_TYPE)args[0].of.i32,
    &SAMPLING_PORT_STATUS__host_64bit,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[2].of.i32]
  );

  void *dl_struct_getset = env;

  // FIXME: pointer is 32bit, but could be 64bit ..
  SAMPLING_PORT_STATUS_TYPE* SAMPLING_PORT_STATUS__guest = (SAMPLING_PORT_STATUS_TYPE*)&wasm_baseaddr[args[1].of.i32];

#if 0
typedef struct {
  MESSAGE_SIZE_TYPE       MAX_MESSAGE_SIZE;   /* port size */
  PORT_DIRECTION_TYPE     PORT_DIRECTION;     /* port direction */
  SYSTEM_TIME_TYPE        REFRESH_PERIOD;     /* refresh period */
  VALIDITY_TYPE           LAST_MSG_VALIDITY;  /* message validity */
} SAMPLING_PORT_STATUS_TYPE;
#endif

  SET__SAMPLING_PORT_STATUS_TYPE__MAX_MESSAGE_SIZE__FNC_PTR_DCL(SET__SAMPLING_PORT_STATUS_TYPE__MAX_MESSAGE_SIZE)
    = (SET__SAMPLING_PORT_STATUS_TYPE__MAX_MESSAGE_SIZE__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__SAMPLING_PORT_STATUS_TYPE__MAX_MESSAGE_SIZE");
  if (! SET__SAMPLING_PORT_STATUS_TYPE__MAX_MESSAGE_SIZE) {
    fprintf(stderr, "ERR: SET__SAMPLING_PORT_STATUS_TYPE__MAX_MESSAGE_SIZE not present!\n");
    exit(-1);
  }

  SET__SAMPLING_PORT_STATUS_TYPE__PORT_DIRECTION__FNC_PTR_DCL(SET__SAMPLING_PORT_STATUS_TYPE__PORT_DIRECTION)
    = (SET__SAMPLING_PORT_STATUS_TYPE__PORT_DIRECTION__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__SAMPLING_PORT_STATUS_TYPE__PORT_DIRECTION");
  if (! SET__SAMPLING_PORT_STATUS_TYPE__PORT_DIRECTION) {
    fprintf(stderr, "ERR: SET__SAMPLING_PORT_STATUS_TYPE__PORT_DIRECTION not present!\n");
    exit(-1);
  }

  SET__SAMPLING_PORT_STATUS_TYPE__REFRESH_PERIOD__FNC_PTR_DCL(SET__SAMPLING_PORT_STATUS_TYPE__REFRESH_PERIOD)
    = (SET__SAMPLING_PORT_STATUS_TYPE__REFRESH_PERIOD__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__SAMPLING_PORT_STATUS_TYPE__REFRESH_PERIOD");
  if (! SET__SAMPLING_PORT_STATUS_TYPE__REFRESH_PERIOD) {
    fprintf(stderr, "ERR: SET__SAMPLING_PORT_STATUS_TYPE__REFRESH_PERIOD not present!\n");
    exit(-1);
  }

  SET__SAMPLING_PORT_STATUS_TYPE__LAST_MSG_VALIDITY__FNC_PTR_DCL(SET__SAMPLING_PORT_STATUS_TYPE__LAST_MSG_VALIDITY)
    = (SET__SAMPLING_PORT_STATUS_TYPE__LAST_MSG_VALIDITY__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__SAMPLING_PORT_STATUS_TYPE__LAST_MSG_VALIDITY");
  if (! SET__SAMPLING_PORT_STATUS_TYPE__LAST_MSG_VALIDITY) {
    fprintf(stderr, "ERR: SET__SAMPLING_PORT_STATUS_TYPE__LAST_MSG_VALIDITY not present!\n");
    exit(-1);
  }

  SET__SAMPLING_PORT_STATUS_TYPE__MAX_MESSAGE_SIZE(SAMPLING_PORT_STATUS__guest, SAMPLING_PORT_STATUS__host_64bit.MAX_MESSAGE_SIZE);
  SET__SAMPLING_PORT_STATUS_TYPE__PORT_DIRECTION(SAMPLING_PORT_STATUS__guest, SAMPLING_PORT_STATUS__host_64bit.PORT_DIRECTION);
  SET__SAMPLING_PORT_STATUS_TYPE__REFRESH_PERIOD(SAMPLING_PORT_STATUS__guest, SAMPLING_PORT_STATUS__host_64bit.REFRESH_PERIOD);
  SET__SAMPLING_PORT_STATUS_TYPE__LAST_MSG_VALIDITY(SAMPLING_PORT_STATUS__guest, SAMPLING_PORT_STATUS__host_64bit.LAST_MSG_VALIDITY);

  return NULL;
}
