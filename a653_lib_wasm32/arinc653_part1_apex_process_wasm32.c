// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: PROCESS

#include <dlfcn.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "arinc653_part1_apex_process_wasm32.h"
#include "wasm32_struct_getset.h" /* auto-generated header */
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

  void *dl_struct_getset = env;

  // FIXME: pointer is 32bit, but could be 64bit ..
  PROCESS_STATUS_TYPE* STATUS_guest = (PROCESS_STATUS_TYPE*)&wasm_baseaddr[args[1].of.i32];

#if 0
/* process status type      */
typedef struct {
  SYSTEM_TIME_TYPE       DEADLINE_TIME;       /* time to next deadline    */
  PRIORITY_TYPE          CURRENT_PRIORITY;    /* process current priority */
  PROCESS_STATE_TYPE     PROCESS_STATE;       /* process state            */
  PROCESS_ATTRIBUTE_TYPE ATTRIBUTES;          /* process attributes       */
} PROCESS_STATUS_TYPE;
#endif

  SET__PROCESS_STATUS_TYPE__DEADLINE_TIME__FNC_PTR_DCL(SET__PROCESS_STATUS_TYPE__DEADLINE_TIME)
    = (SET__PROCESS_STATUS_TYPE__DEADLINE_TIME__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PROCESS_STATUS_TYPE__DEADLINE_TIME");
  if (! SET__PROCESS_STATUS_TYPE__DEADLINE_TIME) {
    fprintf(stderr, "ERR: SET__PROCESS_STATUS_TYPE__DEADLINE_TIME not present!\n");
    exit(-1);
  }

  SET__PROCESS_STATUS_TYPE__CURRENT_PRIORITY__FNC_PTR_DCL(SET__PROCESS_STATUS_TYPE__CURRENT_PRIORITY)
  = (SET__PROCESS_STATUS_TYPE__CURRENT_PRIORITY__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PROCESS_STATUS_TYPE__CURRENT_PRIORITY");
  if (! SET__PROCESS_STATUS_TYPE__CURRENT_PRIORITY) {
    fprintf(stderr, "ERR: SET__PROCESS_STATUS_TYPE__CURRENT_PRIORITY not present!\n");
    exit(-1);
  }

  SET__PROCESS_STATUS_TYPE__PROCESS_STATE__FNC_PTR_DCL(SET__PROCESS_STATUS_TYPE__PROCESS_STATE)
  = (SET__PROCESS_STATUS_TYPE__PROCESS_STATE__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PROCESS_STATUS_TYPE__PROCESS_STATE");
  if (! SET__PROCESS_STATUS_TYPE__PROCESS_STATE) {
    fprintf(stderr, "ERR: SET__PROCESS_STATUS_TYPE__PROCESS_STATE not present!\n");
    exit(-1);
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
#endif

  SET__PROCESS_STATUS_TYPE__ATTRIBUTES__PERIOD__FNC_PTR_DCL(SET__PROCESS_STATUS_TYPE__ATTRIBUTES__PERIOD)
  = (SET__PROCESS_STATUS_TYPE__ATTRIBUTES__PERIOD__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PROCESS_STATUS_TYPE__ATTRIBUTES__PERIOD");
  if (! SET__PROCESS_STATUS_TYPE__ATTRIBUTES__PERIOD) {
    fprintf(stderr, "ERR: SET__PROCESS_STATUS_TYPE__ATTRIBUTES__PERIOD not present!\n");
    exit(-1);
  }

  SET__PROCESS_STATUS_TYPE__ATTRIBUTES__TIME_CAPACITY__FNC_PTR_DCL(SET__PROCESS_STATUS_TYPE__ATTRIBUTES__TIME_CAPACITY)
  = (SET__PROCESS_STATUS_TYPE__ATTRIBUTES__TIME_CAPACITY__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PROCESS_STATUS_TYPE__ATTRIBUTES__TIME_CAPACITY");
  if (! SET__PROCESS_STATUS_TYPE__ATTRIBUTES__TIME_CAPACITY) {
    fprintf(stderr, "ERR: SET__PROCESS_STATUS_TYPE__ATTRIBUTES__TIME_CAPACITY not present!\n");
    exit(-1);
  }

  SET__PROCESS_STATUS_TYPE__ATTRIBUTES__ENTRY_POINT__FNC_PTR_DCL(SET__PROCESS_STATUS_TYPE__ATTRIBUTES__ENTRY_POINT)
  = (SET__PROCESS_STATUS_TYPE__ATTRIBUTES__ENTRY_POINT__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PROCESS_STATUS_TYPE__ATTRIBUTES__ENTRY_POINT");
  if (! SET__PROCESS_STATUS_TYPE__ATTRIBUTES__ENTRY_POINT) {
    fprintf(stderr, "ERR: SET__PROCESS_STATUS_TYPE__ATTRIBUTES__ENTRY_POINT not present!\n");
    exit(-1);
  }

  SET__PROCESS_STATUS_TYPE__ATTRIBUTES__STACK_SIZE__FNC_PTR_DCL(SET__PROCESS_STATUS_TYPE__ATTRIBUTES__STACK_SIZE)
  = (SET__PROCESS_STATUS_TYPE__ATTRIBUTES__STACK_SIZE__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PROCESS_STATUS_TYPE__ATTRIBUTES__STACK_SIZE");
  if (! SET__PROCESS_STATUS_TYPE__ATTRIBUTES__STACK_SIZE) {
    fprintf(stderr, "ERR: SET__PROCESS_STATUS_TYPE__ATTRIBUTES__STACK_SIZE not present!\n");
    exit(-1);
  }

  SET__PROCESS_STATUS_TYPE__ATTRIBUTES__BASE_PRIORITY__FNC_PTR_DCL(SET__PROCESS_STATUS_TYPE__ATTRIBUTES__BASE_PRIORITY)
  = (SET__PROCESS_STATUS_TYPE__ATTRIBUTES__BASE_PRIORITY__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PROCESS_STATUS_TYPE__ATTRIBUTES__BASE_PRIORITY");
  if (! SET__PROCESS_STATUS_TYPE__ATTRIBUTES__BASE_PRIORITY) {
    fprintf(stderr, "ERR: SET__PROCESS_STATUS_TYPE__ATTRIBUTES__BASE_PRIORITY not present!\n");
    exit(-1);
  }

  SET__PROCESS_STATUS_TYPE__ATTRIBUTES__DEADLINE__FNC_PTR_DCL(SET__PROCESS_STATUS_TYPE__ATTRIBUTES__DEADLINE)
  = (SET__PROCESS_STATUS_TYPE__ATTRIBUTES__DEADLINE__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PROCESS_STATUS_TYPE__ATTRIBUTES__DEADLINE");
  if (! SET__PROCESS_STATUS_TYPE__ATTRIBUTES__DEADLINE) {
    fprintf(stderr, "ERR: SET__PROCESS_STATUS_TYPE__ATTRIBUTES__DEADLINE not present!\n");
    exit(-1);
  }

  SET__PROCESS_STATUS_TYPE__ATTRIBUTES__NAME__FNC_PTR_DCL(SET__PROCESS_STATUS_TYPE__ATTRIBUTES__NAME)
  = (SET__PROCESS_STATUS_TYPE__ATTRIBUTES__NAME__FNC_PTR_DCL()) dlsym (dl_struct_getset, "SET__PROCESS_STATUS_TYPE__ATTRIBUTES__NAME");
  if (! SET__PROCESS_STATUS_TYPE__ATTRIBUTES__NAME) {
    fprintf(stderr, "ERR: SET__PROCESS_STATUS_TYPE__ATTRIBUTES__NAME not present!\n");
    exit(-1);
  }

  SET__PROCESS_STATUS_TYPE__DEADLINE_TIME(STATUS_guest, STATUS__host_64bit.DEADLINE_TIME);
  SET__PROCESS_STATUS_TYPE__CURRENT_PRIORITY(STATUS_guest, STATUS__host_64bit.CURRENT_PRIORITY);
  SET__PROCESS_STATUS_TYPE__PROCESS_STATE(STATUS_guest, STATUS__host_64bit.PROCESS_STATE);
  SET__PROCESS_STATUS_TYPE__ATTRIBUTES__PERIOD(STATUS_guest, STATUS__host_64bit.ATTRIBUTES.PERIOD);
  SET__PROCESS_STATUS_TYPE__ATTRIBUTES__TIME_CAPACITY(STATUS_guest, STATUS__host_64bit.ATTRIBUTES.TIME_CAPACITY);
  uint32_t ENTRY_POINT_idx = wasm_processes.ENTRY_POINT[pid];
  SET__PROCESS_STATUS_TYPE__ATTRIBUTES__ENTRY_POINT(STATUS_guest, ENTRY_POINT_idx);
  SET__PROCESS_STATUS_TYPE__ATTRIBUTES__STACK_SIZE(STATUS_guest, STATUS__host_64bit.ATTRIBUTES.STACK_SIZE);
  SET__PROCESS_STATUS_TYPE__ATTRIBUTES__BASE_PRIORITY(STATUS_guest, STATUS__host_64bit.ATTRIBUTES.BASE_PRIORITY);
  SET__PROCESS_STATUS_TYPE__ATTRIBUTES__DEADLINE(STATUS_guest, STATUS__host_64bit.ATTRIBUTES.DEADLINE);
  SET__PROCESS_STATUS_TYPE__ATTRIBUTES__NAME(STATUS_guest, STATUS__host_64bit.ATTRIBUTES.NAME);

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

  void *dl_struct_getset = env;


  GET__PROCESS_ATTRIBUTE_TYPE__PERIOD__FNC_PTR_DCL(GET__PROCESS_ATTRIBUTE_TYPE__PERIOD)
    = (GET__PROCESS_ATTRIBUTE_TYPE__PERIOD__FNC_PTR_DCL()) dlsym (dl_struct_getset, "GET__PROCESS_ATTRIBUTE_TYPE__PERIOD");
  if (! GET__PROCESS_ATTRIBUTE_TYPE__PERIOD) {
    fprintf(stderr, "ERR: GET__PROCESS_ATTRIBUTE_TYPE__PERIOD not present!\n");
    exit(-1);
  }

  GET__PROCESS_ATTRIBUTE_TYPE__TIME_CAPACITY__FNC_PTR_DCL(GET__PROCESS_ATTRIBUTE_TYPE__TIME_CAPACITY)
    = (GET__PROCESS_ATTRIBUTE_TYPE__TIME_CAPACITY__FNC_PTR_DCL()) dlsym (dl_struct_getset, "GET__PROCESS_ATTRIBUTE_TYPE__TIME_CAPACITY");
  if (! GET__PROCESS_ATTRIBUTE_TYPE__TIME_CAPACITY) {
    fprintf(stderr, "ERR: GET__PROCESS_ATTRIBUTE_TYPE__TIME_CAPACITY not present!\n");
    exit(-1);
  }

  GET__PROCESS_ATTRIBUTE_TYPE__ENTRY_POINT__FNC_PTR_DCL(GET__PROCESS_ATTRIBUTE_TYPE__ENTRY_POINT)
    = (GET__PROCESS_ATTRIBUTE_TYPE__ENTRY_POINT__FNC_PTR_DCL()) dlsym (dl_struct_getset, "GET__PROCESS_ATTRIBUTE_TYPE__ENTRY_POINT");
  if (! GET__PROCESS_ATTRIBUTE_TYPE__ENTRY_POINT) {
    fprintf(stderr, "ERR: GET__PROCESS_ATTRIBUTE_TYPE__ENTRY_POINT not present!\n");
    exit(-1);
  }

  GET__PROCESS_ATTRIBUTE_TYPE__STACK_SIZE__FNC_PTR_DCL(GET__PROCESS_ATTRIBUTE_TYPE__STACK_SIZE)
    = (GET__PROCESS_ATTRIBUTE_TYPE__STACK_SIZE__FNC_PTR_DCL()) dlsym (dl_struct_getset, "GET__PROCESS_ATTRIBUTE_TYPE__STACK_SIZE");
  if (! GET__PROCESS_ATTRIBUTE_TYPE__STACK_SIZE) {
    fprintf(stderr, "ERR: GET__PROCESS_ATTRIBUTE_TYPE__STACK_SIZE not present!\n");
    exit(-1);
  }

  GET__PROCESS_ATTRIBUTE_TYPE__BASE_PRIORITY__FNC_PTR_DCL(GET__PROCESS_ATTRIBUTE_TYPE__BASE_PRIORITY)
    = (GET__PROCESS_ATTRIBUTE_TYPE__BASE_PRIORITY__FNC_PTR_DCL()) dlsym (dl_struct_getset, "GET__PROCESS_ATTRIBUTE_TYPE__BASE_PRIORITY");
  if (! GET__PROCESS_ATTRIBUTE_TYPE__BASE_PRIORITY) {
    fprintf(stderr, "ERR: GET__PROCESS_ATTRIBUTE_TYPE__BASE_PRIORITY not present!\n");
    exit(-1);
  }

  GET__PROCESS_ATTRIBUTE_TYPE__DEADLINE__FNC_PTR_DCL(GET__PROCESS_ATTRIBUTE_TYPE__DEADLINE)
    = (GET__PROCESS_ATTRIBUTE_TYPE__DEADLINE__FNC_PTR_DCL()) dlsym (dl_struct_getset, "GET__PROCESS_ATTRIBUTE_TYPE__DEADLINE");
  if (! GET__PROCESS_ATTRIBUTE_TYPE__DEADLINE) {
    fprintf(stderr, "ERR: GET__PROCESS_ATTRIBUTE_TYPE__DEADLINE not present!\n");
    exit(-1);
  }

  GET__PROCESS_ATTRIBUTE_TYPE__NAME__FNC_PTR_DCL(GET__PROCESS_ATTRIBUTE_TYPE__NAME)
    = (GET__PROCESS_ATTRIBUTE_TYPE__NAME__FNC_PTR_DCL()) dlsym (dl_struct_getset, "GET__PROCESS_ATTRIBUTE_TYPE__NAME");
  if (! GET__PROCESS_ATTRIBUTE_TYPE__NAME) {
    fprintf(stderr, "ERR: GET__PROCESS_ATTRIBUTE_TYPE__NAME not present!\n");
    exit(-1);
  }

  PROCESS_ATTRIBUTE_TYPE ATTRIBUTES__host_64bit;

  // FIXME: pointer is 32bit, but could be 64bit ..
  PROCESS_ATTRIBUTE_TYPE* ATTRIBUTES__guest = (PROCESS_ATTRIBUTE_TYPE*)&wasm_baseaddr[args[0].of.i32];

  ATTRIBUTES__host_64bit.PERIOD = GET__PROCESS_ATTRIBUTE_TYPE__PERIOD(ATTRIBUTES__guest);
  ATTRIBUTES__host_64bit.TIME_CAPACITY = GET__PROCESS_ATTRIBUTE_TYPE__TIME_CAPACITY(ATTRIBUTES__guest);
  ATTRIBUTES__host_64bit.ENTRY_POINT = (typeof(ATTRIBUTES__host_64bit.ENTRY_POINT))&wasm_trampoline;
  uint32_t ENTRY_POINT_idx = (uint32_t)GET__PROCESS_ATTRIBUTE_TYPE__ENTRY_POINT(ATTRIBUTES__guest);

  ATTRIBUTES__host_64bit.STACK_SIZE = GET__PROCESS_ATTRIBUTE_TYPE__STACK_SIZE(ATTRIBUTES__guest);
  ATTRIBUTES__host_64bit.BASE_PRIORITY = GET__PROCESS_ATTRIBUTE_TYPE__BASE_PRIORITY(ATTRIBUTES__guest);
  ATTRIBUTES__host_64bit.DEADLINE = GET__PROCESS_ATTRIBUTE_TYPE__DEADLINE(ATTRIBUTES__guest);
  GET__PROCESS_ATTRIBUTE_TYPE__NAME(ATTRIBUTES__host_64bit.NAME, ATTRIBUTES__guest);

#if 0
  PROCESS_ATTRIBUTE_TYPE* pa_guest_32bit = ATTRIBUTES__guest;
  printf("---------- PA_base %p\n", pa_guest_32bit);

  printf("!!!!!!!!!!!!! %p\n", dl_struct_getset);
  printf("!!!! %ld vs %ld (%p)\n", ATTRIBUTES__host_64bit.PERIOD, pa_guest_32bit->PERIOD, (void*)&pa_guest_32bit->PERIOD);
  printf("!!!! %ld vs %ld (%p)\n", ATTRIBUTES__host_64bit.TIME_CAPACITY, pa_guest_32bit->TIME_CAPACITY, (void*)&pa_guest_32bit->TIME_CAPACITY);
  printf("!!!! %d vs %d (%p)\n", ENTRY_POINT_idx, *(uint32_t*)&pa_guest_32bit->ENTRY_POINT, (void*)&pa_guest_32bit->ENTRY_POINT );
  pa_guest_32bit = (PROCESS_ATTRIBUTE_TYPE*)(&wasm_baseaddr[args[0].of.i32] - (sizeof(uintptr_t)-sizeof(uint32_t)));
  printf("!!!! %d vs %d (%p)\n", ATTRIBUTES__host_64bit.STACK_SIZE, pa_guest_32bit->STACK_SIZE, (void*)&pa_guest_32bit->STACK_SIZE);
  printf("!!!! %d vs %d (%p)\n", ATTRIBUTES__host_64bit.BASE_PRIORITY, pa_guest_32bit->BASE_PRIORITY, (void*)&pa_guest_32bit->BASE_PRIORITY);
  printf("!!!! %d vs %d (%p)\n", ATTRIBUTES__host_64bit.DEADLINE, pa_guest_32bit->DEADLINE, (void*)&pa_guest_32bit->DEADLINE);
  printf("!!!! '%s' vs '%s' (%p)\n", ATTRIBUTES__host_64bit.NAME, pa_guest_32bit->NAME, (void*)&pa_guest_32bit->NAME);
#endif

  PROCESS_ID_TYPE* pid = (PROCESS_ID_TYPE*)&wasm_baseaddr[args[1].of.i32];
  CREATE_PROCESS(
    &ATTRIBUTES__host_64bit,
    pid,
    (RETURN_CODE_TYPE*)&wasm_baseaddr[args[2].of.i32]
  );

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

