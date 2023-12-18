/**
 * @file      apexTime.h
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     apex timer handling
 * @details
 */

#ifndef __APEX_TIME_H
#define __APEX_TIME_H


#include "apexType.h"

/* type definitions */

/* function declarations */

extern void TIMED_WAIT (SYSTEM_TIME_TYPE DELAY_TIME, 
			RETURN_CODE_TYPE * RETURN_CODE);
extern void PERIODIC_WAIT (RETURN_CODE_TYPE * RETURN_CODE);
extern void GET_TIME (SYSTEM_TIME_TYPE * SYSTEM_TIME,
		      RETURN_CODE_TYPE * RETURN_CODE);
extern void REPLENISH (SYSTEM_TIME_TYPE   BUDGET_TIME, 
                       RETURN_CODE_TYPE * RETURN_CODE);

#endif	/* __APEX_TIME_H  */
