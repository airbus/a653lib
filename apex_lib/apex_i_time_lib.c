/**
 * @file      apex_i_time_lib.c
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     apex shared memory handling
 * @details
 */


#include <stdio.h>
#include <unistd.h> /*for CLOCK_REALTIME? */


#include "apex_i_time_lib.h"



void initTime(void){
  // printDebug("sizeof(time_t) = %ld\n",sizeof(time_t));
  //printDebug("sizeof(struct timespec) = %ld\n",sizeof(struct timespec));
}

struct timespec getTime (void){
  struct timespec curTime;

  clock_gettime(CLOCK_REALTIME, &curTime);
  //  milli = NANO_TO_MILLI(curTime.tv_nsec);

  
  return(curTime);
}

int my_time_next(struct timespec *t_val, time_t t_nsec){
  
  time_t tmp_sec = 0;
  time_t tmp_nsec = 0;

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
