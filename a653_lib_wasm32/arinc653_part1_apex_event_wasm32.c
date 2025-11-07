// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: EVENT

#include <endian.h>
#include "arinc653_wasm32_helper.h"
#include "arinc653_part1_apex_event_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "a653Event.h"


WASM32_HOST_FUNCTION__iii(CREATE_EVENT, wasm_baseaddr,
{
  int32_t EVENT_NAME; /* is a pointer / address into Wasm linear memory */
  EVENT_NAME = (int32_t)le32toh(GET_ARG_i32(0));

  EVENT_ID_TYPE EVENT_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  CREATE_EVENT(
    (char*)&wasm_baseaddr[EVENT_NAME],
    &EVENT_ID,
    &RETURN_CODE
  );

  camw32_set__EVENT_ID_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)EVENT_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION__ii(SET_EVENT, wasm_baseaddr,
{
  EVENT_ID_TYPE EVENT_ID;
  EVENT_ID = (EVENT_ID_TYPE)le32toh(GET_ARG_i32(0));

  RETURN_CODE_TYPE RETURN_CODE;

  SET_EVENT(
    EVENT_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION__ii(RESET_EVENT, wasm_baseaddr,
{
  EVENT_ID_TYPE EVENT_ID;
  EVENT_ID = (EVENT_ID_TYPE)le32toh(GET_ARG_i32(0));

  RETURN_CODE_TYPE RETURN_CODE;

  RESET_EVENT(
    EVENT_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION__iIi(WAIT_EVENT, wasm_baseaddr,
{
  EVENT_ID_TYPE EVENT_ID;
  EVENT_ID = (EVENT_ID_TYPE)le32toh(GET_ARG_i32(0));
  SYSTEM_TIME_TYPE TIME_OUT;
  TIME_OUT = (SYSTEM_TIME_TYPE)le64toh(GET_ARG_i64(0));

  RETURN_CODE_TYPE RETURN_CODE;

  WAIT_EVENT(
    EVENT_ID,
    TIME_OUT,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION__iii(GET_EVENT_ID, wasm_baseaddr,
{
  int32_t EVENT_NAME; /* is a pointer / address into Wasm linear memory */
  EVENT_NAME = (int32_t)le32toh(GET_ARG_i32(0));

  EVENT_ID_TYPE EVENT_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_EVENT_ID(
    (char*)&wasm_baseaddr[EVENT_NAME],
    &EVENT_ID,
    &RETURN_CODE
  );

  camw32_set__EVENT_ID_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)EVENT_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION__iii(GET_EVENT_STATUS, wasm_baseaddr,
{
  EVENT_ID_TYPE EVENT_ID;
  EVENT_ID = (EVENT_ID_TYPE)le32toh(GET_ARG_i32(0));

  EVENT_STATUS_TYPE EVENT_STATUS;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_EVENT_STATUS(
    EVENT_ID,
    &EVENT_STATUS,
    &RETURN_CODE
  );

  uint8_t* EVENT_STATUS_guest = (uint8_t*)&wasm_baseaddr[le32toh(GET_ARG_i32(1))];
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);

  camw32_set__EVENT_STATUS_TYPE__EVENT_STATE(EVENT_STATUS_guest, EVENT_STATUS.EVENT_STATE);
  camw32_set__EVENT_STATUS_TYPE__WAITING_PROCESSES(EVENT_STATUS_guest, EVENT_STATUS.WAITING_PROCESSES);
})
