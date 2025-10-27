// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
//#include <wasm.h>
//#include <wasmtime/extern.h>
//#include <wasmtime/val.h>

#include "a653_lib_wasm32/a653_i_common_wasm32.h"
#include "a653_lib_wasm32/arinc653_part1_apex_error_wasm32.h"
#include "a653_lib_wasm32/arinc653_part1_apex_partition_wasm32.h"
#include "a653_lib_wasm32/arinc653_part1_apex_process_wasm32.h"
#include "a653_lib_wasm32/arinc653_part1_apex_queuing_port_wasm32.h"
#include "a653_lib_wasm32/arinc653_part1_apex_sampling_port_wasm32.h"
#include "a653_lib_wasm32/arinc653_part2_apex_sampling_port_extension_wasm32.h"
#include "a653_lib_wasm32/arinc653_part1_apex_semaphore_wasm32.h"
#include "a653_lib_wasm32/arinc653_part1_apex_time_wasm32.h"

/*
 * 0. https://clang.llvm.org/docs/AttributeReference.html#import-module
 *    https://clang.llvm.org/docs/AttributeReference.html#import-name
 *   (https://clang.llvm.org/docs/AttributeReference.html#export-name)
 *    https://lld.llvm.org/WebAssembly.html#imports -> --allow-undefined for imports
 *
 *
 *
 * 1. Ugly learning about WASM: re-alloc of memory -> base address moves.
 * The guests memory is - obviously - backed by malloc/mmap.
 * In case, the guest uses more and more memory, a re-alloc is performed,
 * which must not but can very likely lead to a new memory area within the host.
 * Consequently, the base address of this memory region is very likely different,
 * each time the hosts tries to reference this memory. As of this, we need to
 * get over and over again i.e. for each host func call the (new) memory base addr.
 *
 * 2. Ugly learning about WASM: guest is 32bit, host is 64bit -> e.g. function pointers or unsigned long is different!
 * Well ... in case a struct contains a pointer, this pointer has a different size
 * within the host compared to the guest. This is obvious, however ... one forgets.
 * Thus, in case a struct is passed, which contains a pointer, the struct needs to
 * set up newly within this trampoline.
 *
 * 3. Ugly learning about WASM: WASM specs linker, context and store a not thread-safe. Each thread a new module instance with shared mem between!
 * The WASM linker, context and store are not thread-safe. Thus, whenever one wants to initiate a
 * new thread executing a function within the same WASM, one needs to mutex
 * the pthread context.
 *
 * 4. Ugly learning about WASM: function pointers are indexes, moving such from one thread (module instance) to another is extremely problematic!
 * In WASM function pointers are solely indexes. Furthermore, creating a thread
 * means instantiating a new module incl. own store and context. However, the
 * memory must be shared across them.
 * Issue is, that a https://docs.wasmtime.dev/c-api/structwasmtime__func.html
 * employs __private to determine if the function pointer comes from a specific
 * store. If that one is wrong as it was moved from one module instance i.e. thread
 * to another module instance i.e. another thread, it denies the execution.
 * However, what is truly required is solely the function reference, i.e. the index.
 * The whole implementation is however motivated by wasi-threads trampoline of
 * using WASI_ENTRY_POINT. Which is in this case WASM_GUEST_FUNC_TRAMPOLINE
 * https://github.com/bytecodealliance/wasmtime/blob/main/crates/wasi-threads/src/lib.rs
 * https://raw.githubusercontent.com/bytecodealliance/wasmtime/refs/heads/main/crates/wasi-threads/src/lib.rs
 * https://raw.githubusercontent.com/WebAssembly/wasi-libc/refs/heads/main/libc-top-half/musl/src/thread/wasm32/wasi_thread_start.s
 */

// wasmtime_memory_data_size (const wasmtime_context_t *store, const wasmtime_memory_t *memory) // FIXME: could do a bounds check!




extern wasm_processes_t wasm_processes;

#define WASM_HOSTFUNC_SIGNATURE( FNC ) { #FNC, WASM32_##FNC, WASM32_SIGNATURE__##FNC, NULL }
struct {
  const char* symbol;
  wasmtime_func_unchecked_callback_t func_ptr;
  const char* signature;
  void *attachment;
} wasm_hostfuncs[] = {
/* APEX (ARINC 653 Part 1): BUFFER */
  /* missing */
/* APEX (ARINC 653 Part 1): BLACKBOARD */
  /* missing */
/* APEX (ARINC 653 Part 1): ERROR */
  WASM_HOSTFUNC_SIGNATURE( REPORT_APPLICATION_MESSAGE ),
  WASM_HOSTFUNC_SIGNATURE( CREATE_ERROR_HANDLER ),
  WASM_HOSTFUNC_SIGNATURE( GET_ERROR_STATUS ),
  WASM_HOSTFUNC_SIGNATURE( RAISE_APPLICATION_ERROR ),
  WASM_HOSTFUNC_SIGNATURE( CONFIGURE_ERROR_HANDLER ),
/* APEX (ARINC 653 Part 1): EVENT */
  /* missing */
/* APEX (ARINC 653 Part 1): MUTEX */
  /* missing */
/* APEX (ARINC 653 Part 1): PARTITION */
  WASM_HOSTFUNC_SIGNATURE( GET_PARTITION_STATUS ),
  WASM_HOSTFUNC_SIGNATURE( SET_PARTITION_MODE ),
/* APEX (ARINC 653 Part 1): PROCESS */
  WASM_HOSTFUNC_SIGNATURE( CREATE_PROCESS ),
  WASM_HOSTFUNC_SIGNATURE( SET_PRIORITY ),
  WASM_HOSTFUNC_SIGNATURE( SUSPEND_SELF ),
  WASM_HOSTFUNC_SIGNATURE( SUSPEND ),
  WASM_HOSTFUNC_SIGNATURE( RESUME ),
  WASM_HOSTFUNC_SIGNATURE( STOP_SELF ),
  WASM_HOSTFUNC_SIGNATURE( STOP ),
  WASM_HOSTFUNC_SIGNATURE( START ),
  WASM_HOSTFUNC_SIGNATURE( DELAYED_START ),
  WASM_HOSTFUNC_SIGNATURE( LOCK_PREEMPTION ),
  WASM_HOSTFUNC_SIGNATURE( UNLOCK_PREEMPTION ),
  WASM_HOSTFUNC_SIGNATURE( GET_MY_ID ),
  WASM_HOSTFUNC_SIGNATURE( GET_PROCESS_ID ),
  WASM_HOSTFUNC_SIGNATURE( GET_PROCESS_STATUS ),
  WASM_HOSTFUNC_SIGNATURE( INITIALIZE_PROCESS_CORE_AFFINITY ),
  WASM_HOSTFUNC_SIGNATURE( GET_MY_PROCESSOR_CORE_ID ),
  WASM_HOSTFUNC_SIGNATURE( GET_MY_INDEX ),
/* APEX (ARINC 653 Part 1): QUEUING PORT */
  WASM_HOSTFUNC_SIGNATURE( CREATE_QUEUING_PORT ),
  WASM_HOSTFUNC_SIGNATURE( SEND_QUEUING_MESSAGE ),
  WASM_HOSTFUNC_SIGNATURE( RECEIVE_QUEUING_MESSAGE ),
  WASM_HOSTFUNC_SIGNATURE( GET_QUEUING_PORT_ID ),
  WASM_HOSTFUNC_SIGNATURE( GET_QUEUING_PORT_STATUS ),
  WASM_HOSTFUNC_SIGNATURE( CLEAR_QUEUING_PORT ),
/* APEX (ARINC 653 Part 1): SAMPLING PORT */
  WASM_HOSTFUNC_SIGNATURE( CREATE_SAMPLING_PORT ),
  WASM_HOSTFUNC_SIGNATURE( WRITE_SAMPLING_MESSAGE ),
  WASM_HOSTFUNC_SIGNATURE( READ_SAMPLING_MESSAGE ),
  WASM_HOSTFUNC_SIGNATURE( GET_SAMPLING_PORT_ID ),
  WASM_HOSTFUNC_SIGNATURE( GET_SAMPLING_PORT_STATUS ),
/* APEX (ARINC 653 Part 2: SAMPLING PORT EXTENSIONS */
  WASM_HOSTFUNC_SIGNATURE( READ_UPDATED_SAMPLING_MESSAGE ),
  WASM_HOSTFUNC_SIGNATURE( GET_SAMPLING_PORT_CURRENT_STATUS ),
  WASM_HOSTFUNC_SIGNATURE( READ_SAMPLING_MESSAGE_CONDITIONAL ),
/* APEX (ARINC 653 Part 1): SEMAPHORE */
  WASM_HOSTFUNC_SIGNATURE( CREATE_SEMAPHORE ),
  WASM_HOSTFUNC_SIGNATURE( WAIT_SEMAPHORE ),
  WASM_HOSTFUNC_SIGNATURE( SIGNAL_SEMAPHORE ),
  WASM_HOSTFUNC_SIGNATURE( GET_SEMAPHORE_ID ),
  WASM_HOSTFUNC_SIGNATURE( GET_SEMAPHORE_STATUS ),
/* APEX (ARINC 653 Part 1): TIME */
  WASM_HOSTFUNC_SIGNATURE( TIMED_WAIT ),
  WASM_HOSTFUNC_SIGNATURE( PERIODIC_WAIT ),
  WASM_HOSTFUNC_SIGNATURE( GET_TIME ),
  WASM_HOSTFUNC_SIGNATURE( REPLENISH )
};

// Helper to load a binary file (e.g., guest.wasm)
int load_wasm_file(wasm_byte_vec_t* wasm, const char* filename)
{
  FILE* file = fopen(filename, "rb");
  if (!file) {
    fprintf(stderr, "❌ Failed to open wasm file");
    return -1;
  }

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  wasm_byte_vec_new_uninitialized(wasm, file_size);
  if (fread(wasm->data, file_size, 1, file) != 1) {
    fprintf(stderr, "❌ Failed to read wasm file");
    return -1;
  }

  fclose(file);
  return 0;
}

static int signature_parameter_count(const char *signature) {
  int parmc = 0;
  for (char *s = (char*)signature, *bgn_braket = NULL; *s != '\0'; ++s) {
    switch ( *s ) {
      case '(':
        bgn_braket = s;
        break;
      case ')':
        if (*bgn_braket != '(') {
          fprintf(stderr, "ERR: end braket without begin braket!\n");
          return -1;
        }
        return parmc; // done
      break;
      case 'i': // 32-bit integer (i32)
      case 'I': // 64-bit integer (i64)
      case 'f': // 32-bit float (f32)
      case 'F': // 64-bit float (f64)
      case 'r': // externref type (usually a uintptr_t), or GC references
      case '$': // String in WASM memory
        ++parmc;
        break;
      case '*': // Buffer address (pointer) in WASM memory
        ++s;
        if(*s != '~') {
          fprintf(stderr, "ERR: not supported character sequence *%c!\n", *s);
          return -1;
        }
        parmc += 2;
      default:
      case '~': // Byte length of the preceding buffer pointer (*), must follow *
        fprintf(stderr, "ERR: not supported character %c!\n", *s);
        return -1;
    }
  }
  return -1;
}


void initialize_wasm_instance(
  wasm_engine_t* engine,
  wasmtime_sharedmemory_t* shm_memory,
  wasmtime_module_t* module,

  wasmtime_linker_t** _linker,
  wasmtime_store_t** _store,
  wasmtime_context_t** _context,
  wasmtime_instance_t* instance)
{
  // Configure WASI context (currently given for debugging... not for true avionics)
  wasi_config_t* wasi_config = wasi_config_new();
  wasi_config_inherit_argv(wasi_config);
  wasi_config_inherit_env(wasi_config);
  wasi_config_inherit_stdout(wasi_config);
  wasi_config_inherit_stderr(wasi_config);

  wasmtime_store_t* store = *_store = wasmtime_store_new(engine, NULL, NULL);
  wasmtime_context_t* context = *_context = wasmtime_store_context(store);

  wasmtime_error_t* error;
  if ((error = wasmtime_context_set_wasi(context, wasi_config)) != NULL) {
    print_wasmtime_error(error);
    return;
  }

  // Create linker and define WASI
  wasmtime_linker_t* linker = *_linker = wasmtime_linker_new(engine);
  if ((error = wasmtime_linker_define_wasi(linker)) != NULL) {
    print_wasmtime_error(error);
    return;
  }

  wasmtime_extern_t import;
  import.kind = WASMTIME_EXTERN_SHAREDMEMORY;
  import.of.sharedmemory = shm_memory;

  // Link shared memory to "wasi" module (or your module namespace)
  if ((error = wasmtime_linker_define(linker, context, "wasi", 4, "memory", 6, &import)) != NULL) {
    print_wasmtime_error(error);
    return;
  }


#if 0
  if(create_func_table) {
//////////////////////// function table ...
    const size_t table_min = 40; // FIXME: the minimal amount of functions supported, grow if needed ...
/*
    TODO: grow the table i*n case needed ... -> could be long term a race condition issue :-/ at best have sufficient ...
    uint64_t old_size;
    wasmtime_error_t* success = wasmtime_table_grow(context, wasm_processes.shared_table, 5, &init_val, &old_size);
*/
    const size_t table_max = 100;

    // Create an array of wasmtime_val_t, each with kind = WASMTIME_VAL_KIND_FUNCREF and null value to initialize
    // Must initialize the minimum of the table! In case running out of entries -> grow (see below)
    const wasmtime_val_t init_val = { .kind = WASMTIME_FUNCREF };
    //init_val.of.funcref = (wasmtime_func_t) {0}; // Initialize with NULL pointer!

    // Create an element type, e.g. funcref
    wasm_valtype_t *elem_type = wasm_valtype_new(WASM_FUNCREF);

    // Define limits: min and optional max number of elements
    wasm_limits_t limits = { .min = table_min, .max = table_max };

    wasm_tabletype_t *table_type = wasm_tabletype_new(elem_type, &limits);
printf("!!!!!!!!!! works here!\n");
    // Create a shared table in first store
    if((error = wasmtime_table_new(context, table_type, &init_val, &wasm_processes.shared_table)) != NULL) {
      printf("!!!!!!!!!! died!\n");
      exit_with_error("❌ Failed to create shared funcref table\n", error, NULL);
    }
printf("!!!!!!!!!! works here!\n");
////////////////////////
  }
  else {
//////////////////////// function table ...
    // Shared table import
    wasmtime_extern_t table_import;
    table_import.kind = WASMTIME_EXTERN_TABLE;
    table_import.of.table = wasm_processes.shared_table;
    if((error = wasmtime_linker_define(linker, context, "env", 3, "shared_table", 12, &table_import)) != NULL) {
      exit_with_error("❌ Could not import shared table\n", error, NULL);
    }
////////////////////////
  }
#endif

//---------------------------------------------------

  wasm_valtype_vec_t results;
  wasm_valtype_vec_new_empty(&results);

  bool has_64bit_memory = false; // FIXME

  // Create the host function
  wasmtime_extern_t item;
  item.kind = WASMTIME_EXTERN_FUNC;

  for(unsigned i = 0; i < sizeof(wasm_hostfuncs)/sizeof(wasm_hostfuncs[0]); ++i) {
    typeof(wasm_hostfuncs[0])* wasm_hostfunc = &wasm_hostfuncs[i];

    const char *signature = wasm_hostfunc->signature;
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
        case 'R': // funcref ... not official and only applicable for Wasmtime (not WAMR)
          params.data[j++] = wasm_valtype_new(WASM_FUNCREF);
          break;
        case '$': // String in WASM memory
        case '*': // Buffer address (pointer) in WASM memory
          params.data[j++] = wasm_valtype_new(has_64bit_memory ? WASM_I64 : WASM_I32);
          break;
        default:
          fprintf(stderr, "ERR: wrong character %c\n!", *s);
          break;
      }
    }

    wasm_functype_t* func_type = wasm_functype_new(&params, &results);

    wasmtime_func_new_unchecked(
      context,
      func_type,
      wasm_hostfunc->func_ptr,
      wasm_hostfunc->attachment, NULL,
      &item.of.func
    );

    // Define it in the linker
    error = wasmtime_linker_define(
      linker,
      context,
      "arinc653", strlen("arinc653"),
      wasm_hostfunc->symbol, strlen(wasm_hostfunc->symbol),
      &item
    );
    if (error != NULL) {
      print_wasmtime_error(error);
      return;
    }
  }

  // Add the compiled module to the linker
  error = wasmtime_linker_module(linker, context, NULL, 0, module);
  if (error != NULL) {
    print_wasmtime_error(error);
    return;
  }

  // Instantiate the module; all host functions created don't provide a trap
  error = wasmtime_linker_instantiate(linker, context, module, instance, NULL);
  if (error != NULL) {
    print_wasmtime_error(error);
    return;
  }
}



int main(int argc, char* argv[])
{
  // FIXME with proper getopt()
  if (argc < 2) {
    fprintf(stderr, "Usage: %s guest.wasm\n", argv[0]);
    return -1;
  }
  const char *wasm_file = argv[1];

  // there are more occurances, don't just enable!
  // currently only 32bit supported!
  bool has_64bit_memory = false;

  // Configure WASI
  wasm_config_t *wasm_config = wasm_config_new();
  wasmtime_config_wasm_memory64_set(wasm_config, has_64bit_memory);
  wasmtime_config_cranelift_opt_level_set (wasm_config, WASMTIME_OPT_LEVEL_SPEED);
  
  // Initialize Wasmtime
  wasm_processes.engine = wasm_engine_new_with_config(wasm_config);

  // Create shared memory (required for threading)
  wasm_memorytype_t *mem_type = wasmtime_memorytype_new(
    1 /* min 64KB-pages */, true /* max present (must for shared) */, 65536 /* max 64KB-pages -> 4GB */,
    has_64bit_memory /* is_64 */, true /* is shared ! required for threading */
  );
  wasm_processes.shm_memory = NULL;
  wasmtime_sharedmemory_new(wasm_processes.engine, mem_type, &wasm_processes.shm_memory);

  // Load .wasm binary supplied
  wasm_byte_vec_t wasm;
  load_wasm_file(&wasm, wasm_file);

  // Compile the module
  wasmtime_error_t* err;
  if ((err = wasmtime_module_new(wasm_processes.engine, (uint8_t*)wasm.data, wasm.size, &wasm_processes.module)) != NULL) {
    print_wasmtime_error(err);
    return -1;
  }
  wasm_byte_vec_delete(&wasm);

  if ( ! exec_wasm_guest_func(1, -1))
    fprintf(stderr, "ERR: wasm_processid not found\n");

  printf("✅ Module executed successfully via _start\n");

  // Cleanup
  wasmtime_module_delete(wasm_processes.module);
  wasm_engine_delete(wasm_processes.engine);

  return 0;
}
