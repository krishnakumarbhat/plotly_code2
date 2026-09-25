---
applyTo: "software/bbe32/**,software/common/linker/**"
---

# SWE5 Memory — SRAM1 Rules (Repo-Verified)

## Golden Rule
**ALL BBE32 Integration Testing code → SRAM1 sections. NEVER default memory.**

## REAL Macros (from `dsp_integration_test.c` lines 76-81)
```c
#define SRAM1_BSS       __attribute__((section(".sram1.bss")))        /* Zero-init data */
#define SRAM1_DATA      __attribute__((section(".sram1.data")))       /* Non-zero init data */
#define SRAM1_TEXT      __attribute__((section(".sram1.text")))       /* Code/functions */
#define SRAM1_BSS_USED  __attribute__((section(".sram1.bss"), used))  /* + prevent optimization */
#define SRAM1_TEXT_USED __attribute__((section(".sram1.text"), used)) /* + prevent optimization */
#define ATTR_UNUSED     __attribute__((unused))                       /* Suppress unused warnings */
```

## REAL Variable Patterns (from `dsp_integration_test.c` lines 96-120)
```c
/* Debugger-visible globals — volatile + SRAM1_BSS_USED */
volatile SRAM1_BSS_USED G_Pointer_T DSP_Shared_Pi;
volatile SRAM1_BSS_USED G_Pointer_T DSP_Shared_Ri;

/* Zero-initialized test variables — SRAM1_BSS */
static SRAM1_BSS uint32_t DSP_Test_Iface_Num = 0U;
static SRAM1_BSS double DSP_Stub_Val = 0.0;
static SRAM1_BSS uint32_t last_interface_number = 0U;

/* Non-zero initialized — SRAM1_DATA */
static SRAM1_DATA uint32_t DSP_Stub_Len = 1U;
static SRAM1_DATA SIT_Test_Result_T DSP_Res = TEST_NOT_PERFORMED;
```

## REAL Function Patterns (from `dsp_integration_test.c`)
```c
/* Static helper with section + unused */
static SRAM1_TEXT ATTR_UNUSED SIT_Test_Result_T Is_Valid_Interface(uint32_t interface_number) { }
static SRAM1_TEXT ATTR_UNUSED void Stub_Scalar_Values(void *dstb_ptr, DataType_T data_type, double stub_value) { }

/* Inline helper */
static inline SRAM1_TEXT ATTR_UNUSED void Set_Shared_Ptr(volatile G_Pointer_T *dst_ptr, ...) { }

/* Public API */
SRAM1_TEXT void SIT_IPC_Transfer_M2D_Receive(const M2D_Payload_T *m2d_payload_ptr) { }
SRAM1_TEXT void SIT_IPC_Transfer_D2M_Send(D2M_Payload_T *d2m_payload_ptr) { }
```

## Variable Placement Rules

| Initializer | Macro | Section |
|-------------|-------|---------|
| `= 0U`, `= 0.0`, `= NULL` | `SRAM1_BSS` | `.sram1.bss` |
| `= 1U`, non-zero enum | `SRAM1_DATA` | `.sram1.data` |
| Debugger-visible (`volatile`) | `SRAM1_BSS_USED` | `.sram1.bss` + kept |
| Functions (static) | `SRAM1_TEXT ATTR_UNUSED` | `.sram1.text` |
| Functions (public) | `SRAM1_TEXT` | `.sram1.text` |

## CRITICAL: Function-Local Statics (VERIFIED BUG FIX)
```c
// ❌ CAUSES DTCM0 OVERFLOW — compiler puts in default .bss
static void Func(void) {
   static uint32_t local_var = 0U;  // Goes to DTCM0!
}

// ✅ ACTUAL FIX (from repo — `last_interface_number` was moved to file scope)
static SRAM1_BSS uint32_t last_interface_number = 0U;  // File scope
static SRAM1_TEXT ATTR_UNUSED SIT_Test_Result_T Is_Valid_Interface(uint32_t interface_number) { }
```

## Overflow Fix Checklist

When `region 'BBE32_DTCM0' overflowed by X bytes`:

1. ✅ ALL variables in `dsp_integration_test.c` have `SRAM1_BSS` or `SRAM1_DATA`
2. ✅ ALL debugger globals have `SRAM1_BSS_USED` (volatile + used)
3. ✅ NO function-local statics — all moved to file scope
4. ✅ ALL functions have `SRAM1_TEXT` (+ `ATTR_UNUSED` for static helpers)
5. ⛔ DO NOT modify `common.ld` or `linker/BUILD`

## Common Linker Errors

| Error | Cause | Fix |
|-------|-------|-----|
| `non-zero value in '.sram1.bss'` | Non-zero init in .bss | Use `SRAM1_DATA` instead |
| `'BBE32_DTCM0' overflowed` | Missing section attrs | Add SRAM1 macros to all vars |
| `'.itcm.text' will not fit` | Functions in ITCM | Add `SRAM1_TEXT` to all IT funcs |
| Undefined reference to `SIT_*` | Static or missing export | Check `static` / header decl |

## Memory Architecture

| Region | Speed | Use For |
|--------|-------|---------|
| DTCM0 | Fastest | Production real-time vars ONLY |
| ITCM | Fastest | Production real-time code ONLY |
| SRAM1 | Slower | ALL integration test code/data |

## String→Enum Memory Optimization (verified in repo)
- `Set_Shared_Ptr` takes `Test_Interface_Name_T` enum (4 bytes)
- String literals would waste DTCM0 const data
- Repo uses enum exclusively — no strings in IT code
