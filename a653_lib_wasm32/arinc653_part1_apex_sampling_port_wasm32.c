// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: SAMPLING PORT

#include <endian.h>
#include "arinc653_wasm32_helper.h"
#include "arinc653_part1_apex_sampling_port_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_inc/a653Sampling.h"


WASM_HOST_FUNCTION(CREATE_SAMPLING_PORT, wasm_baseaddr,
{
  MESSAGE_SIZE_TYPE MAX_MESSAGE_SIZE;
  MAX_MESSAGE_SIZE = (MESSAGE_SIZE_TYPE)le32toh(args_and_results[1].i32);
  PORT_DIRECTION_TYPE PORT_DIRECTION;
  PORT_DIRECTION = (PORT_DIRECTION_TYPE)le32toh(args_and_results[2].i32);
  SYSTEM_TIME_TYPE REFRESH_PERIOD;
  REFRESH_PERIOD = (SYSTEM_TIME_TYPE)le64toh(args_and_results[3].i64);
  SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  CREATE_SAMPLING_PORT(
    (char*)&wasm_baseaddr[le32toh(args_and_results[0].i32)], // FIXME: only safe as long as char[]
    MAX_MESSAGE_SIZE,
    PORT_DIRECTION,
    REFRESH_PERIOD,
    &SAMPLING_PORT_ID,
    &RETURN_CODE
  );

  camw32_set__SAMPLING_PORT_ID_TYPE(&wasm_baseaddr[le32toh(args_and_results[4].i32)], (int32_t)SAMPLING_PORT_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[5].i32)], (int32_t)RETURN_CODE);
})


WASM_HOST_FUNCTION(WRITE_SAMPLING_MESSAGE, wasm_baseaddr,
{
  SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID;
  SAMPLING_PORT_ID = (SAMPLING_PORT_ID_TYPE)le32toh(args_and_results[0].i32);
  int32_t MESSAGE_ADDR; /* is a pointer / address into Wasm linear memory */
  MESSAGE_ADDR = le32toh(args_and_results[1].i32);
  MESSAGE_SIZE_TYPE LENGTH;
  LENGTH = (MESSAGE_SIZE_TYPE)le32toh(args_and_results[2].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  WRITE_SAMPLING_MESSAGE(
    SAMPLING_PORT_ID,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[MESSAGE_ADDR], // FIXME: only safe as long as char[]
    LENGTH,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[3].i32)], (int32_t)RETURN_CODE);
})


WASM_HOST_FUNCTION(READ_SAMPLING_MESSAGE, wasm_baseaddr,
{
  SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID;
  SAMPLING_PORT_ID = (SAMPLING_PORT_ID_TYPE)le32toh(args_and_results[0].i32);
  int32_t MESSAGE_ADDR; /* is a pointer / address into Wasm linear memory */
  MESSAGE_ADDR = le32toh(args_and_results[1].i32);
  MESSAGE_SIZE_TYPE LENGTH;
  VALIDITY_TYPE VALIDITY;
  RETURN_CODE_TYPE RETURN_CODE;

  READ_SAMPLING_MESSAGE(
    SAMPLING_PORT_ID,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[MESSAGE_ADDR], // FIXME: only safe as long as char[]
    &LENGTH,
    &VALIDITY,
    &RETURN_CODE
  );

  camw32_set__MESSAGE_SIZE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)LENGTH);
  camw32_set__VALIDITY_TYPE(&wasm_baseaddr[le32toh(args_and_results[3].i32)], (int32_t)VALIDITY);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[4].i32)], (int32_t)RETURN_CODE);
})


WASM_HOST_FUNCTION(GET_SAMPLING_PORT_ID, wasm_baseaddr,
{
  SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_SAMPLING_PORT_ID(
    (char*)&wasm_baseaddr[le32toh(args_and_results[0].i32)], // FIXME: only safe as long as char[]
    &SAMPLING_PORT_ID,
    &RETURN_CODE
  );

  camw32_set__SAMPLING_PORT_ID_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)SAMPLING_PORT_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);
})


WASM_HOST_FUNCTION(GET_SAMPLING_PORT_STATUS, wasm_baseaddr,
{
  SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID;
  SAMPLING_PORT_ID = (SAMPLING_PORT_ID_TYPE)le32toh(args_and_results[0].i32);
  SAMPLING_PORT_STATUS_TYPE SAMPLING_PORT_STATUS;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_SAMPLING_PORT_STATUS(
    SAMPLING_PORT_ID,
    &SAMPLING_PORT_STATUS,
    &RETURN_CODE
  );

  uint8_t* SAMPLING_PORT_STATUS__guest = &wasm_baseaddr[le32toh(args_and_results[1].i32)];
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);

  camw32_set__SAMPLING_PORT_STATUS_TYPE__MAX_MESSAGE_SIZE(SAMPLING_PORT_STATUS__guest, SAMPLING_PORT_STATUS.MAX_MESSAGE_SIZE);
  camw32_set__SAMPLING_PORT_STATUS_TYPE__PORT_DIRECTION(SAMPLING_PORT_STATUS__guest, SAMPLING_PORT_STATUS.PORT_DIRECTION);
  camw32_set__SAMPLING_PORT_STATUS_TYPE__REFRESH_PERIOD(SAMPLING_PORT_STATUS__guest, SAMPLING_PORT_STATUS.REFRESH_PERIOD);
  camw32_set__SAMPLING_PORT_STATUS_TYPE__LAST_MSG_VALIDITY(SAMPLING_PORT_STATUS__guest, SAMPLING_PORT_STATUS.LAST_MSG_VALIDITY);
})
