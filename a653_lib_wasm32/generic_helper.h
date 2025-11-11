// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#ifndef GENERIC_HELPER
#define GENERIC_HELPER

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../a653_inc/a653Init.h" // MAX_PRCS

#define HAS_64BIT_MEM   false

typedef struct {
    uint32_t ENTRY_POINT[MAX_PRCS];
    uint32_t ENTRY_POINT_ERROR_HANDLER;
    void* wasm_rt_ctx;
} wasm_prcs_info_t;

typedef struct {
    size_t size;
    unsigned char data[];
} wasm_file_t;

wasm_file_t* load_wasm_file(const char* filename);

int signature_parameter_count(const char *signature);

#endif /* #ifndef GENERIC_HELPER */
