// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#ifndef A653_WASMTIME
#define A653_WASMTIME

#include <wasmtime.h>

#include "generic_helper.h"

void* generate_wasm_runtime_context(wasm_file_t* wasm);
void cleanup_wasm_runtime_context(void* wasm_runtime_context);
/**
 * idx = -1 -> default
 *             otherwise use __indirect_function_table[]
 */
int exec_wasm_guest_func(void* wasm_runtime_context, int32_t idx);

#endif /* #ifndef A653_WASMTIME */
