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
 * @file      a653Init.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     do a653 interface initialization
 * @details 
 */

#ifndef __A653INIT_H__
#define __A653INIT_H__

#ifndef __wasm__ /* Do not expose non ARINC653 functions into WebAssembly */

#include "a653Type.h"

//#define S_TRACE 1
//#define S_DEBUG 1

#define MAX_TRACE_ENTRIES 1000

#define MAX_PRCS_ID 0xff

#define MAX_PARTITION 10           /* maximal number of partitions */
#define MAX_PRCS 12                /* maximal number of processes per partition */
			         
#define MAX_CHANNEL     60         /* Number of maximal posible channels */
#define MAX_CHANNEL_DST 10         /* maximal number of destinations of a channel */
#define MAX_CHANNEL_DATA_SIZE 1500 /* maximal message size over a channel */

#define MAX_TIME_SLICE_NUM 10
#define MAX_CORE_NUM 4

#define MAX_Q_PORT 45
#define MAX_Q_PORT_ENTRIES 40
#define MAX_Q_PORT_SIZE 1500

#define MAX_S_PORT 10
#define MAX_S_PORT_SIZE 200

#define MAX_SEM_NUM 10

typedef struct {
  char name_str[33];
} a653_partition_entry_t;

typedef struct {
  unsigned short PatitionIdx;
} a653_time_slice_config_t;

typedef struct {
  int core_number;
  int partition_number;
  int time_slice_number;
  A653_LONG_INTEGER time_slice_size; /* value in nsec */
  a653_partition_entry_t partition[MAX_PARTITION];
  a653_time_slice_config_t time_slice[MAX_TIME_SLICE_NUM][MAX_CORE_NUM];
  int magic;
} a653_global_config_t;

typedef struct {
  unsigned short ChannelId;
  unsigned short ChannelType;
  unsigned short maxMsgSize;
} a653_channel_config_t;

typedef struct {
  unsigned short ChannelId;
  unsigned short Dir;
  char           name_str[32];
} a653_sampling_port_config_t;

typedef struct {
  unsigned short ChannelId;
  unsigned short Dir;
  unsigned short Type;
  char           name_str[32];  
} a653_queuing_port_config_t;

typedef struct {
  unsigned short PartitionId;
  char           name_str[32];
  a653_sampling_port_config_t sp_config[MAX_S_PORT];
  a653_queuing_port_config_t qp_config[MAX_Q_PORT];
} a653_partition_config_t;

int a653_init_partition(void);
void a653_act_partition(void);

void a653_i_init_channels(void);
void a653_i_init_sync(void);
void a653_i_set_first(void);
void a653_i_set_next(void);
void a653_i_wait_next(void);
void a653_i_update_partitions(void);
void setDebug(int level);
void printDebug(int level,const char *format, ... );

#endif /* #ifndef __wasm__ */

#endif /* __A653INIT_H__ */
