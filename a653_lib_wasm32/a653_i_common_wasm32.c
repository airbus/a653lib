// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#include <stdio.h>

#include "a653_i_common_wasm32.h"

wasm_processes_t wasm_processes;

void print_wasmtime_error(wasmtime_error_t* error)
{
  if (error) {
    wasm_byte_vec_t msg;
    wasmtime_error_message(error, &msg);
    wasmtime_error_delete(error);
    fprintf(stderr, "ERR: %.*s\n", (int)msg.size, msg.data);
    wasm_byte_vec_delete(&msg);
  } else {
    fprintf(stderr, "ERR: Unknown\n");
  }
}


uint8_t* get_linear_memory(wasmtime_caller_t* caller) {
  wasmtime_extern_t ext;
  const char *m = "memory";
  if ( ! wasmtime_caller_export_get(caller, m, strlen(m), &ext)) {
    fprintf(stderr, "ERR: 'memory' export not found!\n");
    return NULL;
  }

  if (ext.kind != WASM_EXTERN_MEMORY) {
    fprintf(stderr, "ERR: export 'memory' is not a memory!\n");
    return NULL;
  }

  wasmtime_context_t *context = wasmtime_caller_context(caller);
  return wasmtime_memory_data(context, &ext.of.memory);
}



extern void initialize_wasm_instance(
  wasm_engine_t* engine,
  wasmtime_sharedmemory_t* shm_memory,
  wasmtime_module_t* module,

  wasmtime_linker_t** _linker,
  wasmtime_store_t** _store,
  wasmtime_context_t** _context,
  wasmtime_instance_t* instance);

/**
 * Either start the default, which is _start()
 * Or based on an index into the __indirect_function_table[]
 */
int exec_wasm_guest_func(int start_default, uint32_t idx)
{
  // linker, store and context are not thread-safe
  wasmtime_linker_t* linker;
  wasmtime_store_t* store;
  wasmtime_context_t* context;
  wasmtime_instance_t instance;
  initialize_wasm_instance(wasm_processes.engine, wasm_processes.shm_memory,
                            wasm_processes.module, &linker, &store, &context, &instance);

  wasmtime_error_t *err;
  wasmtime_func_t fnc;
  if(start_default) {
    if ((err = wasmtime_linker_get_default(linker, context, NULL, 0, &fnc)) != NULL) {
      print_wasmtime_error(err);
      return -1;
    }
  }
  else {
    wasmtime_extern_t ext;
    if ( ! wasmtime_instance_export_get(context, &instance, "__indirect_function_table", strlen("__indirect_function_table"), &ext)
        || ext.kind != WASMTIME_EXTERN_TABLE ) {
      fprintf(stderr, "ERR: __indirect_function_table could not be found.\n");
      return -1;
    }

    wasmtime_val_t val;
    if ( ! wasmtime_table_get(context, &ext.of.table, idx, &val)) {
      fprintf(stderr, "ERR: Index %u not given in __indirect_function_table.\n", idx);
      return -1;
    }

    fnc = val.of.funcref;
  }

  if ((err = wasmtime_func_call(context, &fnc, NULL, 0, NULL, 0, NULL)) != NULL) {
    print_wasmtime_error(err);
    return -1;
  }

  wasmtime_store_delete(store);

  return 0;


#if 0
/*
   motivated by the WASI-THREADS
*/

  bool ok = wasmtime_instance_export_get(context, &instance, "__WASM_GUEST_CALL_INDIRECT", strlen("__WASM_GUEST_CALL_INDIRECT"), &ext );
  if ( ! (ok && export.kind == WASMTIME_EXTERN_FUNC)) {
    fprintf(stderr, "❌ Function __WASM_GUEST_CALL_INDIRECT not found or not a function\n");
    return -1;
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
  args.of.i32 = idx;
  if ((err = wasmtime_func_call(context, &ext.of.func, &args, 1, NULL /*results*/, 0, NULL)) != NULL) {
    print_wasmtime_error(err);
    return -1;
  }
#endif
}
