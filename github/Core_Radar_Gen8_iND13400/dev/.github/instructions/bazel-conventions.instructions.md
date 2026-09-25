---
applyTo: "**/BUILD"
---

# Gen8 BUILD File Conventions

## Formatting
- All BUILD files must pass `buildifier` (enforced by pre-commit)
- Run `buildifier` before committing any BUILD changes

## cc_library Rules (This Repo)

### Attribute Conventions
- `local_defines` preferred over `defines` (avoid viral propagation)
- `defines` ONLY when dependents genuinely need the macro
- `visibility = ["//visibility:public"]` for cross-package targets
- `alwayslink = True` ONLY for callback-registration libs (IT impl, OS hooks)
- `testonly = True` for test-support libraries and mocks

### Include Paths
- Use `strip_include_prefix` for headers in subdirs (e.g., `inc/`)
- Prefer explicit deps over `includes` attribute
- Never use relative `../` includes — declare deps properly

## select() Usage
- ALWAYS include `"//conditions:default": []` fallback
- Variant selection: use `@build_config//:flr8` / `@build_config//:srr8p`
- Communication variant: use `//software/r52/autosar/swc/PLT_SWC/SWC_PLT_Appl_Communication:*` settings
- IT flags: use `//:Integration_Testing_enabled`, `//:ANGLEFINDING_enabled`

## Test Targets
- Test target name: `unit_tests` (convention in this repo)
- Framework: `@com_google_googletest//:gtest_main`
- Always add new tests to `//:all_unit_tests` or `//:tests_bbe` in root BUILD
- Use `size = "small"` for unit tests (fast timeout)

## Common Mistakes
- Don't add headers to `srcs` if they're public API → use `hdrs`
- Don't use `glob(["**/*.c"])` in production code → list files explicitly
- Don't forget `visibility` when target is used cross-package
- Don't put test helpers in production targets → use separate `testonly` lib

## Toolchain-Specific Notes
- WindRiver (R52): compiler flags via `copts`, no C++ exceptions
- Xtensa (BBE32): DSP-specific flags handled by toolchain config
- MinGW/GCC (tests): host platform tests, standard C/C++

## External Dependencies Pattern
- `@spbb`, `@afbb`, `@Calibration_Handler` — building block libs
- `@iND13400_autosar_sip` — AUTOSAR stack
- `@build_config` — variant/ASIC-FPGA selection
- `@appl_inclusion_dep` — application inclusion flags
