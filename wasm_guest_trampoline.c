// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

__attribute__((used)) // don't optimise out!
__attribute__((export_name("WASM_GUEST_FUNC_TRAMPOLINE")))
void WASM_GUEST_FUNC_TRAMPOLINE(void (*fnc)(void)) {
  (*fnc)();
}

#include "a653Lib.h"
#include <assert.h>
__attribute__((constructor)) void check(void)
{
  /* a653Semaphore.h */
  // CREATE_SEMAPHORE
  //assert(sizeof(SEMAPHORE_NAME_TYPE) == 4); // 32-bit char* ptr -> maps to char*
  assert(sizeof(SEMAPHORE_VALUE_TYPE) == 4); // 32-bit int
  assert(sizeof(SEMAPHORE_VALUE_TYPE) == 4); // 32-bit int
  assert(sizeof(QUEUING_DISCIPLINE_TYPE) == 4); // 32-bit int
  assert(sizeof(SEMAPHORE_ID_TYPE*) == 4); // 32-bit int*
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit int*

  assert(sizeof(SEMAPHORE_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // WAIT_SEMAPHORE
  assert(sizeof(SEMAPHORE_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(SYSTEM_TIME_TYPE) == 8); // 64-bit long int
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // SIGNAL_SEMAPHORE
  assert(sizeof(SEMAPHORE_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // GET_SEMAPHORE_ID
  //assert(sizeof(SEMAPHORE_NAME_TYPE) == 4); // 32-bit char* ptr -> maps to char*
  assert(sizeof(SEMAPHORE_ID_TYPE*) == 4); // 32-bit int*
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit int*

  assert(sizeof(SEMAPHORE_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // GET_SEMAPHORE_STATUS
  assert(sizeof(SEMAPHORE_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(SEMAPHORE_STATUS_TYPE*) == 4); // 32-bit int*
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit int*

  assert(sizeof(SEMAPHORE_STATUS_TYPE) == 4*2+4); // enum (3*int)
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?


  /* a653Sampling.h */
  // INIT_SAMPLING_PORT
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // CREATE_SAMPLING_PORT
  //assert(sizeof(SAMPLING_PORT_NAME_TYPE) == 4); // 32-bit char* ptr -> maps to char*
  assert(sizeof(MESSAGE_SIZE_TYPE) == 4); // 32-bit int
  assert(sizeof(PORT_DIRECTION_TYPE) == 4); // enum :-/
  assert(sizeof(SYSTEM_TIME_TYPE) == 8); // 64-bit long int
  assert(sizeof(SAMPLING_PORT_ID_TYPE*) == 4); // 32-bit int*
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(char*) == 4); // see SAMPLING_PORT_NAME_TYPE
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // WRITE_SAMPLING_MESSAGE
  assert(sizeof(SAMPLING_PORT_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(MESSAGE_ADDR_TYPE) == 4); // 32-bit uint8_t*
  assert(sizeof(MESSAGE_SIZE_TYPE) == 4); // 32-bit int
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // READ_SAMPLING_MESSAGE
  assert(sizeof(SAMPLING_PORT_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(MESSAGE_ADDR_TYPE) == 4); // 32-bit uint8_t*
  assert(sizeof(MESSAGE_SIZE_TYPE*) == 4); // 32-bit int*
  assert(sizeof(VALIDITY_TYPE*) == 4); // 32-bit enum* !!
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(VALIDITY_TYPE) == 4); // 32-bit enum?
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // GET_SAMPLING_PORT_ID
  //assert(sizeof(SAMPLING_PORT_NAME_TYPE) == 4); // 32-bit char* ptr -> maps to char*
  assert(sizeof(SAMPLING_PORT_ID_TYPE*) == 4); // 32-bit int*
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(char*) == 4); // see SAMPLING_PORT_NAME_TYPE
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?




  // READ_SAMPLING_MESSAGE_CONDITIONAL
  assert(sizeof(SAMPLING_PORT_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(SYSTEM_TIME_TYPE) == 8); // 64-bit long int
  assert(sizeof(MESSAGE_ADDR_TYPE) == 4); // 32-bit uint8_t*
  assert(sizeof(MESSAGE_SIZE_TYPE*) == 4); // 32-bit int*
  assert(sizeof(SYSTEM_TIME_TYPE*) == 4); // 32-bit uint64_t*
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(SYSTEM_TIME_TYPE) == 8); // 64-bit long int
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  /* a653Time.h */
  // void TIMED_WAIT
  assert(sizeof(SYSTEM_TIME_TYPE) == 8); // 64-bit int
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // PERIODIC_WAIT
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // GET_TIME
  assert(sizeof(SYSTEM_TIME_TYPE*) == 4); // 32-bit uint64_t*
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(SYSTEM_TIME_TYPE) == 8); // 64-bit int
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // REPLENISH
  assert(sizeof(SYSTEM_TIME_TYPE) == 8); // 64-bit int
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?


  // CREATE_QUEUING_PORT
  //assert(sizeof(SAMPLING_PORT_NAME_TYPE) == 4); // 32-bit char* ptr -> maps to char*
  assert(sizeof(MESSAGE_SIZE_TYPE) == 4); // 32-bit int
  assert(sizeof(MESSAGE_RANGE_TYPE) == 4); // 32-bit int
  assert(sizeof(PORT_DIRECTION_TYPE) == 4); // enum :-/
  assert(sizeof(QUEUING_DISCIPLINE_TYPE) == 4); // enum :-/
  assert(sizeof(QUEUING_PORT_ID_TYPE*) == 4); // 32-bit int*
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(char*) == 4); // see SAMPLING_PORT_NAME_TYPE
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // SEND_QUEUING_MESSAGE
  assert(sizeof(QUEUING_PORT_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(MESSAGE_ADDR_TYPE) == 4); // 32-bit uint8_t*
  assert(sizeof(MESSAGE_SIZE_TYPE) == 4); // 32-bit int
  assert(sizeof(SYSTEM_TIME_TYPE) == 8); // 64-bit long int
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // RECEIVE_QUEUING_MESSAGE
  assert(sizeof(QUEUING_PORT_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(SYSTEM_TIME_TYPE) == 8); // 64-bit long int
  assert(sizeof(MESSAGE_ADDR_TYPE) == 4); // 32-bit uint8_t*
  assert(sizeof(MESSAGE_SIZE_TYPE*) == 4); // 32-bit int*
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // GET_QUEUING_PORT_ID
  //assert(sizeof(SAMPLING_PORT_NAME_TYPE) == 4); // 32-bit char* ptr -> maps to char*
  assert(sizeof(QUEUING_PORT_ID_TYPE*) == 4); // 32-bit int*
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(char*) == 4); // see SAMPLING_PORT_NAME_TYPE
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // CLEAR_QUEUING_PORT
  //assert(sizeof(SAMPLING_PORT_NAME_TYPE) == 4); // 32-bit char* ptr -> maps to char*
  assert(sizeof(QUEUING_PORT_ID_TYPE) == 4); // 32-bit int

  assert(sizeof(char*) == 4); // see SAMPLING_PORT_NAME_TYPE
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?


  // GET_PARTITION_STATUS
  assert(sizeof(PARTITION_STATUS_TYPE*) == 4); // 32-bit enum* !!
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

//  assert(sizeof(PARTITION_STATUS_TYPE) == 8*2+4+4+4+4+4);
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // SET_PARTITION_MODE
  assert(sizeof(OPERATING_MODE_TYPE) == 4); // enum :-/
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // GET_QUEUING_PORT_STATUS
  assert(sizeof(QUEUING_PORT_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(QUEUING_PORT_STATUS_TYPE*) == 4); // 32-bit int*
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(QUEUING_PORT_STATUS_TYPE) == 4*5);
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?


  // GET_PROCESS_ID
  assert(sizeof(PROCESS_NAME_TYPE) == MAX_NAME_LENGTH); // 32-bit char* ptr -> maps to char*
  assert(sizeof(PROCESS_ID_TYPE*) == 4); // 32-bit int*
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(PROCESS_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // GET_MY_ID
  assert(sizeof(PROCESS_ID_TYPE*) == 4); // 32-bit int*
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(PROCESS_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // GET_PROCESS_STATUS
  assert(sizeof(PROCESS_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(PROCESS_STATUS_TYPE*) == 4); // 32-bit int*
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(PROCESS_STATUS_TYPE) == 8+4+4+sizeof(PROCESS_ATTRIBUTE_TYPE));
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

#if 0
  // CREATE_PROCESS
  assert(sizeof(PROCESS_ATTRIBUTE_TYPE*) == 4); // 32-bit struct* !!
  assert(sizeof(PROCESS_ID_TYPE*) == 4); // 32-bit int*
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(SYSTEM_TIME_TYPE) == 8);
  assert(sizeof(SYSTEM_ADDRESS_TYPE) == 4); // on the 64-bit issue!
  assert(sizeof(STACK_SIZE_TYPE) == 4);
  assert(sizeof(PRIORITY_TYPE) == 4);
  assert(sizeof(DEADLINE_TYPE) == 4);
  assert(sizeof(PROCESS_NAME_TYPE) == MAX_NAME_LENGTH);

  assert(sizeof(PROCESS_ATTRIBUTE_TYPE) == 8*2+4/*!*/+4+4+4+sizeof(PROCESS_NAME_TYPE)); // func ptr in WASM is 32bit, while in 64bit .. 64bit
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?
#endif

  // SET_PRIORITY
  assert(sizeof(PROCESS_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(PRIORITY_TYPE) == 4);
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // SUSPEND_SELF
  assert(sizeof(SYSTEM_TIME_TYPE) == 8); // 64-bit int
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // SUSPEND / RESUME / STOP / START
  assert(sizeof(PROCESS_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // DELAYED_START
  assert(sizeof(PROCESS_ID_TYPE) == 4); // 32-bit int
  assert(sizeof(SYSTEM_TIME_TYPE) == 8);
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?

  // LOCK_PREEMPTION / UNLOCK_PREEMPTION
  assert(sizeof(LOCK_LEVEL_TYPE*) == 4); // 32-bit int*
  assert(sizeof(RETURN_CODE_TYPE*) == 4); // 32-bit enum* !!

  assert(sizeof(LOCK_LEVEL_TYPE) == 4); // 32-bit int
  assert(sizeof(RETURN_CODE_TYPE) == 4); // 32-bit enum?
}
