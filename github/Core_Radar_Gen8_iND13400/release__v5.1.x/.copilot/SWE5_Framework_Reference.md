# SWE5 Framework Reference Guide

> **⚠️ FOR AI ASSISTANTS:** Before making any SWE5 changes, read the [AI ASSISTANT INSTRUCTIONS](#-ai-assistant-instructions---read-first) section. You MUST build and validate changes, self-debug issues, and update this document with learnings.

---

## 📋 QUICK REFERENCE - MAIN INSTRUCTIONS FOR AI ASSISTANTS

### **🔴 CRITICAL FIRST STEPS:**
1. **ALWAYS check `dsp_integration_test.h` and `dsp_integration_test.c`** - BBE32 test code lives here
2. **ALWAYS check `master_integration_test.h` and `master_integration_test.c`** - R52 test code lives here
3. **Linker issues? Check `software/common/linker/common.ld` and `software/common/linker/BUILD`**
4. **Build BOTH configurations** - with AND without Integration_Testing flags

### **Key Files to Check When Debugging SWE5 Issues:**
| Issue Type | Primary Files | Solution Reference |
|------------|---------------|-------------------|
| DTCM0/ITCM Overflow | `software/bbe32/integration_test/dsp_integration_test.c` | Use SRAM1 section attributes |
| BBE32 Test Code | `software/bbe32/integration_test/dsp_integration_test.c/.h` | Section 10.4 |
| R52 Test Code | `software/r52/integration_test/master_integration_test.c/.h` | Section 9 |
| Hook Headers | `dsp_it_macros_test.h`, `master_it_macros_test.h` | Section 16 |
| IPC Data Transfer | `software/common/ipc/ipc_data.h` | Section 5 |
| Linker Script | `software/common/linker/bbe32.ld`, `common.ld` | Section 10.4.7 (NOTE: No IT content in linker — use SRAM1 only) |

### **Build Commands (MUST RUN EVERY TIME):**
```bash
# Integration Testing build - MUST PASS
bazelisk build //:gen8 --config=flr8 --Integration_Testing=true --Anglefinding_IT=true

# Normal build - MUST ALSO PASS (no regression)
bazelisk build //:gen8 --config=flr8
```

### **Memory Overflow Quick Fix Checklist:**
1. ✅ ALL test variables have `.sram1.bss` or `.sram1.data` section attributes
2. ✅ ALL test functions have `.sram1.text` section attributes
3. ✅ NO function-local static variables (move to file scope with section attribute)
4. ⛔ **DO NOT modify linker scripts or memory layout** - use SRAM1 section attributes only

### **Completed Fixes (Reference - Already Applied):**

| Date | Fix | Files Modified |
|------|-----|----------------|
| 2026-02-19 | All BBE32 test variables moved to SRAM1 sections | `dsp_integration_test.c` |
| 2026-02-19 | All BBE32 test functions moved to `.sram1.text` | `dsp_integration_test.c` |
| 2026-02-17 | Centralized hook headers created | `dsp_it_macros_test.h`, `master_it_macros_test.h` |
| 2026-02-26 | Renamed hook headers (dsp_it_macros_test.h → dsp_it_macros_test.h, master_it_macros_test.h → master_it_macros_test.h) | Hook headers, BUILD files, source includes |
| 2026-02-26 | Replaced string literals with enum identifiers (originally SIT_Shared_Name_T, later consolidated to Test_Interface_Name_T) | `dsp_integration_test.h`, `dsp_integration_test.c`, `master_integration_test.h`, `master_integration_test.c` |
| 2026-02-26 | Applied Aptiv C Coding Standards to all SWE5 files | All integration test headers |
| 2026-02-26 | Fixed SIT_IPC_Transfer_D2M_Send - was only sending test_result, now sends all 4 IPC fields | `dsp_integration_test.c` |
| 2026-02-26 | Fixed SIT_AF_Input_Receiver - added ALL missing RDD interface cases from Gen7_V2 migration (12 cases) | `dsp_integration_test.c` |
| 2026-02-26 | Fixed SIT_USC_Calib_Data_Provider - was empty placeholder, now implements all 19 USC calibration stub cases from Gen7_V2 | `master_integration_test.c` |
| 2026-02-26 | Fixed SIT_USC_Calib_Data_Provider field names for Gen8 - removed `_str` suffix from all USC fields (Gen7 naming), fixed array sizes 189→151, calmat3/4 → TEST_SKIPPED (Gen8 has only 2 cal sections) | `master_integration_test.c` |
| 2026-02-27 | Full codebase audit: corrected G_Pointer_T.name type, fixed file paths, added hook call line numbers, corrected linker status, updated code patterns to use enum | `SWE5_Framework_Reference.md` |
| 2026-02-27 | Added comprehensive Aptiv C Coding Standards section (ESGW_4-2_PE-SWx_00-01-A02_EN): file headers, section banners, function blocks, footers, header guards, extern "C", variable conventions | `SWE5_Framework_Reference.md` |
| 2026-02-27 | Removed duplicate SIT_Shared_Name_T enum — reuse Test_Interface_Name_T (unique Work Item IDs) for G_Pointer_T.name and Set_Shared_Ptr | `dsp_integration_test.h`, `dsp_integration_test.c`, `master_integration_test.h`, `master_integration_test.c`, `SWE5_Framework_Reference.md` |
| 2026-02-27 | Fixed Aptiv coding violations: duplicate section banner, missing `<stdint.h>`/`<stdbool.h>`, `Test_Interface_Dummy`→`Test_Interface_None`, `1e-5`→`1e-5F`, removed redundant `enum_val` from `Data_Ptr_T`, aligned union ordering, fixed function doc blocks (separator, Shared Variables, Change References), updated file descriptions | `master_integration_test.h`, `dsp_integration_test.h` |
| 2026-03-02 | Refactored BUILD files: Removed duplicate IT defines from 7 main library/binary targets (bbe32App, r52App, main_application_lib, dopplerproc_ifc_lib, ipc_dsp_lib, angle_finding_project_interface_lib, dsp_setup_lib). IT defines now propagate solely from dedicated IT libraries (dsp_it_macros_test_h, master_it_macros_test_h) via Bazel transitive deps. | `software/bbe32/BUILD`, `software/bbe32/src/BUILD`, `software/r52/BUILD`, `software/r52/dsp_setup/BUILD` |
| 2026-03-02 | Standardized all IT hook comments to format: `/* Providing <var> info, for Integration Testing */` | `doppler_process_ifc.c`, `anglefinding_project_interface.c`, `ipc_dsp.c`, `dsp_setup.c` |
| 2026-03-02 | Created `dsp_setup_integration_test_lib` — separate cc_library compiling same `dsp_setup.c` with IT defines enabled, keeping original `dsp_setup_lib` clean (no IT defines) | `software/r52/dsp_setup/BUILD` |
| 2026-03-02 | Created header-only targets `dsp_integration_test_h` and `master_integration_test_h` — expose `.h` files so `dsp_it_macros_test_h`/`master_it_macros_test_h` can resolve their `#include` of the implementation headers (even when guarded by `#if defined(Integration_Testing)`) | `software/bbe32/integration_test/BUILD`, `software/r52/integration_test/BUILD` |
| 2026-03-02 | Created 3 separate IT libraries in BBE32 src: `dopplerproc_ifc_integration_test_lib`, `angle_finding_project_interface_integration_test_lib`, `ipc_dsp_integration_test_lib` — each compiles the same source file with IT defines, same deps as original + conditional `dsp_integration_test_lib` dep | `software/bbe32/src/BUILD` |

### **Variable Section Attribute Patterns:**
```c
/* Zero-initialized → .sram1.bss */
static __attribute__((section(".sram1.bss"))) uint32_t my_var = 0U;

/* Non-zero initialized → .sram1.data */
static __attribute__((section(".sram1.data"))) uint32_t my_var = 1U;

/* Functions → .sram1.text */
static __attribute__((section(".sram1.text"))) void My_Function(void) { }
__attribute__((section(".sram1.text"))) void Public_Function(void) { }
```

### **String-to-Enum Memory Optimization:**
String literals (e.g., `"rdd1_bv"`) passed to `Set_Shared_Ptr()` consume const data memory (DTCM0) on BBE32.
Replace all string identifiers with `Test_Interface_Name_T` enum values defined via
`INTERFACE_ENUM_LIST` in `dsp_integration_test.h` and `master_integration_test.h`.
Each value uses a unique Work Item ID as its integer value.

```c
/* ⛔ WRONG - string literal wastes const data memory */
Set_Shared_Ptr("rdd1_bv", (void *)ptr, sizeof(*ptr), TYPE_UINT16);

/* ✅ CORRECT - enum uses no const data memory */
Set_Shared_Ptr(Ri_beam_vector, (void *)ptr, sizeof(*ptr), TYPE_UINT16);
```

- **Enum location:** Defined via `INTERFACE_ENUM_LIST` in `dsp_integration_test.h` and `master_integration_test.h`
- **When adding new interfaces:** Add a new entry to `INTERFACE_ENUM_LIST` in the appropriate header
- **G_Pointer_T.name:** Field type is `Test_Interface_Name_T` (not `const char *`)
- **Set_Shared_Ptr():** First parameter is `Test_Interface_Name_T name` (not `const char *name`)

---

## Critical Rules for SWE5 Implementation

### 🚫 **NEVER BREAK MAIN SOFTWARE CODE**
- **ALL SWE5 code MUST be guarded by `#if defined(Integration_Testing)`**
- **SWE5 should ONLY fetch pointers/data from main software flow - NEVER modify main behavior**
- **Main software must work identically with/without Integration_Testing enabled**
- **EXCEPTION: Centralized hook headers (`*_it_macros_test.h`) can be unconditionally included - they contain proper guards internally (see Section 16)**
- **Implementation headers (e.g., `master_integration_test.h`) MUST have conditional includes**
- **No SWE5 function calls in main execution paths without proper guards**
- **USE MACRO-BASED HOOKS FOR ALL SWE5 FUNCTION CALLS TO AVOID DEAD CODE AND TiCS IMPACTS** - Define macros at the top of files that expand to function calls when testing is enabled, otherwise to no-op. Avoid direct `#if` blocks around function calls in the code body, as this creates dead code flagged by static analysis tools like TiCS and can impact software metrics.
- **PREFERRED: Use centralized hook headers (Section 16) instead of inline macro definitions**

### ✅ **Correct SWE5 Implementation Pattern**

**With Centralized Hook Headers (Preferred - Section 16):**
```c
// ✅ BEST PRACTICE - Unconditional include of centralized hook header
// Hook header contains #ifdef guards internally providing empty macros when disabled
#include "master_it_macros_test.h"

// In code body:
SIT_M2D_ONETIME(data_ptr);  // Safe - expands to no-op when Integration_Testing disabled
```

**Legacy Pattern (Individual File Macros):**
```c
// ❌ WRONG - Unconditional include of implementation header breaks main code
#include "master_integration_test.h"

// ✅ CORRECT - Conditional include of implementation header
#if defined(Integration_Testing)
   #include "master_integration_test.h"
#endif

// ❌ WRONG - Unconditional function call
SIT_Do_Something();

// ❌ WRONG - Direct #if block around function call (creates dead code for TiCS)
#if defined(Integration_Testing) && defined(Anglefinding_IT)
   SIT_Do_Something();
#endif

// ✅ CORRECT - Macro-based hook (no-op when not testing, no dead code)
#if defined(Integration_Testing) && defined(Anglefinding_IT)
   #define SIT_HOOK(ptr) SIT_Do_Something(ptr)
#else
   #define SIT_HOOK(ptr) /* No operation */
#endif

// In code body:
SIT_HOOK(data_ptr);  // Safe call - expands to nothing when testing disabled
```

**⚠️ IMPORTANT:** Use centralized hook headers (Section 16) for all new SWE5 hooks. Legacy inline macros should be migrated to centralized headers.

### 🔧 **SWE5 Integration Points**

SWE5 hooks should be placed at **existing main software interfaces** to:
- **Fetch data pointers** from main structures
- **Inject test values** into data flows
- **Validate data propagation** between cores
- **Never modify main algorithm logic**

---

## 🤖 **AI ASSISTANT INSTRUCTIONS - READ FIRST**

### **Mandatory Build Validation**

**EVERY TIME** you make changes to SWE5 code (C files, headers, or BUILD files), you **MUST**:

1. **Build with Integration Testing flags:**
   ```bash
   bazelisk build //:gen8 --config=flr8 --Integration_Testing=true --Anglefinding_IT=true
   ```

2. **Validate both configurations:**
   ```bash
   # Without Integration Testing (main code must work)
   bazelisk build //:gen8 --config=flr8

   # With Integration Testing (SWE5 code must compile)
   bazelisk build //:gen8 --config=flr8 --Integration_Testing=true --Anglefinding_IT=true
   ```

3. **Self-Debug Build Failures:**
   - Read error messages carefully (compiler errors, linker errors, Bazel errors)
   - Common issues and solutions documented in Section 10.2
   - Check BUILD file patterns in Section 8.3, 8.4, 14, and 16.3
   - Verify header guards and conditional compilation
   - Ensure deps include hook header libraries conditionally
   - Check visibility of config_settings (must use `//:Integration_Testing_enabled` from root)

4. **Update This Document:**
   - If you discover new build patterns → Add to Section 14 (BUILD DEPENDENCY PATTERNS)
   - If you find new Bazel issues → Add to Section 10.2 (Common Issues)
   - If you create new hook patterns → Add to Section 16 (CENTRALIZED HOOK HEADERS)
   - If you learn conditional compilation tricks → Add to relevant sections
   - **Document your learnings so future sessions don't repeat the same debugging**

### **Bazel BUILD Knowledge Requirements**

You must understand and apply:

1. **Centralized IT Define Propagation (CURRENT ARCHITECTURE):**
   IT defines (`Integration_Testing`, `Anglefinding_IT`) are **ONLY** specified in the dedicated IT library targets (`dsp_it_macros_test_h`, `master_it_macros_test_h`, `dsp_integration_test_lib`, `master_integration_test`) **and** in the separate IT compilation libraries (`dsp_setup_integration_test_lib`, `dopplerproc_ifc_integration_test_lib`, `angle_finding_project_interface_integration_test_lib`, `ipc_dsp_integration_test_lib`). Main production library/binary targets **MUST NOT** duplicate these defines. See Section 14.4 for the separate IT library pattern.
   ```python
   # ❌ WRONG - Duplicating IT defines in main library
   cc_library(
       name = "my_main_lib",
       defines = select({
           "//:Integration_Testing_enabled": ["Integration_Testing"],
           "//conditions:default": [],
       }),
       deps = ["//software/bbe32/integration_test:dsp_it_macros_test_h"],  # Already provides defines!
   )

   # ✅ CORRECT - IT defines propagate from dsp_it_macros_test_h dep
   cc_library(
       name = "my_main_lib",
       # Note: Integration Testing defines (Integration_Testing, Anglefinding_IT) are
       # provided by the dedicated IT library (dsp_it_macros_test_h) via deps.
       deps = ["//software/bbe32/integration_test:dsp_it_macros_test_h"],
   )
   ```

2. **Conditional Dependencies Pattern:**
   ```python
   deps = [
       # Always required deps
       "//software/bbe32/integration_test:dsp_it_macros_test_h",  # Unconditional - provides defines + header
   ] + select({
       "//:Integration_Testing_enabled": [
           "//software/bbe32/integration_test:dsp_integration_test_lib",  # Conditional - implementation
       ],
       "//conditions:default": [],
   })
   ```

3. **Separate select() Statements Rule:**
   - **NEVER** combine flags in one config_setting_group
   - **ALWAYS** use separate select() for each flag
   - **WHY:** Visibility across package boundaries

4. **Header-Only Library Pattern:**
   ```python
   cc_library(
       name = "it_macros_test_h",
       hdrs = ["it_macros_test.h"],
       defines = select({...}),  # Conditional macros - SINGLE SOURCE OF TRUTH for IT defines
       deps = [],  # No deps for pure macro headers
       visibility = ["//visibility:public"],
   )
   ```

5. **alwayslink Pattern:**
   - Use `alwayslink = True` for integration test libraries
   - Ensures symbols are linked even if not directly referenced

### **Continuous Improvement Loop**

```
┌─────────────────────────────────────────────────────────────┐
│  1. User requests SWE5 change                               │
│  2. You implement changes (C code, headers, BUILD files)    │
│  3. You build with Integration Testing flags               │
│  4. Build fails? → Self-debug using this document          │
│  5. Build succeeds? → Validate without flags too           │
│  6. Document new learnings in this file                    │
│  7. Next user benefits from your documented knowledge      │
└─────────────────────────────────────────────────────────────┘
```

### **What to Document (Examples)**

✅ **DO Document:**
- "Learned that hook headers need separate BUILD targets for conditional inclusion"
- "Discovered that `strip_include_prefix = '.'` is required for integration_test directories"
- "Found that TiCS requires test headers to end with `_test.h`"
- "Identified that select() visibility requires `//:` prefix for root config_settings"

❌ **DON'T Document:**
- Temporary debugging steps that didn't lead to solutions
- User-specific environment issues
- Obvious facts already well-covered in the document

### **Build Failure Decision Tree**

```
Build fails?
├─ Compiler error?
│  ├─ "undefined reference" → Missing dep or conditional compilation issue
│  ├─ "no such file" → Missing include path or header not exposed in BUILD
│  ├─ "redefinition" → Duplicate macro definitions (migrate to hook headers!)
│  └─ Type mismatch → Check Gen7 vs Gen8 type differences (Section 2)
│
├─ Linker error?
│  ├─ "undefined symbol" → Missing alwayslink or conditional dep
│  ├─ "multiple definitions" → Check guards and conditional compilation
│  ├─ "region 'BBE32_DTCM0' overflowed" → See immediate fix below
│  └─ ".bss will not fit in region" → Move variables to SRAM1 sections
│
└─ Bazel error?
   ├─ "no such target" → Check BUILD file target names
   ├─ "package not found" → Check visibility and package structure
   └─ "no visible configs" → Use `//:` prefix for root config_settings
```

### **⚠️ CRITICAL: BBE32 Memory Overflow Handling**

When you encounter `region 'BBE32_DTCM0' overflowed by X bytes`:

**⛔ DO NOT MODIFY LINKER SCRIPTS OR MEMORY LAYOUT**
- Memory regions are shared with production code (F360 Tracker, etc.)
- Changing memory layout can break other features
- Use SRAM1 section attributes instead

**Root Causes (in order of likelihood):**
1. **Test variables without section attributes**: Default to DTCM0/ITCM - MUST use SRAM1
2. **Function-local static variables**: Go to default `.bss` (DTCM0) - MUST be moved to file scope
3. **IPC payload expansion**: `D2M_Payload_T` and `M2D_Payload_T` include `SIT_Data_T` when Integration_Testing enabled (~32 bytes each)

**IMMEDIATE FIX CHECKLIST:**
```
1. Check dsp_integration_test.c - ALL variables must have:
   - `.sram1.bss` for zero-initialized (SRAM1_BSS macro)
   - `.sram1.data` for non-zero initialized (SRAM1_DATA macro)

2. Check dsp_integration_test.c - ALL functions must have:
   - `.sram1.text` section attribute (SRAM1_TEXT macro)

3. Search for function-local static variables (CRITICAL!):
   grep -r "static.*=.*;" in function bodies
   → Move to file scope with section attribute

4. ⛔ DO NOT modify common.ld or linker/BUILD for memory expansion
```

**Correct Approach - Use SRAM1 Section Macros:**
```c
/* Define at top of dsp_integration_test.c */
#define SRAM1_BSS         __attribute__((section(".sram1.bss")))
#define SRAM1_DATA        __attribute__((section(".sram1.data")))
#define SRAM1_TEXT        __attribute__((section(".sram1.text")))
#define SRAM1_BSS_USED    __attribute__((section(".sram1.bss"), used))
#define SRAM1_TEXT_USED   __attribute__((section(".sram1.text"), used))

/* Usage */
static SRAM1_BSS uint32_t my_var = 0U;
static SRAM1_DATA uint32_t my_nonzero_var = 1U;
SRAM1_TEXT void My_Function(void) { }
```

---

### 📋 **Pre-Commit Checklist**

Before committing SWE5 changes:
- [ ] Build works without `--Integration_Testing=true` flag
- [ ] Main software behavior unchanged
- [ ] No SWE5 headers included unconditionally
- [ ] All SWE5 function calls use macro-based hooks (no direct `#if` blocks in code body to avoid dead code and TiCS impacts)
- [ ] BUILD files use `select()` for conditional dependencies
- [ ] No global variables declared outside Integration_Testing guards

### 📋 **SWE5 Implementation Checklist**

#### **Before Adding Any SWE5 Code:**
- [ ] **Build main code WITHOUT Integration_Testing flag**
- [ ] **Verify main functionality works correctly**
- [ ] **Document current behavior/values of variables being tested**

#### **During SWE5 Implementation:**
- [ ] **ALL SWE5 code guarded by `#if defined(Integration_Testing)`**
- [ ] **SWE5 functions ONLY read from main code structures**
- [ ] **NEVER modify main code variables or data flow**
- [ ] **Use macro-based hooks for main code integration**
- [ ] **Conditional BUILD dependencies using `select()`**

#### **After SWE5 Implementation:**
- [ ] **Build main code WITHOUT Integration_Testing flag**
- [ ] **Verify main functionality unchanged**
- [ ] **Build WITH Integration_Testing flag**
- [ ] **Verify SWE5 functionality works**
- [ ] **Test both builds on hardware if possible**

### 🔧 **Macro-Based Hook Pattern (REQUIRED)**

```c
// In main code file (e.g., dsp_setup.c)
#if defined(Integration_Testing) && defined(Anglefinding_IT)
#include "dsp_integration_test.h"
#define SIT_AF_RECEIVER(ptr) SIT_AF_Input_Receiver(ptr)
#else
#define SIT_AF_RECEIVER(ptr) /* No operation */
#endif

// In main function
void Some_Main_Function(const Data_T *data_ptr)
{
   // ... main code logic ...

   SIT_AF_RECEIVER(data_ptr);  // Safe hook - no-op when SWE5 disabled
}
```

### 🚫 **FORBIDDEN Patterns**

```c
// ❌ NEVER DO THIS - Direct #if blocks around function calls (creates dead code for TiCS)
void main_function(void)
{
   // Main code
   #if defined(Integration_Testing)
   SIT_Test_Function();  // DEAD CODE - flagged by TiCS!
   #endif
}

// ❌ NEVER DO THIS - Modifying main data
void SIT_Function(Main_Data_T *main_data)
{
   main_data->field = new_value;  // BREAKS MAIN CODE!
}

// ❌ NEVER DO THIS - Unconditional include
#include "swe5_header.h"  // BREAKS MAIN CODE!
```

### ✅ **ALLOWED Patterns**

```c
// ✅ CORRECT - Conditional include
#if defined(Integration_Testing)
#include "swe5_header.h"
#endif

// ✅ CORRECT - Read-only access to main data
void SIT_Function(const Main_Data_T *main_data)
{
   if (main_data != NULL)
   {
      // Read values for validation
      Validate_Scalar_Values(&main_data->field, DATATYPE_FLOAT32, expected);
   }
}

// ✅ CORRECT - Macro-based hook (preferred pattern from anglefinding_project_interface.c)
#if defined(Integration_Testing) && defined(Anglefinding_IT)
#include "dsp_integration_test.h"
#define SIT_AF_INPUT(ptr) SIT_AF_Input_Receiver(ptr)
#else
#define SIT_AF_INPUT(ptr) /* No operation */
#endif

// Alternative valid pattern:
#define SIT_HOOK(ptr) /* No-op by default */
#if defined(Integration_Testing)
#undef SIT_HOOK
#define SIT_HOOK(ptr) SIT_Test_Function(ptr)
#endif
```

### 🏗️ **BUILD File Requirements**

```bazel
# ✅ CORRECT - Conditional dependency with unconditional hook header
deps = [
    # ... main deps
    "//software/bbe32/integration_test:dsp_it_macros_test_h",  # Unconditional - provides IT defines + header
] + select({
    "//:Integration_Testing_enabled": ["//software/bbe32/integration_test:dsp_integration_test_lib"],
    "//conditions:default": [],
}),

# ❌ WRONG - Duplicating IT defines (they propagate from dsp_it_macros_test_h)
defines = select({
    "//:Integration_Testing_enabled": ["Integration_Testing"],
    "//conditions:default": [],
}),
# IT defines are provided by dsp_it_macros_test_h / master_it_macros_test_h via transitive deps.
# DO NOT duplicate them in main library or binary targets.
```

### 📝 **Standard Comment Format for IT Hook Calls**

All integration testing hook macro calls in caller-side source files **MUST** use this comment format:

```c
/* Providing <variable_name> info, for Integration Testing */
SIT_MACRO_NAME(variable_ptr);
```

**Examples from codebase:**
```c
/* Providing m2d_one_time_init_ptr info, for Integration Testing */
SIT_M2D_ONETIME((M2D_One_Time_Msg_T *)m2d_one_time_init_ptr);

/* Providing M2D_msg_ptr info, for Integration Testing */
SIT_M2D_PERLOOK((M2D_Payload_T *)&m2d_msg_ptr->payload);

/* Providing d2m_msg_ptr info, for Integration Testing */
SIT_D2M_MSG_BUFFER(d2m_msg_ptr);

/* Providing af_input_data_ptr info, for Integration Testing */
SIT_AF_INPUT(af_input_data_ptr);

/* Providing D2M payload info, for Integration Testing */
SIT_IPC_TRANSFER_D2M_SEND((D2M_Payload_T *)&d2m_msg_ptr->payload);

/* Providing M2D payload info, for Integration Testing */
SIT_IPC_TRANSFER_M2D_RECEIVE((const M2D_Payload_T *)&m2d_msg_ptr->payload);

/* Providing M2D payload info for IPC M2D send, for Integration Testing */
SIT_IPC_TRANSFER_M2D_SEND((M2D_Payload_T *)&m2d_msg_ptr->payload);

/* Providing D2M payload info for IPC D2M receive, for Integration Testing */
SIT_IPC_TRANSFER_D2M_RECEIVE((const D2M_Payload_T *)&IPC_Sp_Post_Proc_Buf_Read_Addr->payload);
```

**⛔ FORBIDDEN comment styles:**
```c
/* SWE5 Integration Test Hook - Provider: Stub RDD data for Angle Finding input */  // ❌ Too verbose
/*Passing address to verify Integration Testing; only runs when ...*/                // ❌ Non-standard
/* SWE5 Integration Testing: D2M send hook */                                        // ❌ Missing variable name
/* To send IT Data from M2D, for Integration Testing */                              // ❌ Missing variable name
```

### � **NO-DUPLICATION RULES (CRITICAL - REVIEW-BLOCKING)**

> **These rules are MANDATORY. Violations WILL be caught in code review and WILL block submission.**

1. **NEVER create a new enum/type that duplicates an existing one.** If a type already uniquely identifies something, REUSE it. Do not create a parallel enum with different names for the same concepts.
   - ❌ `SIT_Shared_Name_T` (auto-incrementing 0,1,2...) duplicating `Test_Interface_Name_T` (unique Work Item IDs)
   - ✅ Use `Test_Interface_Name_T` everywhere — Work Item IDs are already unique across all interfaces

2. **NEVER add redundant union members.** If `uint8_t *u8` already exists in `Data_Ptr_T`, do NOT add `uint8_t *enum_val` — it's the same type, wastes nothing but adds confusion.

3. **Both header files MUST stay in sync.** `DataType_T`, `Data_Ptr_T`, and `G_Pointer_T` definitions must be IDENTICAL between `dsp_integration_test.h` and `master_integration_test.h` (member names, ordering, types).

4. **Enum naming must be consistent across cores.** Both headers must use `Test_Interface_None = 0U` as the default entry — NOT different names like `Test_Interface_Dummy`.

5. **Before adding ANY new type or enum, search the entire codebase** for existing types that serve the same purpose. Prefer reuse over creation.

### 🔴 **APTIV CODING STANDARD COMPLIANCE CHECKLIST (CRITICAL - REVIEW-BLOCKING)**

> **Run this checklist EVERY TIME before committing. Violations here result in review rejection.**

| # | Rule | How to Check |
|---|------|-------------|
| 1 | **All `#define` constants use `U` suffix for unsigned** | `0U`, `1U`, not `0`, `1` |
| 2 | **Float constants use `F` suffix** | `1e-5F`, `0.0F`, not `1e-5`, `0.0` |
| 3 | **No duplicate section banners** | Each banner ("Local Preprocessor #define MACROS", etc.) appears EXACTLY once |
| 4 | **Standard headers explicitly included** | `<stdint.h>`, `<stdbool.h>` in every file that uses their types |
| 5 | **Function doc blocks have ALL required sections** | `Name:`, `Shared Variables:`, `Parameters:`, `Return Value:`, `Design Information:`, `Change References:` |
| 6 | **Function doc block separator line is full width** | `/*****...78 asterisks...**/` — NOT shorter |
| 7 | **Types/unions identical between both headers** | `Data_Ptr_T`, `DataType_T`, `G_Pointer_T` must match exactly |
| 8 | **No redundant/dead code** | No unused union members, no duplicate enums, no unreachable code |
| 9 | **File description accurate** | Must reflect actual content — update after struct/enum changes |
| 10 | **Revision history updated** | Every change gets a new entry with date, revision, AID, JIRA |

### 🔍 **Common Mistakes to Avoid**

1. **Unconditional header includes** in main source files
2. **Unguarded function calls** in main execution paths
3. **Global variables** declared outside `#if defined(Integration_Testing)`
4. **Modifying main algorithm logic** for testing purposes
5. **Hard dependencies** on SWE5 libraries in main BUILD files
6. **Creating duplicate enums/types** instead of reusing existing ones (see NO-DUPLICATION RULES)
7. **Inconsistent types between DSP and Master headers** (member ordering, naming, count)
8. **Missing `U`/`F` suffixes** on numeric constants
9. **Missing standard header includes** (relying on transitive includes)
10. **Copy-pasting sections** without checking for duplicates (e.g. duplicate section banners)
11. **Duplicating IT defines** in main library/binary targets — IT defines propagate from `dsp_it_macros_test_h` / `master_it_macros_test_h` deps
12. **Using non-standard IT hook comments** — use format: `/* Providing <var> info, for Integration Testing */`

### 📖 **SWE5 Testing Guide**

See [software/SWE5_Testing_Guide.md](software/SWE5_Testing_Guide.md) for complete testing procedures.

---

## Implementation Details

### Aptiv C Coding Standards (ESGW_4-2_PE-SWx_00-01-A02_EN)

All SWE5 files MUST follow the Aptiv C Coding Standards. This section documents the exact templates and patterns used in the current codebase.

#### File Header Template (Required for ALL .c and .h files)

```c
/*===========================================================================*/
/**
 * @file <filename>.c  (or .h)
 *
 * <Brief one-line description>
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitive Business - Restricted Aptiv information. Do not disclose.
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *   <Multi-line detailed description of the file's purpose>
 *
 * @section ABBR ABBREVIATIONS:
 *   - IPC  - Inter-Processor Communication
 *   - SWE5 - Software Engineering Level 5 (Integration Testing)
 *   - SIT  - Software Integration Testing
 *   - <Add module-specific abbreviations>
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - SWE5 Integration Test Framework Design
 *
 *   - Requirements Document(s):
 *     - Integration test framework for Gen8 radar system
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - ESGW_4-2_PE-SWx_00-01-A02_EN - C Coding Standards [20120506]
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None.
 *
 * @defgroup <group_name> <Group Display Name>
 * @{
 */
/*==========================================================================*/
```

#### Section Banner Comments (Required in source files)

Sections MUST appear in this order with these exact banner comments:

```c
/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/

/*===========================================================================*
 * Local Preprocessor #define Constants
 *===========================================================================*/

/*===========================================================================*
 * Local Preprocessor #define MACROS
 *===========================================================================*/

/*===========================================================================*
 * Local Type Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Object Definitions
 *===========================================================================*/

/*===========================================================================*
 * Local Const Object Definitions
 *===========================================================================*/

/*===========================================================================*
 * Local Object Definitions
 *===========================================================================*/

/*===========================================================================*
 * Local Function Prototypes
 *===========================================================================*/

/*===========================================================================*
 * Local Inline Function Definitions and Function-Like Macros
 *===========================================================================*/

/*===========================================================================*
 * Exported Const Object Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Function Prototypes
 *===========================================================================*/

/*===========================================================================*
 * Exported Inline Function Definitions and #define Function-Like Macros
 *===========================================================================*/
```

#### Header File Section Banners (Required in header files)

Headers use slightly different sections. Declaration sections include:

```c
/*===========================================================================*
 * Exported Type Declarations (Shared - outside Integration_Testing guard)
 *===========================================================================*/

/*===========================================================================*
 * Global Variable Declarations (external linkage)
 *===========================================================================*/

/*===========================================================================*
 * Function Prototypes
 *===========================================================================*/
```

#### Function Documentation Block (Required for ALL functions)

```c
/******************************************************************************
 * Name:  <FunctionName>
 *   <Brief description of what the function does.>
 *
 * Shared Variables: <list of module-level variables accessed, or "none">
 *
 * Parameters: <param1> - <description>
 *             <param2> - <description>
 *
 * Return Value: <description, or "none">
 *
 * Design Information:
 *  - <Where/when this function is called>
 *
 *  Change References:
 *  - <Change description or "Initial implementation">
 *
 ******************************************************************************/
```

**Example from actual code (master_integration_test.c):**
```c
/******************************************************************************
 * Name:  SIT_IPC_Transfer_M2D_Send
 *   Send test inputs from R52 to BBE32 via M2D IPC buffer.
 *
 * Shared Variables: Test_Iface_Num, Stub_Val,
 *                   Stub_Len, Master_Res
 *
 * Parameters: m2d_payload_ptr - Pointer to M2D payload structure
 *
 * Return Value: none
 *
 * Design Information:
 *  - Called from dsp_setup.c before M2D header update
 *
 *  Change References:
 *  - Initial implementation
 *
 ******************************************************************************/
void SIT_IPC_Transfer_M2D_Send(M2D_Payload_T *m2d_payload_ptr)
{
   /* ... */
}
```

#### File Footer Template (Required for ALL .c and .h files)

```c
/** @} doxygen end group */

/*============================================================================*\
 * AUTHOR(S) IDENTITY (AID)
 *-----------------------------------------------------------------------------
 *
 *  AID         NAME
 *  ---------------------------------------------------------------------------
 *  JSD         Jonnalagadda Sai Dheeraj
\*============================================================================*/

/*============================================================================*\
 * FILE REVISION HISTORY
 *-----------------------------------------------------------------------------
 *
 *  DATE          REVISION         AID          JIRA Ticket ID
 *      SUMMARY OF CHANGES
 *  ---------------------------------------------------------------------------
 *  YY-MM-DD      1.0              JSD          EAH-XXXX
 *      Initial creation - <brief description>
 *  ---------------------------------------------------------------------------
\*============================================================================*/

/* END OF FILE -------------------------------------------------------------- */
```

#### Variable and Comment Conventions

| Convention | Example |
|------------|---------|
| **Module-scope statics** | `static uint32_t Master_Test_Iface_Num = 0U;` |
| **Zero-init suffix** | Always use `0U`, `0.0F`, not bare `0` or `0.0` |
| **Float literal suffix** | Always use `F` suffix: `1e-5F`, `0.0F`, `1.0F` (CRITICAL - prevents implicit double promotion) |
| **Unsigned literal suffix** | Always use `U` suffix: `0U`, `1U`, `151U` (CRITICAL - required by MISRA) |
| **Volatile + used** for debugger globals | `volatile __attribute__((used)) G_Pointer_T Master_Shared_Pi;` |
| **Inline comment for variables** | `static uint32_t Stub_Len = 1U; /* Global Test Stub Length User Input */` |
| **Brace style** | Allman style (opening brace on own line for functions) |
| **If/else blocks** | Always use braces, even for single statements |
| **Empty else** | `else { /* Invalid condition - do nothing */ }` |
| **Guard comments** | `#endif /* Integration_Testing */` (comment matches `#if`) |
| **Switch default** | Always present, even if just `break;` |
| **NULL checks** | Check pointer parameters at function entry |
| **Cast style** | C-style casts: `(uint16_t *)src_ptr` |
| **No duplicate types** | NEVER create a new enum/union member that duplicates an existing one |
| **Cross-core consistency** | Types shared between headers MUST be identical in both |

#### Header Guard Convention

```c
/* Header files: */
#ifndef MASTER_INTEGRATION_TEST_H
#define MASTER_INTEGRATION_TEST_H
/* ... content ... */
#endif /* MASTER_INTEGRATION_TEST_H */

/* Naming: uppercase filename with underscores */
/* master_integration_test.h → MASTER_INTEGRATION_TEST_H */
/* dsp_it_macros_test.h → DSP_IT_MACROS_TEST_H */
```

#### extern "C" Guard (Required for headers that may be included by C++)

```c
#ifdef __cplusplus
extern "C"
{ /* ! Inclusion of header files should NOT be inside the extern "C" block */
#endif /* __cplusplus */

/* ... declarations ... */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
```

> **Note:** The comment `/* ! Inclusion of header files should NOT be inside the extern "C" block */` is part of the Aptiv standard template.

---

### Header File Structure (Full Aptiv Template)

```c
/*===========================================================================*/
/**
 * @file <module>_integration_test.h
 *
 * <Brief description of the integration test header>
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitive Business - Restricted Aptiv information. Do not disclose.
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * @section ABBR ABBREVIATIONS:
 * @section TRACE TRACEABILITY INFO:
 *   - Applicable Standards:
 *     - ESGW_4-2_PE-SWx_00-01-A02_EN - C Coding Standards [20120506]
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None.
 *
 * @defgroup <module>_integration_test <Module> Integration Test
 * @{
 */
/*==========================================================================*/

#ifndef <MODULE>_INTEGRATION_TEST_H
#define <MODULE>_INTEGRATION_TEST_H

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include <stdint.h>
#include <stdbool.h>

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
/* NOTE: Includes go OUTSIDE the extern "C" block */

#ifdef __cplusplus
extern "C"
{ /* ! Inclusion of header files should NOT be inside the extern "C" block */
#endif /* __cplusplus */

/*===========================================================================*
 * Exported Type Declarations (Shared - outside Integration_Testing guard)
 *===========================================================================*/
/* Types needed by both production and test code go here */

#if defined(Integration_Testing)

/*===========================================================================*
 * Local Preprocessor #define Constants
 *===========================================================================*/

/*===========================================================================*
 * Local Preprocessor #define MACROS
 *===========================================================================*/

/*===========================================================================*
 * Exported Type Declarations
 *===========================================================================*/

/*===========================================================================*
 * Global Variable Declarations
 *===========================================================================*/

/*===========================================================================*
 * Function Prototypes
 *===========================================================================*/

#endif /* Integration_Testing */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* <MODULE>_INTEGRATION_TEST_H */

/** @} doxygen end group */

/*============================================================================*\
 * AUTHOR(S) IDENTITY (AID)
 * ... (see File Footer Template in Aptiv Coding Standards section above)
\*============================================================================*/

/*============================================================================*\
 * FILE REVISION HISTORY
 * ...
\*============================================================================*/

/* END OF FILE -------------------------------------------------------------- */
```

### Source File Structure (Full Aptiv Template)

```c
/*===========================================================================*/
/**
 * @file <module>_integration_test.c
 *
 * <Brief description>
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitive Business - Restricted Aptiv information. Do not disclose.
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * @section ABBR ABBREVIATIONS:
 * @section TRACE TRACEABILITY INFO:
 *   - Applicable Standards:
 *     - ESGW_4-2_PE-SWx_00-01-A02_EN - C Coding Standards [20120506]
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None.
 *
 * @defgroup <module>_integration_test <Module> Integration Test
 * @{
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include <stdint.h>
#include <string.h>

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "<module>_integration_test.h"

#if defined(Integration_Testing)

/*===========================================================================*
 * Local Preprocessor #define Constants
 *===========================================================================*/

/*===========================================================================*
 * Local Preprocessor #define MACROS
 *===========================================================================*/

/*===========================================================================*
 * Local Type Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Object Definitions
 *===========================================================================*/

/*===========================================================================*
 * Local Const Object Definitions
 *===========================================================================*/

/*===========================================================================*
 * Local Object Definitions
 *===========================================================================*/
static uint32_t Test_Iface_Num = 0U;
static uint32_t Stub_Val      = 0U;
static uint32_t Stub_Len      = 1U;

/*===========================================================================*
 * Local Function Prototypes
 *===========================================================================*/

/*===========================================================================*
 * Local Inline Function Definitions and Function-Like Macros
 *===========================================================================*/

/******************************************************************************
 * Name:  <FunctionName>
 *   <Brief description>
 *
 * Shared Variables: <variables accessed>
 * Parameters: <param> - <description>
 * Return Value: none
 *
 * Design Information:
 *  - <When/where called>
 *
 *  Change References:
 *  - Initial implementation
 *
 ******************************************************************************/
void <FunctionName>(void)
{
   /* Implementation */
}

#endif /* Integration_Testing */

/** @} doxygen end group */

/*============================================================================*\
 * AUTHOR(S) IDENTITY (AID)
 * ... (see File Footer Template)
\*============================================================================*/

/*============================================================================*\
 * FILE REVISION HISTORY
 * ...
\*============================================================================*/

/* END OF FILE -------------------------------------------------------------- */
```

### Main Code Integration

```c
// In main software files - CORRECT MACRO-BASED PATTERN
#if defined(Integration_Testing) && defined(Anglefinding_IT)
#include "dsp_integration_test.h"
#define SIT_AF_INPUT(ptr) SIT_AF_Input_Receiver(ptr)
#else
#define SIT_AF_INPUT(ptr) /* No operation */
#endif

void Main_Function(void)
{
    // Main logic here - NEVER TOUCH THIS

    SIT_AF_INPUT(data_ptr);  // Safe hook - no-op when SWE5 disabled
}
```

**❌ WRONG - Direct #if blocks create dead code for TiCS:**
```c
void Main_Function(void)
{
    // Main logic here - NEVER TOUCH THIS

    #if defined(Integration_Testing)
    // SWE5 hooks here - only for testing (DEAD CODE!)
    SIT_Test_Hook();
    #endif
}
```

---

## Revision History

| Date | Author | Description |
|------|--------|-------------|
| 2026-02-02 | Dheeraj | Added critical rules to prevent main code breakage |
| 2026-02-06 | Dheeraj | Updated BUILD patterns to use separate Integration_Testing_enabled and ANGLEFINDING_enabled config settings |
| 2026-02-17 | Dheeraj | Added centralized hook headers (master_it_macros_test.h, dsp_it_macros_test.h) to eliminate macro duplication |
| 2026-02-17 | Dheeraj | Added AI Assistant Instructions for mandatory build validation, self-debugging, and continuous documentation updates |
| 2026-02-17 | Dheeraj | Implemented two-tier conditional BUILD dependency structure: hook headers unconditional, implementation libraries conditional |
| 2026-02-17 | Dheeraj | Updated Section 16.3-16.5: Added BUILD patterns, unconditional include guidance, common pitfalls, and net savings metrics |
| 2026-02-19 | Dheeraj | Added Section 10.4: BBE32 Memory Management - SRAM1 placement for variables/functions to prevent DTCM0/ITCM overflow |
| 2026-02-19 | Dheeraj | CRITICAL FIX: Documented function-local static variable issue causing DTCM0 overflow - must move to file scope |
| 2026-02-19 | Dheeraj | ~~LINKER FIX: REVERTED~~ - DO NOT modify memory layout; use SRAM1 section attributes only |
| 2026-02-19 | Dheeraj | ~~BUILD FIX: REVERTED~~ - DO NOT modify linker/BUILD; use SRAM1 section attributes only |
| 2026-02-19 | Dheeraj | ENHANCED AI INSTRUCTIONS: Added Quick Reference section with complete fix checklist, file mapping, and section attribute patterns |
| 2026-02-19 | Dheeraj | Added Build Failure Decision Tree with IMMEDIATE FIX CHECKLIST for DTCM0 overflow |
| 2026-02-19 | Dheeraj | Added revision history to: dsp_it_macros_test.h, master_it_macros_test.h, dsp_integration_test.c/h |
| 2026-02-20 | Dheeraj | POLICY UPDATE: Memory layout must NOT be modified for integration testing - use SRAM1 section attributes only |
| 2026-02-26 | Dheeraj | Fixed SIT_USC_Calib_Data_Provider Gen7→Gen8 field naming: removed `_str` suffix, fixed array sizes 189→151, calmat3/4 → TEST_SKIPPED |
| 2026-02-27 | AI Audit | Full codebase audit: updated G_Pointer_T.name type (const char* → Test_Interface_Name_T), fixed anglefinding_project_interface.c path, added function defined-in/called-from columns, corrected Section 10.4.7 linker status (NOT implemented), updated all Set_Shared_Ptr patterns to use enum, added file sizes to Appendix B, fixed Azimuth LUT size 189→151 |
| 2026-02-27 | AI Audit | Added NO-DUPLICATION RULES and APTIV CODING STANDARD COMPLIANCE CHECKLIST to Quick Reference. Fixed master_integration_test.h: duplicate section banner, missing std headers, enum naming, tolerance suffix, Data_Ptr_T alignment, function doc blocks |



## Table of Contents
0. [AI ASSISTANT INSTRUCTIONS - READ FIRST](#-ai-assistant-instructions---read-first)
1. [Framework Overview](#1-framework-overview)
2. [Architecture Comparison: Gen7_V2 vs Gen8](#2-architecture-comparison)
3. [Core Components](#3-core-components)
4. [Data Flow Diagram](#4-data-flow-diagram)
5. [File Structure & Files Changed](#5-file-structure--files-changed)
6. [Interface Enumeration (Work Items)](#6-interface-enumeration)
7. [Function Reference](#7-function-reference)
8. [Build Configuration](#8-build-configuration)
   - [8.1 Bazel Flags](#81-bazel-flags)
   - [8.2 Preprocessor Macros](#82-preprocessor-macros)
   - [8.3 BUILD File Pattern](#83-build-file-pattern)
   - [8.4 Conditional Dependencies in Parent BUILD Files](#84-conditional-dependencies-in-parent-build-files)
   - [8.5 Config Setting Visibility Fix](#85-config-setting-visibility-fix)
9. [Test Execution Flow](#9-test-execution-flow)
10. [Debugging Guide](#10-debugging-guide)
11. [CODE GENERATION PATTERNS](#11-code-generation-patterns)
12. [ADDING NEW INTERFACES - STEP BY STEP](#12-adding-new-interfaces---step-by-step)
13. [VALIDATION & BOUNDARY CHECKING PATTERNS](#13-validation--boundary-checking-patterns)
14. [BUILD DEPENDENCY PATTERNS](#14-build-dependency-patterns)
   - [14.1 Conditional Dependency Pattern](#141-conditional-dependency-pattern)
   - [14.2 Header Library Pattern](#142-header-library-pattern)
   - [14.3 Adding Integration Test Dependency to Source](#143-adding-integration-test-dependency-to-source)
   - [14.4 Separate IT Library Pattern (Parallel Compilation)](#144-separate-it-library-pattern-parallel-compilation)
   - [14.5 Header-Only Targets for Implementation Headers](#145-header-only-targets-for-implementation-headers)
15. [HOOK INTEGRATION PATTERNS](#15-hook-integration-patterns)
16. [CENTRALIZED HOOK HEADERS](#16-centralized-hook-headers)
   - [16.1 Architecture Overview](#161-architecture-overview)
   - [16.2 Hook Header File Structure](#162-hook-header-file-structure)
   - [16.3 BUILD File Pattern for Hook Headers](#163-build-file-pattern-for-hook-headers)
   - [16.4 Usage in Source Files](#164-usage-in-source-files)
   - [16.5 Benefits and Best Practices](#165-benefits-and-best-practices)

---

## 1. Framework Overview

### What is SWE5?
SWE5 (Software Engineering Level 5) Integration Testing validates data flow between radar processing modules by:
- **Stubbing**: Injecting known test values at Provider points
- **Validation**: Verifying received values match expected values at Receiver points
- **IPC Verification**: Confirming correct inter-processor communication

### Source Commits (Gen7_V2)
The framework originates from two Gen7_V2 commits:
1. `3e6f42f431af8d793b98ee23c183d24183ac068f` - Initial framework implementation (28 files, +3389 lines)
2. `9ed47bef404815ebdd9ece215081cdb0284c8a00` - Build fix with IF_ANGLEFINDING() guards

---

## 2. Architecture Comparison

| Component | Gen7_V2 (SAF85xx) | Gen8 (iND13400) | Notes |
|-----------|-------------------|-----------------|-------|
| **Master Core** | M7 (Cortex-M7) | R52 (Cortex-R52) | Controls radar operation, IPC orchestration |
| **DSP Core** | BBE32 (Tensilica) | BBE32 (Tensilica) | Signal processing, RDD, Angle Finding |
| **Post-Proc Core** | A53 (Cortex-A53) | ❌ Not Present | Gen8 does all post-proc on R52/BBE32 |
| **IPC Flow** | M7 ↔ BBE32 ↔ A53 | R52 ↔ BBE32 | Simplified without A53 |
| **Master Path** | `software/m7/` | `software/r52/` | Directory mapping |
| **D2A Functions** | Required | Not Needed | No A53 in Gen8 |
| **AF_CAL_SEC_N** | 4 | 2 | Calibration section count differs |

### Key Type Differences (Gen7_V2 vs Gen8)
| Gen7_V2 Type | Gen8 Type | Notes |
|--------------|-----------|-------|
| `anglefinding_input_data_T` | `Angle_Finding_Input_T` | AF input structure |
| `af_detect_output_gen8_T` | `Angle_Finding_Output_T` | AF output structure |
| `hostVehSpeed` | `p_veh_speed` (pointer) | Vehicle speed access |
| `radarPosition` | `p_radar_position` (pointer) | Radar position access |
| `det_az_rad[]` | `afbb_det_output.af_data.theta[]` | Azimuth output |
| `det_el_rad[]` | `afbb_det_output.af_data.phi[]` | Elevation output |
| `k_calmat1_az_sin_lut_str[189]` | `k_calmat1_az_sin_lut[151][5]` | USC AF cal LUT - Gen7 uses `_str` suffix, Gen8 does not; array dimensions differ |
| `k_calmat1_lut_az_str[189]` | `k_calmat1_lut_az[151]` | USC AF az LUT - Gen7 uses `_str` suffix and flat 189, Gen8 uses 151 |
| `k_calmat1_Az_search_range_str` | `k_calmat1_az_search_range` | USC AF search range - Gen7 has `_str` suffix and capital `A`, Gen8 lowercase |
| `USC_AF_Cal_T` (calmat1-4) | `USC_AF_Cal_T` (calmat1-2 only) | Gen8 only has calmat1/calmat2 fields; calmat3/4 do not exist (AF_CAL_SEC_N=2) |
| `AF_USC_T.cal_ang_min_az[0..3]` | `AF_USC_T.cal_ang_min_az[0..1]` | BBE32 uses array access; R52 uses named fields `k_calmat1_ang_min_az` |

---

## 3. Core Components

### 3.1 IPC Data Structure (ipc_data.h, lines 226–241)
```c
/* Test result enumeration (line 226) */
typedef enum {
   TEST_NOT_PERFORMED = 0,  /* Test not yet executed */
   TEST_INTERFACE_INVALID,  /* Invalid interface number */
   TEST_PASSED,             /* Validation successful */
   TEST_FAILED,             /* Validation failed */
   TEST_SKIPPED             /* Interface not applicable (e.g., calmat3/4 on Gen8) */
} SIT_Test_Result_T;

/* IPC transfer structure for test data */
typedef struct SIT_Data_Tag {
   double stub_value;                     /* Value to inject/validate */
   uint32_t test_interface_number;        /* Work Item ID (from INTERFACE_ENUM_LIST) */
   uint32_t no_of_elements_in_stub_array; /* Array size for batch operations */
   SIT_Test_Result_T test_result;         /* Test outcome */
} __attribute__((aligned(32))) SIT_Data_T;
```

### 3.2 Data Type Enumeration
```c
typedef enum {
   DATATYPE_UINT8 = 0U,   /* 8-bit unsigned */
   DATATYPE_UINT16,       /* 16-bit unsigned */
   DATATYPE_UINT32,       /* 32-bit unsigned */
   DATATYPE_INT16,        /* 16-bit signed */
   DATATYPE_INT32,        /* 32-bit signed */
   DATATYPE_FLOAT32,      /* 32-bit float */
   DATATYPE_ENUM          /* Enumeration (treated as uint8_t) */
} DataType_T;
```

### 3.3 Global Pointer Structure (Debugger Visibility)
```c
typedef struct {
   Data_Ptr_T data;            /* Union of typed pointers */
   size_t elem_size;            /* Element size in bytes */
   size_t no_of_elements;       /* Array element count */
   Test_Interface_Name_T name;   /* Enum identifying variable for debugger */
   DataType_T data_type;        /* Type for interpretation */
} G_Pointer_T;

/* Global instances for debugger inspection */
volatile G_Pointer_T Master_Shared_Pi;  /* R52 Provider pointer */
volatile G_Pointer_T Master_Shared_Ri;  /* R52 Receiver pointer */
volatile G_Pointer_T DSP_Shared_Pi;     /* BBE32 Provider pointer */
volatile G_Pointer_T DSP_Shared_Ri;     /* BBE32 Receiver pointer */
```

---

## 4. Data Flow Diagram

```
┌─────────────────────────────────────────────────────────────────────────────────┐
│                          SWE5 Integration Test Data Flow                         │
├─────────────────────────────────────────────────────────────────────────────────┤
│                                                                                  │
│   DEBUGGER                                                                       │
│      │                                                                           │
│      ▼ Set: Test_Iface_Num, Stub_Val, Stub_Len, SIT_Run=true                    │
│                                                                                  │
│   ┌──────────────────────────────────────────────────────────────────────────┐  │
│   │                           R52 MASTER CORE                                 │  │
│   │                                                                           │  │
│   │  ┌─────────────────────┐    ┌─────────────────────┐                      │  │
│   │  │ SIT_M2D_Onetime_    │    │ SIT_M2D_Perlook_    │                      │  │
│   │  │ Provider()          │    │ Provider()          │                      │  │
│   │  │ - Stubs radar_pos   │    │ - Stubs veh_speed   │                      │  │
│   │  └──────────┬──────────┘    └──────────┬──────────┘                      │  │
│   │             │                          │                                  │  │
│   │             ▼                          ▼                                  │  │
│   │  ┌─────────────────────────────────────────────────┐                      │  │
│   │  │ SIT_IPC_Transfer_M2D_Send()                     │                      │  │
│   │  │ - Copies Test_Iface_Num, Stub_Val, Stub_Len    │                      │  │
│   │  │ - Into M2D_Payload.sit_data                     │                      │  │
│   │  └──────────────────────┬──────────────────────────┘                      │  │
│   └─────────────────────────┼────────────────────────────────────────────────┘  │
│                             │ M2D IPC                                            │
│                             ▼                                                    │
│   ┌──────────────────────────────────────────────────────────────────────────┐  │
│   │                           BBE32 DSP CORE                                  │  │
│   │                                                                           │  │
│   │  ┌─────────────────────────────────────────────────┐                      │  │
│   │  │ SIT_IPC_Transfer_M2D_Receive()                  │                      │  │
│   │  │ - Extracts test parameters from M2D payload     │                      │  │
│   │  │ - Stores in DSP_Test_Iface_Num, DSP_Stub_Val   │                      │  │
│   │  └──────────────────────┬──────────────────────────┘                      │  │
│   │                         │                                                  │  │
│   │                         ▼                                                  │  │
│   │  ┌─────────────────────────────────────────────────┐                      │  │
│   │  │ SIT_D2M_Msg_Buffer_Provider()                   │                      │  │
│   │  │ - Called in Appl_Doppler_Rdd_Processing()       │                      │  │
│   │  │ - Stubs RDD data: beam_vector, snr, range, etc  │                      │  │
│   │  └──────────────────────┬──────────────────────────┘                      │  │
│   │                         │                                                  │  │
│   │                         ▼                                                  │  │
│   │  ┌─────────────────────────────────────────────────┐                      │  │
│   │  │ SIT_AF_Input_Receiver()                         │                      │  │
│   │  │ - Called in Angle_Finding_Process_Init()        │                      │  │
│   │  │ - Validates: p_veh_speed, p_radar_position      │                      │  │
│   │  │ - Sets DSP_Res = TEST_PASSED/FAILED             │                      │  │
│   │  └──────────────────────┬──────────────────────────┘                      │  │
│   │                         │                                                  │  │
│   │                         ▼                                                  │  │
│   │  ┌─────────────────────────────────────────────────┐                      │  │
│   │  │ SIT_AF_USC_Receiver()                           │                      │  │
│   │  │ - Validates USC calibration parameters          │                      │  │
│   │  │ - az_sin_lut, az_lut, cal_ang_min/max_az/el    │                      │  │
│   │  └──────────────────────┬──────────────────────────┘                      │  │
│   │                         │                                                  │  │
│   │                         ▼                                                  │  │
│   │  ┌─────────────────────────────────────────────────┐                      │  │
│   │  │ SIT_IPC_Transfer_D2M_Send()                     │                      │  │
│   │  │ - Copies DSP_Res into D2M_Payload.sit_data     │                      │  │
│   │  └──────────────────────┬──────────────────────────┘                      │  │
│   └─────────────────────────┼────────────────────────────────────────────────┘  │
│                             │ D2M IPC                                            │
│                             ▼                                                    │
│   ┌──────────────────────────────────────────────────────────────────────────┐  │
│   │                           R52 MASTER CORE                                 │  │
│   │                                                                           │  │
│   │  ┌─────────────────────────────────────────────────┐                      │  │
│   │  │ SIT_IPC_Transfer_D2M_Receive()                  │                      │  │
│   │  │ - Extracts test_result from D2M payload         │                      │  │
│   │  │ - Updates DSP_Final_Res for debugger view       │                      │  │
│   │  └─────────────────────────────────────────────────┘                      │  │
│   └──────────────────────────────────────────────────────────────────────────┘  │
│                                                                                  │
│   DEBUGGER                                                                       │
│      │                                                                           │
│      ▼ Read: DSP_Final_Res, Master_Final_Res                                    │
│                                                                                  │
└─────────────────────────────────────────────────────────────────────────────────┘
```

---

## 5. File Structure & Files Changed

### 5.1 Root Configuration Files
| File | Purpose | Location |
|------|---------|----------|
| `.bazelrc` | Flag aliases: `--Integration_Testing`, `--Anglefinding_IT` | Lines 28–29 (`common` scope) |
| `BUILD` | `bool_flag` and `config_setting` definitions | Lines 288–310 |

**`.bazelrc` Changes (lines 28–29):**
```bazelrc
common --flag_alias=Integration_Testing=//:Integration_Testing
common --flag_alias=Anglefinding_IT=//:Anglefinding_IT
```

**`BUILD` Changes (lines 288–310):**
```python
bool_flag(
    name = "Integration_Testing",
    build_setting_default = False,
    visibility = ["//visibility:public"],
)

config_setting(
    name = "Integration_Testing_enabled",
    flag_values = {":Integration_Testing": "true"},
    visibility = ["//visibility:public"],
)

bool_flag(
    name = "Anglefinding_IT",
    build_setting_default = False,
    visibility = ["//visibility:public"],
)

config_setting(
    name = "ANGLEFINDING_enabled",
    flag_values = {":Anglefinding_IT": "true"},
    visibility = ["//visibility:public"],
)
```

### 5.2 R52 Master Core Files
| File | Purpose |
|------|---------|
| `software/r52/integration_test/BUILD` | Library definition with conditional deps |
| `software/r52/integration_test/master_integration_test.h` | Header with types, enums, function prototypes |
| `software/r52/integration_test/master_integration_test.c` | Provider implementations |
| `software/r52/main.c` | `SIT_WAIT_FOR_DEBUGGER()` macro |
| `software/r52/dsp_setup/src/dsp_setup.c` | Provider and IPC hooks |

### 5.3 BBE32 DSP Core Files
| File | Purpose |
|------|---------|
| `software/bbe32/integration_test/BUILD` | Library definition |
| `software/bbe32/integration_test/dsp_integration_test.h` | Header with AF_USC_T, enums |
| `software/bbe32/integration_test/dsp_integration_test.c` | Receiver/Provider implementations |
| `software/bbe32/src/ipc_dsp.c` | M2D receive, D2M send hooks |
| `software/bbe32/src/doppler_process_ifc.c` | D2M Msg Buffer Provider hook |
| `software/bbe32/src/anglefinding_project_interface.c` | AF Input Receiver hook |

### 5.4 Common Files
| File | Purpose |
|------|---------|
| `software/common/ipc/ipc_data.h` | `SIT_Data_T`, `SIT_Test_Result_T` (lines 226–241) |

### 5.5 Hook Include & Call Locations (Verified Feb 2027)

**R52 Core Hook Calls:**
| File | Line | Hook / Include |
|------|------|----------------|
| `software/r52/main.c` | L35 | `#include "master_it_macros_test.h"` |
| `software/r52/main.c` | L37–40 | `volatile bool SIT_Run = false;` (guarded) |
| `software/r52/main.c` | L144 | `SIT_WAIT_FOR_DEBUGGER();` |
| `software/r52/dsp_setup/src/dsp_setup.c` | L66 | `#include "master_it_macros_test.h"` |
| `software/r52/dsp_setup/src/dsp_setup.c` | L286 | `SIT_M2D_ONETIME(...)` |
| `software/r52/dsp_setup/src/dsp_setup.c` | L287 | `SIT_USC_CALIB_DATA_PROVIDER(...)` |
| `software/r52/dsp_setup/src/dsp_setup.c` | L694 | `SIT_M2D_PERLOOK(...)` |
| `software/r52/dsp_setup/src/dsp_setup.c` | L695 | `SIT_IPC_TRANSFER_M2D_SEND(...)` |
| `software/r52/dsp_setup/src/dsp_setup.c` | L871 | `SIT_IPC_TRANSFER_D2M_RECEIVE(...)` |

**BBE32 Core Hook Calls:**
| File | Line | Hook / Include |
|------|------|----------------|
| `software/bbe32/src/ipc_dsp.c` | L58 | `#include "dsp_it_macros_test.h"` |
| `software/bbe32/src/ipc_dsp.c` | L377 | `SIT_IPC_TRANSFER_D2M_SEND(...)` |
| `software/bbe32/src/ipc_dsp.c` | L522 | `SIT_IPC_TRANSFER_M2D_RECEIVE(...)` |
| `software/bbe32/src/doppler_process_ifc.c` | L72 | `#include "dsp_it_macros_test.h"` |
| `software/bbe32/src/doppler_process_ifc.c` | L698 | `SIT_D2M_MSG_BUFFER(...)` |
| `software/bbe32/src/anglefinding_project_interface.c` | L62 | `#include "dsp_it_macros_test.h"` |
| `software/bbe32/src/anglefinding_project_interface.c` | L342 | `SIT_AF_INPUT(...)` |

---

## 6. Interface Enumeration

### 6.1 X-Macro Pattern
The framework uses the DCS_X-macro pattern for type-safe interface enumeration:

```c
/* Conditional guard macro */
#if defined(Anglefinding_IT)
   #define IF_ANGLEFINDING(x) x
#else
   #define IF_ANGLEFINDING(x)
#endif

/* Interface list with Work Item IDs */
#define INTERFACE_ENUM_LIST                                          \
   IF_ANGLEFINDING(DCS_X(Ri_beam_vector, 392701U))                   \
   IF_ANGLEFINDING(DCS_X(Ri_rdd_snr, 362404U))                       \
   /* ... more interfaces ... */

/* Enum generation */
typedef enum {
   Test_Interface_None = 0U,
   #define DCS_X(name, value) name = value,
   INTERFACE_ENUM_LIST
   #undef DCS_X
} Test_Interface_Name_T;
```

### 6.2 Complete Interface List

#### RDD Stream Interfaces (Angle Finding Input)
| Interface Name | Work Item | Data Structure | Description |
|----------------|-----------|----------------|-------------|
| `Ri_beam_vector` | 392701 | `rdd1_bv[det][TX][RX]` | Beam vector array |
| `Ri_rdd_snr` | 362404 | `rdd2_snr[det]` | SNR per detection |
| `Ri_range_rate` | 362403 | `rdd2_range_rate[det]` | Range rate per detection |
| `Ri_range` | 362402 | `rdd2_range[det]` | Range per detection |
| `Ri_scan_index` | 362399 | `look_data.scan_index` | Scan index |
| `Ri_look_type` | 362398 | `look_data.look_id` | Look type ID |
| `Ri_sp_fail_flag` | 294068 | `rdd2_sp_fail_flag[det]` | SP failure flags |
| `Ri_sp_num_detections` | 294066 | `rdd2_num_detect` | SP detection count |
| `Ri_fp_num_detections` | 294052 | `rdd1_num_detect` | FP detection count |
| `Ri_range_doppler_amplitude` | 294048 | `rdd1_rdop_amp[det]` | R-D amplitude |
| `Ri_CR_response` | 294047 | `bwdep_cr_resp[rbin]` | CR response |
| `Ri_look_index` | 399606 | `look_data.look_index` | Look index |
| `Ri_range_coverage` | 379779 | `look_data.range_coverage` | Range coverage |
| `Ri_doppler_coverage` | 379778 | `look_data.doppler_coverage` | Doppler coverage |

#### Master Core Interfaces
| Interface Name | Work Item | Data Structure | Description |
|----------------|-----------|----------------|-------------|
| `Ri_radar_position` | 362359 | `p_radar_position` (pointer) | Radar position |
| `Ri_host_vehicle_speed` | 362349 | `p_veh_speed` (pointer) | Host vehicle speed |
| `Ri_usc_calib_offset` | 392702 | USC calibration | Calibration offset |

#### USC Calibration Interfaces
| Interface Name | Work Item | Data Structure | Description |
|----------------|-----------|----------------|-------------|
| `Ri_usc_af_az_sin_lut` | 29404001 | `p_az_sin_lut[]` | Azimuth sine LUT |
| `Ri_usc_af_az_lut` | 29404002 | `p_az_lut[]` | Azimuth LUT |
| `Ri_usc_af_az_search_range` | 29404003 | `p_az_search_range[]` | Az search range |
| `Ri_usc_af_calmat1_ang_min_az` | 29404004 | `cal_ang_min_az[0]` | Min azimuth section 1 |
| `Ri_usc_af_calmat2_ang_min_az` | 29404005 | `cal_ang_min_az[1]` | Min azimuth section 2 |
| `Ri_usc_af_calmat3_ang_min_az` | 29404006 | `cal_ang_min_az[2]` | Min azimuth section 3 (Gen7 only) — **TEST_SKIPPED on Gen8** |
| `Ri_usc_af_calmat4_ang_min_az` | 29404007 | `cal_ang_min_az[3]` | Min azimuth section 4 (Gen7 only) — **TEST_SKIPPED on Gen8** |
| `Ri_usc_af_calmat1_ang_max_az` | 29404008 | `cal_ang_max_az[0]` | Max azimuth section 1 |
| `Ri_usc_af_calmat2_ang_max_az` | 29404009 | `cal_ang_max_az[1]` | Max azimuth section 2 |
| `Ri_usc_af_calmat3_ang_max_az` | 29404010 | `cal_ang_max_az[2]` | Max azimuth section 3 (Gen7 only) — **TEST_SKIPPED on Gen8** |
| `Ri_usc_af_calmat4_ang_max_az` | 29404011 | `cal_ang_max_az[3]` | Max azimuth section 4 (Gen7 only) — **TEST_SKIPPED on Gen8** |
| `Ri_usc_af_calmat1_ang_min_el` | 29404012 | `cal_ang_min_el[0]` | Min elevation section 1 |
| `Ri_usc_af_calmat2_ang_min_el` | 29404013 | `cal_ang_min_el[1]` | Min elevation section 2 |
| `Ri_usc_af_calmat3_ang_min_el` | 29404014 | `cal_ang_min_el[2]` | Min elevation section 3 (Gen7 only) — **TEST_SKIPPED on Gen8** |
| `Ri_usc_af_calmat4_ang_min_el` | 29404015 | `cal_ang_min_el[3]` | Min elevation section 4 (Gen7 only) — **TEST_SKIPPED on Gen8** |
| `Ri_usc_af_calmat1_ang_max_el` | 29404016 | `cal_ang_max_el[0]` | Max elevation section 1 |
| `Ri_usc_af_calmat2_ang_max_el` | 29404017 | `cal_ang_max_el[1]` | Max elevation section 2 |
| `Ri_usc_af_calmat3_ang_max_el` | 29404018 | `cal_ang_max_el[2]` | Max elevation section 3 (Gen7 only) — **TEST_SKIPPED on Gen8** |
| `Ri_usc_af_calmat4_ang_max_el` | 29404019 | `cal_ang_max_el[3]` | Max elevation section 4 (Gen7 only) — **TEST_SKIPPED on Gen8** |

#### Output Interfaces (Provider on DSP)
| Interface Name | Work Item | Data Structure | Description |
|----------------|-----------|----------------|-------------|
| `Ro_det_az_rad` | 6003 | `afbb_det_output.af_data.theta[det]` | Detection azimuth |
| `Ro_det_el_rad` | 6004 | `afbb_det_output.af_data.phi[det]` | Detection elevation |

---

## 7. Function Reference

### 7.1 R52 Master Core Functions

| Function | Defined In | Called From | Purpose |
|----------|-----------|-------------|---------|
| `SIT_IPC_Transfer_M2D_Send` | master_integration_test.c | dsp_setup.c (L695) | Pack test params into M2D payload |
| `SIT_IPC_Transfer_D2M_Receive` | master_integration_test.c | dsp_setup.c (L871) | Extract DSP result from D2M payload |
| `SIT_M2D_Onetime_Provider` | master_integration_test.c | dsp_setup.c (L286) | Stub onetime message (radar_position) |
| `SIT_USC_Calib_Data_Provider` | master_integration_test.c | dsp_setup.c (L287) | Stub USC calibration data |
| `SIT_M2D_Perlook_Provider` | master_integration_test.c | dsp_setup.c (L694) | Stub per-look message (veh_speed) |

### 7.2 BBE32 DSP Core Functions

| Function | Defined In | Called From | Purpose |
|----------|-----------|-------------|---------|
| `SIT_IPC_Transfer_M2D_Receive` | dsp_integration_test.c | ipc_dsp.c (L522) | Extract test params from M2D |
| `SIT_IPC_Transfer_D2M_Send` | dsp_integration_test.c | ipc_dsp.c (L377) | Pack result into D2M payload |
| `SIT_D2M_Msg_Buffer_Provider` | dsp_integration_test.c | doppler_process_ifc.c (L698) | Stub RDD data |
| `SIT_AF_Input_Receiver` | dsp_integration_test.c | anglefinding_project_interface.c (L342) | Validate AF input |
| `SIT_AF_Output_Provider` | dsp_integration_test.c | (internal call) | Stub AF output |
| `SIT_AF_USC_Receiver` | dsp_integration_test.c | (internal call) | Validate USC calibration |

### 7.3 Helper Functions (Both Cores)

| Function | Purpose |
|----------|---------|
| `Stub_Scalar_Values()` | Stub single value of any type |
| `Stub_1D_Array_Values()` | Stub 1D array |
| `Stub_3D_Array_Values()` | Stub 3D array |
| `Validate_Scalar_Values()` | Validate single value with tolerance |
| `Validate_1D_Array_Values()` | Validate 1D array |
| `Validate_3D_Array_Values()` | Validate 3D array |
| `Is_Valid_Interface()` | Check interface number is valid |
| `Set_Shared_Ptr()` | Set global pointer for debugger |

---

## 8. Build Configuration

### 8.1 Bazel Flags
```bash
# Enable Integration Testing
bazel build //software:app --Integration_Testing=true

# Enable both Integration Testing and Angle Finding IT
bazel build //software:app --Integration_Testing=true --Anglefinding_IT=true

# Gen8 specific build
bazelisk build //:gen8 --config=flr8 --Integration_Testing=true --Anglefinding_IT=true
```

### 8.2 Preprocessor Macros
| Macro | Effect |
|-------|--------|
| `Integration_Testing` | Enables basic IPC hooks and SIT_Data_T |
| `Anglefinding_IT` | Enables Angle Finding specific interfaces and functions |

### 8.3 BUILD File Pattern

**Dedicated IT Libraries (single source of truth for IT defines):**
```python
# In software/bbe32/integration_test/BUILD or software/r52/integration_test/BUILD
cc_library(
    name = "dsp_it_macros_test_h",  # or "master_it_macros_test_h" for R52
    hdrs = ["dsp_it_macros_test.h"],
    defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    visibility = ["//visibility:public"],
    deps = [],
)

cc_library(
    name = "dsp_integration_test_lib",
    srcs = ["dsp_integration_test.c"],
    hdrs = ["dsp_integration_test.h"],
    defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    visibility = ["//visibility:public"],
    deps = [...],
    alwayslink = True,  # Ensures all functions are linked even if not directly referenced
)
```

**Main library targets (DO NOT duplicate IT defines — they propagate from deps):**
```python
cc_library(
    name = "my_main_lib",
    srcs = ["my_source.c"],
    # Note: Integration Testing defines (Integration_Testing, Anglefinding_IT) are
    # provided by the dedicated IT library (dsp_it_macros_test_h) via deps.
    deps = [
        # ... main deps ...
        "//software/bbe32/integration_test:dsp_it_macros_test_h",  # Unconditional - provides IT defines + header
    ] + select({
        "//:Integration_Testing_enabled": [
            "//software/bbe32/integration_test:dsp_integration_test_lib",
        ],
        "//conditions:default": [],
    }),
)
```

### 8.4 Conditional Dependencies in Parent BUILD Files
To ensure SWE5 code is compiled and linked into the main build, parent BUILD files must include conditional dependencies and defines based on the Integration Testing flags.

#### Root BUILD Configuration (BUILD lines 288–310)
```python
# In BUILD (root level)
bool_flag(
    name = "Integration_Testing",
    build_setting_default = False,
    visibility = ["//visibility:public"],
)

config_setting(
    name = "Integration_Testing_enabled",
    flag_values = {":Integration_Testing": "true"},
    visibility = ["//visibility:public"],
)

bool_flag(
    name = "Anglefinding_IT",
    build_setting_default = False,
    visibility = ["//visibility:public"],
)

config_setting(
    name = "ANGLEFINDING_enabled",
    flag_values = {":Anglefinding_IT": "true"},
    visibility = ["//visibility:public"],
)
```

#### R52 Master Core (software/r52/dsp_setup/BUILD)
```python
cc_library(
    name = "dsp_setup_lib",
    srcs = ["src/dsp_setup.c"],
    defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    deps = [
        # ... main dependencies ...
        "//software/r52/integration_test:master_it_macros_test_h",  # UNCONDITIONAL
    ] + select({
        "//:Integration_Testing_enabled": ["//software/r52/integration_test:master_integration_test"],
        "//conditions:default": [],
    }),
)
```

#### BBE32 DSP Core (software/bbe32/src/BUILD)

4 targets reference Integration Testing: `main_application_lib`, `angle_finding_project_interface_lib`, `dopplerproc_ifc_lib`, `ipc_dsp_lib`.

```python
# main_application_lib - defines only (deps come transitively)
cc_library(
    name = "main_application_lib",
    defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    deps = [...],
)

# angle_finding_project_interface_lib - unconditional hook header + conditional IT lib
cc_library(
    name = "angle_finding_project_interface_lib",
    defines = [...] + select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    deps = [
        "//software/bbe32/integration_test:dsp_it_macros_test_h",  # UNCONDITIONAL
    ] + select({
        "//:Integration_Testing_enabled": ["//software/bbe32/integration_test:dsp_integration_test_lib"],
        "//:ANGLEFINDING_enabled": ["//software/bbe32/integration_test:dsp_integration_test_lib"],
        "//conditions:default": [],
    }),
)

# dopplerproc_ifc_lib - uses local_defines (not defines)
cc_library(
    name = "dopplerproc_ifc_lib",
    local_defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    deps = [
        "//software/bbe32/integration_test:dsp_it_macros_test_h",  # UNCONDITIONAL
    ] + select({
        "//:Integration_Testing_enabled": ["//software/bbe32/integration_test:dsp_integration_test_lib"],
        "//:ANGLEFINDING_enabled": ["//software/bbe32/integration_test:dsp_integration_test_lib"],
        "//conditions:default": [],
    }),
)

# ipc_dsp_lib - uses local_defines
cc_library(
    name = "ipc_dsp_lib",
    local_defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    deps = [
        "//software/bbe32/integration_test:dsp_it_macros_test_h",  # UNCONDITIONAL
    ] + select({
        "//:Integration_Testing_enabled": ["//software/bbe32/integration_test:dsp_integration_test_lib"],
        "//:ANGLEFINDING_enabled": ["//software/bbe32/integration_test:dsp_integration_test_lib"],
        "//conditions:default": [],
    }),
)
```

**Critical Note:** All BUILD files must use separate select() statements for each flag condition. Do NOT create local config_setting_groups as they will not be visible across package boundaries, causing linking failures. Use the separate `//:Integration_Testing_enabled` and `//:ANGLEFINDING_enabled` config_settings from the root BUILD file.

### 8.5 Config Setting Visibility Fix

**Issue:** Local config_setting_groups defined in sub-package BUILD files are not visible to other packages, causing select() statements to fail and integration test libraries to not be linked.

**Solution:** Use separate select() statements for each flag condition rather than combined config_setting_groups. This ensures visibility across package boundaries.

**❌ WRONG - Combined config_setting_group:**
```python
# In root BUILD
config_setting(
    name = "Integration_Testing_Anglefinding",
    flag_values = {
        ":Integration_Testing": "true",
        ":Anglefinding_IT": "true",
    },
    visibility = ["//visibility:public"],
)

# In software/bbe32/src/BUILD - This won't work reliably!
deps = select({
    "//:Integration_Testing_Anglefinding": [...],  # May not be visible!
})
```

**✅ CORRECT - Separate select statements:**
```python
# Use in any BUILD file:
deps = select({
    "//:Integration_Testing_enabled": ["//software/r52/integration_test:master_integration_test"],
    "//conditions:default": [],
}) + select({
    "//:ANGLEFINDING_enabled": ["//software/bbe32/integration_test:dsp_integration_test_lib"],
    "//conditions:default": [],
})
```

---

## 9. Test Execution Flow

### Step 1: Flash Integration Test Build
```bash
bazel build //software:app --Integration_Testing=true --Anglefinding_IT=true
# Flash to target hardware
```

### Step 2: Attach Debugger
1. Connect debugger to R52 core
2. Set breakpoint at `SIT_WAIT_FOR_DEBUGGER()`

### Step 3: Configure Test
```c
// In debugger watch window, set:
Test_Iface_Num = 362359;  // e.g., Ri_radar_position
Stub_Val = 3.0;           // Value to inject
Stub_Len = 1;             // Scalar value
SIT_Run = true;           // Release from wait loop
```

### Step 4: Run Test
1. Continue execution
2. Framework stubs value at Provider
3. Value flows through processing
4. Receiver validates

### Step 5: Check Results
```c
// In debugger watch window, read:
DSP_Final_Res      // Should be TEST_PASSED (2)
Master_Final_Res   // Should be TEST_PASSED (2)
DSP_Shared_Ri      // Pointer to validated data
```

---

## 10. Debugging Guide

### 10.1 Key Global Variables
| Variable | Core | Purpose |
|----------|------|---------|
| `SIT_Run` | R52 | Release debugger wait |
| `Test_Iface_Num` | R52 | Interface to test (user input) |
| `Stub_Val` | R52 | Value to inject (user input) |
| `Stub_Len` | R52 | Array length (user input) |
| `Master_Final_Res` | R52 | R52 test result |
| `DSP_Final_Res` | R52 | BBE32 test result (received via IPC) |
| `DSP_Test_Iface_Num` | BBE32 | Interface being tested |
| `DSP_Stub_Val` | BBE32 | Value received from R52 |
| `DSP_Res` | BBE32 | Current validation result |

### 10.2 Common Issues

| Issue | Cause | Solution |
|-------|-------|----------|
| `TEST_INTERFACE_INVALID` | Invalid interface number | Check INTERFACE_ENUM_LIST |
| `TEST_NOT_PERFORMED` | Test didn't execute | Verify hooks are called |
| `TEST_FAILED` | Validation mismatch | Check stub value, tolerance |
| `TEST_SKIPPED` | Interface not applicable | Gen8 has AF_CAL_SEC_N=2 |
| Build fails | Missing deps | Add integration_test lib to deps |
| Undefined symbol | Wrong guard | Check Integration_Testing/Anglefinding_IT |
| `attempt to store non-zero value in section '.sram1.bss'` | Non-zero initializer in .bss | Move variable to `.sram1.data` (see Section 10.4) |
| `region 'BBE32_DTCM0' overflowed by X bytes` | Function-local static in default .bss | Move static to file scope with `.sram1.bss` (see Section 10.4.2) |
| BBE32 DTCM0/ITCM overflow | Test code in fast memory | Move to SRAM1 sections (see Section 10.4) |

### 10.3 Validation Tolerance
```c
#define SWE5_VALIDATION_TOLERANCE (1e-5F)  // 0.001% relative tolerance

// Validation formula:
bool passed = fabs(actual - expected) <= SWE5_VALIDATION_TOLERANCE * fabs(expected);
```

### 10.4 BBE32 Memory Management for Integration Testing

**⚠️ CRITICAL: BBE32 DTCM0/ITCM Memory Overflow Prevention**

When Integration Testing is enabled, SWE5 code adds significant variables and functions that can cause **BBE32 DTCM0 (data) and ITCM (code) memory overflow**. To prevent build failures, **ALL Integration Testing variables and functions MUST be placed in SRAM1**.

#### 10.4.1 Memory Section Rules

| Section | Purpose | Initialization | Use For |
|---------|---------|----------------|---------|
| `.bss` | Uninitialized data | Zero or no initializer | Variables initialized to 0 or without initializer |
| `.data` | Initialized data | Non-zero values | Variables with non-zero initial values |
| `.text` | Code/Functions | Executable code | All function implementations |

**Common Linker Errors:**
```
Error: attempt to store non-zero value in section `.sram1.bss'
```
**Cause:** Variable with non-zero initializer placed in `.bss` section.
**Solution:** Move to `.data` section.

#### 10.4.2 Variable Placement Patterns

**✅ CORRECT Patterns:**

```c
/* Zero-initialized or uninitialized variables → .sram1.bss */
volatile __attribute__((used)) __attribute__((section(".sram1.bss"))) G_Pointer_T DSP_Shared_Pi;
volatile __attribute__((used)) __attribute__((section(".sram1.bss"))) G_Pointer_T DSP_Shared_Ri;
static __attribute__((section(".sram1.bss"))) uint32_t DSP_Test_Iface_Num = 0U;  // Zero init OK
static __attribute__((section(".sram1.bss"))) double DSP_Stub_Val = 0.0;         // Zero init OK

/* Non-zero initialized variables → .sram1.data */
static __attribute__((section(".sram1.data"))) uint32_t DSP_Stub_Len = 1U;       // Non-zero init
static __attribute__((section(".sram1.data"))) SIT_Test_Result_T DSP_Res = TEST_NOT_PERFORMED;  // Enum value
```

**❌ WRONG - Causes Linker Error:**
```c
// Non-zero initializer in .bss section - LINKER ERROR!
static __attribute__((section(".sram1.bss"))) uint32_t DSP_Stub_Len = 1U;
static __attribute__((section(".sram1.bss"))) SIT_Test_Result_T DSP_Res = TEST_NOT_PERFORMED;
```

**⚠️ CRITICAL: Function-Local Static Variables**

Function-local static variables **CANNOT** have section attributes in their declaration. They **MUST** be moved to file scope:

```c
// ❌ WRONG - Function-local static goes to default .bss (DTCM0 overflow!)
static SIT_Test_Result_T Is_Valid_Interface(uint32_t interface_number)
{
   bool is_valid = false;
   static uint32_t last_interface_number = 0U;  // ← CAUSES DTCM0 OVERFLOW!
   // ...
}

// ✅ CORRECT - Move to file scope with .sram1.bss attribute
/* At file scope with other variables */
static __attribute__((section(".sram1.bss"))) uint32_t last_interface_number = 0U;

static SIT_Test_Result_T Is_Valid_Interface(uint32_t interface_number)
{
   bool is_valid = false;
   // last_interface_number now accessible - no local static needed
   // ...
}
```

**Why This Matters:**
- Function-local static variables are allocated in `.bss` section automatically
- Compiler ignores section attributes on function-local statics in many toolchains
- Even 4 bytes can cause DTCM0 overflow when memory is 99%+ full
- Build error: `region 'BBE32_DTCM0' overflowed by X bytes`

**Rule:** ALL static variables in Integration Testing code must be at FILE SCOPE with explicit `.sram1.bss` or `.sram1.data` attributes.

#### 10.4.3 Function Placement Patterns

**ALL Integration Testing functions** should be placed in SRAM1 text section to avoid ITCM overflow:

```c
/* Static helper functions */
static __attribute__((section(".sram1.text"))) void Stub_Scalar_Values(...) { }
static __attribute__((section(".sram1.text"))) void Stub_1D_Array_Values(...) { }
static __attribute__((section(".sram1.text"))) bool Validate_Scalar_Values(...) { }
static inline void __attribute__((section(".sram1.text"))) Set_Shared_Ptr(...) { }

/* Public API functions */
__attribute__((section(".sram1.text"))) void SIT_IPC_Transfer_M2D_Receive(...) { }
__attribute__((section(".sram1.text"))) void SIT_IPC_Transfer_D2M_Send(...) { }
__attribute__((section(".sram1.text"))) void SIT_D2M_Msg_Buffer_Provider(...) { }
__attribute__((section(".sram1.text"))) void SIT_AF_Input_Receiver(...) { }
__attribute__((section(".sram1.text"))) void SIT_AF_Output_Provider(...) { }
__attribute__((section(".sram1.text"))) void SIT_AF_USC_Receiver(...) { }
```

**Attribute Order:**
- **Static functions:** `static __attribute__((section(".sram1.text"))) ReturnType FunctionName(...)`
- **Public functions:** `__attribute__((section(".sram1.text"))) ReturnType FunctionName(...)`
- **Inline functions:** `static inline void __attribute__((section(".sram1.text"))) FunctionName(...)`

#### 10.4.4 Why SRAM1 for Integration Testing?

| Memory Location | Size | Performance | Use For |
|----------------|------|-------------|---------|
| **DTCM0** | Limited (~100KB) | Fastest | Critical real-time variables |
| **ITCM** | Limited (~100KB) | Fastest | Critical real-time functions |
| **SRAM1** | Larger (~MB) | Slightly slower | Non-critical test code/data |

**Rationale:**
- Integration test code runs only during SWE5 validation (not in production)
- Performance impact of SRAM1 is acceptable for test framework
- Prevents DTCM0/ITCM overflow that breaks production builds
- Main application code remains in fast memory

#### 10.4.5 Complete Example - dsp_integration_test.c

```c
#if defined(Integration_Testing)

/* ========== VARIABLES ========== */

/* Zero-initialized → .sram1.bss */
volatile __attribute__((used)) __attribute__((section(".sram1.bss"))) G_Pointer_T DSP_Shared_Pi;
volatile __attribute__((used)) __attribute__((section(".sram1.bss"))) G_Pointer_T DSP_Shared_Ri;
static __attribute__((section(".sram1.bss"))) uint32_t DSP_Test_Iface_Num = 0U;
static __attribute__((section(".sram1.bss"))) double DSP_Stub_Val = 0.0;
static __attribute__((section(".sram1.bss"))) uint32_t last_interface_number = 0U;  // Moved from function scope

/* Non-zero initialized → .sram1.data */
static __attribute__((section(".sram1.data"))) uint32_t DSP_Stub_Len = 1U;
static __attribute__((section(".sram1.data"))) SIT_Test_Result_T DSP_Res = TEST_NOT_PERFORMED;

/* ========== FUNCTIONS ========== */

/* Static helper functions → .sram1.text */
static __attribute__((section(".sram1.text"))) SIT_Test_Result_T Is_Valid_Interface(uint32_t interface_number)
{
   /* Implementation */
}

static __attribute__((section(".sram1.text"))) void Stub_Scalar_Values(void *dstb_ptr, DataType_T data_type, double stub_value)
{
   /* Implementation */
}

/* Public API functions → .sram1.text */
__attribute__((section(".sram1.text"))) void SIT_IPC_Transfer_M2D_Receive(const M2D_Payload_T *m2d_payload_ptr)
{
   /* Implementation */
}

#endif /* Integration_Testing */
```

#### 10.4.6 Troubleshooting Memory Issues

| Error Message | Root Cause | Solution |
|--------------|------------|----------|
| `attempt to store non-zero value in section '.sram1.bss'` | Non-zero initializer in `.bss` | Change to `.sram1.data` |
| `region 'BBE32_DTCM0' overflowed by X bytes` | Function-local static variables in default `.bss` | Move ALL static variables to file scope with `.sram1.bss` attribute |
| `section '.dtcm0.bss' will not fit in region 'dtcm0_ram'` | Too many variables in DTCM0 | Move test variables to `.sram1.bss` or `.sram1.data` |
| `section '.itcm.text' will not fit in region 'itcm_ram'` | Too much code in ITCM | Move test functions to `.sram1.text` |
| Undefined reference to `SIT_xxx` | Function in SRAM1 but not exported | Verify function is not static or header declares it |

#### 10.4.7 Linker Script Configuration for Integration Testing

**⚠️ CRITICAL: Linker Memory Region Expansion**

When Integration_Testing is enabled, IPC payload structures (`D2M_Payload_T`, `M2D_Payload_T`) include additional `SIT_Data_T` fields (~32 bytes each with alignment). This can cause `.bss` section overflow in `BBE32_DTCM0` even after placing all test code in SRAM1.

**⚠️ CURRENT STATE (as of Feb 2027): Linker script has NO Integration_Testing conditionals.**

Neither `software/common/linker/common.ld` nor `software/common/linker/BUILD` contain any Integration_Testing–related content. The previously documented conditional memory expansion (59K→64K) was **REVERTED / never merged**. The ONLY mitigation is placing ALL test code in SRAM1 sections.

**When You Get DTCM0 Overflow:**
1. **First** ensure ALL test variables use `.sram1.bss` or `.sram1.data` attributes
2. **Move** ALL static function-local variables to file scope with section attributes
3. **If overflow persists** due to IPC payload expansion (`SIT_Data_T` in `D2M_Payload_T`/`M2D_Payload_T`), consider adding `-DIntegration_Testing` to the linker BUILD's `ld_preprocess` args and adding a conditional block in `common.ld` — but this has not been done yet
4. **⛔ DO NOT modify memory region sizes** without understanding impact on F360 Tracker and other production features

**Build Validation:**
```bash
# After adding/modifying BBE32 Integration Testing code
bazelisk build //:gen8 --config=flr8 --Integration_Testing=true --Anglefinding_IT=true

# Verify normal build still works (without test code overhead)
bazelisk build //:gen8 --config=flr8
```

---

## 11. CODE GENERATION PATTERNS

> **CRITICAL FOR COPILOT**: This section contains exact code patterns to follow when generating SWE5 framework code for new interfaces.

### 11.1 Adding Interface to INTERFACE_ENUM_LIST

**Pattern (in dsp_integration_test.h or master_integration_test.h):**
```c
#define INTERFACE_ENUM_LIST                                                      \
   /* Existing interfaces... */                                                   \
   IF_MODULENAME(DCS_X(Interface_Name, WorkItemID_U))  /* Comment describing data */ \
```

**Example - Adding new Tracker interface:**
```c
#define INTERFACE_ENUM_LIST                                                      \
   /* ... existing interfaces ... */                                              \
   IF_TRACKER(DCS_X(Ri_tracker_target_id, 500001U))    /* WI-500001: target_id */ \
   IF_TRACKER(DCS_X(Ri_tracker_range, 500002U))        /* WI-500002: target range */ \
```

### 11.2 Provider Function Switch Case Pattern (Stubbing)

**Pattern for SCALAR values:**
```c
case Interface_Name: /* WI-XXXXXX: description */
   Set_Shared_Ptr(&DSP_Shared_Pi, &structure->field, sizeof(DataType), 1, Ri_field_name, DATATYPE_XXXXX);
   Stub_Scalar_Values(&structure->field, DATATYPE_XXXXX, DSP_Stub_Val);
   break;
```

**Pattern for 1D ARRAY values with boundary check:**
```c
case Interface_Name: /* WI-XXXXXX: array[N] */
   if (num_elem > MAX_ARRAY_SIZE)
   {
      num_elem = MAX_ARRAY_SIZE;
   }
   Set_Shared_Ptr(&DSP_Shared_Pi, structure->array, sizeof(ElementType), num_elem, Ri_array_name, DATATYPE_XXXXX);
   Stub_1D_Array_Values(structure->array, DATATYPE_XXXXX, DSP_Stub_Val, num_elem);
   break;
```

**Pattern for 3D ARRAY values:**
```c
case Interface_Name: /* WI-XXXXXX: array[dim1][dim2][dim3] */
   if (num_elem > MAX_DIM1 * MAX_DIM2 * MAX_DIM3)
   {
      num_elem = MAX_DIM1 * MAX_DIM2 * MAX_DIM3;
   }
   Set_Shared_Ptr(&DSP_Shared_Pi, structure->array, sizeof(ElementType), num_elem, Ri_array_name, DATATYPE_XXXXX);
   Stub_3D_Array_Values(structure->array, DATATYPE_XXXXX, DSP_Stub_Val, MAX_DIM1, MAX_DIM2, MAX_DIM3);
   break;
```

> **Note:** `Ri_field_name` / `Ri_array_name` are `Test_Interface_Name_T` values (from `INTERFACE_ENUM_LIST`, which uses unique Work Item IDs as values). When adding new interfaces, add a new entry to `INTERFACE_ENUM_LIST` in the appropriate header.

### 11.3 Receiver Function Switch Case Pattern (Validation)

**Pattern for SCALAR values:**
```c
case Interface_Name:
{
   is_valid = Validate_Scalar_Values(&structure->field, DATATYPE_XXXXX, DSP_Stub_Val);
   Set_Shared_Ptr(&DSP_Shared_Ri, &structure->field, sizeof(DataType), 1, Ri_field_name, DATATYPE_XXXXX);
}
break;
```

**Pattern for SCALAR via POINTER (Gen8 style):**
```c
case Interface_Name:
{
   is_valid = Validate_Scalar_Values(structure->p_field, DATATYPE_XXXXX, DSP_Stub_Val);
   Set_Shared_Ptr(&DSP_Shared_Ri, structure->p_field, sizeof(DataType), 1, Ri_p_field_name, DATATYPE_XXXXX);
}
break;
```

**Pattern for 1D ARRAY values:**
```c
case Interface_Name:
{
   uint32_t num_elem = DSP_Stub_Len;
   if (num_elem > MAX_ARRAY_SIZE)
   {
      num_elem = MAX_ARRAY_SIZE;
   }
   is_valid = Validate_1D_Array_Values(structure->array, DATATYPE_XXXXX, DSP_Stub_Val, num_elem);
   Set_Shared_Ptr(&DSP_Shared_Ri, structure->array, sizeof(ElementType), num_elem, Ri_array_name, DATATYPE_XXXXX);
}
break;
```

### 11.4 Conditional Compilation for Array Index Bounds

**Pattern for calibration sections that may not exist in Gen8:**
```c
case Ri_usc_af_calmat3_ang_min_az: /* WI-29404006: calmat3_ang_min_az */
#if (AF_CAL_SEC_N >= 3U)
   Set_Shared_Ptr(&DSP_Shared_Ri, &af_usc_ptr->cal_ang_min_az[2], sizeof(float), 1,
                  Ri_usc_af_calmat3_ang_min_az, DATATYPE_FLOAT32);
   validation_result = Validate_Scalar_Values(&af_usc_ptr->cal_ang_min_az[2], DATATYPE_FLOAT32, DSP_Stub_Val);
   DSP_Res           = (DSP_Res == TEST_PASSED && validation_result) ? TEST_PASSED : TEST_FAILED;
#else
   /* Gen8 has only 2 calibration sections - skip this interface */
   DSP_Res = TEST_SKIPPED;
#endif
   break;
```

### 11.5 Result Update Pattern

**For Receiver functions - accumulating result:**
```c
if (!is_valid)
{
   DSP_Res = TEST_FAILED;
}
/* Note: Only set to FAILED, never reset to PASSED after failure */
```

**For USC Receiver - explicit result update:**
```c
DSP_Res = (DSP_Res == TEST_PASSED && validation_result) ? TEST_PASSED : TEST_FAILED;
```

---

## 12. ADDING NEW INTERFACES - STEP BY STEP

### Step 1: Define the Interface

Given: Work Item ID, Interface Name (Ri_xxx for Receiver, Ro_xxx for Output/Provider), Data Structure Path

### Step 2: Add to INTERFACE_ENUM_LIST

**File:** `dsp_integration_test.h` and/or `master_integration_test.h`

```c
#define INTERFACE_ENUM_LIST                                                      \
   /* ... existing ... */                                                         \
   IF_MODULENAME(DCS_X(Ri_new_interface, 123456U))  /* WI-123456: description */
```

### Step 3: Add Module Guard Macro (if new module)

**If new module, add guard macro:**
```c
#if defined(NewModule_IT)
   #define IF_NEWMODULE(x) x
#else
   #define IF_NEWMODULE(x)
#endif
```

### Step 4: Add Provider Case (for stubbing)

**File:** Appropriate Provider function (e.g., `SIT_D2M_Msg_Buffer_Provider`)

```c
case Ri_new_interface: /* WI-123456: new_field */
   Set_Shared_Ptr(&DSP_Shared_Pi, &structure->new_field, sizeof(type), 1, Ri_new_field, DATATYPE_XXXXX);
   Stub_Scalar_Values(&structure->new_field, DATATYPE_XXXXX, DSP_Stub_Val);
   break;
```

### Step 5: Add Receiver Case (for validation)

**File:** Appropriate Receiver function (e.g., `SIT_AF_Input_Receiver`)

```c
case Ri_new_interface:
{
   is_valid = Validate_Scalar_Values(&structure->new_field, DATATYPE_XXXXX, DSP_Stub_Val);
   Set_Shared_Ptr(&DSP_Shared_Ri, &structure->new_field, sizeof(type), 1, Ri_new_field, DATATYPE_XXXXX);
}
break;
```

> **Note:** Add `Ri_new_field` to `INTERFACE_ENUM_LIST` in the appropriate header (`master_integration_test.h` or `dsp_integration_test.h`).

### Step 6: Update BUILD Dependencies

**If new headers needed:**
```python
deps = [
    # ... existing deps ...
    "//path/to:new_header_h",
],
```

### Step 7: Add Hook (if new function)

**File:** Source file where data is available

```c
#if defined(Integration_Testing) && defined(NewModule_IT)
   #include "dsp_integration_test.h"
   #define SIT_NEW_HOOK(ptr) SIT_New_Function(ptr)
#else
   #define SIT_NEW_HOOK(ptr) /*No operation*/
#endif

// In function body:
SIT_NEW_HOOK(data_ptr);
```

---

## 13. VALIDATION & BOUNDARY CHECKING PATTERNS

### 13.1 Array Boundary Check Pattern

**ALWAYS check array bounds before stubbing/validating:**
```c
case Ri_array_interface:
   if (num_elem > MAX_ALLOWED_SIZE)
   {
      num_elem = MAX_ALLOWED_SIZE;
   }
   // ... stub or validate ...
   break;
```

### 13.2 Common Maximum Values

| Array | Max Size | Constant |
|-------|----------|----------|
| Detections (first pass) | 256 | `MAX_DETS_FIRST_PASS` |
| Detections (second pass) | 128 | `MAX_DETS_SECOND_PASS` |
| Range bins | 512 | `MAX_RANGE_BINS` |
| TX channels | 3 | `NUM_TX_CHANNELS` |
| RX antennas | 4 | `NUM_RX_ANTENNAS` |
| Azimuth LUT | 151 (Gen8), 189 (Gen7) | `k_calmat1_lut_az[]` |
| Search range | 10 | Hardcoded |
| Calibration sections | 2 (Gen8), 4 (Gen7) | `AF_CAL_SEC_N` |

### 13.3 Null Pointer Check Pattern

**ALWAYS check pointers at function entry:**
```c
void SIT_Function(const Structure_T *ptr)
{
   if (ptr == NULL)
   {
      return;
   }
   // ... rest of function ...
}
```

### 13.4 Interface Validity Check Pattern

**Use Is_Valid_Interface() before validation:**
```c
if (Is_Valid_Interface(DSP_Test_Iface_Num) != TEST_PASSED)
{
   return;
}
```

---

## 14. BUILD DEPENDENCY PATTERNS

> **ARCHITECTURE NOTE (Updated 2026-03-02):** IT defines (`Integration_Testing`, `Anglefinding_IT`) are
> centralized in the dedicated IT library targets only. They propagate to main library targets via
> Bazel's transitive `defines`. Main library/binary targets **MUST NOT** duplicate these defines.

### 14.1 Conditional Dependency Pattern

**For combined flag dependencies (Integration Testing + Angle Finding):**
```python
deps = [
    "//software/common/ipc:ipc_data_h",  # Always needed
    # SWE5 Integration Testing: IT defines propagate from dsp_it_macros_test_h
    "//software/bbe32/integration_test:dsp_it_macros_test_h",  # Unconditional - provides IT defines + header
] + select({
    "//:Integration_Testing_enabled": [
        "//software/bbe32/integration_test:dsp_integration_test_lib",
    ],
    "//conditions:default": [],
}) + select({
    "//:ANGLEFINDING_enabled": [
        "//software/bbe32/integration_test:dsp_integration_test_lib",
    ],
    "//conditions:default": [],
})
```

**For single flag dependencies (R52 side):**
```python
deps = [
    "//software/common/ipc:ipc_data_h",  # Always needed
    # SWE5 Integration Testing: IT defines propagate from master_it_macros_test_h
    "//software/r52/integration_test:master_it_macros_test_h",  # Unconditional
] + select({
    "//:Integration_Testing_enabled": [
        "//software/r52/integration_test:master_integration_test",
    ],
    "//conditions:default": [],
}),
```

**⛔ DO NOT duplicate IT defines in main targets:**
```python
# ❌ WRONG - duplicates defines already provided by dsp_it_macros_test_h dep
defines = select({
    "//:Integration_Testing_enabled": ["Integration_Testing"],
    "//conditions:default": [],
}),
```

### 14.2 Header Library Pattern

**Create separate header library for includes:**
```python
cc_library(
    name = "dsp_integration_test_h",
    hdrs = ["dsp_integration_test.h"],
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [
        "//software/common:radar_look_types",
        "//software/common:rdd_stream_h",
        "//software/common/ipc:ipc_data_h",
    ],
)
```

### 14.3 Adding Integration Test Dependency to Source

**For source files that need to call integration test functions:**
```python
deps = [
    # ... existing deps ...
    # SWE5 Integration Testing: IT defines propagate from dsp_it_macros_test_h
    "//software/bbe32/integration_test:dsp_it_macros_test_h",  # Unconditional - header + defines
] + select({
    "//:Integration_Testing_enabled": [
        "//software/bbe32/integration_test:dsp_integration_test_lib",  # Conditional - implementation
    ],
    "//conditions:default": [],
}),
# Note: DO NOT add IT defines to this library's defines/local_defines - they propagate from deps.
```

### 14.4 Separate IT Library Pattern (Parallel Compilation)

> **Added 2026-03-02:** When a source file needs to be compiled both with and without IT defines,
> create a **separate** `*_integration_test_lib` target that compiles the SAME source file
> with IT defines enabled. This keeps the original production library clean.

**Why:** The original library targets (`dsp_setup_lib`, `dopplerproc_ifc_lib`, etc.) should NOT
contain IT defines in production builds. A separate IT library target compiles the same `.c` file
with `Integration_Testing` / `Anglefinding_IT` defines and links against the IT implementation library.

**Pattern (R52 side — `software/r52/dsp_setup/BUILD`):**
```python
# Original production library — NO IT defines
cc_library(
    name = "dsp_setup_lib",
    srcs = ["src/dsp_setup.c"],
    defines = [],  # Clean — no IT defines
    deps = [
        # ... main deps ...
        "//software/r52/integration_test:master_it_macros_test_h",  # Unconditional (for #include)
    ],
)

# Separate IT library — SAME source, WITH IT defines
cc_library(
    name = "dsp_setup_integration_test_lib",
    srcs = ["src/dsp_setup.c"],
    defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    deps = [
        # ... same main deps as dsp_setup_lib ...
        "//software/r52/integration_test:master_it_macros_test_h",
    ] + select({
        "//:Integration_Testing_enabled": [
            "//software/r52/integration_test:master_integration_test",
        ],
        "//conditions:default": [],
    }),
)
```

**Pattern (BBE32 side — `software/bbe32/src/BUILD`):**
```python
# Separate IT library using local_defines
cc_library(
    name = "dopplerproc_ifc_integration_test_lib",
    srcs = ["doppler_process_ifc.c"],
    local_defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    deps = [
        # ... same deps as dopplerproc_ifc_lib ...
        "//software/bbe32/integration_test:dsp_it_macros_test_h",
    ] + select({
        "//:Integration_Testing_enabled": [
            "//software/bbe32/integration_test:dsp_integration_test_lib",
        ],
        "//:ANGLEFINDING_enabled": [
            "//software/bbe32/integration_test:dsp_integration_test_lib",
        ],
        "//conditions:default": [],
    }),
)
```

**All Separate IT Libraries Created:**

| IT Library Target | Original Target | BUILD File | Source File |
|---|---|---|---|
| `dsp_setup_integration_test_lib` | `dsp_setup_lib` | `software/r52/dsp_setup/BUILD` | `dsp_setup.c` |
| `dopplerproc_ifc_integration_test_lib` | `dopplerproc_ifc_lib` | `software/bbe32/src/BUILD` | `doppler_process_ifc.c` |
| `angle_finding_project_interface_integration_test_lib` | `angle_finding_project_interface_lib` | `software/bbe32/src/BUILD` | `anglefinding_project_interface.c` |
| `ipc_dsp_integration_test_lib` | `ipc_dsp_lib` | `software/bbe32/src/BUILD` | `ipc_dsp.c` |

**Key Rules:**
1. The IT library has **the same `srcs`** as the original library
2. The IT library has **the same `deps`** as the original, plus conditional IT deps
3. The IT library adds **IT defines** via `local_defines` or `defines` + `select()`
4. The original library keeps `dsp_it_macros_test_h` / `master_it_macros_test_h` as an unconditional dep (because the source file unconditionally includes the hook header)
5. The original library does **NOT** have IT defines — it stays clean for production

### 14.5 Header-Only Targets for Implementation Headers

> **Added 2026-03-02:** The hook headers (`dsp_it_macros_test.h`, `master_it_macros_test.h`)
> contain `#include "dsp_integration_test.h"` / `#include "master_integration_test.h"` guarded
> by `#if defined(Integration_Testing)`. Even though the include is conditional, the compiler
> still needs the header file to be findable in the include path. Header-only targets solve this.

```python
# Header-only target — just exposes the .h file, no deps needed
# (all #includes inside the .h are guarded by #if defined(Integration_Testing))
cc_library(
    name = "dsp_integration_test_h",
    hdrs = ["dsp_integration_test.h"],
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [],  # Safe — all includes inside are #ifdef-guarded
)

cc_library(
    name = "master_integration_test_h",
    hdrs = ["master_integration_test.h"],
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [],  # Safe — all includes inside are #ifdef-guarded
)
```

**These are depended on by the hook header targets:**
```python
cc_library(
    name = "dsp_it_macros_test_h",
    hdrs = ["dsp_it_macros_test.h"],
    deps = [":dsp_integration_test_h"],  # So compiler can find dsp_integration_test.h
)
```

---

## 15. HOOK INTEGRATION PATTERNS

### 15.1 Macro-Based Hook Pattern

**Define hook macro with guards:**
```c
#if defined(Integration_Testing) && defined(Anglefinding_IT)
   #include "dsp_integration_test.h"
   #define SIT_AF_INPUT(ptr) SIT_AF_Input_Receiver(ptr)
#else
   #define SIT_AF_INPUT(ptr) /*No operation*/
#endif
```

**Use hook in function:**
```c
void Angle_Finding_Process_Init(const Angle_Finding_Input_T *input_ptr)
{
   // ... processing code ...

   SIT_AF_INPUT(input_ptr);  /* Integration test hook */
}
```

### 15.2 Hook Placement Guidelines

| Hook Type | When to Call | Location |
|-----------|--------------|----------|
| **Provider** | After data is populated | End of data preparation function |
| **Receiver** | After data is received/copied | After IPC receive or copy |
| **IPC Send** | Before header update | Before `header_update()` call |
| **IPC Receive** | After payload extraction | After extracting from IPC buffer |

### 15.3 Multiple Hook Macros

**When multiple hooks needed in same file:**
```c
#if defined(Integration_Testing)
   #include "master_integration_test.h"
   #define SIT_M2D_SEND(ptr) SIT_IPC_Transfer_M2D_Send(ptr)
#else
   #define SIT_M2D_SEND(ptr) /*No operation*/
#endif

#if defined(Integration_Testing) && defined(Anglefinding_IT)
   #define SIT_M2D_ONETIME(ptr) SIT_M2D_Onetime_Provider(ptr)
   #define SIT_M2D_PERLOOK(ptr) SIT_M2D_Perlook_Provider(ptr)
#else
   #define SIT_M2D_ONETIME(ptr) /*No operation*/
   #define SIT_M2D_PERLOOK(ptr) /*No operation*/
#endif
```

---

## 16. CENTRALIZED HOOK HEADERS

### 16.1 Architecture Overview

**Problem:** Prior to this improvement, every C file that needed SWE5 hooks had to duplicate the same macro definitions, leading to:
- Code duplication across multiple files
- Maintenance burden when adding new hooks
- Inconsistency risks
- Increased TiCS dead code complexity

**Solution:** Centralized hook headers that consolidate all SWE5 macro definitions per core:
- **`master_it_macros_test.h`** - All R52 master core hook macros
- **`dsp_it_macros_test.h`** - All BBE32 DSP core hook macros

**Key Benefits:**
- ✅ Single source of truth for all hook definitions
- ✅ One `#include` statement instead of repeated macro definitions
- ✅ Easier to add new hooks (one location only)
- ✅ Consistent macro naming and behavior
- ✅ Follows TiCS naming convention (`*_test.h`)
- ✅ Properly guarded for conditional compilation
- ✅ Separate BUILD targets for clean dependency management

### 16.2 Hook Header File Structure

#### R52 Core: `master_it_macros_test.h`

**Location:** `software/r52/integration_test/master_it_macros_test.h`

**File Structure:**
```c
/**
 * @file master_it_macros_test.h
 * @brief Centralized SWE5 Integration Testing Hooks for R52 Core
 */

#ifndef MASTER_IT_MACROS_TEST_H
#define MASTER_IT_MACROS_TEST_H

/*===========================================================================*
 * Header Includes
 *===========================================================================*/
#if defined(Integration_Testing)
   #include "master_integration_test.h"
#endif

/*===========================================================================*
 * SWE5 Integration Testing Hooks - Debug Control
 *===========================================================================*/
#if defined(Integration_Testing)
extern volatile bool SIT_Run;

#define SIT_WAIT_FOR_DEBUGGER()                                              \
   do                                                                        \
   {                                                                         \
      while (!SIT_Run)                                                       \
      {                                                                      \
         /* Wait for debugger to set SIT_Run = true */                      \
      }                                                                      \
   } while (0)
#else
   #define SIT_WAIT_FOR_DEBUGGER() /* No-op */
#endif

/*===========================================================================*
 * SWE5 Integration Testing Hooks - Anglefinding Test Points
 *===========================================================================*/
#if defined(Integration_Testing) && defined(Anglefinding_IT)
   #define SIT_M2D_ONETIME(ptr)             SIT_M2D_Onetime_Provider(ptr)
   #define SIT_USC_CALIB_DATA_PROVIDER(ptr) SIT_USC_Calib_Data_Provider(ptr)
   #define SIT_M2D_PERLOOK(ptr)             SIT_M2D_Perlook_Provider(ptr)
#else
   #define SIT_M2D_ONETIME(ptr)             /* No operation */
   #define SIT_USC_CALIB_DATA_PROVIDER(ptr) /* No operation */
   #define SIT_M2D_PERLOOK(ptr)             /* No operation */
#endif

/*===========================================================================*
 * SWE5 Integration Testing Hooks - IPC Transfer
 *===========================================================================*/
#if defined(Integration_Testing)
   #define SIT_IPC_TRANSFER_M2D_SEND(ptr)    SIT_IPC_Transfer_M2D_Send(ptr)
   #define SIT_IPC_TRANSFER_D2M_RECEIVE(ptr) SIT_IPC_Transfer_D2M_Receive(ptr)
#else
   #define SIT_IPC_TRANSFER_M2D_SEND(ptr)    /* No operation */
   #define SIT_IPC_TRANSFER_D2M_RECEIVE(ptr) /* No operation */
#endif

#endif /* MASTER_IT_MACROS_TEST_H */
```

#### BBE32 Core: `dsp_it_macros_test.h`

**Location:** `software/bbe32/integration_test/dsp_it_macros_test.h`

**File Structure:**
```c
/**
 * @file dsp_it_macros_test.h
 * @brief Centralized SWE5 Integration Testing Hooks for BBE32 DSP Core
 */

#ifndef DSP_IT_MACROS_TEST_H
#define DSP_IT_MACROS_TEST_H

/*===========================================================================*
 * Header Includes
 *===========================================================================*/
#if defined(Integration_Testing)
   #include "dsp_integration_test.h"
#endif

/*===========================================================================*
 * SWE5 Integration Testing Hooks - Anglefinding Test Points
 *===========================================================================*/
#if defined(Integration_Testing) && defined(Anglefinding_IT)
   #define SIT_AF_INPUT(ptr)       SIT_AF_Input_Receiver(ptr)
   #define SIT_D2M_MSG_BUFFER(ptr) SIT_D2M_Msg_Buffer_Provider(ptr)
#else
   #define SIT_AF_INPUT(ptr)       /* No operation */
   #define SIT_D2M_MSG_BUFFER(ptr) /* No operation */
#endif

/*===========================================================================*
 * SWE5 Integration Testing Hooks - IPC Transfer
 *===========================================================================*/
#if defined(Integration_Testing)
   #define SIT_IPC_TRANSFER_M2D_RECEIVE(ptr) SIT_IPC_Transfer_M2D_Receive(ptr)
   #define SIT_IPC_TRANSFER_D2M_SEND(ptr)    SIT_IPC_Transfer_D2M_Send(ptr)
#else
   #define SIT_IPC_TRANSFER_M2D_RECEIVE(ptr) /* No operation */
   #define SIT_IPC_TRANSFER_D2M_SEND(ptr)    /* No operation */
#endif

#endif /* DSP_IT_MACROS_TEST_H */
```

### 16.3 BUILD File Pattern for Hook Headers

#### Critical: Two-Tier Conditional Dependency Structure

**Key Principle:** Hook headers must be **UNCONDITIONALLY** available as dependencies, while implementation libraries are **CONDITIONALLY** linked. This allows:
- ✅ Source files use unconditional `#include` statements (no `#ifdef` needed)
- ✅ Headers provide empty macros when Integration Testing is disabled
- ✅ Implementation code only linked when Integration Testing is enabled
- ✅ Zero overhead in production builds

#### Create Separate Header-Only Library Target

**Pattern for R52 (software/r52/integration_test/BUILD):**
```python
load("@rules_cc//cc:defs.bzl", "cc_library")

# Separate header-only library for hook macros
# NOTE: This target should be UNCONDITIONALLY available in source file deps
cc_library(
    name = "master_it_macros_test_h",
    hdrs = ["master_it_macros_test.h"],
    defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [],  # No dependencies - pure macro definitions
)

# Main integration test library depends on hooks
cc_library(
    name = "master_integration_test",
    srcs = ["master_integration_test.c"],
    hdrs = ["master_integration_test.h"],
    defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    linkopts = ["-lm"],
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [
        "//software/common/calibrations/usc:usc_h",
        "//software/common/ipc:ipc_data_h",
        ":master_it_macros_test_h",  # Dependency on hooks header
    ],
    alwayslink = True,
)
```

**Pattern for BBE32 (software/bbe32/integration_test/BUILD):**
```python
load("@rules_cc//cc:defs.bzl", "cc_library")

# Separate header-only library for hook macros
# NOTE: This target should be UNCONDITIONALLY available in source file deps
cc_library(
    name = "dsp_it_macros_test_h",
    hdrs = ["dsp_it_macros_test.h"],
    defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [],  # No dependencies - pure macro definitions
)

# Main integration test library depends on hooks
cc_library(
    name = "dsp_integration_test_lib",
    srcs = ["dsp_integration_test.c"],
    hdrs = ["dsp_integration_test.h"],
    defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    includes = ["."],
    visibility = ["//visibility:public"],
    deps = select({
        "//:Integration_Testing_enabled": [
            "//software/common/calibrations/usc:usc_h",
            "//software/common/ipc:ipc_data_h",
            "//software/bbe32/inc:angle_finding_project_interface_h",
            "//software/common:rdd_stream_h",
            "//software/common:stream_header_h",
            "//software/common:UDPLoggingStreamHeaders",
            "//software/common/satellite_can_standalone_common:detection_stream_module_h",
            "//software/common/satellite_can_standalone_common:rdd_stream_module_h",
            ":dsp_it_macros_test_h",  # Dependency on hooks header
        ],
        "//conditions:default": [],
    }),
    alwayslink = True,
)
```

#### Add Hook Header Dependencies to Source Files

**R52 Source File BUILD Pattern (e.g., dsp_setup/BUILD):**

```python
cc_library(
    name = "dsp_setup_lib",
    srcs = ["src/dsp_setup.c"],
    defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    deps = [
        # ... main dependencies ...
        "//software/r52/integration_test:master_it_macros_test_h",  # ✅ UNCONDITIONAL
    ] + select({
        "//:Integration_Testing_enabled": [
            "//software/r52/integration_test:master_integration_test",  # ✅ CONDITIONAL
        ],
        "//conditions:default": [],
    }),
)
```

**Key Pattern:**
1. **Hook header**: Always in `deps` list (unconditional)
2. **Implementation library**: In `select()` block (conditional)

**BBE32 Source File BUILD Pattern (e.g., src/BUILD):**

For libraries with `local_defines` (doppler, IPC, etc.):
```python
cc_library(
    name = "dopplerproc_ifc_lib",
    srcs = ["doppler_process_ifc.c"],
    # ✅ IMPORTANT: Must have local_defines for conditional compilation
    local_defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    deps = [
        # ... main dependencies ...
        "//software/bbe32/integration_test:dsp_it_macros_test_h",  # ✅ UNCONDITIONAL
    ] + select({
        "//:Integration_Testing_enabled": [
            "//software/bbe32/integration_test:dsp_integration_test_lib",  # ✅ CONDITIONAL
        ],
        "//:ANGLEFINDING_enabled": [
            "//software/bbe32/integration_test:dsp_integration_test_lib",  # ✅ CONDITIONAL
        ],
        "//conditions:default": [],
    }),
)
```

For libraries with `defines` (angle finding, etc.):
```python
cc_library(
    name = "angle_finding_project_interface_lib",
    srcs = ["anglefinding_project_interface.c"],
    defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    deps = [
        # ... main dependencies ...
        "//software/bbe32/integration_test:dsp_it_macros_test_h",  # ✅ UNCONDITIONAL
    ] + select({
        "//:Integration_Testing_enabled": [
            "//software/bbe32/integration_test:dsp_integration_test_lib",  # ✅ CONDITIONAL
        ],
        "//:ANGLEFINDING_enabled": [
            "//software/bbe32/integration_test:dsp_integration_test_lib",  # ✅ CONDITIONAL
        ],
        "//conditions:default": [],
    }),
)
```

**R52 main.c Binary Pattern (software/r52/BUILD):**
```python
cc_binary_windriver(
    name = "r52App",
    srcs = ["main.c"],
    defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    deps = [
        # ... many dependencies ...
        "//software/r52/integration_test:master_it_macros_test_h",  # ✅ UNCONDITIONAL
    ] + select({
        "//:Integration_Testing_enabled": [
            "//software/r52/integration_test:master_integration_test",  # ✅ CONDITIONAL
        ],
        "//conditions:default": [],
    }),
)
```

#### Common Pitfalls and Solutions

**❌ WRONG: Conditional hook header dependency**
```python
deps = [...] + select({
    "//:Integration_Testing_enabled": [
        "//software/r52/integration_test:master_it_macros_test_h",  # ❌ Causes "file not found" errors
    ],
    "//conditions:default": [],
})
```

**✅ CORRECT: Unconditional hook header, conditional implementation**
```python
deps = [
    # ...
    "//software/r52/integration_test:master_it_macros_test_h",  # ✅ Always available
] + select({
    "//:Integration_Testing_enabled": [
        "//software/r52/integration_test:master_integration_test",  # ✅ Linked conditionally
    ],
    "//conditions:default": [],
})
```

**❌ WRONG: Missing local_defines for libraries using both conditional attributes**
```python
cc_library(
    name = "dopplerproc_ifc_lib",
    srcs = ["doppler_process_ifc.c"],
    local_defines = [],  # ❌ Macros won't be defined!
    deps = [...]
)
```

**✅ CORRECT: Proper local_defines + defines separation**
```python
cc_library(
    name = "dopplerproc_ifc_lib",
    srcs = ["doppler_process_ifc.c"],
    local_defines = select({  # ✅ For this target only
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }),
    deps = [...]  # ✅ Headers get defines from hook library
)
```

### 16.4 Usage in Source Files

**Before (Duplicated Macros):**
```c
// software/r52/dsp_setup/src/dsp_setup.c
#include "vid_stream.h"

#if defined(Integration_Testing)
   #include "master_integration_test.h"
#endif

/* SWE5 Integration Testing Macros */
#if defined(Integration_Testing) && defined(Anglefinding_IT)
   #define SIT_M2D_ONETIME(ptr)             SIT_M2D_Onetime_Provider(ptr)
   #define SIT_USC_CALIB_DATA_PROVIDER(ptr) SIT_USC_Calib_Data_Provider(ptr)
   #define SIT_M2D_PERLOOK(ptr)             SIT_M2D_Perlook_Provider(ptr)
#else
   #define SIT_M2D_ONETIME(ptr)             /* No operation */
   #define SIT_USC_CALIB_DATA_PROVIDER(ptr) /* No operation */
   #define SIT_M2D_PERLOOK(ptr)             /* No operation */
#endif

#if defined(Integration_Testing)
   #define SIT_IPC_TRANSFER_M2D_SEND(ptr)    SIT_IPC_Transfer_M2D_Send(ptr)
   #define SIT_IPC_TRANSFER_D2M_RECEIVE(ptr) SIT_IPC_Transfer_D2M_Receive(ptr)
#else
   #define SIT_IPC_TRANSFER_M2D_SEND(ptr)    /* No operation */
   #define SIT_IPC_TRANSFER_D2M_RECEIVE(ptr) /* No operation */
#endif

// ... implementation ...
void Some_Function(void)
{
   SIT_M2D_ONETIME(data_ptr);
}
```

**After (Centralized Hook Header):**
```c
// software/r52/dsp_setup/src/dsp_setup.c
#include "vid_stream.h"

/*===========================================================================*
 * SWE5 Integration Testing Support
 *===========================================================================*/
#include "master_it_macros_test.h"  // ✅ UNCONDITIONAL include

// ... implementation ...
void Some_Function(void)
{
   SIT_M2D_ONETIME(data_ptr);  // Macro defined in master_it_macros_test.h
                                // Expands to no-op if Integration_Testing not defined
}
```

**Result:** ~29 lines of macro boilerplate eliminated, replaced with single `#include` statement!

#### Why Unconditional Include Works

The hook header itself contains conditional guards that provide empty macros when Integration Testing is disabled:

```c
// Inside master_it_macros_test.h:
#if defined(Integration_Testing) && defined(Anglefinding_IT)
   #define SIT_M2D_ONETIME(ptr) SIT_M2D_Onetime_Provider(ptr)
#else
   #define SIT_M2D_ONETIME(ptr) /* No operation */  // ✅ Zero overhead
#endif
```

This pattern ensures:
- ✅ **Build succeeds** without IT flags (header always found, macros expand to nothing)
- ✅ **Build succeeds** with IT flags (header found, macros expand to function calls)
- ✅ **Zero runtime overhead** in production (empty macros optimized away completely)
- ✅ **No conditional includes** needed in source files (cleaner code)

#### Refactored Files Summary

Five files were refactored to use centralized hook headers:

| Core | File | Lines Removed | Lines Added |
|------|------|---------------|-------------|
| R52 | `main.c` | 30 | 1 |
| R52 | `dsp_setup/src/dsp_setup.c` | 29 | 1 |
| BBE32 | `src/anglefinding_project_interface.c` | 11 | 1 |
| BBE32 | `src/doppler_process_ifc.c` | 9 | 1 |
| BBE32 | `src/ipc_dsp.c` | 14 | 1 |
| **TOTAL** | **5 files** | **93 lines** | **5 lines** |

**Net savings:** 88 lines of duplicate macro code eliminated!

### 16.5 Benefits and Best Practices

#### Benefits

1. **Eliminates Code Duplication**
   - Before: 5 files × ~20-30 lines each = 93 lines of duplicated macros
   - After: 2 centralized headers (~150 lines total each, single source of truth)
   - Net savings: 88 lines eliminated

2. **Easier Maintenance**
   - Add new hook: Update 1 header file instead of N source files
   - Modify hook behavior: Change once, affects all users
   - Review changes: Single diff location

3. **Consistency Guaranteed**
   - All files use same macro definitions
   - No risk of copy-paste errors
   - Uniform naming and behavior

4. **Better TiCS Compliance**
   - Follows `*_test.h` naming convention
   - No dead code in source files (macros in headers)
   - Cleaner cyclomatic complexity metrics

5. **Clean Dependency Management**
   - Separate BUILD targets for hooks
   - Two-tier conditional structure (headers always available, implementations conditional)
   - Clear visibility control

6. **Zero Overhead in Production**
   - Empty macros when Integration_Testing disabled
   - Compiler optimizes away completely
   - No runtime cost, no code size increase

#### Best Practices

**✅ DO: Use Unconditional Includes**
```c
// No #ifdef needed around include!
#include "master_it_macros_test.h"
SIT_M2D_ONETIME(ptr);
```
**Why:** Hook header contains its own `#ifdef` guards providing empty macros when disabled.

**❌ DON'T: Conditionally Include Hook Headers**
```c
// WRONG - causes build errors without IT flags
#if defined(Integration_Testing)
   #include "master_it_macros_test.h"
#endif
```

**✅ DO: Use Two-Tier Dependency Structure**
```python
deps = [
    # ... other deps ...
    "//software/r52/integration_test:master_it_macros_test_h",  # ✅ Unconditional
] + select({
    "//:Integration_Testing_enabled": [
        "//software/r52/integration_test:master_integration_test",  # ✅ Conditional
    ],
    "//conditions:default": [],
})
```

**❌ DON'T: Make Hook Header Dependencies Conditional**
```python
# WRONG - causes "file not found" errors
deps = [...] + select({
    "//:Integration_Testing_enabled": [
        "//software/r52/integration_test:master_it_macros_test_h",  # ❌ Error!
        "//software/r52/integration_test:master_integration_test",
    ],
})
```

**✅ DO: Add local_defines for Libraries with Multiple Conditional Features**
```python
cc_library(
    name = "dopplerproc_ifc_lib",
    srcs = ["doppler_process_ifc.c"],
    local_defines = select({  # ✅ Needed for proper macro expansion
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }) + select({
        "//:ANGLEFINDING_enabled": ["Anglefinding_IT"],
        "//conditions:default": [],
    }),
    # ...
)
```

**✅ DO: Add New Hooks to Centralized Headers**
- Edit `master_it_macros_test.h` or `dsp_it_macros_test.h`
- Add appropriate guards (`Integration_Testing`, `Anglefinding_IT`, etc.)
- Provide empty macro for non-IT builds
- Document with Doxygen comments

**❌ DON'T: Define Hooks Inline in Source Files**
```c
// WRONG - duplicates definitions
#if defined(Integration_Testing)
   #define SIT_M2D_ONETIME(ptr) SIT_M2D_Onetime_Provider(ptr)
#else
   #define SIT_M2D_ONETIME(ptr) /* No operation */
#endif
```

**✅ DO: Follow TiCS Naming Convention**
- Hook headers MUST end with `_test.h`
- Examples: `master_it_macros_test.h`, `dsp_it_macros_test.h`, `tracker_it_macros_test.h`

**✅ DO: Keep Hooks Header-Only**
- No `.c` file needed
- Pure macro definitions and extern declarations
- `deps = []` in BUILD file (no dependencies)
- Zero runtime overhead when Integration_Testing disabled

**✅ DO: Verify Both Build Configurations**
```bash
# Without IT flags (production build)
bazelisk build //:gen8 --config=flr8

# With IT flags (test build)
bazelisk build //:gen8 --config=flr8 --Integration_Testing=true --Anglefinding_IT=true
```
Both should build successfully!

#### Migration Pattern for New Hooks

**When adding a new module (e.g., Tracker):**

1. Create `software/r52/integration_test/tracker_it_macros_test.h`:
   ```c
   #ifndef TRACKER_IT_MACROS_TEST_H
   #define TRACKER_IT_MACROS_TEST_H

   #if defined(Integration_Testing) && defined(Tracker_IT)
      #include "tracker_integration_test.h"
      #define SIT_TRACKER_INPUT(ptr) SIT_Tracker_Input_Receiver(ptr)
   #else
      #define SIT_TRACKER_INPUT(ptr) /* No operation */
   #endif

   #endif /* TRACKER_IT_MACROS_TEST_H */
   ```

2. Add BUILD target:
   ```python
   cc_library(
       name = "tracker_it_macros_test_h",
       hdrs = ["tracker_it_macros_test.h"],
       defines = select({...}),
       visibility = ["//visibility:public"],
   )
   ```

3. Use in source files:
   ```c
   #include "tracker_it_macros_test.h"

   void Process_Tracker_Data(Tracker_Data_T *data)
   {
      SIT_TRACKER_INPUT(data);
   }
   ```

#### File Naming Convention Summary

| Core | Hook Header Name | Implementation Header | Implementation Source |
|------|------------------|----------------------|----------------------|
| R52 | `master_it_macros_test.h` | `master_integration_test.h` | `master_integration_test.c` |
| BBE32 | `dsp_it_macros_test.h` | `dsp_integration_test.h` | `dsp_integration_test.c` |
| Tracker | `tracker_it_macros_test.h` | `tracker_integration_test.h` | `tracker_integration_test.c` |

**Pattern:** `{module}_it_macros_test.h` for hook macros, `{module}_integration_test.{h,c}` for implementation


---

## Appendix A: Quick Reference Checklist

### Adding a New Interface
1. ✅ Add to `INTERFACE_ENUM_LIST` with unique Work Item ID
2. ✅ Guard with appropriate `IF_MODULENAME()` macro
3. ✅ Add case in Provider function (stub the data)
4. ✅ Add case in Receiver function (validate the data)
5. ✅ Add boundary checks for arrays
6. ✅ Add `#if (CONST >= N)` guards for indexed arrays
7. ✅ Update BUILD deps if new headers needed

### Adding a New Module
1. ✅ Create `IF_MODULENAME()` guard macro
2. ✅ Add Bazel bool_flag in root BUILD
3. ✅ Add config_setting in root BUILD
4. ✅ Add flag alias to `.bazelrc`
5. ✅ Add interfaces to INTERFACE_ENUM_LIST
6. ✅ Create Provider/Receiver functions
7. ✅ Add hooks in relevant source files
8. ✅ Update BUILD dependencies

### Data Type Mapping
| C Type | DataType_T | Notes |
|--------|------------|-------|
| `uint8_t` | `DATATYPE_UINT8` | |
| `uint16_t` | `DATATYPE_UINT16` | |
| `uint32_t` | `DATATYPE_UINT32` | |
| `int16_t` | `DATATYPE_INT16` | |
| `int32_t` | `DATATYPE_INT32` | |
| `float` | `DATATYPE_FLOAT32` | |
| `enum` | `DATATYPE_ENUM` | Treated as uint8_t |
| `s8p23_T` | `DATATYPE_INT32` | Fixed point |
| `s10p21_T` | `DATATYPE_INT32` | Fixed point |

---

## Appendix B: Gen8 Implementation Status

### Completed Tasks
- [x] Root BUILD flags added (`Integration_Testing`, `Anglefinding_IT`) — at BUILD lines 288–310
- [x] `.bazelrc` flag aliases added — at .bazelrc lines 28–29 (`common` scope)
- [x] `SIT_Data_T` added to `ipc_data.h` — at lines 235–241
- [x] `SIT_Test_Result_T` enum with `TEST_SKIPPED` value — at lines 226–232
- [x] R52 integration test library created (4 files, 1585 total lines)
- [x] BBE32 integration test library created (4 files, 1956 total lines)
- [x] AF Input/Output interfaces implemented
- [x] USC calibration interfaces with `#if (AF_CAL_SEC_N >= N)` guards
- [x] All Gen8 type mappings updated
- [x] Centralized hook headers (master_it_macros_test.h, dsp_it_macros_test.h)
- [x] String-to-enum refactor (`Test_Interface_Name_T` via `INTERFACE_ENUM_LIST` — unique Work Item IDs)
- [x] Aptiv C Coding Standards applied to all SWE5 files
- [x] Header-only targets for implementation headers (dsp_integration_test_h, master_integration_test_h)
- [x] Separate IT library for dsp_setup (dsp_setup_integration_test_lib)
- [x] Separate IT libraries for BBE32 src (dopplerproc_ifc_integration_test_lib, angle_finding_project_interface_integration_test_lib, ipc_dsp_integration_test_lib)
- [ ] Linker script conditional memory allocation (NOT implemented — use SRAM1 only)

### Current File Sizes
| File | Lines | Notes |
|------|-------|-------|
| master_integration_test.h | 372 | 21 interfaces in INTERFACE_ENUM_LIST |
| master_integration_test.c | 978 | 19 USC cases in SIT_USC_Calib_Data_Provider |
| master_it_macros_test.h | 195 | 6 macro hooks |
| dsp_integration_test.h | 423 | 36 interfaces in INTERFACE_ENUM_LIST |
| dsp_integration_test.c | 1317 | All code in SRAM1 sections |
| dsp_it_macros_test.h | 161 | 4 macro hooks |
| R52 integration_test/BUILD | 49 | 3 cc_library targets (master_integration_test_h, master_it_macros_test_h, master_integration_test) |
| BBE32 integration_test/BUILD | 62 | 3 cc_library targets (dsp_integration_test_h, dsp_it_macros_test_h, dsp_integration_test_lib) |
| R52 dsp_setup/BUILD | — | Contains dsp_setup_lib + dsp_setup_integration_test_lib |
| BBE32 src/BUILD | ~515 | Contains 3 separate IT libs: dopplerproc_ifc_integration_test_lib, angle_finding_project_interface_integration_test_lib, ipc_dsp_integration_test_lib |
| ipc_data.h (IT additions) | ~20 | SIT_Data_T, SIT_Test_Result_T, conditional sit_data fields |

### Known Differences from Gen7_V2
- AF_CAL_SEC_N = 2 (Gen7 = 4) - calmat3/4 cases use `TEST_SKIPPED`
- No A53 core - D2A functions removed
- Pointer-based access for AF input (p_veh_speed, p_radar_position)
- Output path uses `afbb_det_output.af_data.theta/phi`

---

## Appendix C: Key Constants and Macros

```c
#define SWE5_VALIDATION_TOLERANCE (1e-5F)  /* Tolerance for floating point comparison */
#define AF_CAL_SEC_N (2)                    /* Number of calibration sections (Gen8) */
/* Note: Gen7_V2 has AF_CAL_SEC_N = 4 */
```

---

## Appendix D: Example Complete Interface Implementation

### Example: Adding a New Tracker Interface (Ri_tracker_target_count)

**Given Information:**
- Work Item: 600001
- Interface Name: `Ri_tracker_target_count`
- Data Path: `tracker_output->num_targets`
- Data Type: `uint16_t` (scalar)
- Module: Tracker

**Step 1: Add Guard Macro (dsp_integration_test.h)**
```c
#if defined(Tracker_IT)
   #define IF_TRACKER(x) x
#else
   #define IF_TRACKER(x)
#endif
```

**Step 2: Add to INTERFACE_ENUM_LIST (dsp_integration_test.h)**
```c
#define INTERFACE_ENUM_LIST                                                      \
   /* ... existing interfaces ... */                                              \
   /* Tracker Module Interfaces */                                                \
   IF_TRACKER(DCS_X(Ri_tracker_target_count, 600001U))  /* WI-600001: num_targets */
```

**Step 3: Add Provider Case (in appropriate Provider function)**
```c
case Ri_tracker_target_count: /* WI-600001: num_targets */
   Set_Shared_Ptr(&DSP_Shared_Pi, &tracker_output->num_targets, sizeof(uint16_t), 1, Ri_tracker_target_count, DATATYPE_UINT16);
   Stub_Scalar_Values(&tracker_output->num_targets, DATATYPE_UINT16, DSP_Stub_Val);
   break;
```

**Step 4: Add Receiver Case (in appropriate Receiver function)**
```c
case Ri_tracker_target_count:
{
   is_valid = Validate_Scalar_Values(&tracker_input->num_targets, DATATYPE_UINT16, DSP_Stub_Val);
   Set_Shared_Ptr(&DSP_Shared_Ri, &tracker_input->num_targets, sizeof(uint16_t), 1, Ri_tracker_target_count, DATATYPE_UINT16);
}
break;
```

> **Note:** Add `Ri_tracker_target_count` to `INTERFACE_ENUM_LIST` in the appropriate header (`master_integration_test.h` or `dsp_integration_test.h`).

**Step 5: Update BUILD (add new flag)**
```python
# In root BUILD file
bool_flag(
    name = "Tracker_IT",
    build_setting_default = False,
    visibility = ["//visibility:public"],
)

config_setting(
    name = "TRACKER_enabled",
    flag_values = {":Tracker_IT": "true"},
    visibility = ["//visibility:public"],
)
```

**Step 6: Update .bazelrc**
```bazelrc
common --flag_alias=Tracker_IT=//:Tracker_IT
```

---

*Document Version: 4.0*
*Last Updated: March 02, 2026*
*Based on Gen7_V2 commits: 3e6f42f4, 9ed47bef*
*Adapted for Gen8 (Core_Radar_Gen8_iND13400)*
*Major Updates:*
*- Added centralized hook headers architecture (master_it_macros_test.h, dsp_it_macros_test.h)*
*- Added AI Assistant Instructions for build validation and self-debugging*
*- Implemented two-tier conditional dependency structure (headers unconditional, implementations conditional)*
*- Updated Section 16 with BUILD patterns, common pitfalls, and best practices*
*- Refactored 5 source files eliminating 88 lines of duplicate macro code*
*- v3.8: Full audit against codebase — updated G_Pointer_T, file paths, function locations, linker status, code patterns, file sizes*
*- v3.9: Added comprehensive Aptiv C Coding Standards (ESGW_4-2_PE-SWx_00-01-A02_EN) section with full templates for file headers, section banners, function comment blocks, file footers, header guards, extern "C" patterns, and variable/comment conventions*
*- v4.0: Added Separate IT Library pattern (Section 14.4) — created 4 parallel compilation targets (dsp_setup_integration_test_lib, dopplerproc_ifc_integration_test_lib, angle_finding_project_interface_integration_test_lib, ipc_dsp_integration_test_lib). Added header-only targets (Section 14.5) for dsp_integration_test.h and master_integration_test.h. Removed duplicate IT defines from main targets. Build verified with --Integration_Testing --Anglefinding_IT flags.*

### **Lessons Learned (AI Migration Mistakes)**

| Date | Lesson | Impact |
|------|--------|--------|
| 2026-02-26 | **CRITICAL: During Gen7→Gen8 migration, only 2 out of 16 interface cases were migrated in `SIT_AF_Input_Receiver`.** The entire RDD data chain (beam_vector, snr, range, range_rate, sp_fail_flag, num_detections, rdop_amp, cr_response, look_type) was missing, causing test failures for all those interfaces. | High - Most SWE5 tests failed |
| 2026-02-26 | **`SIT_IPC_Transfer_D2M_Send` only sent `test_result` back to R52, but Gen7_V2 sends ALL 4 fields** (test_interface_number, stub_value, no_of_elements_in_stub_array, test_result). R52 side needs all fields to correlate results. | High - R52 lost context |
| 2026-02-26 | **Gen8 `Angle_Finding_Input_T` uses pointer fields** (e.g., `p_rdd1_bv`, `p_rdd2_snr`, `p_look_id`) instead of Gen7's direct array/struct members (e.g., `rdd1_bv`, `rdd2_snr`, `look_data.look_id`). Always check the actual struct mapping in `anglefinding_project_interface.c` when migrating. | Medium - Type access pattern |
| 2026-02-26 | **Gen8 AF Input does NOT include `scan_index`, `look_index`, `range_coverage`, `doppler_coverage`** — these fields exist in `Look_Data_T` but are not mapped into `Angle_Finding_Input_T`. The provider stubs them in D2M, but the receiver CANNOT validate them via AF Input. | Medium - Architectural gap |
| 2026-02-26 | **When migrating functions, ALWAYS compare switch-case counts** between Gen7_V2 and Gen8. If a function has 16 cases in Gen7 but only 2 in Gen8, something was clearly missed. | Process - Checklist item |
| 2026-02-26 | **`SIT_USC_Calib_Data_Provider` was left as empty placeholder** on the R52/Master side while `SIT_AF_USC_Receiver` on BBE32 had complete validation. ALWAYS check Provider↔Receiver pairs across cores: if one side has implementation, the other MUST have matching coverage. | High - All USC tests failed |
| 2026-02-26 | **Gen8 has NO A53 core.** Gen7 had `appl_integration_test.c` with `SIT_IPC_Transfer_D2A_Receive`, `SIT_IPC_Transfer_A2M_Send`, and `Appl_Final_Res`. These are intentionally NOT needed in Gen8. Do NOT re-add them. | Architectural - No action |
| 2026-02-26 | **ALWAYS cross-check ALL 3 SWE5 files** (master_integration_test.c, dsp_integration_test.c, and Gen7's appl_integration_test.c). Migration gaps can exist in any file, not just the one being actively developed. | Process - Checklist item |
| 2026-02-26 | **Gen7 USC field names use `_str` suffix (e.g., `k_calmat1_az_sin_lut_str`), Gen8 does NOT (e.g., `k_calmat1_az_sin_lut`).** The RCBB test header (`rcbb/module/test/usc_cal.h`) retains Gen7 naming; Gen8 production header (`usc_flr8/cals_c_files/usc_cal.h`) uses clean names without suffix. Always verify field names against the actual Gen8 external header, not RCBB test headers. | High - 20 compile errors |
| 2026-02-26 | **Gen8 `USC_AF_Cal_T` array sizes differ from Gen7:** `k_calmat1_az_sin_lut[151][5]` (Gen8) vs `k_calmat1_az_sin_lut_str[189]` (Gen7), `k_calmat1_lut_az[151]` vs `k_calmat1_lut_az_str[189]`. Always check actual struct field dimensions, not Gen7 constants. | Medium - Wrong loop bounds |
| 2026-02-26 | **Gen8 R52 uses individual named fields** (`k_calmat1_ang_min_az`, `k_calmat2_ang_min_az`) while BBE32 uses array-based access (`cal_ang_min_az[0]`, `cal_ang_min_az[1]`). R52 `SIT_USC_Calib_Data_Provider` must use the named-field pattern — no array indexing possible for calmat on R52 side. | Medium - Struct access pattern |
*- Added continuous improvement loop for knowledge retention*
