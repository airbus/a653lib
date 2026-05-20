// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: BUFFER

#include <endian.h>
#include "arinc653_wasm32_helper.h"
#include "arinc653_part1_apex_buffer_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "a653Buffer.h"


WASM32_HOST_FUNCTION(CREATE_BUFFER, wasm_baseaddr,
{
  int32_t BUFFER_NAME; /* is a pointer / address into Wasm linear memory */
  BUFFER_NAME = (int32_t)le32toh(GET_ARG_i32(0));
  MESSAGE_SIZE_TYPE MAX_MESSAGE_SIZE;
  MAX_MESSAGE_SIZE = (MESSAGE_SIZE_TYPE)le32toh(GET_ARG_i32(1));
  MESSAGE_RANGE_TYPE MAX_NB_MESSAGE;
  MAX_NB_MESSAGE = (MESSAGE_RANGE_TYPE)le32toh(GET_ARG_i32(2));
  QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE;
  QUEUING_DISCIPLINE = (QUEUING_DISCIPLINE_TYPE)le32toh(GET_ARG_i32(3));

  BUFFER_ID_TYPE BUFFER_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  CREATE_BUFFER(
    (char*)&wasm_baseaddr[BUFFER_NAME],
    MAX_MESSAGE_SIZE,
    MAX_NB_MESSAGE,
    QUEUING_DISCIPLINE,
    &BUFFER_ID,
    &RETURN_CODE
  );

  camw32_set__BUFFER_ID_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(4))], (int32_t)BUFFER_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(5))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(SEND_BUFFER, wasm_baseaddr,
{
  BUFFER_ID_TYPE BUFFER_ID;
  BUFFER_ID = (BUFFER_ID_TYPE)le32toh(GET_ARG_i32(0));
  int32_t MESSAGE_ADDR;
  MESSAGE_ADDR = (int32_t)le32toh(GET_ARG_i32(1));
  MESSAGE_SIZE_TYPE LENGTH;
  LENGTH = (MESSAGE_SIZE_TYPE)le32toh(GET_ARG_i32(2));
  SYSTEM_TIME_TYPE TIME_OUT;
  TIME_OUT = (SYSTEM_TIME_TYPE)le64toh(GET_ARG_i64(3));

  RETURN_CODE_TYPE RETURN_CODE;

  SEND_BUFFER(
    BUFFER_ID,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[MESSAGE_ADDR],
    LENGTH,
    TIME_OUT,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(4))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(RECEIVE_BUFFER, wasm_baseaddr,
{
  BUFFER_ID_TYPE BUFFER_ID;
  BUFFER_ID = (BUFFER_ID_TYPE)le32toh(GET_ARG_i32(0));
  SYSTEM_TIME_TYPE TIME_OUT;
  TIME_OUT = (SYSTEM_TIME_TYPE)le64toh(GET_ARG_i64(1));
  int32_t MESSAGE_ADDR;
  MESSAGE_ADDR = (int32_t)le32toh(GET_ARG_i32(2));

  MESSAGE_SIZE_TYPE LENGTH;
  RETURN_CODE_TYPE RETURN_CODE;

  RECEIVE_BUFFER(
    BUFFER_ID,
    TIME_OUT,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[MESSAGE_ADDR],
    &LENGTH,
    &RETURN_CODE
  );

  camw32_set__MESSAGE_SIZE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(3))], (int32_t)LENGTH);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(4))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(GET_BUFFER_ID, wasm_baseaddr,
{
  int32_t BUFFER_NAME; /* is a pointer / address into Wasm linear memory */
  BUFFER_NAME = (int32_t)le32toh(GET_ARG_i32(0));

  BUFFER_ID_TYPE BUFFER_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_BUFFER_ID(
    (char*)&wasm_baseaddr[BUFFER_NAME],
    &BUFFER_ID,
    &RETURN_CODE
  );

  camw32_set__BUFFER_ID_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)BUFFER_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(GET_BUFFER_STATUS, wasm_baseaddr,
{
  BUFFER_ID_TYPE BUFFER_ID; /* is a pointer / address into Wasm linear memory */
  BUFFER_ID = (int32_t)le32toh(GET_ARG_i32(0));

  BUFFER_STATUS_TYPE BUFFER_STATUS;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_BUFFER_STATUS(
    BUFFER_ID,
    &BUFFER_STATUS,
    &RETURN_CODE
  );

  uint8_t* BUFFER_STATUS_guest = (uint8_t*)&wasm_baseaddr[le32toh(GET_ARG_i32(1))];
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);

  camw32_set__BUFFER_STATUS_TYPE__NB_MESSAGE(BUFFER_STATUS_guest, BUFFER_STATUS.NB_MESSAGE);
  camw32_set__BUFFER_STATUS_TYPE__MAX_NB_MESSAGE(BUFFER_STATUS_guest, BUFFER_STATUS.MAX_NB_MESSAGE);
  camw32_set__BUFFER_STATUS_TYPE__MAX_MESSAGE_SIZE(BUFFER_STATUS_guest, BUFFER_STATUS.MAX_MESSAGE_SIZE);
  camw32_set__BUFFER_STATUS_TYPE__WAITING_PROCESSES(BUFFER_STATUS_guest, BUFFER_STATUS.WAITING_PROCESSES);
})
