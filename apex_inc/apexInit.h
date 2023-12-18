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
 * @file      apexInit.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     do apex interface initialization
 * @details 
 */

#include <sys/types.h>

#ifndef __APEXINIT_H__
#define __APEXINIT_H__

#define MAX_PRCS_ID 0xff

#define MAX_PARTITION 10
#define MAX_PRCS 5

#define MAX_TIME_SLICE_NUM 10
#define MAX_CORE_NUM 2

#define MAX_Q_PORT 3
#define MAX_Q_PORT_ENTRIES 5
#define MAX_Q_PORT_SIZE 200

#define MAX_S_PORT 10
#define MAX_S_PORT_SIZE 200

typedef struct {
  char name_str[33];
} apex_partition_config_t;

typedef struct {
  unsigned short PatitionIdx;
} apex_time_slice_config_t;

typedef struct {
  int core_number;
  int partition_number;
  int time_slice_number;
  int64_t time_slice_size; /* value in nsec */
  apex_partition_config_t partition[MAX_PARTITION];
  apex_time_slice_config_t time_slice[MAX_TIME_SLICE_NUM][MAX_CORE_NUM];
  int magic;
} apex_global_config_t;


typedef struct {
  unsigned short PrcsId;
  unsigned short PartitionIdx;
  char           name_str[33];
} apex_process_config_t;

typedef struct {
  unsigned short PortId;
  unsigned short SrcPartitionIdx;
  unsigned short DstPartitionIdx;
  char           name_str[32];
} apex_sampling_port_config_t;

typedef struct {
  unsigned short PortId;
  unsigned short SrcPartitionIdx;
  unsigned short DstPartitionIdx;
  unsigned short Type;
  char           name_str[32];
  
} apex_queuing_port_config_t;




int apex_init_queuing_ports(int max_port_num, apex_queuing_port_config_t *config);
int apex_init_sampling_ports(int max_port_num, apex_sampling_port_config_t *config);

void apex_act_partition(void);

void apex_i_init_sync(void);
void apex_i_set_first(void);
void apex_i_set_next(void);
void apex_i_wait_next(void);
void apex_i_update_partitions(void);
void setDebug(int level);
void printDebug(int level,const char *format, ... );

#endif /* __APEXINIT_H__ */
