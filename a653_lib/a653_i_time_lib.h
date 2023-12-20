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
 * @file      a653_i_time_lib.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     a653 shared memory handling
 * @details
 */

#ifndef __A653_I_TIME_LIB_H__
#define __A653_I_TIME_LIB_H__

#include <time.h>
#include <sys/types.h>

void initTime(void);
struct timespec getTime (void);
int my_time_next(struct timespec *t_val, int64_t t_nsec);
int64_t my_time_diff(struct timespec *t_val_1, struct timespec *t_val_2);

#endif /* __A653_I_TIME_LIB_H__ */
