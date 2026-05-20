// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#ifndef APEX_HOST_FNCS_WASM32
#define APEX_HOST_FNCS_WASM32

#include <stddef.h>
#ifdef __WAMR__
#include <wasm_export.h>

#define WASM_HOSTFUNC_SIGNATURE( FNC ) { #FNC, (void*)(uintptr_t)WASM32_##FNC, WASM32_SIGNATURE__##FNC, NULL }

#endif
#ifdef __WASMTIME__
#include <wasmtime.h>

/* not equal (see func_ptr), but similar as defined in WAMR */
/* https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/core/iwasm/include/lib_export.h */
typedef struct {
  const char* symbol;
  wasm_trap_t * (*func_ptr)(void *, wasmtime_caller_t *, wasmtime_val_raw_t *, size_t);
  const char *signature;
  void *attachment;
} NativeSymbol;

#define WASM_HOSTFUNC_SIGNATURE( FNC ) { #FNC, WASM32_##FNC, WASM32_SIGNATURE__##FNC, NULL }

#endif

unsigned getNativeSymbols(NativeSymbol** _native_symbols);

#endif /* #ifndef APEX_HOST_FNCS_WASM32 */
