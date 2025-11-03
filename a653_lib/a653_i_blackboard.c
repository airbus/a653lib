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
 * @file      a653_i_blackboard.c
 * @copyright Airbus Defence and Space
 * @author    patrick.siegl@airbus.com
 * @date      Mon Nov  3 21:28:21 CET 2025
 * @brief     a653 blackboard
 * @details    
 */

/* includes */

#include "a653Lib.h"
#include "a653Blackboard.h"

void CREATE_BLACKBOARD (
  /*in */ BLACKBOARD_NAME_TYPE     BLACKBOARD_NAME,
  /*in */ MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE,
  /*out*/ BLACKBOARD_ID_TYPE       *BLACKBOARD_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void DISPLAY_BLACKBOARD (
  /*in */ BLACKBOARD_ID_TYPE       BLACKBOARD_ID,
  /*in */ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,       /* by reference */
  /*in */ MESSAGE_SIZE_TYPE        LENGTH,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void READ_BLACKBOARD (
  /*in */ BLACKBOARD_ID_TYPE       BLACKBOARD_ID,
  /*in */ SYSTEM_TIME_TYPE         TIME_OUT,
  /*in */ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
    /* The message address is passed IN, although */
    /* the respective message is passed OUT       */
  /*out*/ MESSAGE_SIZE_TYPE        *LENGTH,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void CLEAR_BLACKBOARD (
  /*in */ BLACKBOARD_ID_TYPE       BLACKBOARD_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void GET_BLACKBOARD_ID (
  /*in */ BLACKBOARD_NAME_TYPE     BLACKBOARD_NAME,
  /*out*/ BLACKBOARD_ID_TYPE       *BLACKBOARD_ID,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void GET_BLACKBOARD_STATUS (
  /*in */ BLACKBOARD_ID_TYPE       BLACKBOARD_ID,
  /*out*/ BLACKBOARD_STATUS_TYPE   *BLACKBOARD_STATUS,
  /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}
