// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: PROCESS

#include <endian.h>
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
wasm_trap_t* WASM32_GET_PROCESS_ID(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  PROCESS_ID_TYPE PROCESS_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_PROCESS_ID(
    (char*)&wasm_baseaddr[le32toh(args_and_results[0].i32)],  // FIXME: only safe as long as char[]
    &PROCESS_ID,
    &RETURN_CODE
  );

  camw32_set__PROCESS_ID_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)PROCESS_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);

  return NULL;
}



extern wasm_processes_t wasm_processes;
extern prcs_info_t *prcs_info;

void *wasm_trampoline(void) {
  /* trying to reverse engineer the index within the a653lib */
  pthread_t tid = pthread_self();

  int ret = 0;
  for (unsigned i = 0; i < MAX_PRCS; ++i) {
    if(prcs_info[i].t_ctx == tid) {
      uint64_t idx = wasm_processes.ENTRY_POINT[prcs_info[i].id];
      ret = exec_wasm_guest_func(0, idx);
      break;
    }
  }
  if ( ! ret)
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
wasm_trap_t* WASM32_GET_PROCESS_STATUS(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  PROCESS_ID_TYPE pid;
  pid = (PROCESS_ID_TYPE)le32toh(args_and_results[0].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  PROCESS_STATUS_TYPE PROCESS_STATUS;
  GET_PROCESS_STATUS(
    pid,
    &PROCESS_STATUS,
    &RETURN_CODE
  );

  uint8_t* PROCESS_STATUS_guest = (uint8_t*)&wasm_baseaddr[le32toh(args_and_results[1].i32)];
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);


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


  camw32_set__PROCESS_STATUS_TYPE__DEADLINE_TIME(PROCESS_STATUS_guest, PROCESS_STATUS.DEADLINE_TIME);
  camw32_set__PROCESS_STATUS_TYPE__CURRENT_PRIORITY(PROCESS_STATUS_guest, PROCESS_STATUS.CURRENT_PRIORITY);
  camw32_set__PROCESS_STATUS_TYPE__PROCESS_STATE(PROCESS_STATUS_guest, PROCESS_STATUS.PROCESS_STATE);

  uint8_t* ATTRIBUTES_guest = camw32_get_struct_base_addr__PROCESS_STATUS_TYPE__ATTRIBUTES(PROCESS_STATUS_guest);
  camw32_set__PROCESS_ATTRIBUTE_TYPE__PERIOD(ATTRIBUTES_guest, PROCESS_STATUS.ATTRIBUTES.PERIOD);
  camw32_set__PROCESS_ATTRIBUTE_TYPE__TIME_CAPACITY(ATTRIBUTES_guest, PROCESS_STATUS.ATTRIBUTES.TIME_CAPACITY);
  uint32_t ENTRY_POINT_idx = wasm_processes.ENTRY_POINT[pid];
  camw32_set__PROCESS_ATTRIBUTE_TYPE__ENTRY_POINT(ATTRIBUTES_guest, ENTRY_POINT_idx);
  camw32_set__PROCESS_ATTRIBUTE_TYPE__STACK_SIZE(ATTRIBUTES_guest, PROCESS_STATUS.ATTRIBUTES.STACK_SIZE);
  camw32_set__PROCESS_ATTRIBUTE_TYPE__BASE_PRIORITY(ATTRIBUTES_guest, PROCESS_STATUS.ATTRIBUTES.BASE_PRIORITY);
  camw32_set__PROCESS_ATTRIBUTE_TYPE__DEADLINE(ATTRIBUTES_guest, PROCESS_STATUS.ATTRIBUTES.DEADLINE);
  camw32_write__PROCESS_ATTRIBUTE_TYPE__NAME(ATTRIBUTES_guest, (uint8_t*)PROCESS_STATUS.ATTRIBUTES.NAME);

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
wasm_trap_t* WASM32_CREATE_PROCESS(void *env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  uint8_t* ATTRIBUTES__guest = (uint8_t*)&wasm_baseaddr[le32toh(args_and_results[0].i32)];

  PROCESS_ATTRIBUTE_TYPE ATTRIBUTES;
  ATTRIBUTES.PERIOD = camw32_get__PROCESS_ATTRIBUTE_TYPE__PERIOD(ATTRIBUTES__guest);
  ATTRIBUTES.TIME_CAPACITY = camw32_get__PROCESS_ATTRIBUTE_TYPE__TIME_CAPACITY(ATTRIBUTES__guest);
  ATTRIBUTES.ENTRY_POINT = (typeof(ATTRIBUTES.ENTRY_POINT))&wasm_trampoline;
  uint32_t ENTRY_POINT_idx = (uint32_t)camw32_get__PROCESS_ATTRIBUTE_TYPE__ENTRY_POINT(ATTRIBUTES__guest);

  ATTRIBUTES.STACK_SIZE = camw32_get__PROCESS_ATTRIBUTE_TYPE__STACK_SIZE(ATTRIBUTES__guest);
  ATTRIBUTES.BASE_PRIORITY = camw32_get__PROCESS_ATTRIBUTE_TYPE__BASE_PRIORITY(ATTRIBUTES__guest);
  ATTRIBUTES.DEADLINE = camw32_get__PROCESS_ATTRIBUTE_TYPE__DEADLINE(ATTRIBUTES__guest);
  camw32_read__PROCESS_ATTRIBUTE_TYPE__NAME(ATTRIBUTES__guest, (uint8_t*)ATTRIBUTES.NAME);

  PROCESS_ID_TYPE PROCESS_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  CREATE_PROCESS(
    &ATTRIBUTES,
    &PROCESS_ID,
    &RETURN_CODE
  );

  camw32_set__PROCESS_ID_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)PROCESS_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);


  // we get the pid late, and the real start of the thread will be in CREATE_PROCESS
  wasm_processes.ENTRY_POINT[PROCESS_ID] = ENTRY_POINT_idx;

  return NULL;
}


#if 0
extern void SET_PRIORITY (
  /* in  */ PROCESS_ID_TYPE PROCESS_ID,
  /* in  */ PRIORITY_TYPE PRIORITY,
  /* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
wasm_trap_t* WASM32_SET_PRIORITY(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  PROCESS_ID_TYPE PROCESS_ID;
  PROCESS_ID = (PROCESS_ID_TYPE)le32toh(args_and_results[0].i32);
  PRIORITY_TYPE PRIORITY;
  PRIORITY = (PRIORITY_TYPE)le32toh(args_and_results[1].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  SET_PRIORITY(
    PROCESS_ID,
    PRIORITY,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void SUSPEND_SELF (
  /* in  */ SYSTEM_TIME_TYPE TIME_OUT,
  /* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
wasm_trap_t* WASM32_SUSPEND_SELF(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  SYSTEM_TIME_TYPE TIME_OUT;
  TIME_OUT = (SYSTEM_TIME_TYPE)le64toh(args_and_results[0].i64);
  RETURN_CODE_TYPE RETURN_CODE;

  SUSPEND_SELF(
    TIME_OUT,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void SUSPEND (
  /* in  */ PROCESS_ID_TYPE PROCESS_ID,
  /* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
wasm_trap_t* WASM32_SUSPEND(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  PROCESS_ID_TYPE PROCESS_ID;
  PROCESS_ID = (PROCESS_ID_TYPE)le32toh(args_and_results[0].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  SUSPEND(
    PROCESS_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void RESUME (
  /* in  */ PROCESS_ID_TYPE PROCESS_ID,
  /* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
wasm_trap_t* WASM32_RESUME(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  PROCESS_ID_TYPE PROCESS_ID;
  PROCESS_ID = (PROCESS_ID_TYPE)le32toh(args_and_results[0].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  RESUME(
    PROCESS_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void STOP_SELF  (void);
#endif
wasm_trap_t* WASM32_STOP_SELF(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
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
wasm_trap_t* WASM32_STOP(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  PROCESS_ID_TYPE PROCESS_ID;
  PROCESS_ID = (PROCESS_ID_TYPE)le32toh(args_and_results[0].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  STOP(
    PROCESS_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void START (
  /* in  */ PROCESS_ID_TYPE PROCESS_ID,
  /* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
wasm_trap_t* WASM32_START(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  PROCESS_ID_TYPE PROCESS_ID;
  PROCESS_ID = (PROCESS_ID_TYPE)le32toh(args_and_results[0].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  START(
    PROCESS_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void DELAYED_START (
  /* in  */ PROCESS_ID_TYPE PROCESS_ID,
  /* in  */ SYSTEM_TIME_TYPE  DELAY_TIME,
  /* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
wasm_trap_t* WASM32_DELAYED_START(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  PROCESS_ID_TYPE PROCESS_ID;
  PROCESS_ID = (PROCESS_ID_TYPE)le32toh(args_and_results[0].i32);
  SYSTEM_TIME_TYPE DELAY_TIME;
  DELAY_TIME = (SYSTEM_TIME_TYPE)le64toh(args_and_results[1].i64);
  RETURN_CODE_TYPE RETURN_CODE;

  DELAYED_START(
    PROCESS_ID,
    DELAY_TIME,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void LOCK_PREEMPTION (
  /* out */ LOCK_LEVEL_TYPE * LOCK_LEVEL,
  /* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
wasm_trap_t* WASM32_LOCK_PREEMPTION(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  LOCK_LEVEL_TYPE LOCK_LEVEL;
  RETURN_CODE_TYPE RETURN_CODE;

  LOCK_PREEMPTION(
    &LOCK_LEVEL,
    &RETURN_CODE
  );

  camw32_set__LOCK_LEVEL_TYPE(&wasm_baseaddr[le32toh(args_and_results[0].i32)], (int32_t)LOCK_LEVEL);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void UNLOCK_PREEMPTION (
  /* out */ LOCK_LEVEL_TYPE * LOCK_LEVEL,
  /* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
wasm_trap_t* WASM32_UNLOCK_PREEMPTION(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  LOCK_LEVEL_TYPE LOCK_LEVEL;
  RETURN_CODE_TYPE RETURN_CODE;

  LOCK_PREEMPTION(
    &LOCK_LEVEL,
    &RETURN_CODE
  );

  camw32_set__LOCK_LEVEL_TYPE(&wasm_baseaddr[le32toh(args_and_results[0].i32)], (int32_t)LOCK_LEVEL);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  return NULL;
}



#if 0
extern void GET_MY_ID (
/* out */ PROCESS_ID_TYPE * PROCESS_ID,
/* out */ RETURN_CODE_TYPE * RETURN_CODE);
#endif
wasm_trap_t* WASM32_GET_MY_ID(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  PROCESS_ID_TYPE PROCESS_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_MY_ID(
    &PROCESS_ID,
    &RETURN_CODE
  );

  camw32_set__PROCESS_ID_TYPE(&wasm_baseaddr[le32toh(args_and_results[0].i32)], (int32_t)PROCESS_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void INITIALIZE_PROCESS_CORE_AFFINITY (
  /*in */ PROCESS_ID_TYPE          PROCESS_ID,
  /*in */ PROCESSOR_CORE_ID_TYPE   PROCESSOR_CORE_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );
#endif
wasm_trap_t* WASM32_INITIALIZE_PROCESS_CORE_AFFINITY(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  PROCESS_ID_TYPE PROCESS_ID;
  PROCESS_ID = (PROCESS_ID_TYPE)le32toh(args_and_results[0].i32);
  PROCESSOR_CORE_ID_TYPE PROCESSOR_CORE_ID;
  PROCESSOR_CORE_ID = (PROCESSOR_CORE_ID_TYPE)le32toh(args_and_results[1].i32);
  RETURN_CODE_TYPE RETURN_CODE;

  INITIALIZE_PROCESS_CORE_AFFINITY(
    PROCESS_ID,
    PROCESSOR_CORE_ID,
    &RETURN_CODE
  );

  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[2].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void GET_MY_PROCESSOR_CORE_ID (
  /*out*/ PROCESSOR_CORE_ID_TYPE   *PROCESSOR_CORE_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );
#endif
wasm_trap_t* WASM32_GET_MY_PROCESSOR_CORE_ID(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  PROCESSOR_CORE_ID_TYPE PROCESSOR_CORE_ID;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_MY_PROCESSOR_CORE_ID(
    &PROCESSOR_CORE_ID,
    &RETURN_CODE
  );

  camw32_set__PROCESSOR_CORE_ID_TYPE(&wasm_baseaddr[le32toh(args_and_results[0].i32)], (int32_t)PROCESSOR_CORE_ID);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  return NULL;
}


#if 0
extern void GET_MY_INDEX (
  /*out*/ PROCESS_INDEX_TYPE       *PROCESS_INDEX,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );
#endif
wasm_trap_t* WASM32_GET_MY_INDEX(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results)
{
  uint8_t* wasm_baseaddr = get_linear_memory(caller);


  PROCESS_INDEX_TYPE PROCESS_INDEX;
  RETURN_CODE_TYPE RETURN_CODE;

  GET_MY_INDEX(
    &PROCESS_INDEX,
    &RETURN_CODE
  );

  camw32_set__PROCESS_INDEX_TYPE(&wasm_baseaddr[le32toh(args_and_results[0].i32)], (int32_t)PROCESS_INDEX);
  camw32_set__RETURN_CODE_TYPE(&wasm_baseaddr[le32toh(args_and_results[1].i32)], (int32_t)RETURN_CODE);

  return NULL;
}

