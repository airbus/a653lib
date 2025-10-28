// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: QUEUING PORT

#include <endian.h>
#include "arinc653_wasm32_helper.h"
#include "arinc653_part1_apex_queuing_port_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_inc/a653Queuing.h"


WASM_HOST_FUNCTION(CREATE_QUEUING_PORT, wasm_baseaddr,
{
  MESSAGE_SIZE_TYPE MAX_MESSAGE_SIZE;
  MAX_MESSAGE_SIZE = (MESSAGE_SIZE_TYPE)le32toh(args_and_results[1].i32);
  MESSAGE_RANGE_TYPE MAX_NB_MESSAGE;
  MAX_NB_MESSAGE = (MESSAGE_RANGE_TYPE)le32toh(args_and_results[2].i32);
  PORT_DIRECTION_TYPE PORT_DIRECTION;
  PORT_DIRECTION = (PORT_DIRECTION_TYPE)le32toh(args_and_results[3].i32);
  QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE;
  QUEUING_DISCIPLINE = (QUEUING_DISCIPLINE_TYPE)le32toh(args_and_results[4].i32);
  QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  CREATE_QUEUING_PORT(
    (char*)&wasm_baseaddr[le32toh(args_and_results[0].i32)], // FIXME: only safe as long as char[]
    MAX_MESSAGE_SIZE,
    MAX_NB_MESSAGE,
    PORT_DIRECTION,
    QUEUING_DISCIPLINE,
    &QUEUING_PORT_ID,
    &RETURN_CODE
  );

  camw32_set__QUEUING_PORT_ID_TYPE(&wasm_baseaddr[le32toh(args_and_results[5].i32)], (int32_t)QUEUING_PORT_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[6].i32)], (int32_t)RETURN_CODE);
})


WASM_HOST_FUNCTION(SEND_QUEUING_MESSAGE, wasm_baseaddr,
{
  QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
  QUEUING_PORT_ID = (QUEUING_PORT_ID_TYPE)le32toh(args_and_results[0].i32);
  int32_t MESSAGE_ADDR; /* is a pointer / address into Wasm linear memory */
  MESSAGE_ADDR = le32toh(args_and_results[1].i32);
  MESSAGE_SIZE_TYPE LENGTH;
  LENGTH = (MESSAGE_SIZE_TYPE)le32toh(args_and_results[2].i32);
  SYSTEM_TIME_TYPE TIME_OUT;
  TIME_OUT = (SYSTEM_TIME_TYPE)le64toh(args_and_results[3].i64);
  RETURN_CODE_TYPE RETURN_CODE;

  SEND_QUEUING_MESSAGE(
    QUEUING_PORT_ID,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[MESSAGE_ADDR], // FIXME: only safe as long as char[]
    LENGTH,
    TIME_OUT,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[4].i32)], (int32_t)RETURN_CODE);
})


WASM_HOST_FUNCTION(RECEIVE_QUEUING_MESSAGE, wasm_baseaddr,
{
  QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
  QUEUING_PORT_ID = (QUEUING_PORT_ID_TYPE)le32toh(args_and_results[0].i32);
  SYSTEM_TIME_TYPE TIME_OUT;
  TIME_OUT = (SYSTEM_TIME_TYPE)le64toh(args_and_results[1].i64);
  int32_t MESSAGE_ADDR; /* is a pointer / address into Wasm linear memory */
  MESSAGE_ADDR = le32toh(args_and_results[2].i32);
  MESSAGE_SIZE_TYPE LENGTH;
  RETURN_CODE_TYPE RETURN_CODE;

  RECEIVE_QUEUING_MESSAGE(
    QUEUING_PORT_ID,
    TIME_OUT,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[MESSAGE_ADDR], // FIXME: only safe as long as char[]
    &LENGTH,
    &RETURN_CODE
  );

  camw32_set__MESSAGE_SIZE_TYPE(&wasm_baseaddr[le32toh(args_and_results[3].i32)], (int32_t)LENGTH);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[4].i32)], (int32_t)RETURN_CODE);
})


WASM_HOST_FUNCTION(GET_QUEUING_PORT_ID, wasm_baseaddr,
{
  QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_QUEUING_PORT_ID(
    (char*)&wasm_baseaddr[le32toh(args_and_results[0].i32)], // FIXME: only safe as long as char[]
    &QUEUING_PORT_ID,
    &RETURN_CODE
  );

  camw32_set__QUEUING_PORT_ID_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)QUEUING_PORT_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);
})


WASM_HOST_FUNCTION(GET_QUEUING_PORT_STATUS, wasm_baseaddr,
{
  QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
  QUEUING_PORT_ID = (QUEUING_PORT_ID_TYPE)le32toh(args_and_results[0].i32);
  QUEUING_PORT_STATUS_TYPE QUEUING_PORT_STATUS;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_QUEUING_PORT_STATUS(
    QUEUING_PORT_ID,
    &QUEUING_PORT_STATUS,
    &RETURN_CODE
  );

  uint8_t* QUEUING_PORT_STATUS_guest = (uint8_t*)&wasm_baseaddr[le32toh(args_and_results[1].i32)];
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);

  camw32_set__QUEUING_PORT_STATUS_TYPE__NB_MESSAGE(QUEUING_PORT_STATUS_guest, QUEUING_PORT_STATUS.NB_MESSAGE);
  camw32_set__QUEUING_PORT_STATUS_TYPE__MAX_NB_MESSAGE(QUEUING_PORT_STATUS_guest, QUEUING_PORT_STATUS.MAX_NB_MESSAGE);
  camw32_set__QUEUING_PORT_STATUS_TYPE__MAX_MESSAGE_SIZE(QUEUING_PORT_STATUS_guest, QUEUING_PORT_STATUS.MAX_MESSAGE_SIZE);
  camw32_set__QUEUING_PORT_STATUS_TYPE__PORT_DIRECTION(QUEUING_PORT_STATUS_guest, QUEUING_PORT_STATUS.PORT_DIRECTION);
  camw32_set__QUEUING_PORT_STATUS_TYPE__WAITING_PROCESSES(QUEUING_PORT_STATUS_guest, QUEUING_PORT_STATUS.WAITING_PROCESSES);
})


WASM_HOST_FUNCTION(CLEAR_QUEUING_PORT, wasm_baseaddr,
{
  QUEUING_PORT_ID_TYPE QUEUING_PORT_ID;
  QUEUING_PORT_ID = (QUEUING_PORT_ID_TYPE)le32toh(args_and_results[0].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  CLEAR_QUEUING_PORT(
    QUEUING_PORT_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);
})
