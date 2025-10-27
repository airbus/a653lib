// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 2: APEX Interface: SAMPLING PORT EXTENSION

#include <endian.h>
#include "arinc653_part2_apex_sampling_port_extension_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_inc/a653Sampling.h"


#if 0
extern void READ_UPDATED_SAMPLING_MESSAGE (
  /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
  /*in */ MESSAGE_ADDR_TYPE          MESSAGE_ADDR,
  /*out*/ MESSAGE_SIZE_TYPE          *LENGTH,
  /*out*/ UPDATED_TYPE               *UPDATED,
  /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );
#endif
wasm_trap_t* WASM32_READ_UPDATED_SAMPLING_MESSAGE(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID;
  SAMPLING_PORT_ID = (SAMPLING_PORT_ID_TYPE)le32toh(args_and_results[0].i32);
  int32_t MESSAGE_ADDR; /* is a pointer / address into Wasm linear memory */
  MESSAGE_ADDR = (int32_t)le32toh(args_and_results[1].i32);
  MESSAGE_SIZE_TYPE LENGTH;
  UPDATED_TYPE UPDATED;
  RETURN_CODE_TYPE RETURN_CODE;

  READ_UPDATED_SAMPLING_MESSAGE(
    SAMPLING_PORT_ID,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[MESSAGE_ADDR],
    &LENGTH,
    &UPDATED,
    &RETURN_CODE
  );

  camw32_set__MESSAGE_SIZE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)LENGTH);
  camw32_set__UPDATED_TYPE(&wasm_baseaddr[le32toh(args_and_results[3].i32)], (int32_t)UPDATED);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[4].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void GET_SAMPLING_PORT_CURRENT_STATUS (
  /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
  /*out*/ SAMPLING_PORT_CURRENT_STATUS_TYPE
  *SAMPLING_PORT_CURRENT_STATUS,
  / *out*/ RETURN_CODE_TYPE           *RETURN_CODE );*
#endif
wasm_trap_t* WASM32_GET_SAMPLING_PORT_CURRENT_STATUS(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID;
  SAMPLING_PORT_ID = (SAMPLING_PORT_ID_TYPE)le32toh(args_and_results[0].i32);
  SAMPLING_PORT_CURRENT_STATUS_TYPE SAMPLING_PORT_CURRENT_STATUS;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_SAMPLING_PORT_CURRENT_STATUS(
    SAMPLING_PORT_ID,
    &SAMPLING_PORT_CURRENT_STATUS,
    &RETURN_CODE
  );

  // TODO: could still be an issue, with using the args_and_results[].i32 directly due to LE/BE
  uint8_t* SAMPLING_PORT_CURRENT_STATUS__guest = (uint8_t*)&wasm_baseaddr[le32toh(args_and_results[1].i32)];
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);

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

  camw32_set__SAMPLING_PORT_CURRENT_STATUS_TYPE__REFRESH_PERIOD(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS.REFRESH_PERIOD);
  camw32_set__SAMPLING_PORT_CURRENT_STATUS_TYPE__TIME_STAMP(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS.TIME_STAMP);
  camw32_set__SAMPLING_PORT_CURRENT_STATUS_TYPE__MAX_MESSAGE_SIZE(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS.MAX_MESSAGE_SIZE);
  camw32_set__SAMPLING_PORT_CURRENT_STATUS_TYPE__PORT_DIRECTION(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS.PORT_DIRECTION);
  camw32_set__SAMPLING_PORT_CURRENT_STATUS_TYPE__MESSAGE_AGE(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS.MESSAGE_AGE);
  camw32_set__SAMPLING_PORT_CURRENT_STATUS_TYPE__UPDATED(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS.UPDATED);

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
wasm_trap_t* WASM32_READ_SAMPLING_MESSAGE_CONDITIONAL(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID;
  SAMPLING_PORT_ID = (SAMPLING_PORT_ID_TYPE)le32toh(args_and_results[0].i32);
  SYSTEM_TIME_TYPE REF_TIME_STAMP;
  REF_TIME_STAMP = (SYSTEM_TIME_TYPE)le64toh(args_and_results[1].i64);
  int32_t MESSAGE_ADDR; /* is a pointer / address into Wasm linear memory */
  MESSAGE_ADDR = (int32_t)le32toh(args_and_results[2].i32);
  MESSAGE_SIZE_TYPE LENGTH;
  SYSTEM_TIME_TYPE TIME_STAMP;
  RETURN_CODE_TYPE RETURN_CODE;

  READ_SAMPLING_MESSAGE_CONDITIONAL(
    SAMPLING_PORT_ID,
    REF_TIME_STAMP,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[MESSAGE_ADDR],
    &LENGTH,
    &TIME_STAMP,
    &RETURN_CODE
  );

  camw32_set__MESSAGE_SIZE_TYPE(&wasm_baseaddr[le32toh(args_and_results[3].i32)], (int32_t)LENGTH);
  camw32_set__SYSTEM_TIME_TYPE(&wasm_baseaddr[le32toh(args_and_results[4].i32)], (int64_t)TIME_STAMP);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[5].i32)], (int32_t)RETURN_CODE);

  return NULL;
}
