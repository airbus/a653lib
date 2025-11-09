// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#ifndef APEX_HOST_FNCS_WASM32
#define APEX_HOST_FNCS_WASM32

#include <stddef.h>
#ifdef __WAMR__
#include <wasm_export.h>
#endif
#ifdef __WASMTIME__
#include <wasmtime.h>

/* defined in WAMR */
typedef struct {
  const char* symbol;
  void* func_ptr;
  const char *signature;
  void *attachment;
} NativeSymbol;

#endif

unsigned getNativeSymbols(NativeSymbol** _native_symbols);

#endif /* #ifndef APEX_HOST_FNCS_WASM32 */
