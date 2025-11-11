// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wasm_export.h>
#include <lib_export.h>
#include "a653_wamr.h"
#include "apex_host_fncs_wasm32.h"

#define STACK_SIZE        (64 * 1024)
#define MAX_PAGES         64
#define HOST_HEAP         0

static char error[128] = {0};
InstantiationArgs inst_args = {
  .default_stack_size = STACK_SIZE,
  .host_managed_heap_size = HOST_HEAP,
  .max_memory_pages = MAX_PAGES
};

typedef struct {
  wasm_module_t module;

  /* needs to be at the end due to [] */
  wasm_file_t wasm;
} wamr_data_t;

void* generate_wasm_runtime_context(wasm_file_t* wasm)
{
  RuntimeInitArgs init_args;

  /* must be cleaned, otherwise .. there are a lot of possible settings */
  memset(&init_args, 0, sizeof(RuntimeInitArgs));

  init_args.mem_alloc_type = Alloc_With_System_Allocator;

  if ( ! wasm_runtime_full_init(&init_args)) {
    fprintf(stderr, "ERR: Failed to initialize WASM runtime!\n");
    return NULL;
  }

  NativeSymbol *native_symbols;
  unsigned n_native_symbols = getNativeSymbols(&native_symbols);
  if ( ! wasm_runtime_register_natives_raw("arinc653", native_symbols, n_native_symbols)) {
    fprintf(stderr, "ERR: Failed to initialize WASM host functions!\n");
    return NULL;
  }

  wamr_data_t* wamr_data = (wamr_data_t*) malloc (sizeof(wamr_data_t) + wasm->size);

  /*
   * Wasmtime copies the loaded binary to somewhere, thus the agnostic main is assuming it can free it.
   * WAMR in contrast uses the given origin of the binary, thus we need to copy here to have same behaviour.
   */
  wamr_data->wasm.size = wasm->size;
  memcpy(wamr_data->wasm.data, wasm->data, wasm->size);

  /**
   * https://github.com/bytecodealliance/wasm-micro-runtime/discussions/3697
   * WAMRs approach of having shared linear memory considers to solely set up 1 WASM module.
   * Which is then used by any thread ..
   */
  if ( ! (wamr_data->module = wasm_runtime_load((uint8_t*)wamr_data->wasm.data, wamr_data->wasm.size, error, sizeof(error)))) {
    fprintf(stderr, "ERR[wasm_runtime_load()]: %s\n", error);
    return NULL;
  }

  return wamr_data;
}

void cleanup_wasm_runtime_context(void* wasm_runtime_context)
{
  wasm_runtime_destroy();
  free(wasm_runtime_context);
}

int exec_wasm_guest_func(void* wasm_runtime_context, int32_t idx)
{
  wamr_data_t *wamr_data = (wamr_data_t*)wasm_runtime_context;

//  wasm_runtime_set_wasi_args(wamr_data->module, NULL, 0, NULL, 0, NULL, 0, NULL, 0);

#if 0
  wasm_module_inst_t module_inst = wasm_runtime_instantiate_ex(
    wamr_data->module, &inst_args, error, sizeof(error));
#else

  wasm_module_inst_t module_inst = wasm_runtime_instantiate(
    wamr_data->module, STACK_SIZE, HOST_HEAP, error, sizeof(error));
#endif
  if ( ! module_inst) {
    fprintf(stderr, "ERR[wasm_runtime_instantiate()]: %s\n", error);
    return -1;
  }

  wasm_exec_env_t exec_env =
    wasm_runtime_create_exec_env(module_inst, STACK_SIZE);
  if (!exec_env) {
    fprintf(stderr, "ERR: Failed to create execution environment\n");
    return -1;
  }

  if (idx == -1) {
#if 0
    wasm_function_inst_t fnc;
    if ( ! (fnc = wasm_runtime_lookup_wasi_start_function(module_inst))) {
      fprintf(stderr, "ERR: Failed to lookup main function inside WASM module: %p\n", fnc);
      return -1;
    }
    if ( ! wasm_runtime_call_wasm(exec_env, fnc, 0, NULL)) {
      fprintf(stderr, "ERR[wasm_runtime_call_wasm()] %s\n", wasm_runtime_get_exception(module_inst));
      return -1;
    }
#else
    if ( ! wasm_application_execute_main(module_inst, 0, NULL)) {
      fprintf(stderr, "ERR[wasm_application_execute_main()] %s\n", wasm_runtime_get_exception(module_inst));
      return -1;
    }
#endif
  }
  else {
    // idx to function name?? wasm_runtime_lookup_function()

    if ( ! wasm_runtime_init_thread_env()) {
      fprintf(stderr, "ERR: Failed to initiate thread environment\n");
    }

#if 1
    if ( ! wasm_runtime_call_indirect(exec_env, idx, 0, NULL)) {
      fprintf(stderr, "ERR[wasm_runtime_call_indirect()] %s\n", wasm_runtime_get_exception(module_inst));
      return -1;
    }
#else
    wasm_table_inst_t wasm_table;
    if ( ! wasm_runtime_get_export_table_inst(module_inst, "__indirect_function_table",
        &wasm_table)) {
      fprintf(stderr, "ERR: Failed to get __indirect_function_table\n");
    }

    wasm_function_inst_t fnc;
    if ( ! (fnc = wasm_table_get_func_inst(module_inst, &wasm_table, idx))) {
      fprintf(stderr, "ERR: Failed to get function in __indirect_function_table at idx %u\n", idx);
    }

    if ( ! wasm_runtime_call_wasm(exec_env, fnc, 0, NULL)) {
      fprintf(stderr, "ERR[wasm_runtime_call_wasm()] %s\n", wasm_runtime_get_exception(module_inst));
      return -1;
    }
#endif

    wasm_runtime_destroy_thread_env();
  }

  wasm_runtime_destroy_exec_env(exec_env);

  return 0;
}
