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
 * @file      a653_i_sync.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     a653 shared memory handling
 * @details
 */


#ifndef __A653_I_SYNC_H__
#define __A653_I_SYNC_H__

#include "a653_i_shm_if.h"


void a653_i_init_sync(void);
void a653_i_set_first(void);
void a653_i_set_next(void);
void a653_i_wait_next(void);
void a653_i_update_partitions(void);

void a653_update_queuing_ports(void);


#endif /* __A653_I_SYNC_H__ */
