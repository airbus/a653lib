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
 * @file      a653Types.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     a653 types
 * @details
 */


#ifndef __A653_TYPES_H
#define __A653_TYPES_H

/* defines */

/* clang attribute for the WebAssembly target */
#ifdef __wasm__
#define WASM_IMPORT_MODULE(name) __attribute__((import_module(name)))
#else
#define WASM_IMPORT_MODULE(name)
#endif

/* PORT queuing definition */

/* PORT sampling definition */


/* Global type definitions */

typedef unsigned char       A653_BYTE;              /*  8 bits unsigned   */
typedef unsigned short int  A653_WORD;              /* 16 bits unsigned   */
typedef long int            A653_INTEGER;           /* 32 bits            */
typedef unsigned long       A653_UNSIGNED;          /* 32 bits unsigned   */
typedef long long           A653_LONG_INTEGER;      /* 64 bits            */

typedef A653_INTEGER        PROCESS_ID_TYPE;        /* object ident type  */
typedef A653_INTEGER     PROCESSOR_CORE_ID_TYPE;

#define NULL_PROCESS_ID     0

/* return code type */
typedef enum {
  NO_ERROR       = 0, /* request valid and operation performed       */
  NO_ACTION      = 1, /* status of system unaffected by request      */
  NOT_AVAILABLE  = 2, /* resource required by request is unavailable */
  INVALID_PARAM  = 3, /* invalid parameter specified in request      */
  INVALID_CONFIG = 4, /* parameter incompatible with configuration   */
  INVALID_MODE   = 5, /* request incompatible with current mode      */
  TIMED_OUT      = 6  /* timeout tied-up with request has expired    */
} RETURN_CODE_TYPE;


typedef enum {
  READ = 0,
  READ_WRITE = 1
} A653_ACCESS_MODE_TYPE;

#define MAX_NAME_LENGTH                 30    /* name length                */

typedef char            NAME_TYPE[MAX_NAME_LENGTH];
typedef void            (*SYSTEM_ADDRESS_TYPE); /* address type       */
typedef A653_BYTE *     MESSAGE_ADDR_TYPE;  /* message type             */
typedef A653_INTEGER    MESSAGE_SIZE_TYPE;  /* message size type        */
typedef A653_INTEGER    MESSAGE_RANGE_TYPE; /* message number type      */

/* port direction type */
typedef enum {
  SOURCE = 0,
  DESTINATION = 1
} PORT_DIRECTION_TYPE;

/* queuing discipline type */
typedef enum {
  FIFO = 0,
  PRIORITY = 1
} QUEUING_DISCIPLINE_TYPE;

typedef A653_LONG_INTEGER    SYSTEM_TIME_TYPE;   /* system time type         */


typedef A653_INTEGER     WAITING_RANGE_TYPE; /* priority type      */
typedef A653_UNSIGNED    STACK_SIZE_TYPE;    /* stack size type    */
typedef A653_INTEGER     LOCK_LEVEL_TYPE;    /* lock level type    */
typedef A653_BYTE *         MEMORY_BLOCK_ADDR_TYPE;
typedef A653_INTEGER        MEMORY_BLOCK_SIZE_TYPE;
typedef NAME_TYPE           MEMORY_BLOCK_NAME_TYPE;


#endif /* __A653_TYPES_H */
