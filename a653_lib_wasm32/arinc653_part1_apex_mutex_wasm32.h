// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: MUTEX

#ifndef ARINC653_PART1_APEX_MUTEX_WASM32
#define ARINC653_PART1_APEX_MUTEX_WASM32

#include "arinc653_wasm32_helper.h"

/* APEX (ARINC 653 Part 1): MUTEX */
#define WASM32_SIGNATURE__CREATE_MUTEX                      "(iiiii)"
#define WASM32_SIGNATURE__ACQUIRE_MUTEX                     "(iIi)"
#define WASM32_SIGNATURE__RELEASE_MUTEX                     "(ii)"
#define WASM32_SIGNATURE__RESET_MUTEX                       "(iii)"
#define WASM32_SIGNATURE__GET_MUTEX_ID                      "(iii)"
#define WASM32_SIGNATURE__GET_MUTEX_STATUS                  "(iii)"
#define WASM32_SIGNATURE__GET_PROCESS_MUTEX_STATE           "(iii)"

WASM32_HOST_FUNC_HEADER(CREATE_MUTEX);
WASM32_HOST_FUNC_HEADER(ACQUIRE_MUTEX);
WASM32_HOST_FUNC_HEADER(RELEASE_MUTEX);
WASM32_HOST_FUNC_HEADER(RESET_MUTEX);
WASM32_HOST_FUNC_HEADER(GET_MUTEX_ID);
WASM32_HOST_FUNC_HEADER(GET_MUTEX_STATUS);
WASM32_HOST_FUNC_HEADER(GET_PROCESS_MUTEX_STATE);

#endif /* #ifndef ARINC653_PART1_APEX_MUTEX_WASM32 */
