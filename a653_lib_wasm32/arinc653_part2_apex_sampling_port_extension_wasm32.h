// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>
// ARINC 653 Part 2: APEX Interface: SAMPLING PORT EXTENSION

#ifndef ARINC653_PART2_APEX_SAMPLING_PORT_EXTENSION_WASM32
#define ARINC653_PART2_APEX_SAMPLING_PORT_EXTENSION_WASM32

#include "a653_i_common_wasm32.h"

/* APEX (ARINC 653 Part 2): SAMPLING PORT EXTENSIONS */
#define WASM32_SIGNATURE__READ_UPDATED_SAMPLING_MESSAGE     "(iiiii)"
#define WASM32_SIGNATURE__GET_SAMPLING_PORT_CURRENT_STATUS  "(iii)"
#define WASM32_SIGNATURE__READ_SAMPLING_MESSAGE_CONDITIONAL "(iIiiii)"

wasm_trap_t* WASM32_READ_UPDATED_SAMPLING_MESSAGE(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_GET_SAMPLING_PORT_CURRENT_STATUS(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);
wasm_trap_t* WASM32_READ_SAMPLING_MESSAGE_CONDITIONAL(void* env,
  wasmtime_caller_t *caller,
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);

#endif /* #ifndef ARINC653_PART2_APEX_SAMPLING_PORT_EXTENSION_WASM32 */
