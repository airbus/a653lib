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
 * @file      a653_i_buffer.c
 * @copyright Airbus Defence and Space
 * @author    patrick.siegl@airbus.com
 * @date      Mon Nov  3 21:28:21 CET 2025
 * @brief     a653 buffer
 * @details    
 */

/* includes */

#include "a653Lib.h"
#include "a653Buffer.h"

void CREATE_BUFFER (
  /*in */ BUFFER_NAME_TYPE         BUFFER_NAME,
  /*in */ MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE,
  /*in */ MESSAGE_RANGE_TYPE       MAX_NB_MESSAGE,
  /*in */ QUEUING_DISCIPLINE_TYPE  QUEUING_DISCIPLINE,
  /*out*/ BUFFER_ID_TYPE           *BUFFER_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void SEND_BUFFER (
  /*in */ BUFFER_ID_TYPE           BUFFER_ID,
  /*in */ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,       /* by reference */
  /*in */ MESSAGE_SIZE_TYPE        LENGTH,
  /*in */ SYSTEM_TIME_TYPE         TIME_OUT,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void RECEIVE_BUFFER (
  /*in */ BUFFER_ID_TYPE           BUFFER_ID,
  /*in */ SYSTEM_TIME_TYPE         TIME_OUT,
  /*in */ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
    /* The message address is passed IN, although */
    /* the respective message is passed OUT       */
  /*out*/ MESSAGE_SIZE_TYPE        *LENGTH,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void GET_BUFFER_ID (
  /*in */ BUFFER_NAME_TYPE         BUFFER_NAME,
  /*out*/ BUFFER_ID_TYPE           *BUFFER_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void GET_BUFFER_STATUS (
  /*in */ BUFFER_ID_TYPE           BUFFER_ID,
  /*out*/ BUFFER_STATUS_TYPE       *BUFFER_STATUS,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}
