---
mode: agent
description: "Debug SWE5 build failures"
---

# Debug SWE5 Build Failure

## Input
- Paste the build error output
- Which build command failed (with or without IT flags)

## Error → Fix Map (Repo-Verified)

### Compiler Errors
| Error | Cause | Fix |
|-------|-------|-----|
| `undefined reference to SIT_*` | `alwayslink = True` missing on impl lib | Add to `dsp_integration_test_lib` |
| `file not found` (header) | Hook header in `select()` in consumer | Move to FLAT dep (no select wrapper) |
| `redefinition of` | Duplicate define in main target | Remove — defines propagate from hook header |
| `implicit declaration` | Source missing `#include "dsp_it_macros_test.h"` | Add unconditional include |
| Type mismatch | Gen7→Gen8 struct change | Check Gen8 struct defs (151 not 189, no `_str`) |

### Linker Errors
| Error | Cause | Fix |
|-------|-------|-----|
| `region 'BBE32_DTCM0' overflowed` | Test code in default memory | Add SRAM1 macros to ALL test vars/funcs |
| `non-zero value in '.sram1.bss'` | Non-zero init with `SRAM1_BSS` | Use `SRAM1_DATA` instead |
| `undefined symbol` | Impl lib not linked | Check `alwayslink = True` |
| `multiple definitions` | Missing `#if defined()` guard | Add `#if defined(Integration_Testing)` |

### Bazel Errors
| Error | Cause | Fix |
|-------|-------|-----|
| `no such target` | Wrong BUILD target name | Verify exact name in BUILD file |
| `no visible configs` | Missing `//:` prefix | Use `//:Integration_Testing_enabled` |
| `not visible` | Missing visibility attr | Add `visibility = ["//visibility:public"]` |

## Fix Procedure
1. Identify error category (compiler/linker/Bazel)
2. Match to table above → apply fix
3. Rebuild both:
```bash
bazelisk build //:gen8 --config=flr8
bazelisk build //:gen8 --config=flr8 --Integration_Testing=true --Anglefinding_IT=true
```

## Critical Reminders
- Hook header dep: ALWAYS flat/unconditional in consumer BUILD
- IT defines: ONLY in hook header target's `defines = select(...)`
- Consumer targets: NO IT defines in their `defines`/`local_defines`
- `ipc_dsp_lib` has `local_defines` for XCP only — NOT for IT
- `alwayslink = True` on `dsp_integration_test_lib` and `master_integration_test`
