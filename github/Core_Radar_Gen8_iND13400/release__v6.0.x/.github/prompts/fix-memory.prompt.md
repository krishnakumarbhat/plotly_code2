---
mode: agent
description: "Fix BBE32 memory overflow in SWE5 code"
---

# Fix SWE5 Memory Overflow

## Input
- Paste linker error (e.g., `region 'BBE32_DTCM0' overflowed by X bytes`)

## Fix Steps

### 1. Audit Variables (use repo macros)
File: `software/bbe32/integration_test/dsp_integration_test.c`

```c
// Macros defined at top of file:
#define SRAM1_BSS       __attribute__((section(".sram1.bss")))
#define SRAM1_DATA      __attribute__((section(".sram1.data")))
#define SRAM1_TEXT      __attribute__((section(".sram1.text")))
#define SRAM1_BSS_USED  __attribute__((section(".sram1.bss"), used))
#define SRAM1_TEXT_USED __attribute__((section(".sram1.text"), used))
#define ATTR_UNUSED     __attribute__((unused))
```

Find missing attrs:
```bash
grep -n "^static " dsp_integration_test.c | grep -v "SRAM1"
grep -n "^volatile " dsp_integration_test.c | grep -v "SRAM1"
```

### 2. Fix Variables
| Value | Use |
|-------|-----|
| `= 0U`, `= 0.0`, `= NULL` | `static SRAM1_BSS type var = 0U;` |
| `= 1U`, non-zero enum | `static SRAM1_DATA type var = 1U;` |
| Debugger-visible volatile | `volatile SRAM1_BSS_USED G_Pointer_T DSP_Shared_Pi;` |

### 3. Find Function-Local Statics (CRITICAL BUG)
```bash
# These ALWAYS go to DTCM0 regardless of section attrs!
grep -A5 "^static.*void\|^SRAM1_TEXT" dsp_integration_test.c | grep "static.*=.*;"
```
Fix: Move ALL to file scope with `SRAM1_BSS`/`SRAM1_DATA`.

### 4. Fix Functions
```c
// Static helpers:
static SRAM1_TEXT ATTR_UNUSED void Helper(void) { }
// Public API:
SRAM1_TEXT void SIT_Function(void) { }
// Inline:
static inline SRAM1_TEXT ATTR_UNUSED void Set_Shared_Ptr(...) { }
```

### 5. Build Both
```bash
bazelisk build //:gen8 --config=flr8 --Integration_Testing=true --Anglefinding_IT=true
bazelisk build //:gen8 --config=flr8
```

## Rules
- ⛔ NEVER modify `software/common/linker/common.ld`
- ⛔ NEVER expand memory regions
- ✅ ONLY use SRAM1 macros (defined in same file)
- ✅ ALL statics at file scope (never inside functions)
- ✅ ALL statics at file scope (never function-local)

## Quick Diagnosis
| Overflow amount | Likely cause |
|-----------------|--------------|
| < 100 bytes | 1-2 missed variables or a local static |
| 100-500 bytes | Several functions/vars without SRAM1 |
| > 500 bytes | Many items missing attrs or IPC payload expansion |
