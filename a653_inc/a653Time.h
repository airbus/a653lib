/**
 * @file      a653Time.h
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     a653 timer handling
 * @details
 */

#ifndef __A653_TIME_H
#define __A653_TIME_H


#include "a653Type.h"

/* type definitions */

/* function declarations */

extern void TIMED_WAIT (SYSTEM_TIME_TYPE DELAY_TIME, /* 64bit - 1 nanosecond LSB */
			RETURN_CODE_TYPE * RETURN_CODE);

extern void PERIODIC_WAIT (RETURN_CODE_TYPE * RETURN_CODE);

extern void GET_TIME (SYSTEM_TIME_TYPE * SYSTEM_TIME, /* 64bit - 1 nanosecond LSB */
		      RETURN_CODE_TYPE * RETURN_CODE);

extern void REPLENISH (SYSTEM_TIME_TYPE   BUDGET_TIME, 
                       RETURN_CODE_TYPE * RETURN_CODE);

#endif	/* __A653_TIME_H  */
