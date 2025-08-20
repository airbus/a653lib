// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 2: APEX Interface: SAMPLING PORT EXTENSION

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "arinc653_part2_apex_sampling_port_extension_wasm32.h"
#include "wasm32_struct_getset.h" /* auto-generated header */
#include "../a653_inc/a653Sampling.h"


#if 0
extern void READ_UPDATED_SAMPLING_MESSAGE (
/*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
/*in */ MESSAGE_ADDR_TYPE          MESSAGE_ADDR,
/*out*/ MESSAGE_SIZE_TYPE          *LENGTH,
/*out*/ UPDATED_TYPE               *UPDATED,
/*out*/ RETURN_CODE_TYPE           *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__READ_UPDATED_SAMPLING_MESSAGE = "(iiiii)";
wasm_trap_t* WASM32_READ_UPDATED_SAMPLING_MESSAGE(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  READ_UPDATED_SAMPLING_MESSAGE(
    (SAMPLING_PORT_ID_TYPE)args[0].of.i32,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[args[1].of.i32],
    (MESSAGE_SIZE_TYPE*)&wasm_baseaddr[args[2].of.i32],
    (UPDATED_TYPE*)&wasm_baseaddr[args[3].of.i32],
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[4].of.i32]
  );

  return NULL;
}


#if 0
extern void GET_SAMPLING_PORT_CURRENT_STATUS (
/*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
/*out*/ SAMPLING_PORT_CURRENT_STATUS_TYPE *SAMPLING_PORT_CURRENT_STATUS,
/*out*/ RETURN_CODE_TYPE           *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__GET_SAMPLING_PORT_CURRENT_STATUS = "(iii)";
wasm_trap_t* WASM32_GET_SAMPLING_PORT_CURRENT_STATUS(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  SAMPLING_PORT_CURRENT_STATUS_TYPE SAMPLING_PORT_CURRENT_STATUS__host_64bit;
  GET_SAMPLING_PORT_CURRENT_STATUS(
    (SAMPLING_PORT_ID_TYPE)args[0].of.i32,
    &SAMPLING_PORT_CURRENT_STATUS__host_64bit,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[2].of.i32]
  );

  void *dl_struct_getset = env;

  // FIXME: pointer is 32bit, but could be 64bit ..
  SAMPLING_PORT_CURRENT_STATUS_TYPE* SAMPLING_PORT_CURRENT_STATUS__guest = (SAMPLING_PORT_CURRENT_STATUS_TYPE*)&wasm_baseaddr[args[1].of.i32];

#if 0
/* sampling port status type */
typedef struct {
  SYSTEM_TIME_TYPE        REFRESH_PERIOD;     /* refresh period */
  SYSTEM_TIME_TYPE        TIME_STAMP;         /* when message was written */
  MESSAGE_SIZE_TYPE       MAX_MESSAGE_SIZE;   /* port max byte size */
  PORT_DIRECTION_TYPE     PORT_DIRECTION;     /* port direction */
  AGE_TYPE                MESSAGE_AGE;        /* message age - is msg is older than ports refresh period */
  UPDATED_TYPE            UPDATED;            /* empty, consumed, or new message */
} SAMPLING_PORT_CURRENT_STATUS_TYPE;
#endif

  SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__REFRESH_PERIOD__FNC_PTR_DCL(SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__REFRESH_PERIOD)
    = (SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__REFRESH_PERIOD__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__REFRESH_PERIOD");
  if (! SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__REFRESH_PERIOD) {
    fprintf(stderr, "ERR: SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__REFRESH_PERIOD not present!\n");
    exit(-1);
  }

  SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__TIME_STAMP__FNC_PTR_DCL(SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__TIME_STAMP)
  = (SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__TIME_STAMP__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__TIME_STAMP");
  if (! SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__TIME_STAMP) {
    fprintf(stderr, "ERR: SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__TIME_STAMP not present!\n");
    exit(-1);
  }

  SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__MAX_MESSAGE_SIZE__FNC_PTR_DCL(SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__MAX_MESSAGE_SIZE)
  = (SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__MAX_MESSAGE_SIZE__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__MAX_MESSAGE_SIZE");
  if (! SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__MAX_MESSAGE_SIZE) {
    fprintf(stderr, "ERR: SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__MAX_MESSAGE_SIZE not present!\n");
    exit(-1);
  }

  SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__PORT_DIRECTION__FNC_PTR_DCL(SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__PORT_DIRECTION)
  = (SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__PORT_DIRECTION__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__PORT_DIRECTION");
  if (! SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__PORT_DIRECTION) {
    fprintf(stderr, "ERR: SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__PORT_DIRECTION not present!\n");
    exit(-1);
  }

  SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__MESSAGE_AGE__FNC_PTR_DCL(SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__MESSAGE_AGE)
  = (SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__MESSAGE_AGE__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__MESSAGE_AGE");
  if (! SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__MESSAGE_AGE) {
    fprintf(stderr, "ERR: SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__MESSAGE_AGE not present!\n");
    exit(-1);
  }

  SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__UPDATED__FNC_PTR_DCL(SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__UPDATED)
  = (SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__UPDATED__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__UPDATED");
  if (! SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__UPDATED) {
    fprintf(stderr, "ERR: SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__UPDATED not present!\n");
    exit(-1);
  }

  SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__REFRESH_PERIOD(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS__host_64bit.REFRESH_PERIOD);
  SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__TIME_STAMP(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS__host_64bit.TIME_STAMP);
  SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__MAX_MESSAGE_SIZE(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS__host_64bit.MAX_MESSAGE_SIZE);
  SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__PORT_DIRECTION(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS__host_64bit.PORT_DIRECTION);
  SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__MESSAGE_AGE(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS__host_64bit.MESSAGE_AGE);
  SET__SAMPLING_PORT_CURRENT_STATUS_TYPE__UPDATED(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS__host_64bit.UPDATED);

  return NULL;
}


#if 0
extern void READ_SAMPLING_MESSAGE_CONDITIONAL (
/* in  */ SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID,
/* in  */ SYSTEM_TIME_TYPE        REF_TIME_STAMP,
/* in  */ MESSAGE_ADDR_TYPE       MESSAGE_ADDR,
/* out */ MESSAGE_SIZE_TYPE *     LENGTH,
/* out */ SYSTEM_TIME_TYPE *      TIME_STAMP,
/* out */ RETURN_CODE_TYPE *      RETURN_CODE);
#endif
const char* WASM32_SIGNATURE__READ_SAMPLING_MESSAGE_CONDITIONAL = "(iIiiii)";
wasm_trap_t* WASM32_READ_SAMPLING_MESSAGE_CONDITIONAL(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  READ_SAMPLING_MESSAGE_CONDITIONAL(
    (SAMPLING_PORT_ID_TYPE)args[0].of.i32,
    (SYSTEM_TIME_TYPE)args[1].of.i32,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[args[2].of.i32],
    (MESSAGE_SIZE_TYPE*)&wasm_baseaddr[args[3].of.i32],
    (SYSTEM_TIME_TYPE*)&wasm_baseaddr[args[4].of.i32],
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[5].of.i32]
  );
  return NULL;
}
