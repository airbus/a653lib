// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#include <stdio.h>
#include <stdlib.h>

#include "generic_helper.h"
#ifdef __WAMR__
#include "a653_wamr.h"
#else
#include "a653_wasmtime.h"
#endif

/*
 * 1. Ugly learning about WASM: re-alloc of memory -> base address moves.
 * The guests memory is - obviously - backed by malloc/mmap.
 * In case, the guest uses more and more memory, a re-alloc is performed,
 * which must not but can very likely lead to a new memory area within the host.
 * Consequently, the base address of this memory region is very likely different,
 * each time the hosts tries to reference this memory. As of this, we need to
 * get over and over again i.e. for each host func call the (new) memory base addr.
 *
 */

// wasmtime_memory_data_size (const wasmtime_context_t *store, const wasmtime_memory_t *memory) // FIXME: could do a bounds check!

wasm_prcs_info_t wasm_prcs_info;

int main(int argc, char* argv[])
{
  // FIXME with proper getopt()
  if (argc < 2) {
    fprintf(stderr, "Usage: %s guest.wasm\n", argv[0]);
    return -1;
  }
  const char *wasm_file = argv[1];

  // Load .wasm binary supplied
  wasm_file_t* wasm = load_wasm_file(wasm_file);

  wasm_prcs_info.wasm_rt_ctx = generate_wasm_runtime_context(wasm);
  free(wasm);
  if ( ! wasm_prcs_info.wasm_rt_ctx)
    return -1;

  if ( ! exec_wasm_guest_func(wasm_prcs_info.wasm_rt_ctx, -1))
    fprintf(stderr, "ERR: wasm_processid not found\n");

  cleanup_wasm_runtime_context(wasm_prcs_info.wasm_rt_ctx);
  free(wasm_prcs_info.wasm_rt_ctx);

  return 0;
}
