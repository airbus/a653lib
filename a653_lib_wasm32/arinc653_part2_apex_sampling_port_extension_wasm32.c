// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 2: APEX Interface: SAMPLING PORT EXTENSION

#include <endian.h>
#include "arinc653_wasm32_helper.h"
#include "arinc653_part2_apex_sampling_port_extension_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_inc/a653Sampling.h"


WASM_HOST_FUNCTION(READ_UPDATED_SAMPLING_MESSAGE, wasm_baseaddr,
{
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
})


WASM_HOST_FUNCTION(GET_SAMPLING_PORT_CURRENT_STATUS, wasm_baseaddr,
{
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

  camw32_set__SAMPLING_PORT_CURRENT_STATUS_TYPE__REFRESH_PERIOD(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS.REFRESH_PERIOD);
  camw32_set__SAMPLING_PORT_CURRENT_STATUS_TYPE__TIME_STAMP(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS.TIME_STAMP);
  camw32_set__SAMPLING_PORT_CURRENT_STATUS_TYPE__MAX_MESSAGE_SIZE(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS.MAX_MESSAGE_SIZE);
  camw32_set__SAMPLING_PORT_CURRENT_STATUS_TYPE__PORT_DIRECTION(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS.PORT_DIRECTION);
  camw32_set__SAMPLING_PORT_CURRENT_STATUS_TYPE__MESSAGE_AGE(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS.MESSAGE_AGE);
  camw32_set__SAMPLING_PORT_CURRENT_STATUS_TYPE__UPDATED(SAMPLING_PORT_CURRENT_STATUS__guest, SAMPLING_PORT_CURRENT_STATUS.UPDATED);
})


WASM_HOST_FUNCTION(READ_SAMPLING_MESSAGE_CONDITIONAL, wasm_baseaddr,
{
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
})
