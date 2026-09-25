---
description: "SWE5 Integration Testing expert for Gen8 embedded radar (BBE32/R52, Bazel, IPC, SRAM1)"
---

# SWE5 Skill — Expert Decision Engine (Repo-Verified)

## Quick Rules (MEMORIZE)

1. ALL SWE5 → `#if defined(Integration_Testing)` guard
2. ALL BBE32 test code → SRAM1 macros (`SRAM1_BSS`, `SRAM1_DATA`, `SRAM1_TEXT`)
3. Hooks → `#include "dsp_it_macros_test.h"` UNCONDITIONAL
4. Enums → `IF_ANGLEFINDING(DCS_X(Ri_name, WorkItemID_U))`
5. BUILD → hook header is FLAT dep (no select wrapper needed)
6. IT defines propagate FROM hook header target via Bazel transitive deps
7. Build BOTH → `--config=flr8` AND `--config=flr8 --Integration_Testing=true --Anglefinding_IT=true`
8. Comment → `/* Providing <var> info, for Integration Testing */`
9. Functions use `SRAM1_TEXT ATTR_UNUSED` (static) or `SRAM1_TEXT` (public)
10. `alwayslink = True` on `dsp_integration_test_lib` / `master_integration_test`

## Decision Tree

```
Task?
├─ Adding interface
│  ├─ Add to INTERFACE_ENUM_LIST (with IF_MODULE guard)
│  ├─ Add Provider case (Stub + Set_Shared_Ptr)
│  ├─ Add Receiver case (Validate + Set_Shared_Ptr)
│  └─ Build both configs
├─ Build failing
│  ├─ Undefined ref → check alwayslink, check dep on impl lib
│  ├─ Header not found → hook header must be flat dep (not in select)
│  ├─ DTCM0 overflow → SRAM1 attrs on all test code
│  ├─ Redefinition → remove duplicate defines from main target
│  └─ No visible config → use `//:` prefix
├─ Memory overflow
│  ├─ Check all vars have SRAM1_BSS/SRAM1_DATA
│  ├─ Find function-local statics → move to file scope
│  ├─ Check all functions have SRAM1_TEXT
│  └─ NEVER modify linker scripts
├─ Adding hook to source
│  ├─ Add macro to centralized *_it_macros_test.h
│  ├─ Source: `#include "dsp_it_macros_test.h"` (already there?)
│  ├─ Call: `/* Providing <var> info, for Integration Testing */`
│  ├─ BUILD: verify hook header in flat deps
│  └─ Build both configs
└─ Adding new module
   ├─ Root BUILD: bool_flag + config_setting
   ├─ .bazelrc: flag_alias
   ├─ Header: IF_MODULE guard macro
   ├─ Header: INTERFACE_ENUM_LIST entries
   ├─ Source: Provider/Receiver functions
   ├─ Hook header: new macro definitions
   └─ BUILD: new targets + consumer deps
```

## Real File Locations (from repo)

| File | Path | Lines |
|------|------|-------|
| BBE32 hook header | `software/bbe32/integration_test/dsp_it_macros_test.h` | ~160 |
| BBE32 impl header | `software/bbe32/integration_test/dsp_integration_test.h` | ~420 |
| BBE32 impl source | `software/bbe32/integration_test/dsp_integration_test.c` | ~1300 |
| R52 hook header | `software/r52/integration_test/master_it_macros_test.h` | ~195 |
| R52 impl header | `software/r52/integration_test/master_integration_test.h` | ~370 |
| R52 impl source | `software/r52/integration_test/master_integration_test.c` | ~980 |
| IPC data | `software/common/ipc/ipc_data.h` | SIT_Data_T at ~226 |
| Root flags | `BUILD` | lines 288–310 |

## Real Hook Usage (verified locations)

| Source File | Include Line | Hook Calls |
|-------------|-------------|------------|
| `bbe32/src/ipc_dsp.c` | L69 | `SIT_IPC_TRANSFER_D2M_SEND` (L413), `SIT_IPC_TRANSFER_M2D_RECEIVE` (L581) |
| `bbe32/src/doppler_process_ifc.c` | L75 | `SIT_D2M_MSG_BUFFER` (L735) |
| `bbe32/src/anglefinding_project_interface.c` | L64 | `SIT_AF_INPUT` (L604) |
| `r52/main.c` | L38 | `SIT_WAIT_FOR_DEBUGGER` |
| `r52/dsp_setup/src/dsp_setup.c` | L74 | `SIT_M2D_ONETIME` (L294), `SIT_M2D_PERLOOK` (L828), `SIT_IPC_TRANSFER_M2D_SEND` (L831) |

## Real BUILD Dep Pattern (verified)

The actual pattern is simpler than documented — just flat dep:
```python
deps = [
    "//software/bbe32/integration_test:dsp_it_macros_test_h",
    # ... other deps
]
```
NO `select()` around the hook header. The `select()` is INSIDE the hook header's own `defines`.

## Aptiv Coding Checklist
- [ ] `0U`, `1U` suffixes on all unsigned literals
- [ ] `1e-5F`, `0.0F` float suffixes
- [ ] Function doc: Name, Shared Variables, Parameters, Return Value, Design Info, Change References
- [ ] Header guard: `#ifndef MODULE_H` / `#define MODULE_H`
- [ ] File header with copyright
- [ ] Section banners in standard order
- [ ] `extern "C"` with standard comment
- [ ] Allman braces, always braces on if/else

## Error Quick Fix Map (from real debugging)

| Error Message/Pattern | Root Cause | Fix |
|----------------------|------------|-----|
| `region 'BBE32_DTCM0' overflowed by N bytes` | Test vars/funcs in default memory | Add `SRAM1_BSS`/`SRAM1_DATA` on ALL test vars, `SRAM1_TEXT` on ALL funcs |
| `non-zero value in '.sram1.bss'` | Initialized var using `SRAM1_BSS` | Change to `SRAM1_DATA` (for non-zero init values) |
| `undefined reference to 'SIT_*'` | Impl lib not force-linked | Add `alwayslink = True` on `dsp_integration_test_lib` |
| `fatal error: 'dsp_it_macros_test.h' file not found` | Hook header in `select()` in consumer | Move to FLAT dep list (not inside select wrapper) |
| `warning: unused function 'SIT_*'` | Missing `ATTR_UNUSED` on static | Use `static SRAM1_TEXT ATTR_UNUSED void func()` |
| `redefinition of 'Integration_Testing'` | Duplicate define in consumer BUILD | Remove — defines propagate from hook header target |
| `error: implicit declaration of function 'SIT_*'` | Source missing hook include | Add `#include "dsp_it_macros_test.h"` (unconditional) |
| `Dead code (TiCS): code never executed` | Direct `#if` around function calls | Use hook macros (compile to nothing when IT disabled) |
| `calmat3/4 access` | Module uses 3+ cal sections | Guard with `#if (AF_CAL_SEC_N >= 3U)` → else `TEST_SKIPPED` |
| `type 'XXX_str' not found` | Gen7→Gen8 struct rename | Remove `_str` suffix, check array dims (151 not 189) |
| `error: no member named 'field' in 'struct_T'` | Gen8 struct field renamed | Check current struct definition in header |
| `multiple definition of 'SIT_*'` | Missing `#if defined()` guard in impl | All IT code must be inside `#if defined(Integration_Testing)` |
| `no such target '//:IT_flag'` | Wrong config_setting reference | Use `//:Integration_Testing_enabled` (exact name) |
| `target not visible from //software/bbe32/src` | Missing visibility on IT lib | Add `visibility = ["//visibility:public"]` |
| Linker: `cannot find -l<lib>` | Transitive dep missing | Check dep chain; add explicit dep if needed |
| `sizeof applied to incomplete type` | Header include order issue | Ensure struct-defining header included before use |

## Anti-Patterns (NEVER DO)
- ❌ Put `dsp_it_macros_test_h` inside `select()` in consumer BUILD
- ❌ Add `defines = ["Integration_Testing"]` to main target
- ❌ Use `#include "dsp_integration_test.h"` unconditionally in main source
- ❌ Use `#if defined(Integration_Testing)` around function calls (dead code)
- ❌ `Set_Shared_Ptr("string_name", ...)` — use enum
- ❌ `static uint32_t var;` inside function body (DTCM0 overflow)
- ❌ Modify `software/common/linker/common.ld`
- ❌ Create new enum duplicating `Test_Interface_Name_T`
