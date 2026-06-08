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
  wasmtime_config_shared_memory_set(wasm_config, true);
//  wasmtime_config_wasm_threads_set(wasm_config, true);

  // Initialize Wasmtime
  wasmtime_data_t* wd = (wasmtime_data_t*) malloc (sizeof(wasmtime_data_t));
  wd->engine = wasm_engine_new_with_config(wasm_config);

  // Create shared memory (required for threading)
  wasm_memorytype_t* mem_type;
  wasmtime_error_t* err;
  if ((err = wasmtime_memorytype_new(
      1 /* min 64KB-pages */,
      true /* max present (must for shared) */,
      65536 /* max 64KB-pages -> 4GB */,
      HAS_64BIT_MEM /* is_64 */,
      true /* is shared ! required for threading */,
      0     /* page_size_log2 (64 KiB pages), 0 = use default (64KB) */,
      &mem_type
    )) != NULL) {
    print_wasmtime_error(err);
    return NULL;
  }
  wd->shm_memory = NULL;
  if((err = wasmtime_sharedmemory_new(wd->engine, mem_type, &wd->shm_memory)) != NULL) {
    print_wasmtime_error(err);
    free(wd);
    return NULL;
  }

  // Compile the module
  if ((err = wasmtime_module_new(wd->engine, (uint8_t*)wasm->data, wasm->size, &wd->module)) != NULL) {
    print_wasmtime_error(err);
    free(wd);
    return NULL;
  }
  return wd;
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
  wasmtime_data_t* wd = (wasmtime_data_t*)wasm_runtime_context;

  // Fresh store+context per call, just like WAMR's fresh module_inst
  wasmtime_store_t* store = wasmtime_store_new(wd->engine, NULL, NULL);
  wasmtime_context_t* context = wasmtime_store_context(store);

  // Configure WASI context (currently given for debugging... not for true avionics)
  wasi_config_t* wasi_config = wasi_config_new();
  wasi_config_inherit_argv(wasi_config);
  wasi_config_inherit_env(wasi_config);
  wasi_config_inherit_stdout(wasi_config);
  wasi_config_inherit_stderr(wasi_config);

  wasmtime_error_t* err;
  if ((err = wasmtime_context_set_wasi(context, wasi_config)) != NULL) {
    print_wasmtime_error(err);
    wasmtime_store_delete(store);
    return -1;
  }

  // Create linker and define WASI
  wasmtime_linker_t* linker = wasmtime_linker_new(wd->engine);
  if ((err = wasmtime_linker_define_wasi(linker)) != NULL) {
    print_wasmtime_error(err);
    wasmtime_linker_delete(linker);
    wasmtime_store_delete(store);
    return -1;
  }

  wasmtime_extern_t import;
  import.kind = WASMTIME_EXTERN_SHAREDMEMORY;
  import.of.sharedmemory = wd->shm_memory;

  // Link shared memory to "wasi" module (or your module namespace)
  if ((err = wasmtime_linker_define(linker, context, "wasi", 4, "memory", 6, &import)) != NULL) {
    print_wasmtime_error(err);
    printf("shared memory didn't work!\n");
    return - 1;
  }

  wasm_valtype_vec_t results;
  wasm_valtype_vec_new_empty(&results);

  // Create the host function
  wasmtime_extern_t item;
  item.kind = WASMTIME_EXTERN_FUNC;

  NativeSymbol* native_symbols;
  for (unsigned i = 0; i < getNativeSymbols(&native_symbols); ++i) {
    NativeSymbol* ns = &native_symbols[i];
    int parms_c = signature_parameter_count(ns->signature);
    if (parms_c == -1) {
      wasmtime_linker_delete(linker);
      wasmtime_store_delete(store);
      return -1;
    }

    wasm_valtype_vec_t params;
    wasm_valtype_vec_new_uninitialized(&params, parms_c);

    // https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/core/iwasm/common/wasm_runtime_common.c
    int j = 0;
    for (char* s = (char*)ns->signature; *s != '\0' && j < parms_c; ++s) {
      switch (*s) {
        case '(': case ')': break;
        // 32-bit integer (i32)
        // Byte length of the preceding buffer pointer (*), must follow *
        case 'i': case '~': params.data[j++] = wasm_valtype_new(WASM_I32); break;
        // 64-bit integer (i64)
        case 'I':           params.data[j++] = wasm_valtype_new(WASM_I64); break;
        // 32-bit float (f32)
        case 'f':           params.data[j++] = wasm_valtype_new(WASM_F32); break;
        // 64-bit float (f64)
        case 'F':           params.data[j++] = wasm_valtype_new(WASM_F64); break;
        // externref type (usually a uintptr_t), or GC references
        case 'r':           params.data[j++] = wasm_valtype_new(WASM_EXTERNREF); break;
        // String in WASM memory
        // Buffer address (pointer) in WASM memory
        case '$': case '*': params.data[j++] = wasm_valtype_new(HAS_64BIT_MEM ? WASM_I64 : WASM_I32); break;
        default: fprintf(stderr, "ERR: unknown sig char %c\n", *s); break;
      }
    }

    wasm_functype_t* func_type = wasm_functype_new(&params, &results);
    wasmtime_func_new_unchecked(context, func_type,
                                (wasmtime_func_unchecked_callback_t)ns->func_ptr,
                                ns->attachment, NULL, &item.of.func);

    if ((err = wasmtime_linker_define(linker, context, "arinc653", strlen("arinc653"), ns->symbol, strlen(ns->symbol), &item)) != NULL) {
      print_wasmtime_error(err);
      wasmtime_linker_delete(linker);
      wasmtime_store_delete(store);
      return -1;
    }
  }

  // Fresh instantiation per call, same as WAMR's wasm_runtime_instantiate()
  wasm_trap_t* trap = NULL;
  wasmtime_instance_t instance;
  if ((err = wasmtime_linker_instantiate(linker, context, wd->module, &instance, &trap)) != NULL) {
    print_wasmtime_error(err);
    wasmtime_linker_delete(linker);
    wasmtime_store_delete(store);
    return -1;
  }

  if (trap) {
    // instantiation trapped (e.g. start function failed)
    wasm_byte_vec_t msg;
    wasm_trap_message(trap, &msg);
    fprintf(stderr, "ERR trap: %.*s\n", (int)msg.size, msg.data);
    wasm_byte_vec_delete(&msg);
    wasm_trap_delete(trap);
    wasmtime_linker_delete(linker);
    wasmtime_store_delete(store);
    return -1;
  }

  wasmtime_func_t fnc;
  if (idx == -1) {
    // mirrors wasm_application_execute_main()
    wasmtime_extern_t ext;
    if (!wasmtime_instance_export_get(context, &instance, "_start", strlen("_start"), &ext)
      || ext.kind != WASMTIME_EXTERN_FUNC) {
      fprintf(stderr, "ERR: _start not found.\n");
      wasmtime_linker_delete(linker);
      wasmtime_store_delete(store);
      return -1;
    }
    fnc = ext.of.func;
  }
  else {
    // mirrors wasm_runtime_call_indirect()
    wasmtime_extern_t ext;
    if (!wasmtime_instance_export_get(context, &instance, "__indirect_function_table", strlen("__indirect_function_table"), &ext)
      || ext.kind != WASMTIME_EXTERN_TABLE) {
      fprintf(stderr, "ERR: __indirect_function_table not found.\n");
      wasmtime_linker_delete(linker);
      wasmtime_store_delete(store);
      return -1;
    }

    wasmtime_val_t val;
    if (!wasmtime_table_get(context, &ext.of.table, idx, &val)) {
      fprintf(stderr, "ERR: Index %d not in __indirect_function_table.\n", idx);
      wasmtime_linker_delete(linker);
      wasmtime_store_delete(store);
      return -1;
    }

    fnc = val.of.funcref;
  }

  trap = NULL;
  if ((err = wasmtime_func_call(context, &fnc, NULL, 0, NULL, 0, &trap)) != NULL) {
    print_wasmtime_error(err);
    wasmtime_linker_delete(linker);
    wasmtime_store_delete(store);
    return -1;
  }

  if (trap) {
    // instantiation trapped (e.g. start function failed)
    wasm_byte_vec_t msg;
    wasm_trap_message(trap, &msg);
    fprintf(stderr, "ERR trap: %.*s\n", (int)msg.size, msg.data);
    wasm_byte_vec_delete(&msg);
    wasm_trap_delete(trap);
    wasmtime_linker_delete(linker);
    wasmtime_store_delete(store);
    return -1;
  }

  wasmtime_linker_delete(linker);
  wasmtime_store_delete(store);
  return 0;
}
