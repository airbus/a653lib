// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: TIME

#include <endian.h>
#include "arinc653_wasm32_helper.h"
#include "arinc653_part1_apex_time_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_inc/a653Time.h"


WASM32_HOST_FUNCTION__Ii(TIMED_WAIT, wasm_baseaddr,
{
  SYSTEM_TIME_TYPE DELAY_TIME;
  DELAY_TIME = (SYSTEM_TIME_TYPE)le64toh(GET_ARG_i64(0));
  RETURN_CODE_TYPE RETURN_CODE;

  TIMED_WAIT(
    DELAY_TIME,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION__i(PERIODIC_WAIT, wasm_baseaddr,
{
  RETURN_CODE_TYPE RETURN_CODE;

  PERIODIC_WAIT(
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(0))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION__ii(GET_TIME, wasm_baseaddr,
{
  SYSTEM_TIME_TYPE SYSTEM_TIME;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_TIME(
    &SYSTEM_TIME,
    &RETURN_CODE
  );

  camw32_set__SYSTEM_TIME_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(0))], (int64_t)SYSTEM_TIME);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION__Ii(REPLENISH, wasm_baseaddr,
{
  SYSTEM_TIME_TYPE BUDGET_TIME;
  BUDGET_TIME = (SYSTEM_TIME_TYPE)le64toh(GET_ARG_i64(0));
  RETURN_CODE_TYPE RETURN_CODE;

  REPLENISH(
    BUDGET_TIME,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})
