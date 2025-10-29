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
 * @file      a653Partition.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     a653 partition
 * @details
 */

#ifndef A653_PARTITION
#define A653_PARTITION

#include "a653Type.h"

typedef
   enum {
        IDLE       = 0,
        COLD_START = 1,
        WARM_START = 2,
        NORMAL     = 3
   } OPERATING_MODE_TYPE;

typedef A653_INTEGER PARTITION_ID_TYPE;
typedef A653_UNSIGNED NUM_CORES_TYPE;

typedef
   enum {
        NORMAL_START         = 0,
        PARTITION_RESTART    = 1,
        HM_MODULE_RESTART    = 2,
        HM_PARTITION_RESTART = 3
   } START_CONDITION_TYPE;

typedef
   struct {
      SYSTEM_TIME_TYPE       PERIOD;
      SYSTEM_TIME_TYPE       DURATION;
      PARTITION_ID_TYPE      IDENTIFIER;
      LOCK_LEVEL_TYPE        LOCK_LEVEL;
      OPERATING_MODE_TYPE    OPERATING_MODE;
      START_CONDITION_TYPE   START_CONDITION;
      NUM_CORES_TYPE         NUM_ASSIGNED_CORES;
   } PARTITION_STATUS_TYPE;

WASM_IMPORT_MODULE("arinc653")
extern void GET_PARTITION_STATUS (
       /*out*/ PARTITION_STATUS_TYPE      *PARTITION_STATUS,
       /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );

WASM_IMPORT_MODULE("arinc653")
extern void SET_PARTITION_MODE (
       /*in */ OPERATING_MODE_TYPE        OPERATING_MODE,
       /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );

#endif /* A653_PARTITION */
