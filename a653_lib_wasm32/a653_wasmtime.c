// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#include <stdio.h>
#include <stdlib.h>
#include "a653_wasmtime.h"
#include "apex_host_fncs_wasm32.h"
#include "generic_helper.h"

typedef struct {
  wasm_engine_t* engine;
  wasmtime_sharedmemory_t* shm_memory;
  wasmtime_module_t* module;
} wasmtime_data_t;


static void print_wasmtime_error(wasmtime_error_t* error)
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

void* generate_wasm_runtime_context(wasm_file_t* wasm)
{
  // Configure WASI
  wasm_config_t *wasm_config = wasm_config_new();
  wasmtime_config_wasm_memory64_set(wasm_config, HAS_64BIT_MEM);
  wasmtime_config_cranelift_opt_level_set (wasm_config, WASMTIME_OPT_LEVEL_SPEED);

  // Initialize Wasmtime
  wasmtime_data_t* wasmtime_data = (wasmtime_data_t*) malloc (sizeof(wasmtime_data_t));
  wasmtime_data->engine = wasm_engine_new_with_config(wasm_config);

  // Create shared memory (required for threading)
  wasm_memorytype_t *mem_type = wasmtime_memorytype_new(
    1 /* min 64KB-pages */, true /* max present (must for shared) */, 65536 /* max 64KB-pages -> 4GB */,
    HAS_64BIT_MEM /* is_64 */, true /* is shared ! required for threading */
  );
  wasmtime_data->shm_memory = NULL;
  wasmtime_sharedmemory_new(wasmtime_data->engine, mem_type, &wasmtime_data->shm_memory);

  // Compile the module
  wasmtime_error_t* err;
  if ((err = wasmtime_module_new(wasmtime_data->engine, (uint8_t*)wasm->data, wasm->size, &wasmtime_data->module)) != NULL) {
    print_wasmtime_error(err);
    free(wasmtime_data);
    return NULL;
  }
  return wasmtime_data;
}

void cleanup_wasm_runtime_context(void* wasm_runtime_context)
{
  wasmtime_data_t* _wasm_runtime_context = (wasmtime_data_t*)wasm_runtime_context;
  wasmtime_module_delete(_wasm_runtime_context->module);
  wasm_engine_delete(_wasm_runtime_context->engine);
}


/**
 * Either start the default, which is _start()
 * Or based on an index into the __indirect_function_table[]
 */
int exec_wasm_guest_func(void* wasm_runtime_context, int32_t idx)
{
  wasmtime_data_t* wasmtime_data = (wasmtime_data_t*)wasm_runtime_context;

  // Configure WASI context (currently given for debugging... not for true avionics)
  wasi_config_t* wasi_config = wasi_config_new();
  wasi_config_inherit_argv(wasi_config);
  wasi_config_inherit_env(wasi_config);
  wasi_config_inherit_stdout(wasi_config);
  wasi_config_inherit_stderr(wasi_config);

  wasmtime_store_t* store = wasmtime_store_new(wasmtime_data->engine, NULL, NULL);
  wasmtime_context_t* context = wasmtime_store_context(store);

  wasmtime_error_t* error;
  if ((error = wasmtime_context_set_wasi(context, wasi_config)) != NULL) {
    print_wasmtime_error(error);
    return -1;
  }

  // Create linker and define WASI
  wasmtime_linker_t* linker = wasmtime_linker_new(wasmtime_data->engine);
  if ((error = wasmtime_linker_define_wasi(linker)) != NULL) {
    print_wasmtime_error(error);
    return -1;
  }

  wasmtime_extern_t import;
  import.kind = WASMTIME_EXTERN_SHAREDMEMORY;
  import.of.sharedmemory = wasmtime_data->shm_memory;

  // Link shared memory to "wasi" module (or your module namespace)
  if ((error = wasmtime_linker_define(linker, context, "wasi", 4, "memory", 6, &import)) != NULL) {
    print_wasmtime_error(error);
    return - 1;
  }

  wasm_valtype_vec_t results;
  wasm_valtype_vec_new_empty(&results);

  // Create the host function
  wasmtime_extern_t item;
  item.kind = WASMTIME_EXTERN_FUNC;

  NativeSymbol *native_symbols;
  for(unsigned i = 0; i < getNativeSymbols(&native_symbols); ++i) {
    NativeSymbol* native_symbol = &native_symbols[i];

    const char *signature = native_symbol->signature;
    int parms_c = signature_parameter_count(signature);

    wasm_valtype_vec_t params;
    wasm_valtype_vec_new_uninitialized(&params, parms_c);

    // https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/core/iwasm/common/wasm_runtime_common.c
    unsigned j = 0;
    for (char *s = (char*)signature; *s != '\0' && j < parms_c; ++s) {
      switch ( *s ) {
        case '(':
        case ')':
          break;
        case 'i': // 32-bit integer (i32)
        case '~': // Byte length of the preceding buffer pointer (*), must follow *
          params.data[j++] = wasm_valtype_new(WASM_I32);
          break;
        case 'I': // 64-bit integer (i64)
          params.data[j++] = wasm_valtype_new(WASM_I64);
          break;
        case 'f': // 32-bit float (f32)
          params.data[j++] = wasm_valtype_new(WASM_F32);
          break;
        case 'F': // 64-bit float (f64)
          params.data[j++] = wasm_valtype_new(WASM_F64);
          break;
        case 'r': // externref type (usually a uintptr_t), or GC references
          params.data[j++] = wasm_valtype_new(WASM_EXTERNREF);
          break;
        case '$': // String in WASM memory
        case '*': // Buffer address (pointer) in WASM memory
          params.data[j++] = wasm_valtype_new(HAS_64BIT_MEM ? WASM_I64 : WASM_I32);
          break;
        default:
          fprintf(stderr, "ERR: wrong character %c\n!", *s);
          break;
      }
    }

    wasm_functype_t* func_type = wasm_functype_new(&params, &results);
    wasmtime_func_new_unchecked(context, func_type,
                                (wasmtime_func_unchecked_callback_t)native_symbol->func_ptr,
                                native_symbol->attachment, NULL, &item.of.func );

    error = wasmtime_linker_define(linker, context, "arinc653", strlen("arinc653"),
                                   native_symbol->symbol, strlen(native_symbol->symbol),
                                   &item );
    if (error != NULL) {
      print_wasmtime_error(error);
      return -1;
    }
  }

  wasmtime_instance_t instance;
  if (// Add the compiled module to the linker
    (error = wasmtime_linker_module(linker, context, NULL, 0, wasmtime_data->module)) != NULL
    // Instantiate the module; all host functions created don't provide a trap
    || (error = wasmtime_linker_instantiate(linker, context, wasmtime_data->module, &instance, NULL)) != NULL) {
    print_wasmtime_error(error);
  }

  wasmtime_func_t fnc;
  if (idx == -1) {
    if ((error = wasmtime_linker_get_default(linker, context, NULL, 0, &fnc)) != NULL) {
      print_wasmtime_error(error);
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

  if ((error = wasmtime_func_call(context, &fnc, NULL, 0, NULL, 0, NULL)) != NULL) {
    print_wasmtime_error(error);
    return -1;
  }

  wasmtime_store_delete(store);

  return 0;
}
