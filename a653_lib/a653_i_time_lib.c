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
 * @file      a653_i_time_lib.c
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     a653 shared memory handling
 * @details
 */


#include <stdio.h>
#include <unistd.h> /*for CLOCK_REALTIME? */
#include <sys/types.h>

#include "a653_i_time_lib.h"



void initTime(void){
  // printDebug("sizeof(int64_t) = %ld\n",sizeof(int64_t));
  //printDebug("sizeof(struct timespec) = %ld\n",sizeof(struct timespec));
}

struct timespec getTime (void){
  struct timespec curTime;

  clock_gettime(CLOCK_REALTIME, &curTime);
  //  milli = NANO_TO_MILLI(curTime.tv_nsec);

  
  return(curTime);
}

int my_time_next(struct timespec *t_val, int64_t t_nsec){
  
  int64_t tmp_sec = 0;
  int64_t tmp_nsec = 0;

  tmp_nsec = t_val->tv_nsec + t_nsec;

  if (t_val->tv_nsec > 1000000000ll){
    tmp_sec = tmp_nsec / 1000000000ll;
    t_val->tv_sec += tmp_sec;
    t_val->tv_nsec  = tmp_nsec - tmp_sec * 1000000000ll;
  } else {
    t_val->tv_nsec = tmp_nsec;
  }

  return (0);
}

int64_t my_time_diff(struct timespec *t_val_1, struct timespec *t_val_2){
  
  int64_t tmp_sec = 0;
  int64_t tmp_nsec = 0;
 
  int64_t ret_val;

  if (t_val_1->tv_sec == t_val_2->tv_sec){
    ret_val = t_val_1->tv_nsec - t_val_2->tv_nsec;
  } else {
    if (t_val_1->tv_sec > t_val_2->tv_sec){
      tmp_sec = t_val_1->tv_sec - t_val_2->tv_sec;
      tmp_nsec = tmp_sec * 1000000000ll;
      if(t_val_1->tv_nsec > t_val_2->tv_nsec){
	ret_val = tmp_nsec + (t_val_1->tv_nsec - t_val_2->tv_nsec);
      }
    } else {
      tmp_sec = t_val_2->tv_sec - t_val_1->tv_sec;
      tmp_nsec = tmp_sec * 1000000000ll;
      ret_val = tmp_nsec + (t_val_2->tv_nsec - t_val_1->tv_nsec);
      ret_val = (-1) * ret_val;
    }   
  }
  return (ret_val);
}
