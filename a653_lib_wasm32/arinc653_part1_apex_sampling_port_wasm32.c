// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: SAMPLING PORT

#include "arinc653_part1_apex_sampling_port_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_inc/a653Sampling.h"


#if 0
extern void CREATE_SAMPLING_PORT (
  /*in */ SAMPLING_PORT_NAME_TYPE    SAMPLING_PORT_NAME,
  /*in */ MESSAGE_SIZE_TYPE          MAX_MESSAGE_SIZE,
  /*in */ PORT_DIRECTION_TYPE        PORT_DIRECTION,
  /*in */ SYSTEM_TIME_TYPE           REFRESH_PERIOD,
  /*out*/ SAMPLING_PORT_ID_TYPE      *SAMPLING_PORT_ID,
  /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__CREATE_SAMPLING_PORT = "(iiiIii)";
wasm_trap_t* WASM32_CREATE_SAMPLING_PORT(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);


  MESSAGE_SIZE_TYPE MAX_MESSAGE_SIZE;
  MAX_MESSAGE_SIZE = (MESSAGE_SIZE_TYPE)camw32_get__MESSAGE_SIZE_TYPE((uint8_t*)&args[1].of.i32);
  PORT_DIRECTION_TYPE PORT_DIRECTION;
  PORT_DIRECTION = (PORT_DIRECTION_TYPE)camw32_get__PORT_DIRECTION_TYPE((uint8_t*)&args[2].of.i32);
  SYSTEM_TIME_TYPE REFRESH_PERIOD;
  REFRESH_PERIOD = (SYSTEM_TIME_TYPE)camw32_get__SYSTEM_TIME_TYPE((uint8_t*)&args[3].of.i64);
  SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  CREATE_SAMPLING_PORT(
    (char*)&wasm_baseaddr[args[0].of.i32], // FIXME: address args[].of.i32 could be LE/BE swapped
    MAX_MESSAGE_SIZE,
    PORT_DIRECTION,
    REFRESH_PERIOD,
    &SAMPLING_PORT_ID,
    &RETURN_CODE
  );

  // TODO: could still be an issue, with using the args[].of.i32 directly due to LE/BE
  camw32_set__SAMPLING_PORT_ID_TYPE(&wasm_baseaddr[args[4].of.i32], (int32_t)SAMPLING_PORT_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[args[5].of.i32], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void WRITE_SAMPLING_MESSAGE (
  /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
  /*in */ MESSAGE_ADDR_TYPE          MESSAGE_ADDR,     /* by reference */
  /*in */ MESSAGE_SIZE_TYPE          LENGTH,
  /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__WRITE_SAMPLING_MESSAGE = "(iiii)";
wasm_trap_t* WASM32_WRITE_SAMPLING_MESSAGE(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);


  SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID;
  SAMPLING_PORT_ID = (SAMPLING_PORT_ID_TYPE)camw32_get__SAMPLING_PORT_ID_TYPE((uint8_t*)&args[0].of.i32);
  int32_t MESSAGE_ADDR; /* is a pointer / address into Wasm linear memory */
  MESSAGE_ADDR = camw32_get__MESSAGE_ADDR_TYPE((uint8_t*)&args[1].of.i32);
  MESSAGE_SIZE_TYPE LENGTH;
  LENGTH = (MESSAGE_SIZE_TYPE)camw32_get__MESSAGE_SIZE_TYPE((uint8_t*)&args[2].of.i32);
  RETURN_CODE_TYPE RETURN_CODE;

  WRITE_SAMPLING_MESSAGE(
    SAMPLING_PORT_ID,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[MESSAGE_ADDR],
    LENGTH,
    &RETURN_CODE
  );

  // TODO: could still be an issue, with using the args[].of.i32 directly due to LE/BE
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[args[3].of.i32], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void READ_SAMPLING_MESSAGE (
  /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
  /*in */ MESSAGE_ADDR_TYPE          MESSAGE_ADDR,
    /* The message address is passed IN, although */
    /* the respective message is passed OUT       */
  /*out*/ MESSAGE_SIZE_TYPE          *LENGTH,
  /*out*/ VALIDITY_TYPE              *VALIDITY,
  /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__READ_SAMPLING_MESSAGE = "(iiiii)";
wasm_trap_t* WASM32_READ_SAMPLING_MESSAGE(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);


  SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID;
  SAMPLING_PORT_ID = (SAMPLING_PORT_ID_TYPE)camw32_get__SAMPLING_PORT_ID_TYPE((uint8_t*)&args[0].of.i32);
  int32_t MESSAGE_ADDR; /* is a pointer / address into Wasm linear memory */
  MESSAGE_ADDR = camw32_get__MESSAGE_ADDR_TYPE((uint8_t*)&args[1].of.i32);
  MESSAGE_SIZE_TYPE LENGTH;
  VALIDITY_TYPE VALIDITY;
  RETURN_CODE_TYPE RETURN_CODE;

  READ_SAMPLING_MESSAGE(
    SAMPLING_PORT_ID,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[MESSAGE_ADDR],
    &LENGTH,
    &VALIDITY,
    &RETURN_CODE
  );

  // TODO: could still be an issue, with using the args[].of.i32 directly due to LE/BE
  camw32_set__MESSAGE_SIZE_TYPE(&wasm_baseaddr[args[2].of.i32], (int32_t)LENGTH);
  camw32_set__VALIDITY_TYPE(&wasm_baseaddr[args[3].of.i32], (int32_t)VALIDITY);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[args[4].of.i32], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void GET_SAMPLING_PORT_ID (
  /*in */ SAMPLING_PORT_NAME_TYPE    SAMPLING_PORT_NAME,
  /*out*/ SAMPLING_PORT_ID_TYPE      *SAMPLING_PORT_ID,
  /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__GET_SAMPLING_PORT_ID = "(iii)";
wasm_trap_t* WASM32_GET_SAMPLING_PORT_ID(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);


  SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_SAMPLING_PORT_ID(
    (char*)&wasm_baseaddr[args[0].of.i32], // FIXME: address args[].of.i32 could be LE/BE swapped
    &SAMPLING_PORT_ID,
    &RETURN_CODE
  );

  // TODO: could still be an issue, with using the args[].of.i32 directly due to LE/BE
  camw32_set__SAMPLING_PORT_ID_TYPE(&wasm_baseaddr[args[1].of.i32], (int32_t)SAMPLING_PORT_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[args[2].of.i32], (int32_t)RETURN_CODE);

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


  SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID;
  SAMPLING_PORT_ID = (SAMPLING_PORT_ID_TYPE)camw32_get__SAMPLING_PORT_ID_TYPE((uint8_t*)&args[0].of.i32);
  SAMPLING_PORT_STATUS_TYPE SAMPLING_PORT_STATUS;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_SAMPLING_PORT_STATUS(
    SAMPLING_PORT_ID,
    &SAMPLING_PORT_STATUS,
    &RETURN_CODE
  );

#if 0
typedef struct {
  MESSAGE_SIZE_TYPE       MAX_MESSAGE_SIZE;   /* port size */
  PORT_DIRECTION_TYPE     PORT_DIRECTION;     /* port direction */
  SYSTEM_TIME_TYPE        REFRESH_PERIOD;     /* refresh period */
  VALIDITY_TYPE           LAST_MSG_VALIDITY;  /* message validity */
} SAMPLING_PORT_STATUS_TYPE;
#endif

// TODO: could still be an issue, with using the args[].of.i32 directly due to LE/BE
  uint8_t* SAMPLING_PORT_STATUS__guest = &wasm_baseaddr[args[1].of.i32];
  camw32_set__SAMPLING_PORT_STATUS_TYPE__MAX_MESSAGE_SIZE(SAMPLING_PORT_STATUS__guest, SAMPLING_PORT_STATUS.MAX_MESSAGE_SIZE);
  camw32_set__SAMPLING_PORT_STATUS_TYPE__PORT_DIRECTION(SAMPLING_PORT_STATUS__guest, SAMPLING_PORT_STATUS.PORT_DIRECTION);
  camw32_set__SAMPLING_PORT_STATUS_TYPE__REFRESH_PERIOD(SAMPLING_PORT_STATUS__guest, SAMPLING_PORT_STATUS.REFRESH_PERIOD);
  camw32_set__SAMPLING_PORT_STATUS_TYPE__LAST_MSG_VALIDITY(SAMPLING_PORT_STATUS__guest, SAMPLING_PORT_STATUS.LAST_MSG_VALIDITY);

  // TODO: could still be an issue, with using the args[].of.i32 directly due to LE/BE
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[args[2].of.i32], (int32_t)RETURN_CODE);

  return NULL;
}
