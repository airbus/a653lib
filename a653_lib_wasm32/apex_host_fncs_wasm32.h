// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#ifndef APEX_HOST_FNCS_WASM32
#define APEX_HOST_FNCS_WASM32

#ifdef __WAMR__

/* WAMR has a define for NativeSymbol */

#else

#include <wasmtime.h>

typedef struct {
  const char* symbol;
  wasmtime_func_unchecked_callback_t func_ptr;
  const char* signature;
  void *attachment;
} NativeSymbol;

#endif

unsigned getNativeSymbols(NativeSymbol** _native_symbols);

#endif /* #ifndef APEX_HOST_FNCS_WASM32 */
