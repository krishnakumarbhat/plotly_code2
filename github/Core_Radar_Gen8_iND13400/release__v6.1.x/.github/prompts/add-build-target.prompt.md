---
mode: agent
description: "Add a new Bazel build target (cc_library, cc_test, bool_flag)"
---

# Add Build Target

## Input Required
- **Type**: `cc_library` / `cc_test` / `bool_flag` / `filegroup`
- **Name**: Target name
- **Location**: Which BUILD file (package path)
- **Sources**: .c/.cc files
- **Headers**: .h files (public vs private)
- **Dependencies**: Other targets needed

## cc_library Pattern

```python
cc_library(
    name = "my_module_lib",
    srcs = [
        "src/my_module.c",
    ],
    hdrs = [
        "inc/my_module.h",
    ],
    deps = [
        "//software/common:some_dep",
        "//other/package:other_lib",
    ],
    # Use local_defines for THIS target only (preferred):
    local_defines = ["MY_PRIVATE_DEFINE"],
    # Use defines ONLY if dependents need it too (propagates!):
    # defines = ["MY_PUBLIC_DEFINE"],
    visibility = ["//visibility:public"],
)
```

### Key Rules:
- `hdrs` = public API headers (consumers can `#include`)
- `srcs` = implementation files + private headers
- `defines` propagates to ALL dependents — use `local_defines` by default
- `includes` propagates too — prefer `-I` in `copts` when possible
- `alwayslink = True` if code registers callbacks (not explicitly called)

## cc_test Pattern (GoogleTest)

```python
cc_test(
    name = "unit_tests",
    srcs = ["test_my_module.cc"],
    deps = [
        ":my_module_lib",
        "@com_google_googletest//:gtest_main",
    ],
    size = "small",
)
```

### Then register in root BUILD:
Add to `//:all_unit_tests` test_suite `tests` list.

## bool_flag + config_setting Pattern

### 1. In root BUILD:
```python
bool_flag(
    name = "my_feature",
    build_setting_default = False,
    visibility = ["//visibility:public"],
)

config_setting(
    name = "my_feature_enabled",
    flag_values = {":my_feature": "true"},
    visibility = ["//visibility:public"],
)
```

### 2. In .bazelrc:
```
build --flag_alias=my_feature=//:my_feature
```

### 3. Usage in BUILD:
```python
deps = select({
    "//:my_feature_enabled": [":optional_dep"],
    "//conditions:default": [],
})
```

## Conditional Compilation (select)

```python
cc_library(
    name = "my_lib",
    srcs = ["src.c"],
    deps = [
        "//always:needed",
    ] + select({
        "//:Integration_Testing_enabled": [
            "//software/bbe32/integration_test:dsp_it_macros_test_h",
        ],
        "//conditions:default": [],
    }),
    defines = select({
        "//:Integration_Testing_enabled": ["Integration_Testing"],
        "//conditions:default": [],
    }),
)
```

## Header-Only Target (for shared headers)

```python
cc_library(
    name = "my_header_h",
    hdrs = ["my_header.h"],
    visibility = ["//visibility:public"],
)
```

## Filegroup (non-compiled files)

```python
filegroup(
    name = "config_files",
    srcs = glob(["config/*.json"]),
    visibility = ["//visibility:public"],
)
```

## Checklist After Adding Target
- [ ] Target builds: `bazelisk build //my/package:target --config=flr8`
- [ ] If test: added to `//:all_unit_tests` or `//:tests_bbe`
- [ ] If library with tests: `cc_test` target created
- [ ] Visibility set correctly (public if cross-package)
- [ ] No `defines` when `local_defines` suffices
- [ ] If SWE5: build with `--Integration_Testing=true` too
