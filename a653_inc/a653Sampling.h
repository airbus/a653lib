/**
 * @file      a653Sampling.h
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     a653 sampling port
 * @details    
 */


#ifndef __A653_SAMPLING_H
#define __A653_SAMPLING_H

#include "a653Type.h"
#include "a653Error.h"

/* constant definitions */ 
  
#define SAMPLING_PORT_ID_MAX 5000

/* type definitions */ 

typedef NAME_TYPE SAMPLING_PORT_NAME_TYPE; /* port name type */ 

typedef A653_INTEGER SAMPLING_PORT_ID_TYPE; /* sampling port ident type */ 

typedef enum VALIDITY_VALUE_TYPE {
  INVALID = 0,
  VALID   = 1
} VALIDITY_TYPE;

/* sampling port status type */ 
typedef struct SAMPLING_PORT_STATUS {
  MESSAGE_SIZE_TYPE       MAX_MESSAGE_SIZE;   /* port size */
  PORT_DIRECTION_TYPE     PORT_DIRECTION;     /* port direction */
  SYSTEM_TIME_TYPE        REFRESH_PERIOD;     /* refresh period */
  VALIDITY_TYPE           LAST_MSG_VALIDITY;  /* message validity */
} SAMPLING_PORT_STATUS_TYPE;

typedef enum UPDATED_VALUE_TYPE {    
  EMPTY_PORT,
  CONSUMED_MESSAGE,
  NEW_MESSAGE
} UPDATED_TYPE;

typedef enum AGE_VALUE_TYPE {
  STALE,
  FRESH
} AGE_TYPE;

/* sampling port status type */ 
typedef struct SAMPLING_PORT_CURRENT_STATUS { 
  SYSTEM_TIME_TYPE        REFRESH_PERIOD;     /* refresh period */
  SYSTEM_TIME_TYPE        TIME_STAMP;         /* when message was written */
  MESSAGE_SIZE_TYPE       MAX_MESSAGE_SIZE;   /* port max byte size */
  PORT_DIRECTION_TYPE     PORT_DIRECTION;     /* port direction */
  AGE_TYPE                MESSAGE_AGE;        /* message age - is msg is older than ports refresh period */
  UPDATED_TYPE            UPDATED;            /* empty, consumed, or new message */
} SAMPLING_PORT_CURRENT_STATUS_TYPE;

/*-----------------------------------------------*/
/*  sampling port access function pointer types  */
/*-----------------------------------------------*/

typedef void (* SAMPLING_PORT_CURRENT_STATUS_FUNCPTR)
(void *const pPortId, void *const pStatus, RETURN_CODE_TYPE * pRetCode);

typedef void (* SAMPLING_PORT_STATUS_FUNCPTR)
(void *const pPortId, void *const pStatus, RETURN_CODE_TYPE *pRetCode);

typedef void (* SAMPLING_PORT_SEND_FUNCPTR)
(void *const pPortId, const char *const msg, MESSAGE_SIZE_TYPE msgSize,
 RETURN_CODE_TYPE * pRetCode);

typedef void (* SAMPLING_PORT_RECEIVE_FUNCPTR)
(void *const pPortId, char *const msg, MESSAGE_SIZE_TYPE *const pMsgSize,
 VALIDITY_TYPE *palidity, RETURN_CODE_TYPE *pRetCode);

/*------------------------------------------*/
/*  sampling port access function pointers  */
/*------------------------------------------*/

typedef struct sample_port_funcs_s {
  SAMPLING_PORT_STATUS_FUNCPTR                funcStatus;
  SAMPLING_PORT_CURRENT_STATUS_FUNCPTR        funcCurrentStatus;
  SAMPLING_PORT_SEND_FUNCPTR                  funcSend;
  SAMPLING_PORT_RECEIVE_FUNCPTR               funcReceive;
} SAMPLING_PORT_FUNCS_TYPE;



/* function declarations */


//int a653_init_sampling_ports(int max_port_num, int max_port_size, a653_sampling_port_config_t *config);

void INIT_SAMPLING_PORT (RETURN_CODE_TYPE        *RETURN_CODE);

void CREATE_SAMPLING_PORT (SAMPLING_PORT_NAME_TYPE   SAMPLING_PORT_NAME, 
			   MESSAGE_SIZE_TYPE         MAX_MESSAGE_SIZE, 
			   PORT_DIRECTION_TYPE       PORT_DIRECTION, 
			   SYSTEM_TIME_TYPE          REFRESH_PERIOD, 
			   SAMPLING_PORT_ID_TYPE   * SAMPLING_PORT_ID, 
			   RETURN_CODE_TYPE        * RETURN_CODE);

void WRITE_SAMPLING_MESSAGE (SAMPLING_PORT_ID_TYPE   SAMPLING_PORT_ID, 
			     MESSAGE_ADDR_TYPE       MESSAGE_ADDR,
			     MESSAGE_SIZE_TYPE       LENGTH, 
			     RETURN_CODE_TYPE      * RETURN_CODE);

void READ_SAMPLING_MESSAGE (SAMPLING_PORT_ID_TYPE    SAMPLING_PORT_ID, 
			    MESSAGE_ADDR_TYPE        MESSAGE_ADDR, 
			    MESSAGE_SIZE_TYPE      * LENGTH, 
			    VALIDITY_TYPE          * VALIDITY, 
			    RETURN_CODE_TYPE       * RETURN_CODE);

void READ_UPDATED_SAMPLING_MESSAGE (SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID,
				    MESSAGE_ADDR_TYPE       MESSAGE_ADDR,
				    MESSAGE_SIZE_TYPE *     LENGTH,
				    UPDATED_TYPE *          UPDATED,
				    RETURN_CODE_TYPE *      RETURN_CODE);

void READ_SAMPLING_MESSAGE_CONDITIONAL (SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID,
					SYSTEM_TIME_TYPE        REF_TIME_STAMP,
					MESSAGE_ADDR_TYPE       MESSAGE_ADDR,
					MESSAGE_SIZE_TYPE *     LENGTH,
					SYSTEM_TIME_TYPE *      TIME_STAMP,
					RETURN_CODE_TYPE *      RETURN_CODE);

void GET_SAMPLING_PORT_ID (SAMPLING_PORT_NAME_TYPE   SAMPLING_PORT_NAME,
			   SAMPLING_PORT_ID_TYPE   * SAMPLING_PORT_ID, 
			   RETURN_CODE_TYPE        * RETURN_CODE);

void GET_SAMPLING_PORT_STATUS (SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID, 
			       SAMPLING_PORT_STATUS_TYPE * SAMPLING_PORT_STATUS, 
			       RETURN_CODE_TYPE    * RETURN_CODE);

void GET_SAMPLING_PORT_CURRENT_STATUS (SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID,
				       SAMPLING_PORT_CURRENT_STATUS_TYPE * SAMPLING_PORT_CURRENT_STATUS,
				       RETURN_CODE_TYPE *      RETURN_CODE);


void create_sampling_port_pp (SAMPLING_PORT_NAME_TYPE   SAMPLING_PORT_NAME, 
			      MESSAGE_SIZE_TYPE         MAX_MESSAGE_SIZE, 
			      PORT_DIRECTION_TYPE       PORT_DIRECTION, 
			      SYSTEM_TIME_TYPE          REFRESH_PERIOD, 
			      SAMPLING_PORT_ID_TYPE   * SAMPLING_PORT_ID, 
			      RETURN_CODE_TYPE        * RETURN_CODE);

void write_sampling_message_pp(SAMPLING_PORT_ID_TYPE   SAMPLING_PORT_ID, 
			       MESSAGE_ADDR_TYPE       MESSAGE_ADDR,
			       MESSAGE_SIZE_TYPE       LENGTH, 
			       RETURN_CODE_TYPE      * RETURN_CODE);

void read_sampling_message_pp(SAMPLING_PORT_ID_TYPE       SAMPLING_PORT_ID, 
			      MESSAGE_ADDR_TYPE        MESSAGE_ADDR, 
			      MESSAGE_SIZE_TYPE      * LENGTH, 
			      VALIDITY_TYPE          * VALIDITY, 
			      RETURN_CODE_TYPE       * RETURN_CODE);


#endif /* __A653_SAMPLING_H */

