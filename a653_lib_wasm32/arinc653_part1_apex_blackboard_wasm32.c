// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: BLACKBOARD

#include <endian.h>
#include "arinc653_wasm32_helper.h"
#include "arinc653_part1_apex_blackboard_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "a653Blackboard.h"


WASM32_HOST_FUNCTION(CREATE_BLACKBOARD, wasm_baseaddr,
{
  int32_t BLACKBOARD_NAME; /* is a pointer / address into Wasm linear memory */
  BLACKBOARD_NAME = (int32_t)le32toh(GET_ARG_i32(0));
  MESSAGE_SIZE_TYPE MAX_MESSAGE_SIZE;
  MAX_MESSAGE_SIZE = (MESSAGE_SIZE_TYPE)le32toh(GET_ARG_i32(1));

  BLACKBOARD_ID_TYPE BLACKBOARD_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  CREATE_BLACKBOARD(
    (char*)&wasm_baseaddr[BLACKBOARD_NAME],
    MAX_MESSAGE_SIZE,
    &BLACKBOARD_ID,
    &RETURN_CODE
  );

  camw32_set__BLACKBOARD_ID_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)BLACKBOARD_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(3))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(DISPLAY_BLACKBOARD, wasm_baseaddr,
{
  BLACKBOARD_ID_TYPE BLACKBOARD_ID;
  BLACKBOARD_ID = (BLACKBOARD_ID_TYPE)le32toh(GET_ARG_i32(0));
  int32_t MESSAGE_ADDR; /* is a pointer / address into Wasm linear memory */
  MESSAGE_ADDR = (int32_t)le32toh(GET_ARG_i32(1));
  MESSAGE_SIZE_TYPE LENGTH;
  LENGTH = (MESSAGE_SIZE_TYPE)le32toh(GET_ARG_i32(2));

  RETURN_CODE_TYPE RETURN_CODE;

  DISPLAY_BLACKBOARD(
    BLACKBOARD_ID,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[MESSAGE_ADDR],
    LENGTH,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(3))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(READ_BLACKBOARD, wasm_baseaddr,
{
  BLACKBOARD_ID_TYPE BLACKBOARD_ID;
  BLACKBOARD_ID = (BLACKBOARD_ID_TYPE)le32toh(GET_ARG_i32(0));
  SYSTEM_TIME_TYPE TIME_OUT;
  TIME_OUT = (SYSTEM_TIME_TYPE)le64toh(GET_ARG_i64(1));
  int32_t MESSAGE_ADDR; /* is a pointer / address into Wasm linear memory */
  MESSAGE_ADDR = (int32_t)le32toh(GET_ARG_i32(2));

  MESSAGE_SIZE_TYPE LENGTH;
  RETURN_CODE_TYPE RETURN_CODE;

  READ_BLACKBOARD(
    BLACKBOARD_ID,
    TIME_OUT,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[MESSAGE_ADDR],
    &LENGTH,
    &RETURN_CODE
  );

  camw32_set__MESSAGE_SIZE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(3))], (int32_t)LENGTH);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(4))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(CLEAR_BLACKBOARD, wasm_baseaddr,
{
  BLACKBOARD_ID_TYPE BLACKBOARD_ID;
  BLACKBOARD_ID = (BLACKBOARD_ID_TYPE)le32toh(GET_ARG_i32(0));

  RETURN_CODE_TYPE RETURN_CODE;

  CLEAR_BLACKBOARD(
    BLACKBOARD_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(GET_BLACKBOARD_ID, wasm_baseaddr,
{
  int32_t BLACKBOARD_NAME; /* is a pointer / address into Wasm linear memory */
  BLACKBOARD_NAME = (int32_t)le32toh(GET_ARG_i32(0));

  BLACKBOARD_ID_TYPE BLACKBOARD_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_BLACKBOARD_ID(
    (char*)&wasm_baseaddr[BLACKBOARD_NAME],
    &BLACKBOARD_ID,
    &RETURN_CODE
  );

  camw32_set__BLACKBOARD_ID_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)BLACKBOARD_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(GET_BLACKBOARD_STATUS, wasm_baseaddr,
{
  BLACKBOARD_ID_TYPE BLACKBOARD_ID;
  BLACKBOARD_ID = (BLACKBOARD_ID_TYPE)le32toh(GET_ARG_i32(0));

  BLACKBOARD_STATUS_TYPE BLACKBOARD_STATUS;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_BLACKBOARD_STATUS(
    BLACKBOARD_ID,
    &BLACKBOARD_STATUS,
    &RETURN_CODE
  );

  uint8_t* BLACKBOARD_STATUS_guest = (uint8_t*)&wasm_baseaddr[le32toh(GET_ARG_i32(1))];
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);

  camw32_set__BLACKBOARD_STATUS_TYPE__EMPTY_INDICATOR(BLACKBOARD_STATUS_guest, BLACKBOARD_STATUS.EMPTY_INDICATOR);
  camw32_set__BLACKBOARD_STATUS_TYPE__MAX_MESSAGE_SIZE(BLACKBOARD_STATUS_guest, BLACKBOARD_STATUS.MAX_MESSAGE_SIZE);
  camw32_set__BLACKBOARD_STATUS_TYPE__WAITING_PROCESSES(BLACKBOARD_STATUS_guest, BLACKBOARD_STATUS.WAITING_PROCESSES);
})
