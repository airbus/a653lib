// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: PARTITION


#include <endian.h>
#include "arinc653_wasm32_helper.h"
#include "arinc653_part1_apex_partition_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_inc/a653Partition.h"


WASM32_HOST_FUNCTION__ii(GET_PARTITION_STATUS, wasm_baseaddr,
{
  PARTITION_STATUS_TYPE STATUS;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_PARTITION_STATUS(
    &STATUS,
    &RETURN_CODE
  );

  uint8_t* STATUS_guest = &wasm_baseaddr[le32toh(GET_ARG_i32(0))];
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);

  camw32_set__PARTITION_STATUS_TYPE__PERIOD(STATUS_guest, STATUS.PERIOD);
  camw32_set__PARTITION_STATUS_TYPE__DURATION(STATUS_guest, STATUS.DURATION);
  camw32_set__PARTITION_STATUS_TYPE__IDENTIFIER(STATUS_guest, STATUS.IDENTIFIER);
  camw32_set__PARTITION_STATUS_TYPE__LOCK_LEVEL(STATUS_guest, STATUS.LOCK_LEVEL);
  camw32_set__PARTITION_STATUS_TYPE__OPERATING_MODE(STATUS_guest, STATUS.OPERATING_MODE);
  camw32_set__PARTITION_STATUS_TYPE__START_CONDITION(STATUS_guest, STATUS.START_CONDITION);
  camw32_set__PARTITION_STATUS_TYPE__NUM_ASSIGNED_CORES(STATUS_guest, STATUS.NUM_ASSIGNED_CORES);
})


WASM32_HOST_FUNCTION__ii(SET_PARTITION_MODE, wasm_baseaddr,
{
  OPERATING_MODE_TYPE OPERATING_MODE;
  OPERATING_MODE = (OPERATING_MODE_TYPE)le32toh(GET_ARG_i32(0));
  RETURN_CODE_TYPE RETURN_CODE;

  SET_PARTITION_MODE(
    OPERATING_MODE,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})
