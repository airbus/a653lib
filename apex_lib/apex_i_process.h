/**
 * @file      apex_i_partition.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     apex shared memory handling
 * @details
 */

#ifndef __APEX_I_PARTITION_H__
#define __APEX_I_PARTITION_H__

#include "apexType.h"
#include "apexError.h"
#include "apexProcess.h"
#include "apexPartition.h"
#include "apexTime.h"


typedef void *(*__start_routine) (void *);
typedef void (*func_ptr)(void);

typedef struct {
  //  PROCESS_ATTRIBUTE_TYPE attr;
  pthread_mutex_t        t_lock;
  pthread_t              t_ctx;
  time_t                 timerPeriod;
  //  size_t                 timer;
  unsigned int           cycle_cnt;
  char                   name[35];
  func_ptr               prcs_main_func;
} prcs_info_t;

void apex_act_prcs(void);

#endif /* __APEX_I_PARTITION_H__ */
