// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: PROCESS

#include <dlfcn.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "arinc653_part1_apex_process_wasm32.h"
#include "camw32_getset.h" /* auto-generated header */
#include "../a653_lib/a653_i_process.h"
#include "../a653_inc/a653Process.h"


#if 0
extern void GET_PROCESS_ID (
/* in  */ PROCESS_NAME_TYPE PROCESS_NAME,
/* out */ PROCESS_ID_TYPE * PROCESS_ID,
/* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
const char* WASM32_SIGNATURE__GET_PROCESS_ID = "(iii)";
wasm_trap_t* WASM32_GET_PROCESS_ID(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  GET_PROCESS_ID(
    (char*)&wasm_baseaddr[args[0].of.i32],
    (PROCESS_ID_TYPE*)&wasm_baseaddr[args[1].of.i32],
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[2].of.i32]
  );

  return NULL;
}



/*
motivated by the WASI-THREADS

in a nutshell:
- Each thread is spawn as a new instance of a WASM module.
- Each instance of a WASM module uses then shared memory to ensure the threads
can communicate with each other.
*/

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

extern prcs_info_t *prcs_info;
extern wasm_processes_t wasm_processes;

void *wasm_trampoline(void) {
  /* trying to reverse engineer the index within the a653lib */
  pthread_t tid = pthread_self();

  for(unsigned i = 0; i < MAX_PRCS; ++i) {
    if(prcs_info[i].t_ctx == tid) {
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
      args[0].of.i32 = wasm_processes.ENTRY_POINT[prcs_info[i].id];
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
  }
  fprintf(stderr, "ERR: wasm_processid not found\n");

  return NULL;
}


#if 0
/* process attribute type    */
typedef struct {
SYSTEM_TIME_TYPE      PERIOD;               /* process period            */
SYSTEM_TIME_TYPE      TIME_CAPACITY;        /* time capacity             */
SYSTEM_ADDRESS_TYPE   ENTRY_POINT;          /* starting address          */
STACK_SIZE_TYPE       STACK_SIZE;           /* stack size                */
PRIORITY_TYPE         BASE_PRIORITY;        /* process base priority     */
DEADLINE_TYPE         DEADLINE;             /* deadline type (soft/hard) */
PROCESS_NAME_TYPE     NAME;                 /* process name              */
} PROCESS_ATTRIBUTE_TYPE;

/* process status type      */
typedef struct {
SYSTEM_TIME_TYPE       DEADLINE_TIME;       /* time to next deadline    */
PRIORITY_TYPE          CURRENT_PRIORITY;    /* process current priority */
PROCESS_STATE_TYPE     PROCESS_STATE;       /* process state            */
PROCESS_ATTRIBUTE_TYPE ATTRIBUTES;          /* process attributes       */
} PROCESS_STATUS_TYPE;

extern void GET_PROCESS_STATUS (
/* in  */ PROCESS_ID_TYPE PROCESS_ID,
/* out */ PROCESS_STATUS_TYPE * PROCESS_STATUS,
/* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
const char* WASM32_SIGNATURE__GET_PROCESS_STATUS = "(iii)";
wasm_trap_t* WASM32_GET_PROCESS_STATUS(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  PROCESS_ID_TYPE pid = (PROCESS_ID_TYPE)args[0].of.i32;
  PROCESS_STATUS_TYPE STATUS__host_64bit;
  GET_PROCESS_STATUS(
    pid,
    &STATUS__host_64bit,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[2].of.i32]
  );

  // FIXME: pointer is 32bit, but could be 64bit ..
  uint8_t* STATUS_guest = (uint8_t*)&wasm_baseaddr[args[1].of.i32];



#if 0
/* process attribute type    */
typedef struct {
  SYSTEM_TIME_TYPE      PERIOD;               /* process period            */
  SYSTEM_TIME_TYPE      TIME_CAPACITY;        /* time capacity             */
  SYSTEM_ADDRESS_TYPE   ENTRY_POINT;          /* starting address          */
  STACK_SIZE_TYPE       STACK_SIZE;           /* stack size                */
  PRIORITY_TYPE         BASE_PRIORITY;        /* process base priority     */
  DEADLINE_TYPE         DEADLINE;             /* deadline type (soft/hard) */
  PROCESS_NAME_TYPE     NAME;                 /* process name              */
} PROCESS_ATTRIBUTE_TYPE;

/* process status type      */
typedef struct {
  SYSTEM_TIME_TYPE       DEADLINE_TIME;       /* time to next deadline    */
  PRIORITY_TYPE          CURRENT_PRIORITY;    /* process current priority */
  PROCESS_STATE_TYPE     PROCESS_STATE;       /* process state            */
  PROCESS_ATTRIBUTE_TYPE ATTRIBUTES;          /* process attributes       */
} PROCESS_STATUS_TYPE;
#endif


  camw32_set__PROCESS_STATUS_TYPE__DEADLINE_TIME(STATUS_guest, STATUS__host_64bit.DEADLINE_TIME);
  camw32_set__PROCESS_STATUS_TYPE__CURRENT_PRIORITY(STATUS_guest, STATUS__host_64bit.CURRENT_PRIORITY);
  camw32_set__PROCESS_STATUS_TYPE__PROCESS_STATE(STATUS_guest, STATUS__host_64bit.PROCESS_STATE);

  uint8_t* ATTRIBUTES_guest = camw32_get_struct_base_addr__PROCESS_STATUS_TYPE__ATTRIBUTES(STATUS_guest);
  camw32_set__PROCESS_ATTRIBUTE_TYPE__PERIOD(ATTRIBUTES_guest, STATUS__host_64bit.ATTRIBUTES.PERIOD);
  camw32_set__PROCESS_ATTRIBUTE_TYPE__TIME_CAPACITY(ATTRIBUTES_guest, STATUS__host_64bit.ATTRIBUTES.TIME_CAPACITY);
  uint32_t ENTRY_POINT_idx = wasm_processes.ENTRY_POINT[pid];
  camw32_set__PROCESS_ATTRIBUTE_TYPE__ENTRY_POINT(ATTRIBUTES_guest, ENTRY_POINT_idx);
  camw32_set__PROCESS_ATTRIBUTE_TYPE__STACK_SIZE(ATTRIBUTES_guest, STATUS__host_64bit.ATTRIBUTES.STACK_SIZE);
  camw32_set__PROCESS_ATTRIBUTE_TYPE__BASE_PRIORITY(ATTRIBUTES_guest, STATUS__host_64bit.ATTRIBUTES.BASE_PRIORITY);
  camw32_set__PROCESS_ATTRIBUTE_TYPE__DEADLINE(ATTRIBUTES_guest, STATUS__host_64bit.ATTRIBUTES.DEADLINE);
  camw32_write__PROCESS_ATTRIBUTE_TYPE__NAME(ATTRIBUTES_guest, STATUS__host_64bit.ATTRIBUTES.NAME);

  return NULL;
}



#if 0
/* process attribute type    */
typedef struct {
SYSTEM_TIME_TYPE      PERIOD;               /* process period            */
SYSTEM_TIME_TYPE      TIME_CAPACITY;        /* time capacity             */
SYSTEM_ADDRESS_TYPE   ENTRY_POINT;          /* starting address          */
STACK_SIZE_TYPE       STACK_SIZE;           /* stack size                */
PRIORITY_TYPE         BASE_PRIORITY;        /* process base priority     */
DEADLINE_TYPE         DEADLINE;             /* deadline type (soft/hard) */
PROCESS_NAME_TYPE     NAME;                 /* process name              */
} PROCESS_ATTRIBUTE_TYPE;

extern void CREATE_PROCESS (
/* in  */ PROCESS_ATTRIBUTE_TYPE * ATTRIBUTES,
/* out */ PROCESS_ID_TYPE * PROCESS_ID,
/* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
const char* WASM32_SIGNATURE__CREATE_PROCESS = "(iii)";
wasm_trap_t* WASM32_CREATE_PROCESS(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  // FIXME: pointer is 32bit, but could be 64bit ..
  uint8_t* ATTRIBUTES__guest = (uint8_t*)&wasm_baseaddr[args[0].of.i32];

  PROCESS_ATTRIBUTE_TYPE ATTRIBUTES__host_64bit;
  ATTRIBUTES__host_64bit.PERIOD = camw32_get__PROCESS_ATTRIBUTE_TYPE__PERIOD(ATTRIBUTES__guest);
  ATTRIBUTES__host_64bit.TIME_CAPACITY = camw32_get__PROCESS_ATTRIBUTE_TYPE__TIME_CAPACITY(ATTRIBUTES__guest);
  ATTRIBUTES__host_64bit.ENTRY_POINT = (typeof(ATTRIBUTES__host_64bit.ENTRY_POINT))&wasm_trampoline;
  uint32_t ENTRY_POINT_idx = (uint32_t)camw32_get__PROCESS_ATTRIBUTE_TYPE__ENTRY_POINT(ATTRIBUTES__guest);

  ATTRIBUTES__host_64bit.STACK_SIZE = camw32_get__PROCESS_ATTRIBUTE_TYPE__STACK_SIZE(ATTRIBUTES__guest);
  ATTRIBUTES__host_64bit.BASE_PRIORITY = camw32_get__PROCESS_ATTRIBUTE_TYPE__BASE_PRIORITY(ATTRIBUTES__guest);
  ATTRIBUTES__host_64bit.DEADLINE = camw32_get__PROCESS_ATTRIBUTE_TYPE__DEADLINE(ATTRIBUTES__guest);
  camw32_read__PROCESS_ATTRIBUTE_TYPE__NAME(ATTRIBUTES__guest, ATTRIBUTES__host_64bit.NAME);

  PROCESS_ID_TYPE* pid = (PROCESS_ID_TYPE*)&wasm_baseaddr[args[1].of.i32];
  CREATE_PROCESS(
    &ATTRIBUTES__host_64bit,
    pid,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[2].of.i32]
  );

  printf("WASM32_CREATE_PROCESS(..., %d, %d)\n", *pid, wasm_baseaddr[args[2].of.i32]);

  // we get the pid late, and the real start of the thread will be in CREATE_PROCESS
  wasm_processes.ENTRY_POINT[*pid] = ENTRY_POINT_idx;

  return NULL;
}


#if 0
extern void SET_PRIORITY (
/* in  */ PROCESS_ID_TYPE PROCESS_ID,
/* in  */ PRIORITY_TYPE PRIORITY,
/* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
const char* WASM32_SIGNATURE__SET_PRIORITY = "(iii)";
wasm_trap_t* WASM32_SET_PRIORITY(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  SET_PRIORITY(
    (PROCESS_ID_TYPE)args[0].of.i32,
    (PRIORITY_TYPE)args[1].of.i32,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );

  return NULL;
}


#if 0
extern void SUSPEND_SELF (
/* in  */ SYSTEM_TIME_TYPE TIME_OUT,
/* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
const char* WASM32_SIGNATURE__SUSPEND_SELF = "(Ii)";
wasm_trap_t* WASM32_SUSPEND_SELF(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  SUSPEND_SELF(
    (SYSTEM_TIME_TYPE)args[0].of.i64,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );

  return NULL;
}


#if 0
extern void SUSPEND (
/* in  */ PROCESS_ID_TYPE PROCESS_ID,
/* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
const char* WASM32_SIGNATURE__SUSPEND = "(ii)";
wasm_trap_t* WASM32_SUSPEND(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  SUSPEND(
    (PROCESS_ID_TYPE)args[0].of.i32,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );

  return NULL;
}


#if 0
extern void RESUME (
/* in  */ PROCESS_ID_TYPE PROCESS_ID,
/* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
const char* WASM32_SIGNATURE__RESUME = "(ii)";
wasm_trap_t* WASM32_RESUME(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  RESUME(
    (PROCESS_ID_TYPE)args[0].of.i32,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );

  return NULL;
}


#if 0
extern void STOP_SELF  (void);
#endif
const char* WASM32_SIGNATURE__STOP_SELF = "()";
wasm_trap_t* WASM32_STOP_SELF(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  STOP_SELF(
  );

  return NULL;
}


#if 0
extern void STOP (
/* in  */ PROCESS_ID_TYPE PROCESS_ID,
/* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
const char* WASM32_SIGNATURE__STOP = "(ii)";
wasm_trap_t* WASM32_STOP(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  STOP(
    (PROCESS_ID_TYPE)args[0].of.i32,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );

  return NULL;
}


#if 0
extern void START (
/* in  */ PROCESS_ID_TYPE PROCESS_ID,
/* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
const char* WASM32_SIGNATURE__START = "(ii)";
wasm_trap_t* WASM32_START(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  printf("WASM32_START(%d, ...)\n", args[0].of.i32);
  START(
    (PROCESS_ID_TYPE)args[0].of.i32,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );

  return NULL;
}


#if 0
extern void DELAYED_START (
/* in  */ PROCESS_ID_TYPE PROCESS_ID,
/* in  */ SYSTEM_TIME_TYPE  DELAY_TIME,
/* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
const char* WASM32_SIGNATURE__DELAYED_START = "(iIi)";
wasm_trap_t* WASM32_DELAYED_START(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  DELAYED_START(
    (PROCESS_ID_TYPE)args[0].of.i32,
    (SYSTEM_TIME_TYPE)args[1].of.i64,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[2].of.i32]
  );

  return NULL;
}


#if 0
extern void LOCK_PREEMPTION (
/* out */ LOCK_LEVEL_TYPE * LOCK_LEVEL,
/* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
const char* WASM32_SIGNATURE__LOCK_PREEMPTION = "(ii)";
wasm_trap_t* WASM32_LOCK_PREEMPTION(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  LOCK_PREEMPTION(
    (LOCK_LEVEL_TYPE*)&wasm_baseaddr[args[0].of.i32],
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );

  return NULL;
}


#if 0
extern void UNLOCK_PREEMPTION (
/* out */ LOCK_LEVEL_TYPE * LOCK_LEVEL,
/* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
const char* WASM32_SIGNATURE__UNLOCK_PREEMPTION = "(ii)";
wasm_trap_t* WASM32_UNLOCK_PREEMPTION(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  UNLOCK_PREEMPTION(
    (LOCK_LEVEL_TYPE*)&wasm_baseaddr[args[0].of.i32],
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );

  return NULL;
}



#if 0
extern void GET_MY_ID (
/* out */ PROCESS_ID_TYPE * PROCESS_ID,
/* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
const char* WASM32_SIGNATURE__GET_MY_ID = "(ii)";
wasm_trap_t* WASM32_GET_MY_ID(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  GET_MY_ID(
    (PROCESS_ID_TYPE*)&wasm_baseaddr[args[0].of.i32],
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );

  return NULL;
}


#if 0
extern void INITIALIZE_PROCESS_CORE_AFFINITY (
  /*in */ PROCESS_ID_TYPE          PROCESS_ID,
  /*in */ PROCESSOR_CORE_ID_TYPE   PROCESSOR_CORE_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__INITIALIZE_PROCESS_CORE_AFFINITY = "(iii)";
wasm_trap_t* WASM32_INITIALIZE_PROCESS_CORE_AFFINITY(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  INITIALIZE_PROCESS_CORE_AFFINITY(
    (PROCESS_ID_TYPE)args[0].of.i32,
    (PROCESSOR_CORE_ID_TYPE)args[1].of.i32,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[2].of.i32]
  );

  return NULL;
}


#if 0
extern void GET_MY_PROCESSOR_CORE_ID (
  /*out*/ PROCESSOR_CORE_ID_TYPE   *PROCESSOR_CORE_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__GET_MY_PROCESSOR_CORE_ID = "(ii)";
wasm_trap_t* WASM32_GET_MY_PROCESSOR_CORE_ID(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  GET_MY_PROCESSOR_CORE_ID(
    (PROCESSOR_CORE_ID_TYPE*)&wasm_baseaddr[args[0].of.i32],
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );

  return NULL;
}


#if 0
extern void GET_MY_INDEX (
  /*out*/ PROCESS_INDEX_TYPE       *PROCESS_INDEX,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );
#endif
const char* WASM32_SIGNATURE__GET_MY_INDEX = "(ii)";
wasm_trap_t* WASM32_GET_MY_INDEX(void* env,
  wasmtime_caller_t* caller, const wasmtime_val_t* args, size_t nargs,
  wasmtime_val_t* results, size_t nresults)
{
  wasmtime_context_t *context = wasmtime_caller_context(caller);
  wasmtime_memory_t memory;
  get_exported_memory(caller, &memory);
  uint8_t* wasm_baseaddr = wasmtime_memory_data(context, &memory);

  GET_MY_INDEX(
    (PROCESS_INDEX_TYPE*)&wasm_baseaddr[args[0].of.i32],
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[1].of.i32]
  );

  return NULL;
}

