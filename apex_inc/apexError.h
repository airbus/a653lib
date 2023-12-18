/**
 * @file      apexError.h
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     apex error handling
 * @details
 */

#ifndef __APEX_ERROR_H
#define __APEX_ERROR_H

#include "apexType.h"

/* defines */ 

#define MAX_ERROR_MESSAGE_SIZE  128


/* typedefs */ 

typedef APEX_INTEGER    ERROR_MESSAGE_SIZE_TYPE;  /* error message size type */

typedef APEX_BYTE       ERROR_MESSAGE_TYPE[MAX_ERROR_MESSAGE_SIZE]; 

/* error types */ 
typedef enum ERROR_CODE_VALUE_TYPE {
    DEADLINE_MISSED = 0,        /* deadline missed */
    APPLICATION_ERROR,          /* application error */
    NUMERIC_ERROR,              /* numeric error */
    ILLEGAL_REQUEST,            /* invalid or illegal OS call */
    STACK_OVERFLOW,             /* process stack overflow */
    MEMORY_VIOLATION,           /* Memory violation */
    HARDWARE_FAULT,             /* hardware fault */
    POWER_FAIL                  /* power failure indication */
    } ERROR_CODE_TYPE;

/* error status type */
typedef struct             {
  ERROR_CODE_TYPE         ERROR_CODE;
  ERROR_MESSAGE_TYPE      MESSAGE;
  ERROR_MESSAGE_SIZE_TYPE LENGTH;
  PROCESS_ID_TYPE         FAILED_PROCESS_ID;
  SYSTEM_ADDRESS_TYPE     FAILED_ADDRESS;
} ERROR_STATUS_TYPE;

/* function declarations */

extern void REPORT_APPLICATION_MESSAGE (MESSAGE_ADDR_TYPE  MESSAGE_ADDR,
					MESSAGE_SIZE_TYPE  LENGTH, 
					RETURN_CODE_TYPE * RETURN_CODE);

extern void CREATE_ERROR_HANDLER (SYSTEM_ADDRESS_TYPE ENTRY_POINT, 
				  STACK_SIZE_TYPE     STACK_SIZE, 
				  RETURN_CODE_TYPE  * RETURN_CODE); 

extern void GET_ERROR_STATUS (ERROR_STATUS_TYPE * ERROR_STATUS, 
			      RETURN_CODE_TYPE  * RETURN_CODE);

extern void RAISE_APPLICATION_ERROR (ERROR_CODE_TYPE    ERROR_CODE,
				     MESSAGE_ADDR_TYPE  MESSAGE_ADDR,
				     ERROR_MESSAGE_SIZE_TYPE LENGTH,
				     RETURN_CODE_TYPE * RETURN_CODE);

extern void REPORT_APPLICATION_MESSAGE ();
extern void CREATE_ERROR_HANDLER (); 
extern void GET_ERROR_STATUS ();
extern void RAISE_APPLICATION_ERROR ();


#endif	/* __APEX_ERROR_H */
