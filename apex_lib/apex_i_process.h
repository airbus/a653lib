/*
 * Copyright (c) 2022-2023 Airbus Defence and Space
 *
 * This file is part of a653_4_linux.
 *
 * a653_4_linux is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with a653_4_linux; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

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
  int64_t                 timerPeriod;
  unsigned int           cycle_cnt;
  char                   name[35];
  func_ptr               prcs_main_func;
} prcs_info_t;

void apex_act_prcs(void);

#endif /* __APEX_I_PARTITION_H__ */
