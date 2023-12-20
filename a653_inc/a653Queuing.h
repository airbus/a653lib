/**
 * @file      a653Queuing.h
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     a653 queuing port
 * @details    
 */

#ifndef A653_QUEUING_H
#define A653_QUEUING_H

#include "a653Type.h"

#define QUEUING_PORT_ID_MAX 5000

/* type definitions */

typedef A653_INTEGER QUEUING_PORT_ID_TYPE;   /* queuing port ident type */
typedef NAME_TYPE    QUEUING_PORT_NAME_TYPE;  /* name type */

/* queuing port status type */
typedef struct {
  MESSAGE_RANGE_TYPE     NB_MESSAGE;        /* number of messages */
  MESSAGE_RANGE_TYPE     MAX_NB_MESSAGE;    /* maximum number of messages */ 
  MESSAGE_SIZE_TYPE      MAX_MESSAGE_SIZE;  /* max message size */ 
  PORT_DIRECTION_TYPE    PORT_DIRECTION;    /* port direction */
  WAITING_RANGE_TYPE     WAITING_PROCESSES; /* max number of processes */
} QUEUING_PORT_STATUS_TYPE;

/*-----------------------------------------------*/
/*  queuing port access function pointer types   */
/*-----------------------------------------------*/

typedef void (* Q_PORT_STATUS_FUNCPTR) (void *const pPortId,
                                        void *const pStatus,
                                        RETURN_CODE_TYPE *pRetCode);
typedef void (* Q_PORT_SEND_FUNCPTR) (void *const pPortId,
                                      const char *const msg,
                                      MESSAGE_SIZE_TYPE msgSize,
                                      SYSTEM_TIME_TYPE timeout,
                                      RETURN_CODE_TYPE * pRetCode);
typedef void (* Q_PORT_RECEIVE_FUNCPTR) (void *const pPortId,
                                         SYSTEM_TIME_TYPE timeout,
                                         char *const msg,
                                         MESSAGE_SIZE_TYPE *const pMsgSize,
                                         RETURN_CODE_TYPE *pRetCode);

/*------------------------------------------*/
/*  queuing port access function pointers   */
/*------------------------------------------*/

typedef struct q_port_funcs_t {
  Q_PORT_STATUS_FUNCPTR       funcStatus;
  Q_PORT_SEND_FUNCPTR         funcSend;
  Q_PORT_RECEIVE_FUNCPTR      funcReceive;
} Q_PORT_FUNCS_TYPE;

/* function declarations */

void CREATE_QUEUING_PORT (QUEUING_PORT_NAME_TYPE  QUEUING_PORT_NAME,
			  MESSAGE_SIZE_TYPE       MAX_MESSAGE_SIZE, 
			  MESSAGE_RANGE_TYPE      MAX_NB_MESSAGE,
			  PORT_DIRECTION_TYPE     PORT_DIRECTION,
			  QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE,
			  QUEUING_PORT_ID_TYPE  * QUEUING_PORT_ID,
			  RETURN_CODE_TYPE      * RETURN_CODE);

void SEND_QUEUING_MESSAGE (QUEUING_PORT_ID_TYPE   QUEUING_PORT_ID,
			   MESSAGE_ADDR_TYPE      MESSAGE_ADDR,
			   MESSAGE_SIZE_TYPE      LENGTH,
			   SYSTEM_TIME_TYPE       TIME_OUT,
			   RETURN_CODE_TYPE     * RETURN_CODE);

void RECEIVE_QUEUING_MESSAGE (QUEUING_PORT_ID_TYPE QUEUING_PORT_ID,
			      SYSTEM_TIME_TYPE     TIME_OUT,
			      MESSAGE_ADDR_TYPE    MESSAGE_ADDR,
			      MESSAGE_SIZE_TYPE  * LENGTH,
			      RETURN_CODE_TYPE   * RETURN_CODE);

void GET_QUEUING_PORT_ID (QUEUING_PORT_NAME_TYPE   QUEUING_PORT_NAME,
			  QUEUING_PORT_ID_TYPE   * QUEUING_PORT_ID,
			  RETURN_CODE_TYPE       * RETURN_CODE);

void GET_QUEUING_PORT_STATUS (QUEUING_PORT_ID_TYPE       QUEUING_PORT_ID,
			      QUEUING_PORT_STATUS_TYPE * QUEUING_PORT_STATUS,
			      RETURN_CODE_TYPE         * RETURN_CODE);


void CLEAR_QUEUING_PORT (
			 QUEUING_PORT_ID_TYPE QUEUING_PORT_ID, /* in */
			 RETURN_CODE_TYPE *RETURN_CODE ); /* out */


void create_queuing_port_pp (QUEUING_PORT_NAME_TYPE  QUEUING_PORT_NAME,
			     MESSAGE_SIZE_TYPE       MAX_MESSAGE_SIZE, 
			     MESSAGE_RANGE_TYPE      MAX_NB_MESSAGE,
			     PORT_DIRECTION_TYPE     PORT_DIRECTION,
			     QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE,
			     QUEUING_PORT_ID_TYPE   *QUEUING_PORT_ID,
			     RETURN_CODE_TYPE       *RETURN_CODE);

void send_queuing_message_pp (QUEUING_PORT_ID_TYPE   QUEUING_PORT_ID,
			      MESSAGE_ADDR_TYPE      MESSAGE_ADDR,
			      MESSAGE_SIZE_TYPE      LENGTH,
			      SYSTEM_TIME_TYPE       TIME_OUT,
			      RETURN_CODE_TYPE     * RETURN_CODE);

void receive_queuing_message_pp (QUEUING_PORT_ID_TYPE QUEUING_PORT_ID,
				 SYSTEM_TIME_TYPE     TIME_OUT,
				 MESSAGE_ADDR_TYPE    MESSAGE_ADDR,
				 MESSAGE_SIZE_TYPE  * LENGTH,
				 RETURN_CODE_TYPE   * RETURN_CODE);


#endif /* A653_QUEUING_H */

