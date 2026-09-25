---
applyTo: "**/BUILD,**/.bazelrc,**/MODULE.bazel"
---

# SWE5 Build — Bazel Patterns (Repo-Verified)

## Root BUILD Flags (lines 288–310)
```python
bool_flag(name = "Integration_Testing", build_setting_default = False, visibility = ["//visibility:public"])
config_setting(name = "Integration_Testing_enabled", flag_values = {":Integration_Testing": "true"}, visibility = ["//visibility:public"])
bool_flag(name = "Anglefinding_IT", build_setting_default = False, visibility = ["//visibility:public"])
config_setting(name = "ANGLEFINDING_enabled", flag_values = {":Anglefinding_IT": "true"}, visibility = ["//visibility:public"])
```

## .bazelrc Flag Aliases
```
common --flag_alias=Integration_Testing=//:Integration_Testing
common --flag_alias=Anglefinding_IT=//:Anglefinding_IT
```

## REAL BUILD Pattern: Hook Header Target (`software/bbe32/integration_test/BUILD`)
```python
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
    deps = [":dsp_integration_test_h"],
)
```

## REAL BUILD Pattern: Header-Only Target
```python
cc_library(
    name = "dsp_integration_test_h",
    hdrs = ["dsp_integration_test.h"],
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = ["//software/bbe32/inc:angle_finding_project_interface_h"],
)
```

## REAL BUILD Pattern: Implementation Library
```python
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
            ":dsp_it_macros_test_h",
        ],
        "//conditions:default": [],
    }),
    alwayslink = True,
)
```

## REAL BUILD Pattern: Consumer Target (from `software/bbe32/src/BUILD`)
```python
# angle_finding_project_interface_lib — ACTUAL from repo
cc_library(
    name = "angle_finding_project_interface_lib",
    srcs = ["anglefinding_project_interface.c"],
    deps = [
        "//software/bbe32/integration_test:dsp_it_macros_test_h",  # UNCONDITIONAL flat dep
        "//software/common/ipc:ipc_data_h",
        # ... other deps ...
    ],
)

# dopplerproc_ifc_lib — ACTUAL from repo
cc_library(
    name = "dopplerproc_ifc_lib",
    srcs = ["doppler_process_ifc.c"],
    local_defines = [],  # NO IT defines here — they propagate from hook header
    deps = [
        "//software/bbe32/integration_test:dsp_it_macros_test_h",  # UNCONDITIONAL
        # ... other deps ...
    ],
)

# ipc_dsp_lib — ACTUAL from repo
cc_library(
    name = "ipc_dsp_lib",
    srcs = ["ipc_dsp.c"],
    local_defines = select({
        "//:Build_XCP_FAULT_INJECTION_Test_Cases": ["XCP_FAULT_INJECTION"],
        "//conditions:default": [],
    }),  # Only XCP defines here — NOT IT defines
    deps = [
        "//software/bbe32/integration_test:dsp_it_macros_test_h",  # UNCONDITIONAL
        # ... other deps ...
    ],
)
```

## REAL BUILD Pattern: R52 dsp_setup (`software/r52/dsp_setup/BUILD`)
```python
cc_library(
    name = "dsp_setup_lib",
    srcs = ["src/dsp_setup.c"],
    defines = [],  # NO IT defines — they propagate from hook header
    deps = [
        "//software/r52/integration_test:master_it_macros_test_h",  # UNCONDITIONAL
        # ... other deps ...
    ],
)
```

## CRITICAL: How IT Defines Propagate
1. `dsp_it_macros_test_h` target has `defines = select({"//:Integration_Testing_enabled": ["Integration_Testing"],...})`
2. Consumer adds it as flat (unconditional) dep
3. Bazel propagates defines transitively to consumer
4. Consumer's source sees `Integration_Testing` defined → macros expand to function calls
5. Without `--Integration_Testing=true` flag → defines list is empty → macros expand to no-op

## KEY RULES (from actual repo):
- Hook header dep is ALWAYS unconditional (just listed in `deps = [...]`)
- Hook header target contains the `select()` for defines
- Main targets do NOT duplicate IT defines in their own `defines`/`local_defines`
- `alwayslink = True` on implementation library only
- Separate `select()` per flag — never combined

## Build Commands
```bash
bazelisk build //:gen8 --config=flr8                                              # Production
bazelisk build //:gen8 --config=flr8 --Integration_Testing=true --Anglefinding_IT=true  # IT
```

## Adding Hook Header Dep to New Target
```python
# Just add as flat dep — NO select() wrapper needed
deps = [
    "//software/bbe32/integration_test:dsp_it_macros_test_h",
    # ... other deps ...
]
```
