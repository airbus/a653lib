// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

#include "generic_helper.h"

#include <stdio.h>
#include <stdlib.h>

// Helper to load a binary file (e.g., guest.wasm)
wasm_file_t* load_wasm_file(const char* filename)
{
  FILE* file = fopen(filename, "rb");
  if (!file) {
    fprintf(stderr, "❌ Failed to open wasm file");
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  wasm_file_t* wasm = (wasm_file_t*) malloc (sizeof(wasm_file_t) + file_size);
  wasm->size = file_size;
  if (fread(wasm->data, file_size, 1, file) != 1) {
    fprintf(stderr, "ERR: Failed to read wasm file!\n");
    free(wasm);
    return NULL;
  }
  fclose(file);

  return wasm;
}


int signature_parameter_count(const char *signature) {
  int parmc = 0;
  for (char *s = (char*)signature, *bgn_braket = NULL; *s != '\0'; ++s) {
    switch ( *s ) {
      case '(':
        bgn_braket = s;
        break;
      case ')':
        if (*bgn_braket != '(') {
          fprintf(stderr, "ERR: end braket without begin braket!\n");
          return -1;
        }
        return parmc; // done
      break;
      case 'i': // 32-bit integer (i32)
      case 'I': // 64-bit integer (i64)
      case 'f': // 32-bit float (f32)
      case 'F': // 64-bit float (f64)
      case 'r': // externref type (usually a uintptr_t), or GC references
      case '$': // String in WASM memory
        ++parmc;
        break;
      case '*': // Buffer address (pointer) in WASM memory
        ++s;
        if(*s != '~') {
          fprintf(stderr, "ERR: not supported character sequence *%c!\n", *s);
          return -1;
        }
        parmc += 2;
      default:
      case '~': // Byte length of the preceding buffer pointer (*), must follow *
        fprintf(stderr, "ERR: not supported character %c!\n", *s);
        return -1;
    }
  }
  return -1;
}
