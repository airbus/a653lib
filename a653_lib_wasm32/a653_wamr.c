// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#include "a653_wamr.h"

void* generate_wasm_runtime_context(wasm_file_t* wasm)
{
  return NULL;
}

void cleanup_wasm_runtime_context(void* context)
{

}

int exec_wasm_guest_func(void* wasm_runtime_context, int32_t idx)
{
  return -1;
}
