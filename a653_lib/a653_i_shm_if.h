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
 * @file      a653_shm_if.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     a653 shared memory handling
 * @details
 */

#ifndef __A653_SHM_IF_H__
#define __A653_SHM_IF_H__

#include <semaphore.h>

#include "a653Init.h"
#include "a653Type.h"
#include "a653Sampling.h"

/* sampling ports */

typedef struct{
  int                      init_done;  
  VALIDITY_TYPE            LAST_MSG_VALIDITY;  /* message validity */
  MESSAGE_SIZE_TYPE        LAST_SIZE;          /* msg size */
  char                     data[MAX_S_PORT_SIZE];  
} t_sampling_port_shm_data;

/* queuing ports*/
typedef struct {
  int   size;
  char  data[MAX_Q_PORT_SIZE];
} fifo_msg_header_t;


typedef struct {
  int                     init_done;
  int                     max_elem;
  int                     max_size;
  int                     cur_elem;
  int                     tx_next;
  int                     rx_next;
  sem_t                   sem_lock;
  int                     magic_number;
  fifo_msg_header_t       msg[MAX_Q_PORT_ENTRIES];
} t_queuing_port_shm_data;


/* partition */
typedef struct {
  int pid;
  int kill;
  int init;
  int go;
  char name[64];
} partition_info_t;


/* channel */
typedef struct {
  unsigned short Id;
  unsigned short ch_type;
  unsigned short maxMsgSize;
  union port_data_u {
    t_sampling_port_shm_data sp_d;
    t_queuing_port_shm_data qp_d;
  } data;
} a653_channel_shm_info_t;

typedef struct {
  partition_info_t          partition_info[MAX_PARTITION];
  a653_channel_shm_info_t   channel_info[MAX_CHANNEL];
} a653_shm_info_t;

/* function declarations */
int a653_shm_init(void);

void *a653_shm_get( int* shm_id, char* key_file, size_t segsize );
int a653_shm_remove( int* shm_id);

//void *a653_shm_get_ptr(void);

partition_info_t *a653_shm_partitions(void);


#endif /* __A653_SHM_IF_H__ */
