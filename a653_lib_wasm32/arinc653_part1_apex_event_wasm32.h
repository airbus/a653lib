// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: EVENT

#ifndef ARINC653_PART1_APEX_EVENT_WASM32
#define ARINC653_PART1_APEX_EVENT_WASM32

#include "arinc653_wasm32_helper.h"

/* APEX (ARINC 653 Part 1): EVENT */
#define WASM32_SIGNATURE__CREATE_EVENT                      "(iii)"
#define WASM32_SIGNATURE__SET_EVENT                         "(ii)"
#define WASM32_SIGNATURE__RESET_EVENT                       "(ii)"
#define WASM32_SIGNATURE__WAIT_EVENT                        "(iIi)"
#define WASM32_SIGNATURE__GET_EVENT_ID                      "(iii)"
#define WASM32_SIGNATURE__GET_EVENT_STATUS                  "(iii)"

WASM32_HOST_FUNC_HEADER__iii(CREATE_EVENT);
WASM32_HOST_FUNC_HEADER__ii(SET_EVENT);
WASM32_HOST_FUNC_HEADER__ii(RESET_EVENT);
WASM32_HOST_FUNC_HEADER__iIi(WAIT_EVENT);
WASM32_HOST_FUNC_HEADER__iii(GET_EVENT_ID);
WASM32_HOST_FUNC_HEADER__iii(GET_EVENT_STATUS);

#endif /* #ifndef ARINC653_PART1_APEX_EVENT_WASM32 */
