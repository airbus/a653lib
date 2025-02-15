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
 * @file      a653_i_partition.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     a653 shared memory handling
 * @details
 */

#ifndef __A653_I_PARTITION_H__
#define __A653_I_PARTITION_H__

#include "a653Type.h"
#include "a653Error.h"
#include "a653Process.h"
#include "a653Partition.h"
#include "a653Time.h"


typedef void *(*__start_routine) (void *);
typedef void (*func_ptr)(void);

typedef struct {
  //  PROCESS_ATTRIBUTE_TYPE attr;
  pthread_mutex_t        t_lock;
  pthread_t              t_ctx;
  pthread_attr_t         t_attr;
  int64_t                timerPeriod;
  unsigned int           priority;
  unsigned int           cycle_cnt;
  unsigned short         id;
  char                   name[35];
  func_ptr               prcs_main_func;
} prcs_info_t;

void a653_act_prcs(void);

#endif /* __A653_I_PARTITION_H__ */
