// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright 2025 Airbus Defence and Space
// SPDX-FileContributor: Patrick Siegl <patrick.siegl@airbus.com>

__attribute__((used)) // don't optimise out!
__attribute__((export_name("WASM_GUEST_FUNC_TRAMPOLINE")))
void WASM_GUEST_FUNC_TRAMPOLINE(void (*fnc)(void)) {
  (*fnc)();
}
