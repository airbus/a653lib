// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: PROCESS

#include <endian.h>
#include <pthread.h>
#include <stdio.h>
#include "arinc653_wasm32_helper.h"
#include "generic_helper.h"
#include "arinc653_part1_apex_process_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_lib/a653_i_process.h"
#include "../a653_inc/a653Process.h"


WASM32_HOST_FUNCTION(GET_PROCESS_ID, wasm_baseaddr,
{
  PROCESS_ID_TYPE PROCESS_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_PROCESS_ID(
    (char*)&wasm_baseaddr[le32toh(GET_ARG_i32(0))],  // FIXME: only safe as long as char[]
    &PROCESS_ID,
    &RETURN_CODE
  );

  camw32_set__PROCESS_ID_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)PROCESS_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);
})



extern wasm_prcs_info_t wasm_prcs_info;
extern prcs_info_t *prcs_info;

void *wasm_trampoline(void) {
  /* trying to reverse engineer the index within the a653lib */
  pthread_t tid = pthread_self();

  int ret = 0;
  for (unsigned i = 0; i < MAX_PRCS; ++i) {
    if(prcs_info[i].t_ctx == tid) {
      int64_t idx = wasm_prcs_info.ENTRY_POINT[prcs_info[i].id];
      ret = exec_wasm_guest_func(wasm_prcs_info.wasm_rt_ctx, idx);
      break;
    }
  }
  if ( ! ret)
    fprintf(stderr, "ERR: wasm_processid not found\n");

  return NULL;
}


WASM32_HOST_FUNCTION(GET_PROCESS_STATUS, wasm_baseaddr,
{
  PROCESS_ID_TYPE pid;
  pid = (PROCESS_ID_TYPE)le32toh(GET_ARG_i32(0));
  RETURN_CODE_TYPE RETURN_CODE;

  PROCESS_STATUS_TYPE PROCESS_STATUS;
  GET_PROCESS_STATUS(
    pid,
    &PROCESS_STATUS,
    &RETURN_CODE
  );

  uint8_t* PROCESS_STATUS_guest = (uint8_t*)&wasm_baseaddr[le32toh(GET_ARG_i32(1))];
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);

  camw32_set__PROCESS_STATUS_TYPE__DEADLINE_TIME(PROCESS_STATUS_guest, PROCESS_STATUS.DEADLINE_TIME);
  camw32_set__PROCESS_STATUS_TYPE__CURRENT_PRIORITY(PROCESS_STATUS_guest, PROCESS_STATUS.CURRENT_PRIORITY);
  camw32_set__PROCESS_STATUS_TYPE__PROCESS_STATE(PROCESS_STATUS_guest, PROCESS_STATUS.PROCESS_STATE);

  uint8_t* ATTRIBUTES_guest = camw32_get_struct_base_addr__PROCESS_STATUS_TYPE__ATTRIBUTES(PROCESS_STATUS_guest);
  camw32_set__PROCESS_ATTRIBUTE_TYPE__PERIOD(ATTRIBUTES_guest, PROCESS_STATUS.ATTRIBUTES.PERIOD);
  camw32_set__PROCESS_ATTRIBUTE_TYPE__TIME_CAPACITY(ATTRIBUTES_guest, PROCESS_STATUS.ATTRIBUTES.TIME_CAPACITY);
  uint32_t ENTRY_POINT_idx = wasm_prcs_info.ENTRY_POINT[pid];
  camw32_set__PROCESS_ATTRIBUTE_TYPE__ENTRY_POINT(ATTRIBUTES_guest, ENTRY_POINT_idx);
  camw32_set__PROCESS_ATTRIBUTE_TYPE__STACK_SIZE(ATTRIBUTES_guest, PROCESS_STATUS.ATTRIBUTES.STACK_SIZE);
  camw32_set__PROCESS_ATTRIBUTE_TYPE__BASE_PRIORITY(ATTRIBUTES_guest, PROCESS_STATUS.ATTRIBUTES.BASE_PRIORITY);
  camw32_set__PROCESS_ATTRIBUTE_TYPE__DEADLINE(ATTRIBUTES_guest, PROCESS_STATUS.ATTRIBUTES.DEADLINE);
  camw32_write__PROCESS_ATTRIBUTE_TYPE__NAME(ATTRIBUTES_guest, (uint8_t*)PROCESS_STATUS.ATTRIBUTES.NAME);
})


WASM32_HOST_FUNCTION(CREATE_PROCESS, wasm_baseaddr,
{
  uint8_t* ATTRIBUTES__guest = (uint8_t*)&wasm_baseaddr[le32toh(GET_ARG_i32(0))];

  PROCESS_ATTRIBUTE_TYPE ATTRIBUTES;
  ATTRIBUTES.PERIOD = camw32_get__PROCESS_ATTRIBUTE_TYPE__PERIOD(ATTRIBUTES__guest);
  ATTRIBUTES.TIME_CAPACITY = camw32_get__PROCESS_ATTRIBUTE_TYPE__TIME_CAPACITY(ATTRIBUTES__guest);
  ATTRIBUTES.ENTRY_POINT = (typeof(ATTRIBUTES.ENTRY_POINT))&wasm_trampoline;
  uint32_t ENTRY_POINT_idx = (uint32_t)camw32_get__PROCESS_ATTRIBUTE_TYPE__ENTRY_POINT(ATTRIBUTES__guest);

  ATTRIBUTES.STACK_SIZE = camw32_get__PROCESS_ATTRIBUTE_TYPE__STACK_SIZE(ATTRIBUTES__guest);
  ATTRIBUTES.BASE_PRIORITY = camw32_get__PROCESS_ATTRIBUTE_TYPE__BASE_PRIORITY(ATTRIBUTES__guest);
  ATTRIBUTES.DEADLINE = camw32_get__PROCESS_ATTRIBUTE_TYPE__DEADLINE(ATTRIBUTES__guest);
  camw32_read__PROCESS_ATTRIBUTE_TYPE__NAME(ATTRIBUTES__guest, (uint8_t*)ATTRIBUTES.NAME);

  PROCESS_ID_TYPE PROCESS_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  CREATE_PROCESS(
    &ATTRIBUTES,
    &PROCESS_ID,
    &RETURN_CODE
  );

  camw32_set__PROCESS_ID_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)PROCESS_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);

  // we get the pid late, and the real start of the thread will be in CREATE_PROCESS
  wasm_prcs_info.ENTRY_POINT[PROCESS_ID] = ENTRY_POINT_idx;
})


WASM32_HOST_FUNCTION(SET_PRIORITY, wasm_baseaddr,
{
  PROCESS_ID_TYPE PROCESS_ID;
  PROCESS_ID = (PROCESS_ID_TYPE)le32toh(GET_ARG_i32(0));
  PRIORITY_TYPE PRIORITY;
  PRIORITY = (PRIORITY_TYPE)le32toh(GET_ARG_i32(1));
  RETURN_CODE_TYPE RETURN_CODE;

  SET_PRIORITY(
    PROCESS_ID,
    PRIORITY,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(SUSPEND_SELF, wasm_baseaddr,
{
  SYSTEM_TIME_TYPE TIME_OUT;
  TIME_OUT = (SYSTEM_TIME_TYPE)le64toh(GET_ARG_i64(0));
  RETURN_CODE_TYPE RETURN_CODE;

  SUSPEND_SELF(
    TIME_OUT,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(SUSPEND, wasm_baseaddr,
{
  PROCESS_ID_TYPE PROCESS_ID;
  PROCESS_ID = (PROCESS_ID_TYPE)le32toh(GET_ARG_i32(0));
  RETURN_CODE_TYPE RETURN_CODE;

  SUSPEND(
    PROCESS_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(RESUME, wasm_baseaddr,
{
  PROCESS_ID_TYPE PROCESS_ID;
  PROCESS_ID = (PROCESS_ID_TYPE)le32toh(GET_ARG_i32(0));
  RETURN_CODE_TYPE RETURN_CODE;

  RESUME(
    PROCESS_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(STOP_SELF, wasm_baseaddr,
{
  (void)wasm_baseaddr;

  STOP_SELF(
  );
})


WASM32_HOST_FUNCTION(STOP, wasm_baseaddr,
{
  PROCESS_ID_TYPE PROCESS_ID;
  PROCESS_ID = (PROCESS_ID_TYPE)le32toh(GET_ARG_i32(0));
  RETURN_CODE_TYPE RETURN_CODE;

  STOP(
    PROCESS_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(START, wasm_baseaddr,
{
  PROCESS_ID_TYPE PROCESS_ID;
  PROCESS_ID = (PROCESS_ID_TYPE)le32toh(GET_ARG_i32(0));
  RETURN_CODE_TYPE RETURN_CODE;

  START(
    PROCESS_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(DELAYED_START, wasm_baseaddr,
{
  PROCESS_ID_TYPE PROCESS_ID;
  PROCESS_ID = (PROCESS_ID_TYPE)le32toh(GET_ARG_i32(0));
  SYSTEM_TIME_TYPE DELAY_TIME;
  DELAY_TIME = (SYSTEM_TIME_TYPE)le64toh(GET_ARG_i64(1));
  RETURN_CODE_TYPE RETURN_CODE;

  DELAYED_START(
    PROCESS_ID,
    DELAY_TIME,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(LOCK_PREEMPTION, wasm_baseaddr,
{
  LOCK_LEVEL_TYPE LOCK_LEVEL;
  RETURN_CODE_TYPE RETURN_CODE;

  LOCK_PREEMPTION(
    &LOCK_LEVEL,
    &RETURN_CODE
  );

  camw32_set__LOCK_LEVEL_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(0))], (int32_t)LOCK_LEVEL);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(UNLOCK_PREEMPTION, wasm_baseaddr,
{
  LOCK_LEVEL_TYPE LOCK_LEVEL;
  RETURN_CODE_TYPE RETURN_CODE;

  LOCK_PREEMPTION(
    &LOCK_LEVEL,
    &RETURN_CODE
  );

  camw32_set__LOCK_LEVEL_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(0))], (int32_t)LOCK_LEVEL);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(GET_MY_ID, wasm_baseaddr,
{
  PROCESS_ID_TYPE PROCESS_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_MY_ID(
    &PROCESS_ID,
    &RETURN_CODE
  );

  camw32_set__PROCESS_ID_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(0))], (int32_t)PROCESS_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(INITIALIZE_PROCESS_CORE_AFFINITY, wasm_baseaddr,
{
  PROCESS_ID_TYPE PROCESS_ID;
  PROCESS_ID = (PROCESS_ID_TYPE)le32toh(GET_ARG_i32(0));
  PROCESSOR_CORE_ID_TYPE PROCESSOR_CORE_ID;
  PROCESSOR_CORE_ID = (PROCESSOR_CORE_ID_TYPE)le32toh(GET_ARG_i32(1));
  RETURN_CODE_TYPE RETURN_CODE;

  INITIALIZE_PROCESS_CORE_AFFINITY(
    PROCESS_ID,
    PROCESSOR_CORE_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(2))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(GET_MY_PROCESSOR_CORE_ID, wasm_baseaddr,
{
  PROCESSOR_CORE_ID_TYPE PROCESSOR_CORE_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_MY_PROCESSOR_CORE_ID(
    &PROCESSOR_CORE_ID,
    &RETURN_CODE
  );

  camw32_set__PROCESSOR_CORE_ID_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(0))], (int32_t)PROCESSOR_CORE_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})


WASM32_HOST_FUNCTION(GET_MY_INDEX, wasm_baseaddr,
{
  PROCESS_INDEX_TYPE PROCESS_INDEX;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_MY_INDEX(
    &PROCESS_INDEX,
    &RETURN_CODE
  );

  camw32_set__PROCESS_INDEX_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(0))], (int32_t)PROCESS_INDEX);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(GET_ARG_i32(1))], (int32_t)RETURN_CODE);
})
