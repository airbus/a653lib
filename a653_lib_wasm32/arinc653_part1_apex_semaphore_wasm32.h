// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: SEMAPHORE

#ifndef ARINC653_PART1_APEX_SEMAPHORE_WASM32
#define ARINC653_PART1_APEX_SEMAPHORE_WASM32

#include "arinc653_wasm32_helper.h"

/* APEX (ARINC 653 Part 1): SEMAPHORE */
#define WASM32_SIGNATURE__CREATE_SEMAPHORE                  "(iiiiii)"
#define WASM32_SIGNATURE__WAIT_SEMAPHORE                    "(iIi)"
#define WASM32_SIGNATURE__SIGNAL_SEMAPHORE                  "(ii)"
#define WASM32_SIGNATURE__GET_SEMAPHORE_ID                  "(iii)"
#define WASM32_SIGNATURE__GET_SEMAPHORE_STATUS              "(iii)"

WASM_HOST_FUNC_HEADER(CREATE_SEMAPHORE);
WASM_HOST_FUNC_HEADER(WAIT_SEMAPHORE);
WASM_HOST_FUNC_HEADER(SIGNAL_SEMAPHORE);
WASM_HOST_FUNC_HEADER(GET_SEMAPHORE_ID);
WASM_HOST_FUNC_HEADER(GET_SEMAPHORE_STATUS);

#endif /* #ifndef ARINC653_PART1_APEX_SEMAPHORE_WASM32 */
