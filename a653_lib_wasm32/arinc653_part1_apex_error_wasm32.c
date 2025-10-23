// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: ERROR

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "arinc653_part1_apex_error_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_inc/a653Error.h"


#if 0
extern void REPORT_APPLICATION_MESSAGE (
  /*in */   MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
  /*in */   MESSAGE_SIZE_TYPE        LENGTH,
  /*out*/   RETURN_CODE_TYPE         *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__REPORT_APPLICATION_MESSAGE = "(iii)";
wasm_trap_t* WASM32_REPORT_APPLICATION_MESSAGE(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


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

  return NULL;
}


extern void initialize_wasm_instance(
  wasm_engine_t* engine,
  wasmtime_sharedmemory_t* shm_memory,
  wasmtime_module_t* module,

  wasmtime_linker_t** _linker,
  wasmtime_store_t** _store,
  wasmtime_context_t** _context,
  wasmtime_instance_t* instance,
  bool create_func_table);

extern wasm_processes_t wasm_processes;

/*
 * equivalent to wasm_trampoline()
 * However, in wasm_trampline() the args_and_results[0].of.i32 i.e. idx is stored after calling a653lib.
 * Thus, the struct is prefiled, the moment START() is called.
 * This seems to be not working for CREATE_ERROR_HANDLER()
 */
void *error_handler_trampoline(void) {
  // linker, store and context are not thread-safe
  wasmtime_linker_t* linker;
  wasmtime_store_t* store;
  wasmtime_context_t* context;
  wasmtime_instance_t instance;
  initialize_wasm_instance(wasm_processes.engine, wasm_processes.shm_memory,
                            wasm_processes.module, &linker, &store, &context, &instance, false);

  wasmtime_extern_t export;
  bool ok = wasmtime_instance_export_get(
    context, &instance,
    "WASM_GUEST_FUNC_TRAMPOLINE", strlen("WASM_GUEST_FUNC_TRAMPOLINE"),
                                          &export
  );

  if (!(ok && export.kind == WASMTIME_EXTERN_FUNC)) {
    fprintf(stderr, "❌ Function WASM_GUEST_FUNC_TRAMPOLINE not found or not a function\n");
    return NULL;
  }

  wasmtime_val_t args;
  /*
    *       Note: The 'correct' .kind is WASMTIME_FUNCREF !
    *       However, https://docs.wasmtime.dev/c-api/structwasmtime__func.html /
    *       https://docs.wasmtime.dev/c-api/extern_8h.html#ac3661fabd7972df1ade869f17de29dc5
    *       would require the __private stuff, which is store and context bound.
    *       Thus, it would bail out in the new module instance / thread. However,
    *       what is 'truly' required is not wasmtime_func_t, but rather the 'function
    *       reference'. And the 'function reference' is purely the index. Thus, we
    *       supply this here as a int32_t (as a function pointer in 32-bit WASM
    *       is also just int32_t).
    */
  args.kind = WASMTIME_I32;
  args.of.i32 = wasm_processes.ENTRY_POINT_ERROR_HANDLER;
  //      wasmtime_val_t results[1];
  wasm_trap_t* trap = NULL;
  wasmtime_error_t* error = wasmtime_func_call(context, &export.of.func, &args, 1, NULL /*results*/, 0, &trap);
  if (error != NULL || trap != NULL) {
    fprintf(stderr, "❌ Function call failed\n");
    // handle trap or error
  } else {
    printf("✅ Function \n");
  }

  wasmtime_store_delete(store);

  return NULL;
}


#if 0
extern void CREATE_ERROR_HANDLER (
  /*in */   SYSTEM_ADDRESS_TYPE      ENTRY_POINT,
  /*in */   STACK_SIZE_TYPE          STACK_SIZE,
  /*out*/   RETURN_CODE_TYPE         *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__CREATE_ERROR_HANDLER = "(iii)";
wasm_trap_t* WASM32_CREATE_ERROR_HANDLER(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  // FIXME: in case there are more then 1 error handlers supported
  //        and hoping, that the pid is not saved prcs_info.
  wasm_processes.ENTRY_POINT_ERROR_HANDLER = args_and_results[0].i32;


  STACK_SIZE_TYPE STACK_SIZE;
  STACK_SIZE = (STACK_SIZE_TYPE)le32toh(args_and_results[1].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  CREATE_ERROR_HANDLER(
    error_handler_trampoline,
    STACK_SIZE,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void GET_ERROR_STATUS (
/*out*/   ERROR_STATUS_TYPE        *ERROR_STATUS,
/*out*/   RETURN_CODE_TYPE         *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__GET_ERROR_STATUS = "(ii)";
wasm_trap_t* WASM32_GET_ERROR_STATUS(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  ERROR_STATUS_TYPE ERROR_STATUS;
  RETURN_CODE_TYPE RETURN_CODE;
  GET_ERROR_STATUS(
    &ERROR_STATUS,
    &RETURN_CODE
  );

#if 0
/* error status type */
typedef struct             {
  ERROR_CODE_TYPE         ERROR_CODE;
  ERROR_MESSAGE_TYPE      MESSAGE;
  ERROR_MESSAGE_SIZE_TYPE LENGTH;
  PROCESS_ID_TYPE         FAILED_PROCESS_ID;
  SYSTEM_ADDRESS_TYPE     FAILED_ADDRESS;
} ERROR_STATUS_TYPE;
#endif

  uint8_t* ERROR_STATUS__guest = &wasm_baseaddr[le32toh(args_and_results[0].i32)];
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  camw32_set__ERROR_STATUS_TYPE__ERROR_CODE(ERROR_STATUS__guest, (ERROR_MESSAGE_SIZE_TYPE)ERROR_STATUS.ERROR_CODE);
  camw32_write__ERROR_STATUS_TYPE__MESSAGE(ERROR_STATUS__guest, ERROR_STATUS.MESSAGE);
  camw32_set__ERROR_STATUS_TYPE__LENGTH(ERROR_STATUS__guest, ERROR_STATUS.LENGTH);
  camw32_set__ERROR_STATUS_TYPE__FAILED_PROCESS_ID(ERROR_STATUS__guest, ERROR_STATUS.FAILED_PROCESS_ID);

  uint32_t FAILED_ADDRESS_idx = wasm_processes.ENTRY_POINT[ERROR_STATUS.FAILED_PROCESS_ID];
  camw32_set__ERROR_STATUS_TYPE__FAILED_ADDRESS(ERROR_STATUS__guest, FAILED_ADDRESS_idx);

  return NULL;
}


#if 0
extern void RAISE_APPLICATION_ERROR (
  /*in */   ERROR_CODE_TYPE          ERROR_CODE,
  /*in */   MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
  /*in */   ERROR_MESSAGE_SIZE_TYPE  LENGTH,
  /*out*/   RETURN_CODE_TYPE         *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__RAISE_APPLICATION_ERROR = "(iiii)";
wasm_trap_t* WASM32_RAISE_APPLICATION_ERROR(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


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

  return NULL;
}


#if 0
extern void CONFIGURE_ERROR_HANDLER (
  /*in */   ERROR_HANDLER_CONCURRENCY_CONTROL_TYPE  CONCURRENCY_CONTROL,
  /*in */   PROCESSOR_CORE_ID_TYPE                  PROCESSOR_CORE_ID,
  /*out*/   RETURN_CODE_TYPE                        *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__CONFIGURE_ERROR_HANDLER = "(iii)";
wasm_trap_t* WASM32_CONFIGURE_ERROR_HANDLER(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


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

  return NULL;
}
