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
 * @file      a653_i_semaphore.c
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Wed 11 Dec 2024 08:03:18 AM EST
 * @brief     a653 semaphore
 * @details    
 */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "a653Init.h"
#include "a653Type.h"
#include "a653Error.h"
#include "a653Semaphore.h"
#include "a653Partition.h"

void CREATE_SEMAPHORE
( /*in */ SEMAPHORE_NAME_TYPE SEMAPHORE_NAME,
  /*in */ SEMAPHORE_VALUE_TYPE CURRENT_VALUE,
  /*in */ SEMAPHORE_VALUE_TYPE MAXIMUM_VALUE,
  /*in */ QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE,
  /*out*/ SEMAPHORE_ID_TYPE *SEMAPHORE_ID,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
{
}

void WAIT_SEMAPHORE
( /*in */ SEMAPHORE_ID_TYPE SEMAPHORE_ID,
  /*in */ SYSTEM_TIME_TYPE TIME_OUT,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
{
}

void SIGNAL_SEMAPHORE
( /*in */ SEMAPHORE_ID_TYPE SEMAPHORE_ID,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
{
}

void GET_SEMAPHORE_ID
( /*in */ SEMAPHORE_NAME_TYPE SEMAPHORE_NAME,
  /*out*/ SEMAPHORE_ID_TYPE *SEMAPHORE_ID,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
{
}

void GET_SEMAPHORE_STATUS
( /*in */ SEMAPHORE_ID_TYPE SEMAPHORE_ID,
  /*out*/ SEMAPHORE_STATUS_TYPE *SEMAPHORE_STATUS,
  /*out*/ RETURN_CODE_TYPE *RETURN_CODE )
{
}
