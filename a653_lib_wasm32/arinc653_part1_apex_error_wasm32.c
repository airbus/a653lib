// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: ERROR

#include <dlfcn.h>
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
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  REPORT_APPLICATION_MESSAGE(
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[args[0].of.i32],
    (MESSAGE_SIZE_TYPE)args[1].of.i32,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[2].of.i32]
  );
  return NULL;
}


extern void initialize_wasm_instance(
  wasm_engine_t* engine,
  wasmtime_sharedmemory_t* shm_memory,
  wasmtime_module_t* module,
  void* dl_struct_getset,

  wasmtime_linker_t** _linker,
  wasmtime_store_t** _store,
  wasmtime_context_t** _context,
  wasmtime_instance_t* instance,
  bool create_func_table);

extern wasm_processes_t wasm_processes;

/*
 * equivalent to wasm_trampoline()
 * However, in wasm_trampline() the args[0].of.i32 i.e. idx is stored after calling a653lib.
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
                            wasm_processes.module, wasm_processes.dl_struct_getset, &linker, &store, &context, &instance, false);

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

  wasmtime_val_t args[1];
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
  args[0].kind = WASMTIME_I32;
  args[0].of.i32 = wasm_processes.ENTRY_POINT_ERROR_HANDLER;
  //      wasmtime_val_t results[1];
  wasm_trap_t* trap = NULL;
  wasmtime_error_t* error = wasmtime_func_call(context, &export.of.func, args, 1, NULL /*results*/, 0, &trap);
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
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  // FIXME: in case there are more then 1 error handlers supported
  //        and hoping, that the pid is not saved prcs_info.
  wasm_processes.ENTRY_POINT_ERROR_HANDLER = args[0].of.i32;
  CREATE_ERROR_HANDLER(
    error_handler_trampoline,
    (STACK_SIZE_TYPE)args[1].of.i32,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[2].of.i32]
  );

  return NULL;
}


#if 0
extern void GET_ERROR_STATUS (
/*out*/   ERROR_STATUS_TYPE        *ERROR_STATUS,
/*out*/   RETURN_CODE_TYPE         *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__GET_ERROR_STATUS = "(ii)";
wasm_trap_t* WASM32_GET_ERROR_STATUS(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  ERROR_STATUS_TYPE ERROR_STATUS__host_64bit;
  GET_ERROR_STATUS(
    &ERROR_STATUS__host_64bit,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
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

  // FIXME: pointer is 32bit, but could be 64bit ..
  uint8_t* ERROR_STATUS__guest = (uint8_t*)&wasm_baseaddr[args[0].of.i32];
  camw32_set__ERROR_STATUS_TYPE__ERROR_CODE(ERROR_STATUS__guest, ERROR_STATUS__host_64bit.ERROR_CODE);
  camw32_write__ERROR_STATUS_TYPE__MESSAGE(ERROR_STATUS__guest, ERROR_STATUS__host_64bit.MESSAGE);
  camw32_set__ERROR_STATUS_TYPE__LENGTH(ERROR_STATUS__guest, ERROR_STATUS__host_64bit.LENGTH);
  camw32_set__ERROR_STATUS_TYPE__FAILED_PROCESS_ID(ERROR_STATUS__guest, ERROR_STATUS__host_64bit.FAILED_PROCESS_ID);

  uint32_t FAILED_ADDRESS_idx = wasm_processes.ENTRY_POINT[ERROR_STATUS__host_64bit.FAILED_PROCESS_ID];
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
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  RAISE_APPLICATION_ERROR(
    (ERROR_CODE_TYPE)args[0].of.i32,
    (MESSAGE_ADDR_TYPE)&wasm_baseaddr[args[1].of.i32],
    (ERROR_MESSAGE_SIZE_TYPE)args[2].of.i32,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[3].of.i32]
  );

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
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  CONFIGURE_ERROR_HANDLER(
    (ERROR_HANDLER_CONCURRENCY_CONTROL_TYPE)args[0].of.i32,
    (PROCESSOR_CORE_ID_TYPE)args[1].of.i32,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[2].of.i32]
  );

  return NULL;
}
