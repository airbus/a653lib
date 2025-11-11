// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 1: APEX Interface: PARTITION

#ifndef ARINC653_PART1_APEX_PARTITION_WASM32
#define ARINC653_PART1_APEX_PARTITION_WASM32

#include "arinc653_wasm32_helper.h"

/* APEX (ARINC 653 Part 1): PARTITION */
#define WASM32_SIGNATURE__GET_PARTITION_STATUS              "(ii)"
#define WASM32_SIGNATURE__SET_PARTITION_MODE                "(ii)"

WASM32_HOST_FUNC_HEADER(GET_PARTITION_STATUS);
WASM32_HOST_FUNC_HEADER(SET_PARTITION_MODE);

#endif /* #ifndef ARINC653_PART1_APEX_PARTITION_WASM32 */
