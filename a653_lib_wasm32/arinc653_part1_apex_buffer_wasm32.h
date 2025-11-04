// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: BUFFER

#ifndef ARINC653_PART1_APEX_BUFFER_WASM32
#define ARINC653_PART1_APEX_BUFFER_WASM32

#include "arinc653_wasm32_helper.h"

/* APEX (ARINC 653 Part 1): BUFFER */
#define WASM32_SIGNATURE__CREATE_BUFFER                     "(iiiiii)"
#define WASM32_SIGNATURE__SEND_BUFFER                       "(iiiIi)"
#define WASM32_SIGNATURE__RECEIVE_BUFFER                    "(iIiii)"
#define WASM32_SIGNATURE__GET_BUFFER_ID                     "(iii)"
#define WASM32_SIGNATURE__GET_BUFFER_STATUS                 "(iii)"

WASM32_HOST_FUNC_HEADER__iiiiii(CREATE_BUFFER);
WASM32_HOST_FUNC_HEADER__iiiIi(SEND_BUFFER);
WASM32_HOST_FUNC_HEADER__iIiii(RECEIVE_BUFFER);
WASM32_HOST_FUNC_HEADER__iii(GET_BUFFER_ID);
WASM32_HOST_FUNC_HEADER__iii(GET_BUFFER_STATUS);

#endif /* #ifndef ARINC653_PART1_APEX_BUFFER_WASM32 */
