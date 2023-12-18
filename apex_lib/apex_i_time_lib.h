/**
 * @file      apex_i_time_lib.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     apex shared memory handling
 * @details
 */

#ifndef __APEX_I_TIME_LIB_H__
#define __APEX_I_TIME_LIB_H__

#include <time.h>
#include <sys/types.h>

void initTime(void);
struct timespec getTime (void);
int my_time_next(struct timespec *t_val, time_t t_nsec);
int64_t my_time_diff(struct timespec *t_val_1, struct timespec *t_val_2);

#endif /* __APEX_I_TIME_LIB_H__ */
