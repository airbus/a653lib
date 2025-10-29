/*
 * Copyright (c) 2022-2023 Airbus Defence and Space
 *
 * This file is part of liba653.
 *
 * liba653 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * liba653 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with liba653; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file      a653Error.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     a653 error handling
 * @details
 */

#ifndef A653_ERROR_H
#define A653_ERROR_H

#include "a653Type.h"

/* defines */ 

#define MAX_ERROR_MESSAGE_SIZE  128

/* typedefs */ 

typedef A653_INTEGER    ERROR_MESSAGE_SIZE_TYPE;  /* error message size type */

typedef A653_BYTE       ERROR_MESSAGE_TYPE[MAX_ERROR_MESSAGE_SIZE]; 

/* error types */ 
typedef
   enum {
        DEADLINE_MISSED   = 0, /* deadline missed */
        APPLICATION_ERROR = 1, /* application error */
        NUMERIC_ERROR     = 2, /* numeric error */
        ILLEGAL_REQUEST   = 3, /* invalid or illegal OS call */
        STACK_OVERFLOW    = 4, /* process stack overflow */
        MEMORY_VIOLATION  = 5, /* Memory violation */
        HARDWARE_FAULT    = 6, /* hardware fault */
        POWER_FAIL        = 7  /* power failure indication */
   } ERROR_CODE_TYPE;

typedef
   struct {
      SYSTEM_ADDRESS_TYPE     FAILED_ADDRESS;
      PROCESS_ID_TYPE         FAILED_PROCESS_ID;
      ERROR_CODE_TYPE         ERROR_CODE;
      ERROR_MESSAGE_SIZE_TYPE LENGTH;
      ERROR_MESSAGE_TYPE      MESSAGE;
   } ERROR_STATUS_TYPE;

typedef
   enum {
        PROCESSES_PAUSE     = 0,
        PROCESSES_SCHEDULED = 1
   } ERROR_HANDLER_CONCURRENCY_CONTROL_TYPE;

/* function declarations */

WASM_IMPORT_MODULE("arinc653")
extern void REPORT_APPLICATION_MESSAGE (
       /*in */   MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
       /*in */   MESSAGE_SIZE_TYPE        LENGTH,
       /*out*/   RETURN_CODE_TYPE         *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void CREATE_ERROR_HANDLER (
       /*in */   SYSTEM_ADDRESS_TYPE      ENTRY_POINT,
       /*in */   STACK_SIZE_TYPE          STACK_SIZE,
       /*out*/   RETURN_CODE_TYPE         *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void GET_ERROR_STATUS (
       /*out*/   ERROR_STATUS_TYPE        *ERROR_STATUS,
       /*out*/   RETURN_CODE_TYPE         *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void RAISE_APPLICATION_ERROR (
       /*in */   ERROR_CODE_TYPE          ERROR_CODE,
       /*in */   MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
       /*in */   ERROR_MESSAGE_SIZE_TYPE  LENGTH,
       /*out*/   RETURN_CODE_TYPE         *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void CONFIGURE_ERROR_HANDLER (
       /*in */   ERROR_HANDLER_CONCURRENCY_CONTROL_TYPE  CONCURRENCY_CONTROL,
       /*in */   PROCESSOR_CORE_ID_TYPE                  PROCESSOR_CORE_ID,
       /*out*/   RETURN_CODE_TYPE                        *RETURN_CODE );

#endif /* A653_ERROR_H */
