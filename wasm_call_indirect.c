// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

__attribute__((used)) // don't optimise out!
__attribute__((export_name("__WASM_GUEST_CALL_INDIRECT")))
void __WASM_GUEST_CALL_INDIRECT(void (*fnc)(void)) {
  (*fnc)();
}
