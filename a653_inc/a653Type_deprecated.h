
/*
 * Copyright (c) 2022-2023 Airbus Defence and Space
 * Copyright (c) 2026 DLR e. V.
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
 * @file      a653Type_deprecated.h
 * @copyright Airbus Defence and Space
 * @copyright DLR e. V.
 * @author    nicolaus.baer@airbus.com
 * @author    wanja.zaeske@dlr.de
 * @date      Wed Sep  23 16:35 2026
 * @brief     a653 types deprecated
 * @details
 */


#ifndef __A653_TYPES_DEPRECATED_H
#define __A653_TYPES_DEPRECATED_H

#include <stdint.h>
#include "a653Type.h"

/* Global deprecated type definitions These types were offered previously, but the APEX standard
 * uses the `APEX_`, not the `A653_` prefix. For compatibility, these are provided, but it is
 * recommend to migrated away from `A653_` types.
 */

typedef APEX_BYTE           A653_BYTE;              /*  8 bits unsigned   */
typedef int16_t             A653_WORD;              /* 16 bits            */
typedef APEX_INTEGER        A653_INTEGER;           /* 32 bits            */
typedef APEX_UNSIGNED       A653_UNSIGNED;          /* 32 bits unsigned   */
typedef APEX_LONG_INTEGER   A653_LONG_INTEGER;      /* 64 bits            */

#endif /* __A653_TYPES_DEPRECATED_H */
