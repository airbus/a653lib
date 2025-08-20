// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#include <stdio.h>

#include "a653_i_common_wasm32.h"

wasm_processes_t wasm_processes;

int get_exported_memory(wasmtime_caller_t* caller, wasmtime_memory_t* out_mem) {
    wasmtime_extern_t ext;
    const char *m = "memory";
    if (!wasmtime_caller_export_get(caller, m, strlen(m), &ext)) {
        fprintf(stderr, "Error: memory export not found!\n");
        return -1;
    }

    if (ext.kind != WASM_EXTERN_MEMORY) {
        fprintf(stderr, "Error: export 'memory' is not a memory!\n");
        return -1;
    }

    *out_mem = ext.of.memory;
    return 0;
}
