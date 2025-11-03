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
 * @file      a653_i_mutex.c
 * @copyright Airbus Defence and Space
 * @author    patrick.siegl@airbus.com
 * @date      Mon Nov  3 21:28:21 CET 2025
 * @brief     a653 mutex
 * @details    
 */

/* includes */

#include "a653Lib.h"
#include "a653Mutex.h"

void CREATE_MUTEX (
  /*in */ MUTEX_NAME_TYPE          MUTEX_NAME,
  /*in */ PRIORITY_TYPE            MUTEX_PRIORITY,
  /*in */ QUEUING_DISCIPLINE_TYPE  QUEUING_DISCIPLINE,
  /*out*/ MUTEX_ID_TYPE            *MUTEX_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void ACQUIRE_MUTEX (
  /*in */ MUTEX_ID_TYPE            MUTEX_ID,
  /*in */ SYSTEM_TIME_TYPE         TIME_OUT,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void RELEASE_MUTEX (
  /*in */ MUTEX_ID_TYPE            MUTEX_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void RESET_MUTEX (
  /*in */ MUTEX_ID_TYPE            MUTEX_ID,
  /*in */ PROCESS_ID_TYPE          PROCESS_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void GET_MUTEX_ID (
  /*in */ MUTEX_NAME_TYPE          MUTEX_NAME,
  /*out*/ MUTEX_ID_TYPE            *MUTEX_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void GET_MUTEX_STATUS (
  /*in */ MUTEX_ID_TYPE            MUTEX_ID,
  /*out*/ MUTEX_STATUS_TYPE        *MUTEX_STATUS,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void GET_PROCESS_MUTEX_STATE (
  /*in */ PROCESS_ID_TYPE          PROCESS_ID,
  /*out*/ MUTEX_ID_TYPE            *MUTEX_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}
