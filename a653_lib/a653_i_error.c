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
 * @file      a653_i_error.c
 * @copyright Airbus Defence and Space
 * @author    patrick.siegl@airbus.com
 * @date      Sun Oct 12 22:33:28 CEST 2025
 * @brief     a653 error
 * @details    
 */

/* includes */

#include "a653Lib.h"
#include "a653Error.h"

#include "a653_i_error.h"

void REPORT_APPLICATION_MESSAGE (
       /*in */   MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
       /*in */   MESSAGE_SIZE_TYPE        LENGTH,
       /*out*/   RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void CREATE_ERROR_HANDLER (
       /*in */   SYSTEM_ADDRESS_TYPE      ENTRY_POINT,
       /*in */   STACK_SIZE_TYPE          STACK_SIZE,
       /*out*/   RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void GET_ERROR_STATUS (
       /*out*/   ERROR_STATUS_TYPE        *ERROR_STATUS,
       /*out*/   RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void RAISE_APPLICATION_ERROR (
       /*in */   ERROR_CODE_TYPE          ERROR_CODE,
       /*in */   MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
       /*in */   ERROR_MESSAGE_SIZE_TYPE  LENGTH,
       /*out*/   RETURN_CODE_TYPE         *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}

void CONFIGURE_ERROR_HANDLER (
       /*in */   ERROR_HANDLER_CONCURRENCY_CONTROL_TYPE  CONCURRENCY_CONTROL,
       /*in */   PROCESSOR_CORE_ID_TYPE                  PROCESSOR_CORE_ID,
       /*out*/   RETURN_CODE_TYPE                        *RETURN_CODE ) {
  *RETURN_CODE = NO_ACTION;
}
