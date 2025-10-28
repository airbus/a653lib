// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: ERROR

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "arinc653_wasm32_helper.h"
#include "arinc653_part1_apex_error_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_inc/a653Error.h"


WASM_HOST_FUNCTION(REPORT_APPLICATION_MESSAGE, wasm_baseaddr,
{
  int32_t MESSAGE_ADDR; /* is a pointer / address into Wasm linear memory */
  MESSAGE_ADDR = (int32_t)le32toh(args_and_results[0].i32);
  MESSAGE_SIZE_TYPE LENGTH;
  LENGTH = (MESSAGE_SIZE_TYPE)le32toh(args_and_results[1].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  REPORT_APPLICATION_MESSAGE(
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[MESSAGE_ADDR],
    LENGTH,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);
})



extern wasm_processes_t wasm_processes;

void *error_handler_trampoline(void) {

  uint32_t idx = wasm_processes.ENTRY_POINT_ERROR_HANDLER;
  if ( ! exec_wasm_guest_func(0, idx))
    fprintf(stderr, "ERR: wasm_processid not found\n");

  return NULL;
}


/*
 * Note:
 *    in case one patches ARINC653 CREATE_ERROR_HANLDER() with:
 *      CREATE_ERROR_HANLDER(__funcref ENTRY_POINT, STACK_SIZE_TYPE STACK_SIZE, RETURN_CODE_TYPE *RETURN_CODE)
 *
 *   then at least in Wasmtime one can use to store the function pointer raw value:
 *
 *      void* wasm_processes.ENTRY_POINT_ERROR_HANDLER;
 *      wasm_processes.ENTRY_POINT_ERROR_HANDLER = wasmtime_func_to_raw(context, args_and_results[0].funcref);
 *
 *    and recreate the function with:
 *
 *      wasmtime_func_t fn;
 *      wasmtime_func_from_raw(context, wasm_processes.ENTRY_POINT_ERROR_HANDLER, &fn);
 *
 *    However, WAMR does not support WASM_FUNCREF.
 */
WASM_HOST_FUNCTION(CREATE_ERROR_HANDLER, wasm_baseaddr,
{
  wasm_processes.ENTRY_POINT_ERROR_HANDLER = le32toh(args_and_results[0].i32);

  STACK_SIZE_TYPE STACK_SIZE;
  STACK_SIZE = (STACK_SIZE_TYPE)le32toh(args_and_results[1].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  CREATE_ERROR_HANDLER(
    error_handler_trampoline,
    STACK_SIZE,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);
})


WASM_HOST_FUNCTION(GET_ERROR_STATUS, wasm_baseaddr,
{
  ERROR_STATUS_TYPE ERROR_STATUS;
  RETURN_CODE_TYPE RETURN_CODE;
  GET_ERROR_STATUS(
    &ERROR_STATUS,
    &RETURN_CODE
  );

  uint8_t* ERROR_STATUS__guest = &wasm_baseaddr[le32toh(args_and_results[0].i32)];
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  camw32_set__ERROR_STATUS_TYPE__ERROR_CODE(ERROR_STATUS__guest, (ERROR_MESSAGE_SIZE_TYPE)ERROR_STATUS.ERROR_CODE);
  camw32_write__ERROR_STATUS_TYPE__MESSAGE(ERROR_STATUS__guest, ERROR_STATUS.MESSAGE);
  camw32_set__ERROR_STATUS_TYPE__LENGTH(ERROR_STATUS__guest, ERROR_STATUS.LENGTH);
  camw32_set__ERROR_STATUS_TYPE__FAILED_PROCESS_ID(ERROR_STATUS__guest, ERROR_STATUS.FAILED_PROCESS_ID);

  uint32_t FAILED_ADDRESS_idx = wasm_processes.ENTRY_POINT[ERROR_STATUS.FAILED_PROCESS_ID];
  camw32_set__ERROR_STATUS_TYPE__FAILED_ADDRESS(ERROR_STATUS__guest, FAILED_ADDRESS_idx);
})


WASM_HOST_FUNCTION(RAISE_APPLICATION_ERROR, wasm_baseaddr,
{
  ERROR_CODE_TYPE ERROR_CODE;
  ERROR_CODE = (ERROR_CODE_TYPE)le32toh(args_and_results[0].i32);
  int32_t MESSAGE_ADDR; /* is a pointer / address into Wasm linear memory */
  MESSAGE_ADDR = (int32_t)le32toh(args_and_results[1].i32);
  ERROR_MESSAGE_SIZE_TYPE LENGTH;
  LENGTH = (ERROR_MESSAGE_SIZE_TYPE)le32toh(args_and_results[2].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  RAISE_APPLICATION_ERROR(
    ERROR_CODE,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[MESSAGE_ADDR], // FIXME: only safe as long as char[]
    LENGTH,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[3].i32)], (int32_t)RETURN_CODE);
})


WASM_HOST_FUNCTION(CONFIGURE_ERROR_HANDLER, wasm_baseaddr,
{
  ERROR_HANDLER_CONCURRENCY_CONTROL_TYPE CONCURRENCY_CONTROL;
  CONCURRENCY_CONTROL = (ERROR_HANDLER_CONCURRENCY_CONTROL_TYPE)le32toh(args_and_results[0].i32);
  PROCESSOR_CORE_ID_TYPE PROCESSOR_CORE_ID;
  PROCESSOR_CORE_ID = (PROCESSOR_CORE_ID_TYPE)le32toh(args_and_results[1].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  CONFIGURE_ERROR_HANDLER(
    CONCURRENCY_CONTROL,
    PROCESSOR_CORE_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);
})
