/**
 * @file      a653Types.h
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     a653 types
 * @details
 */


#ifndef __A653_TYPES_H
#define __A653_TYPES_H

/* defines */

/* PORT queuing definition */

/* PORT sampling definition */


/* Global type definitions */

typedef unsigned char       A653_BYTE;              /*  8 bits unsigned   */
typedef unsigned short int  A653_WORD;              /* 16 bits unsigned   */
typedef long int            A653_INTEGER;           /* 32 bits            */
typedef unsigned long       A653_UNSIGNED;          /* 32 bits unsigned   */
typedef long long           A653_LONG_INTEGER;      /* 64 bits            */

typedef A653_INTEGER        PROCESS_ID_TYPE;        /* object ident type  */

#define NULL_PROCESS_ID     0

/* return code type */
typedef enum RETURN_CODE_VALUE_TYPE {
  NO_ERROR       = 0, /* request valid and operation performed       */
  NO_ACTION      = 1, /* status of system unaffected by request      */
  NOT_AVAILABLE  = 2, /* resource required by request is unavailable */
  INVALID_PARAM  = 3, /* invalid parameter specified in request      */
  INVALID_CONFIG = 4, /* parameter incompatible with configuration   */
  INVALID_MODE   = 5, /* request incompatible with current mode      */
  TIMED_OUT      = 6  /* timeout tied-up with request has expired    */
} RETURN_CODE_TYPE;


typedef char Boolean;

typedef enum A653_ACCESS_MODE_TYPE{
  READ = 0,
  READ_WRITE = 1
}A653_ACCESS_MODE_TYPE;

#define MAX_NAME_LENGTH                 30    /* name length                */

typedef char            NAME_TYPE[MAX_NAME_LENGTH];
typedef void            (*SYSTEM_ADDRESS_TYPE); /* address type       */
typedef A653_BYTE *     MESSAGE_ADDR_TYPE;  /* message type             */
typedef A653_INTEGER    MESSAGE_SIZE_TYPE;  /* message size type        */
typedef A653_INTEGER    MESSAGE_RANGE_TYPE; /* message number type      */

/* port direction type */
typedef enum PORT_DIRECTION_VALUE_TYPE {
  SOURCE = 0,
  DESTINATION = 1
} PORT_DIRECTION_TYPE;

/* queuing discipline type */
typedef enum QUEUING_DISCIPLINE_VALUE_TYPE {
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
