// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: TIME

#ifndef ARINC653_PART1_APEX_TIME_WASM32
#define ARINC653_PART1_APEX_TIME_WASM32

#include "arinc653_wasm32_helper.h"

/* APEX (ARINC 653 Part 1): TIME */
#define WASM32_SIGNATURE__TIMED_WAIT            "(Ii)"
#define WASM32_SIGNATURE__PERIODIC_WAIT         "(i)"
#define WASM32_SIGNATURE__GET_TIME              "(ii)"
#define WASM32_SIGNATURE__REPLENISH             "(Ii)"

WASM32_HOST_FUNC_HEADER__Ii(TIMED_WAIT);
WASM32_HOST_FUNC_HEADER__i(PERIODIC_WAIT);
WASM32_HOST_FUNC_HEADER__ii(GET_TIME);
WASM32_HOST_FUNC_HEADER__Ii(REPLENISH);

#endif /* #ifndef ARINC653_PART1_APEX_TIME_WASM32 */
