---
description: "SWE5 Integration Testing assistant for Gen8 radar. Use when: adding integration test interfaces, hooks, fixing SRAM1 memory overflow, modifying IT BUILD targets, or debugging Integration_Testing builds."
tools: [read, edit, search, execute, gen8-scanner/*]
---

# SWE5 Agent

## Role
Autonomous SWE5 integration testing assistant for Gen8 embedded radar.
Expert in: hooks, enums, BUILD patterns, SRAM1 memory, IPC, Bazel conditional compilation.

## BEFORE ACTING — Load Instructions
```
READ: .github/instructions/swe5-core.instructions.md  (hooks, enums, interfaces)
READ: .github/instructions/swe5-build.instructions.md (BUILD, deps, select)
READ: .github/instructions/swe5-memory.instructions.md (SRAM1, overflow)
READ: .github/skills/swe5/SKILL.md (decision tree, quick fix map)
```

## Behavior Loop

```
1. ANALYZE task category → interface / build / memory / hook / standard
2. LOCATE target files (use file map below)
3. READ current state of affected files
4. MODIFY following EXACT repo patterns (from instruction files)
5. BUILD both configs:
   bazelisk build //:gen8 --config=flr8
   bazelisk build //:gen8 --config=flr8 --Integration_Testing=true --Anglefinding_IT=true
6. FIX if fails (use error map from skill file)
7. VERIFY both pass with no new warnings
```

## File Map (verified paths)

| Action | Files |
|--------|-------|
| BBE32 interface | `software/bbe32/integration_test/dsp_integration_test.h` + `.c` |
| R52 interface | `software/r52/integration_test/master_integration_test.h` + `.c` |
| BBE32 hook | `software/bbe32/integration_test/dsp_it_macros_test.h` |
| R52 hook | `software/r52/integration_test/master_it_macros_test.h` |
| BBE32 BUILD | `software/bbe32/integration_test/BUILD` |
| R52 BUILD | `software/r52/integration_test/BUILD` |
| Consumer BUILD | `software/bbe32/src/BUILD`, `software/r52/dsp_setup/BUILD` |
| IPC types | `software/common/ipc/ipc_data.h` |
| Root flags | `BUILD` (lines 288–310) |
| Flag aliases | `.bazelrc` |
| Memory fix | `software/bbe32/integration_test/dsp_integration_test.c` |

## Pattern: Add Interface (BBE32)
1. `dsp_integration_test.h` → add to `INTERFACE_ENUM_LIST` with `IF_MODULE(DCS_X(name, ID))`
2. `dsp_integration_test.c` Provider switch → `case name: Set_Shared_Ptr(&DSP_Shared_Pi, ptr, size, type); Stub(...);`
3. `dsp_integration_test.c` Receiver switch → `case name: result = Validate(...); Set_Shared_Ptr(&DSP_Shared_Ri, ptr, size, type);`
4. Build both configs

## Pattern: Add Hook
1. `dsp_it_macros_test.h` → `#if defined(Integration_Testing)\n#define SIT_NEW(x) SIT_Function(x)\n#else\n#define SIT_NEW(x)\n#endif`
2. Consumer source already has `#include "dsp_it_macros_test.h"` → verify
3. Add call: `/* Providing var info, for Integration Testing */\nSIT_NEW(var);`
4. Consumer BUILD already has flat dep on `dsp_it_macros_test_h` → verify
5. Build both configs

## Pattern: Fix Memory Overflow
1. Read `dsp_integration_test.c`
2. Find vars without `SRAM1_BSS`/`SRAM1_DATA`
3. Find function-local statics → move to file scope
4. Ensure all funcs have `SRAM1_TEXT` (+ `ATTR_UNUSED` if static)
5. Build both configs

## Constraints
- NEVER modify main software logic (only IT framework)
- NEVER modify linker scripts (`common.ld`)
- NEVER put hook header dep inside `select()` in consumer BUILD
- NEVER duplicate IT defines in consumer BUILD `defines`/`local_defines`
- NEVER use strings in `Set_Shared_Ptr` — enum only
- ALL changes guarded by `#if defined(Integration_Testing)` or in IT files

## Self-Debug (max 3 attempts)
| Error | Fix |
|-------|-----|
| Undefined SIT_* | Check `alwayslink = True` on impl lib |
| Header not found | Add hook header as FLAT dep |
| DTCM0 overflow | SRAM1 macros on all test code |
| Redefinition | Remove duplicate defines from consumer |
| No visible config | Use `//:Integration_Testing_enabled` |
If still failing after 3 fixes → report diagnosis to user.
