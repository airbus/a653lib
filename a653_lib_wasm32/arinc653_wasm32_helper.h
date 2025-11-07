// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#ifndef ARINC653_WASM32_HELPER

#ifdef __WAMR__
#include <stdint.h>
#include <wasm_export.h>
#include "a653_wamr.h"

#define GET_ARG_i32( X ) arg##X
#define GET_ARG_i64( X ) arg##X
#else // WASMTIME
#include <wasmtime.h>
#include "a653_wasmtime.h"

#define GET_ARG_i32( X ) args_and_results[X].i32
#define GET_ARG_i64( X ) args_and_results[X].i64
#endif


#ifdef __WAMR__

#define WASM32_HOST_FUNC_HEADER(NAME) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env );

#define WASM32_HOST_FUNC_HEADER__i(NAME) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0);

#define WASM32_HOST_FUNC_HEADER__ii(NAME) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1);

#define WASM32_HOST_FUNC_HEADER__Ii(NAME) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint64_t arg0, uint32_t arg1);

#define WASM32_HOST_FUNC_HEADER__iii(NAME) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1, uint32_t arg2);

#define WASM32_HOST_FUNC_HEADER__iIi(NAME) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint64_t arg1, uint32_t arg2);

#define WASM32_HOST_FUNC_HEADER__iiii(NAME) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1, uint32_t arg2, \
  uint32_t arg3);

#define WASM32_HOST_FUNC_HEADER__iiiii(NAME) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1, uint32_t arg2, \
  uint32_t arg3, uint32_t arg4);

#define WASM32_HOST_FUNC_HEADER__iIiii(NAME) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint64_t arg1, uint32_t arg2, \
  uint32_t arg3, uint32_t arg4);

#define WASM32_HOST_FUNC_HEADER__iiiIi(NAME) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1, uint32_t arg2, \
  uint64_t arg3, uint32_t arg4);

#define WASM32_HOST_FUNC_HEADER__iiiiii(NAME) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1, uint32_t arg2, \
  uint32_t arg3, uint32_t arg4, uint32_t arg5 );

#define WASM32_HOST_FUNC_HEADER__iIiiii(NAME) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint64_t arg1, uint32_t arg2, \
  uint32_t arg3, uint32_t arg4, uint32_t arg5 );

#define WASM32_HOST_FUNC_HEADER__iiiIii(NAME) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1, uint32_t arg2, \
  uint64_t arg3, uint32_t arg4, uint32_t arg5 );

#define WASM32_HOST_FUNC_HEADER__iiiiiii(NAME) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1, uint32_t arg2, \
  uint32_t arg3, uint32_t arg4, uint32_t arg5, \
  uint32_t arg6 );



#define WASM32_HOST_FUNCTION(NAME, WASM_BASEADDR, CONTENT) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env ) \
{ \
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env); \
  uint8_t* (WASM_BASEADDR) = wasm_runtime_addr_app_to_native(module_inst, 0); \
  \
  CONTENT \
  \
}

#define WASM32_HOST_FUNCTION__i(NAME, WASM_BASEADDR, CONTENT) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0) \
{ \
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env); \
  uint8_t* (WASM_BASEADDR) = wasm_runtime_addr_app_to_native(module_inst, 0); \
  \
  CONTENT \
  \
}

#define WASM32_HOST_FUNCTION__ii(NAME, WASM_BASEADDR, CONTENT) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1) \
{ \
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env); \
  uint8_t* (WASM_BASEADDR) = wasm_runtime_addr_app_to_native(module_inst, 0); \
  \
  CONTENT \
  \
}

#define WASM32_HOST_FUNCTION__Ii(NAME, WASM_BASEADDR, CONTENT) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint64_t arg0, uint32_t arg1) \
{ \
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env); \
  uint8_t* (WASM_BASEADDR) = wasm_runtime_addr_app_to_native(module_inst, 0); \
  \
  CONTENT \
  \
}

#define WASM32_HOST_FUNCTION__iii(NAME, WASM_BASEADDR, CONTENT) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1, uint32_t arg2) \
{ \
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env); \
  uint8_t* (WASM_BASEADDR) = wasm_runtime_addr_app_to_native(module_inst, 0); \
  \
  CONTENT \
  \
}

#define WASM32_HOST_FUNCTION__iIi(NAME, WASM_BASEADDR, CONTENT) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint64_t arg1, uint32_t arg2) \
{ \
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env); \
  uint8_t* (WASM_BASEADDR) = wasm_runtime_addr_app_to_native(module_inst, 0); \
  \
  CONTENT \
  \
}

#define WASM32_HOST_FUNCTION__iiii(NAME, WASM_BASEADDR, CONTENT) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3) \
{ \
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env); \
  uint8_t* (WASM_BASEADDR) = wasm_runtime_addr_app_to_native(module_inst, 0); \
  \
  CONTENT \
  \
}

#define WASM32_HOST_FUNCTION__iiiii(NAME, WASM_BASEADDR, CONTENT) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1, uint32_t arg2, \
  uint32_t arg3, uint32_t arg4 ) \
{ \
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env); \
  uint8_t* (WASM_BASEADDR) = wasm_runtime_addr_app_to_native(module_inst, 0); \
  \
  CONTENT \
  \
}

#define WASM32_HOST_FUNCTION__iIiii(NAME, WASM_BASEADDR, CONTENT) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint64_t arg1, uint32_t arg2, \
  uint32_t arg3, uint32_t arg4 ) \
{ \
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env); \
  uint8_t* (WASM_BASEADDR) = wasm_runtime_addr_app_to_native(module_inst, 0); \
  \
  CONTENT \
  \
}

#define WASM32_HOST_FUNCTION__iiiIi(NAME, WASM_BASEADDR, CONTENT) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1, uint32_t arg2, \
  uint64_t arg3, uint32_t arg4 ) \
{ \
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env); \
  uint8_t* (WASM_BASEADDR) = wasm_runtime_addr_app_to_native(module_inst, 0); \
  \
  CONTENT \
  \
}

#define WASM32_HOST_FUNCTION__iiiiii(NAME, WASM_BASEADDR, CONTENT) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1, uint32_t arg2, \
  uint32_t arg3, uint32_t arg4, uint32_t arg5 ) \
{ \
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env); \
  uint8_t* (WASM_BASEADDR) = wasm_runtime_addr_app_to_native(module_inst, 0); \
  \
  CONTENT \
  \
}

#define WASM32_HOST_FUNCTION__iIiiii(NAME, WASM_BASEADDR, CONTENT) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint64_t arg1, uint32_t arg2, \
  uint32_t arg3, uint32_t arg4, uint32_t arg5 ) \
{ \
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env); \
  uint8_t* (WASM_BASEADDR) = wasm_runtime_addr_app_to_native(module_inst, 0); \
  \
  CONTENT \
  \
}

#define WASM32_HOST_FUNCTION__iiiIii(NAME, WASM_BASEADDR, CONTENT) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1, uint32_t arg2, \
  uint64_t arg3, uint32_t arg4, uint32_t arg5 ) \
{ \
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env); \
  uint8_t* (WASM_BASEADDR) = wasm_runtime_addr_app_to_native(module_inst, 0); \
  \
  CONTENT \
  \
}

#define WASM32_HOST_FUNCTION__iiiiiii(NAME, WASM_BASEADDR, CONTENT) \
void WASM32_##NAME( \
  wasm_exec_env_t exec_env, \
  uint32_t arg0, uint32_t arg1, uint32_t arg2, \
  uint32_t arg3, uint32_t arg4, uint32_t arg5, \
  uint32_t arg6 ) \
{ \
  wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env); \
  uint8_t* (WASM_BASEADDR) = wasm_runtime_addr_app_to_native(module_inst, 0); \
  \
  CONTENT \
  \
}

#else // WASMTIME

#define WASM_HOST_FUNC_HEADER(NAME) \
wasm_trap_t* WASM32_##NAME(void* env, \
  wasmtime_caller_t *caller, \
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results);

#define WASM32_HOST_FUNC_HEADER           WASM_HOST_FUNC_HEADER
#define WASM32_HOST_FUNC_HEADER__i        WASM_HOST_FUNC_HEADER
#define WASM32_HOST_FUNC_HEADER__ii       WASM_HOST_FUNC_HEADER
#define WASM32_HOST_FUNC_HEADER__Ii       WASM_HOST_FUNC_HEADER
#define WASM32_HOST_FUNC_HEADER__iii      WASM_HOST_FUNC_HEADER
#define WASM32_HOST_FUNC_HEADER__iIi      WASM_HOST_FUNC_HEADER
#define WASM32_HOST_FUNC_HEADER__iiii     WASM_HOST_FUNC_HEADER
#define WASM32_HOST_FUNC_HEADER__iiiii    WASM_HOST_FUNC_HEADER
#define WASM32_HOST_FUNC_HEADER__iIiii    WASM_HOST_FUNC_HEADER
#define WASM32_HOST_FUNC_HEADER__iiiIi    WASM_HOST_FUNC_HEADER
#define WASM32_HOST_FUNC_HEADER__iiiiii   WASM_HOST_FUNC_HEADER
#define WASM32_HOST_FUNC_HEADER__iIiiii   WASM_HOST_FUNC_HEADER
#define WASM32_HOST_FUNC_HEADER__iiiIii   WASM_HOST_FUNC_HEADER
#define WASM32_HOST_FUNC_HEADER__iiiiiii  WASM_HOST_FUNC_HEADER

#define WASM_HOST_FUNCTION(NAME, WASM_BASEADDR, CONTENT) \
wasm_trap_t* WASM32_##NAME(void* env, \
  wasmtime_caller_t *caller, \
  wasmtime_val_raw_t *args_and_results, size_t num_args_and_results) \
{ \
  uint8_t* (WASM_BASEADDR) = get_linear_memory(caller); \
  \
  CONTENT \
  \
  return NULL; \
}

#define WASM32_HOST_FUNCTION          WASM_HOST_FUNCTION
#define WASM32_HOST_FUNCTION__i       WASM_HOST_FUNCTION
#define WASM32_HOST_FUNCTION__ii      WASM_HOST_FUNCTION
#define WASM32_HOST_FUNCTION__Ii      WASM_HOST_FUNCTION
#define WASM32_HOST_FUNCTION__iii     WASM_HOST_FUNCTION
#define WASM32_HOST_FUNCTION__iIi     WASM_HOST_FUNCTION
#define WASM32_HOST_FUNCTION__iiii    WASM_HOST_FUNCTION
#define WASM32_HOST_FUNCTION__iiiii   WASM_HOST_FUNCTION
#define WASM32_HOST_FUNCTION__iIiii   WASM_HOST_FUNCTION
#define WASM32_HOST_FUNCTION__iiiIi   WASM_HOST_FUNCTION
#define WASM32_HOST_FUNCTION__iiiiii  WASM_HOST_FUNCTION
#define WASM32_HOST_FUNCTION__iIiiii  WASM_HOST_FUNCTION
#define WASM32_HOST_FUNCTION__iiiIii  WASM_HOST_FUNCTION
#define WASM32_HOST_FUNCTION__iiiiiii WASM_HOST_FUNCTION

#endif

#endif /* #ifndef ARINC653_WASM32_HELPER */
