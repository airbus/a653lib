// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: MUTEX

#include <endian.h>
#include "arinc653_wasm32_helper.h"
#include "arinc653_part1_apex_mutex_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "a653Mutex.h"


WASM32_HOST_FUNCTION__iiiii(CREATE_MUTEX, wasm_baseaddr,
{
  int32_t MUTEX_NAME; /* is a pointer / address into Wasm linear memory */
  MUTEX_NAME = (int32_t)le32toh(GET_ARG_i32(0));
  PRIORITY_TYPE MUTEX_PRIORITY;
  MUTEX_PRIORITY = (PRIORITY_TYPE)le32toh(GET_ARG_i32(1));
  QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE;
  QUEUING_DISCIPLINE = (QUEUING_DISCIPLINE_TYPE)le32toh(GET_ARG_i32(2));

  MUTEX_ID_TYPE MUTEX_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  CREATE_MUTEX(
    (char*)&wasm_baseaddr[MUTEX_NAME],
    MUTEX_PRIORITY,
    QUEUING_DISCIPLINE,
    &MUTEX_ID,
    &RETURN_CODE
  );

  camw32_set__MUTEX_ID_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(3))], (int32_t)MUTEX_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(4))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION__iIi(ACQUIRE_MUTEX, wasm_baseaddr,
{
  MUTEX_ID_TYPE MUTEX_ID;
  MUTEX_ID = (MUTEX_ID_TYPE)le32toh(GET_ARG_i32(0));
  SYSTEM_TIME_TYPE TIME_OUT;
  TIME_OUT = (SYSTEM_TIME_TYPE)le64toh(GET_ARG_i64(1));

  RETURN_CODE_TYPE RETURN_CODE;

  ACQUIRE_MUTEX(
    MUTEX_ID,
    TIME_OUT,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION__ii(RELEASE_MUTEX, wasm_baseaddr,
{
  MUTEX_ID_TYPE MUTEX_ID;
  MUTEX_ID = (MUTEX_ID_TYPE)le32toh(GET_ARG_i32(0));

  RETURN_CODE_TYPE RETURN_CODE;

  RELEASE_MUTEX(
    MUTEX_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION__iii(RESET_MUTEX, wasm_baseaddr,
{
  MUTEX_ID_TYPE MUTEX_ID;
  MUTEX_ID = (MUTEX_ID_TYPE)le32toh(GET_ARG_i32(0));
  PROCESS_ID_TYPE PROCESS_ID;
  PROCESS_ID = (PROCESS_ID_TYPE)le32toh(GET_ARG_i32(1));

  RETURN_CODE_TYPE RETURN_CODE;

  RESET_MUTEX(
    MUTEX_ID,
    PROCESS_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION__iii(GET_MUTEX_ID, wasm_baseaddr,
{
  int32_t MUTEX_NAME; /* is a pointer / address into Wasm linear memory */
  MUTEX_NAME = (int32_t)le32toh(GET_ARG_i32(0));

  MUTEX_ID_TYPE MUTEX_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_MUTEX_ID(
    (char*)&wasm_baseaddr[MUTEX_NAME],
    &MUTEX_ID,
    &RETURN_CODE
  );

  camw32_set__MUTEX_ID_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)MUTEX_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION__iii(GET_MUTEX_STATUS, wasm_baseaddr,
{
  MUTEX_ID_TYPE MUTEX_ID;
  MUTEX_ID = (MUTEX_ID_TYPE)le32toh(GET_ARG_i32(0));

  MUTEX_STATUS_TYPE MUTEX_STATUS;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_MUTEX_STATUS(
    MUTEX_ID,
    &MUTEX_STATUS,
    &RETURN_CODE
  );

  uint8_t* MUTEX_STATUS_guest = (uint8_t*)&wasm_baseaddr[le32toh(GET_ARG_i32(1))];
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);

  camw32_set__MUTEX_STATUS_TYPE__MUTEX_OWNER(MUTEX_STATUS_guest, MUTEX_STATUS.MUTEX_OWNER);
  camw32_set__MUTEX_STATUS_TYPE__MUTEX_STATE(MUTEX_STATUS_guest, MUTEX_STATUS.MUTEX_STATE);
  camw32_set__MUTEX_STATUS_TYPE__MUTEX_PRIORITY(MUTEX_STATUS_guest, MUTEX_STATUS.MUTEX_PRIORITY);
  camw32_set__MUTEX_STATUS_TYPE__LOCK_COUNT(MUTEX_STATUS_guest, MUTEX_STATUS.LOCK_COUNT);
  camw32_set__MUTEX_STATUS_TYPE__WAITING_PROCESSES(MUTEX_STATUS_guest, MUTEX_STATUS.WAITING_PROCESSES);
})


WASM32_HOST_FUNCTION__iii(GET_PROCESS_MUTEX_STATE, wasm_baseaddr,
{
  PROCESS_ID_TYPE PROCESS_ID;
  PROCESS_ID = (PROCESS_ID_TYPE)le32toh(GET_ARG_i32(0));

  MUTEX_ID_TYPE MUTEX_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_PROCESS_MUTEX_STATE(
    PROCESS_ID,
    &MUTEX_ID,
    &RETURN_CODE
  );

  camw32_set__MUTEX_ID_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)MUTEX_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);
})
