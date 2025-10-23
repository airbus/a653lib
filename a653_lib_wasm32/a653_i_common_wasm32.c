// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#include <stdio.h>

#include "a653_i_common_wasm32.h"

wasm_processes_t wasm_processes;

uint8_t* get_linear_memory(wasmtime_caller_t* caller) {
    wasmtime_extern_t ext;
    const char *m = "memory";
    if ( ! wasmtime_caller_export_get(caller, m, strlen(m), &ext)) {
        fprintf(stderr, "Error: memory export not found!\n");
        return NULL;
    }

    if (ext.kind != WASM_EXTERN_MEMORY) {
        fprintf(stderr, "Error: export 'memory' is not a memory!\n");
        return NULL;
    }

    wasmtime_context_t *context = wasmtime_caller_context(caller);
    return wasmtime_memory_data(context, &ext.of.memory);
}
