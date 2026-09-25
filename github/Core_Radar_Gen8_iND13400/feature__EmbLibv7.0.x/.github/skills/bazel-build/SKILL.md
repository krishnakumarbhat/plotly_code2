---
description: "Bazel build system expert for Gen8 iND13400 radar (bzlmod, cc_library, select, bool_flag, toolchains)"
---

# Bazel Build Skill — Gen8 Radar (Repo-Verified + Official Docs)

## Quick Facts

- **Bazel version:** 7.5.0 via Bazelisk wrapper
- **Module system:** bzlmod enabled (`common --enable_bzlmod`)
- **Lockfile:** off (`common --lockfile_mode=off`)
- **Toolchains:** WindRiver R52 (ARM), Xtensa BBE32 (DSP), MinGW (sim), GCC (UT)
- **Platform resolution:** `--incompatible_enable_cc_toolchain_resolution`
- **Spawn strategy:** local (`build --spawn_strategy=local`)
- **Registry:** Aptiv JFrog (`jfrog.asux.aptiv.com/artifactory/...`)

## Build Commands

| Task | Command |
|------|---------|
| FLR8 CAN | `bazelisk build //:gen8 --config=flr8` |
| SRR8P CAN | `bazelisk build //:gen8 --config=srr8p` |
| FLR8 SomeIP | `bazelisk build //:gen8 --config=flr8 --veh_com=someip` |
| FLR8 Standalone | `bazelisk build //:gen8 --config=flr8 --veh_com=none --tracker_variant=platform_flr8_standalone` |
| All unit tests | `bazelisk test //:all_unit_tests --test_output=all` |
| BBE tests only | `bazelisk test //:tests_bbe` |
| Single test | `bazelisk test //software/r52/.../test:unit_tests --test_output=all` |
| Coverage | `bazelisk test //coverage:report` |
| Coverity R52 | `bazelisk build //software/r52:windriver_r52_cov --config=flr8` |
| Coverity BBE | `bazelisk build //software/bbe32:xtensa_bbe32_cov --config=flr8` |
| compile_commands | `bazel run //:compiledb` |
| Clean | `bazelisk clean` |
| Full clean | `bazelisk clean --expunge` |

## cc_library Reference (from bazel.build/reference/be/c-cpp)

```python
cc_library(
    name = "my_lib",                    # Required: unique target name
    srcs = ["src.c"],                   # .c/.cc/.cpp/.S files (compiled)
    hdrs = ["inc/my_lib.h"],            # Public headers (available to dependents)
    deps = ["//other:lib"],             # Other cc_library/objc_library targets
    defines = ["MY_DEFINE"],            # -D added to THIS + ALL dependents (propagates!)
    local_defines = ["PRIVATE_DEF"],    # -D added to THIS target ONLY
    copts = ["-Wall"],                  # Compile flags for THIS target only
    includes = ["inc"],                 # -isystem paths (propagates to dependents!)
    linkopts = ["-lm"],                 # Linker flags (propagates to dependents)
    alwayslink = True,                  # Force link ALL objects (even unreferenced)
    visibility = ["//visibility:public"],
    testonly = True,                    # Only tests can depend on this
)
```

### Key Attribute Semantics

| Attribute | Scope | Propagates to dependents? |
|-----------|-------|--------------------------|
| `defines` | Compile | YES — all transitive dependents |
| `local_defines` | Compile | NO — this target only |
| `copts` | Compile | NO — this target only |
| `includes` | Compile | YES — all transitive dependents |
| `linkopts` | Link | YES — when linking binary/shared lib |
| `alwayslink` | Link | Forces all objects linked even if unreferenced |

### hdrs vs srcs

- **`hdrs`**: Public interface headers. Dependents can `#include` these directly.
- **`srcs`**: Private headers + source files. Only THIS target can use them.
- Rule: If consumer shouldn't `#include` it → put in `srcs`, not `hdrs`.

## cc_test Reference

```python
cc_test(
    name = "unit_tests",
    srcs = ["test_main.cc"],
    deps = [
        ":my_lib",
        "@com_google_googletest//:gtest_main",
    ],
    size = "small",        # small/medium/large/enormous (timeout control)
    timeout = "short",     # short/moderate/long/eternal
)
```

## select() — Configurable Attributes

```python
# Pattern: conditional deps/defines/srcs based on config
deps = [
    "//always:dep",
] + select({
    "//:Integration_Testing_enabled": ["//software/bbe32/integration_test:dsp_integration_test_lib"],
    "//conditions:default": [],
})

defines = select({
    "//:Integration_Testing_enabled": ["Integration_Testing"],
    "//conditions:default": [],
})
```

### select() Rules
- Can be used on ANY configurable attribute (deps, srcs, defines, copts, linkopts)
- CANNOT be used on: `name`, `visibility`, `testonly`, `tags` (nonconfigurable)
- `"//conditions:default"` = fallback (ALWAYS include one)
- Multiple `select()` can be combined: `select({...}) + select({...})`

## bool_flag + config_setting Pattern (from this repo)

```python
load("@bazel_skylib//rules:common_settings.bzl", "bool_flag")

# 1. Declare the flag
bool_flag(
    name = "Integration_Testing",
    build_setting_default = False,
    visibility = ["//visibility:public"],
)

# 2. Create config_setting that matches when flag=true
config_setting(
    name = "Integration_Testing_enabled",
    flag_values = {":Integration_Testing": "true"},
    visibility = ["//visibility:public"],
)
```

**In .bazelrc (flag alias for CLI convenience):**
```
common --flag_alias=Integration_Testing=//:Integration_Testing
```

**Usage:** `bazelisk build //:gen8 --Integration_Testing=true`

## string_flag Pattern

```python
string_flag(
    name = "mars_hw_revision",
    build_setting_default = "B0",
    values = ["A0", "B0"],          # Restricts allowed values
    visibility = ["//visibility:public"],
)
```

## Repo-Specific Flag Aliases (.bazelrc)

| CLI Flag | Maps To |
|----------|---------|
| `--config=flr8` | `--variant=flr8` |
| `--config=srr8p` | `--variant=srr8p` |
| `--Integration_Testing=true` | `//:Integration_Testing` bool_flag |
| `--Anglefinding_IT=true` | `//:Anglefinding_IT` bool_flag |
| `--veh_com=someip` | `//software/r52/.../SWC_PLT_Appl_Communication:veh_com` |
| `--tracker_variant=...` | `//software/bbe32/emb_tracker:tracker_variant` |
| `--enable_adc_logging=True` | `@spbb//modules/range_process:enable_adc_logging` |
| `--enable_cdc=True` | `//software/bbe32/src:enable_cdc` |
| `--enable_uart=True` | `//:enable_uart` |
| `--mars_hw_rev=A0` | `//:mars_hw_revision` → mars_a_sample config |

## All Root Bool Flags (from BUILD lines 200–450)

| Flag Name | Default | config_setting Name |
|-----------|---------|---------------------|
| `stream_generation_enabled` | False | `enable_stream_generation` |
| `Feature_functions` | False | `Run_Feature_functions` |
| `SQT_Test_Cases` | False | `Build_SQT_Test_Cases` |
| `SIT_Test_Cases` | False | `Build_SIT_Test_Cases` |
| `XCP_FAULT_INJECTION_Test_Cases` | True | `Build_XCP_FAULT_INJECTION_Test_Cases` |
| `Integration_Testing` | False | `Integration_Testing_enabled` |
| `Anglefinding_IT` | False | `ANGLEFINDING_enabled` |
| `Disable_int_Wdg` | False | `Build_Disable_int_Wdg` |
| `enable_uart` | False | `uart_enable` |
| `static_register_safety_enabled` | True | `enable_static_register_safety` |
| `enable_mcal_test` | False | `mcal_test_enable` |

## Test Suites (from BUILD lines 450–530)

### `:all_unit_tests` (50+ targets)
Key subreferences:
- `//software/common/*/test:unit_tests` (board_revision, crc32, crc_calc, ipc, versions)
- `//software/r52/autosar/swc/*/test:unit_tests` (Communication, Diag, FaultMgr, IPC, Logging, etc.)
- `//software/r52/drivers/mcal/*/test:unit_tests` (gpt, mipi_wrapper, power_supply)
- `//software/r52/bb_radar_ctl/*/test:unit_tests`
- `@Calibration_Handler//calib_handler/test:unit_tests`

### `:tests_bbe`
- `//software/bbe32/test:all_tests`
- `//software/bbe32/rdd_proc/test:all_tests`
- `//software/bbe32/*/test:unit_tests` (TOI_char, bbe_self_test, dss_ecc, mpu, src, static_reg)
- `@spbb//:spbb_tests`

## Common Errors & Fixes

| Error | Cause | Fix |
|-------|-------|-----|
| `no such target '//foo:bar'` | Typo or wrong path | Check BUILD file with `bazel query` |
| `target not visible` | Missing visibility | Add `visibility = ["//visibility:public"]` |
| `no matching toolchain` | Platform mismatch | Check `--config=` flag |
| `undeclared inclusion` | Header not in `hdrs`/`srcs` | Add to correct attribute of providing target |
| `undefined reference` | Missing dep or `alwayslink` | Add dep or set `alwayslink = True` |
| `duplicate symbol` | Same lib linked twice | Check `defines` propagation, use `local_defines` |
| Remote cache corruption | Stale cache entries | Add `--noremote_accept_cached` |
| Long path Windows | Path > 260 chars | Set `startup --output_base=C:/bzl` in `user.bazelrc` |

## Dependency Best Practices

1. **`deps` vs `implementation_deps`**: Use `implementation_deps` for deps whose headers should NOT be exposed to consumers
2. **`defines` vs `local_defines`**: ALWAYS prefer `local_defines` unless intentionally propagating. `defines` is viral.
3. **`alwayslink = True`**: Required when code registers callbacks/handlers (not directly called). Used in this repo for IT impl libs.
4. **Header-only targets**: Use `cc_library(hdrs=[...], srcs=[])` for header-only libs
5. **Filegroup for headers**: `filegroup(name = "my_h", srcs = ["my.h"])` then reference in deps as `:my_h`

## Remote Cache (from .bazelrc)

- Cache config imported from: `tools/bazel/config/arb_cache.bazelrc`
- Bypass: `--noremote_accept_cached`
- Registry URLs: JFrog Aptiv (`jfrog.asux.aptiv.com`)
