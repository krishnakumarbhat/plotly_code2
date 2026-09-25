---
name: bazel-build-system
description: 'Master reference for the Gen7 SAF85xx Bazel/Bzlmod build system. Use when asked about: BUILD files, MODULE.bazel, bb.MODULE.bazel, .bazelrc flags, flag aliases, bazelisk commands, variant/board/micro_revision configuration (srr7e/flr7/srr7p/flr7v3, A1, ES1/ES2), config_setting, bool_flag, string_flag, select() usage, selects.config_setting_group match_all/match_any, toolchains (Wind River M7/A53, Xtensa BBE32, mingw, gcc, clang, Coverity), local_repository declarations, http_archive building blocks (SPBB, AFBB, IDBB, RCBB, DABB, SABB, ROTBB, HILBB, boot, SMC/USC calibrations), overriding BBs via user.bazelrc, target patterns (//:gen7, test suites, copy_to_dir, srec_cat, patch_s19_crc, create_flash_image, platforms_transition, cc_binary_bbe32), sanitizer/coverage/mayhem/coverity bazelrc configs, JFrog registries and remote cache (RapidBuild BES), Bzlmod module extensions.bzl, use_extension, use_repo_rule, single_version_override, MVS resolution, apparent vs canonical repo names, stream generation, memory/stack analysis rules, Coverity commit_defects, fuzz_test_runner, CDC / RTM / NvM CRC / SecOC / VLAN / NM / XCP conditional builds, or debugging build failures tied to BUILD/Bazel configuration.'
---

# Gen7 SAF85xx Bazel Build System

Master reference for working with the Bazel build in `Core_Radar_Gen7_SAF85xx`. Use this skill any time a question or change touches [BUILD](../../../BUILD), [MODULE.bazel](../../../MODULE.bazel), [bb.MODULE.bazel](../../../bb.MODULE.bazel), [extensions.bzl](../../../extensions.bzl), [.bazelrc](../../../.bazelrc), [user.bazelrc](../../../user.bazelrc), toolchains, flags, or target wiring.

## When to Use

- Building, testing, or profiling a variant / board / micro revision combination
- Understanding or adding a flag alias, `config_setting`, `bool_flag`, `string_flag`, or `select()` branch
- Adding, updating, or locally overriding a building block (`http_archive`)
- Adding a unit test to a `test_suite`, or explaining `//:all_unit_tests` / `//:tests_bbe` / `//:tests_mmic`
- Debugging missing deps, visibility errors, toolchain resolution failures, or wrong-variant outputs
- Setting up sanitizers (ASAN / UBSAN / Valgrind), Coverity, Mayhem fuzz tests, or TICS `compile_commands.json`
- Understanding how the final `.s19` / `.ptp` / `.zip` image is assembled (`srec_cat`, `create_flash_image`, `patch_s19_crc`, `create_header_image`)
- Troubleshooting JFrog/`.netrc`, RapidBuild remote cache, BES upload, or registry issues

## 1. Bazel Background (Authoritative)

**Bazel version**: pinned by `bazelisk` (see `.bazeliskrc` / `.bazelversion`). Always invoke via `./bazelisk` (Linux) or `bazelisk` (Windows); never `bazel` directly.

**Bzlmod is enabled** (`common --enable_bzlmod` in [.bazelrc](../../../.bazelrc)). Key Bzlmod semantics:

- The root [MODULE.bazel](../../../MODULE.bazel) declares the module (`module(name="gen7_saf85xx", repo_name="Gen7_SAF85xx")`) and direct dependencies via `bazel_dep(name, version)`.
- Transitive dependencies are resolved by **Minimal Version Selection (MVS)** — Bazel picks the highest version requested by any dependent.
- **Only the root module's overrides take effect** (`single_version_override`, `local_path_override`, `archive_override`, `git_override`, `multiple_version_override`). Non-root overrides are ignored.
- **Apparent repo names** (how you reference via `@name//:...`) default to the module name but can be changed by `bazel_dep(..., repo_name="X")`. Example in this repo: `rapid_build_platform` → `@bazel_platform`, `core_radar_swc_plt_appl_diag` → `@SWC_PLT_Appl_Diag`.
- `MODULE.bazel` may contain `include("//:bb.MODULE.bazel")` to split the manifest across files (we use this heavily).
- `use_extension(...)` imports a `module_extension` defined in a `.bzl`; `use_repo(...)` exposes its generated repos as apparent names.
- `use_repo_rule(...)` lets MODULE.bazel invoke a repository rule (e.g. `http_archive`, `local_repository`) directly, without wrapping it in a module extension — this is how [bb.MODULE.bazel](../../../bb.MODULE.bazel) calls `http_archive` from top level.
- `--lockfile_mode=off` is set; do NOT commit a `MODULE.bazel.lock`.

**Custom registries** (from `.bazelrc`) are consulted *instead of* the Bazel Central Registry:
1. `jfrog.asux.aptiv.com/.../bazel_platform-bcr-fork`
2. `jfrog.asux.aptiv.com/.../core_radar-...-building_blocks-local/Bazel_Registry/`
3. `jfrog.asux.aptiv.com/.../rapid_build_platform_registry`

**Starlark build settings** (`@bazel_skylib//rules:common_settings.bzl`):
- `bool_flag(name, build_setting_default)` — CLI: `--//pkg:name` / `--no//pkg:name`.
- `string_flag(name, build_setting_default, values=[...])` — restricted string; values outside the allowlist cause errors.
- Consumed by `config_setting(flag_values={":flag": "value"})` which is then used inside `select({...})`.
- Native flags (`--cpu`, `--compilation_mode`) use `values={...}` on `config_setting` (not `flag_values`).
- `constraint_values=[...]` on `config_setting` matches when ALL listed `constraint_value`s are present in the active `--platforms`.
- `//conditions:default` matches when nothing else does. Multiple matches must be unambiguous (identical values or strict specialization).
- `selects.config_setting_group(match_all=[...])` = AND; `match_any=[...]` = OR.

**Flag aliases** (`.bazelrc`): `common --flag_alias=<alias>=<//pkg:target>` maps `--<alias>=VAL` → `--//pkg:target=VAL`. Both `bool_flag` and `string_flag` can be aliased.

**`http_archive` attributes** (from `@bazel_tools//tools/build_defs/repo:http.bzl`):
- `name` — apparent repo name.
- `urls` (list, preferred) or `url` (single) — tried in order; acts as mirror list.
- **Exactly one of `sha256` (64-hex) or `integrity` (Subresource Integrity: `sha256-<base64>`) is required for hermeticity.**
- `strip_prefix` — strips leading dir from the archive contents.
- `build_file` / `build_file_content` — overlay a BUILD file onto archives that ship without one (used for SMC/USC PTPs).
- `patches`, `patch_args`, `patch_strip`, `patch_cmds` — source patching.
- `type` — auto-detected from URL; supports `zip`, `tar`, `tgz`, `tar.gz`, `tar.xz`, `tar.zst`, `tbz`, `7z`, etc.

## 2. Three-Layer Architecture

1. **[MODULE.bazel](../../../MODULE.bazel)** — root manifest: `bazel_dep`s for `platforms`, `bazel_features`, `rules_pkg`, `com_google_googletest`, `rules_cc`, `bazel_skylib`, `rules_fuzzing`, `rules_mayhem`, `fff`, `rapid_build_platform` (repo name `bazel_platform`), `rules_python` (pinned via `single_version_override` to 1.4.1), `quickflash`, `core_radar_swc_plt_appl_diag`, `core_radar_swc_plt_appl_faultmgr`. Registers extensions: `rules_mayhem_extension`, `coverity` (from `//:extensions.bzl`, `dev_dependency=True`). `include("//:bb.MODULE.bazel")` plus three other `include()`s for toolchains/python/emb_lib. Calls `register_toolchains(...)`. Declares ~15 `local_repository` overlays (see §5).

2. **[bb.MODULE.bazel](../../../bb.MODULE.bazel)** — all external BBs + calibrations as `http_archive`s pinned by `sha256` or `integrity`, hosted on `jfrog.asux.aptiv.com/artifactory/core_radar-...`. Entries: `reuse`, `SAF85xx_MCAL`, `saf85xx_autosar_sip`, `SWC_PLT_Appl_PlatformTime`, `afbb`, `spbb`, `idbb`, `sabb`, `dabb`, `rcbb`, `hilbb`, `rotbb`, `Calibration_Handler`, `SWC_PLT_Appl_ModeManager`, `SWC_PLT_CDD_FailSafe`, `SWC_PLT_CDD_WdgHandler`, `smc_{srr7e,flr7,flr7_standalone,flr7v3,srr7p,srr7p_standalone}`, `usc_{srr7e,flr7,flr7v3,srr7p}`, `boot`, `SWC_PLT_Appl_IoHwAb`, `SWC_PLT_Appl_TimeSync`, `build-tools`, `TOI_Char_Quality_Determination`. Also Bzlmod-migrated deps: `quickflash`, `core_radar_log_struct_gen` (repo name `StreamHeader_Gen`).

3. **Root [BUILD](../../../BUILD) + [software/BUILD](../../../software/BUILD)** — top-level assembly, test suites, fuzz/valgrind runners, flash image generation. See §7.

Sub-`BUILD` files under `software/`, `sil/`, and `tools/` define `cc_library`, per-core binaries (`cc_binary_bbe32` + Wind River variants), tests, and custom rules.

## 3. Flag Aliases (CLI Shortcuts)

Defined in [.bazelrc](../../../.bazelrc) as `common --flag_alias=<alias>=<target>`. Any new alias MUST be documented in [README.md](../../../README.md) under "Conditional Builds".

| Alias | Target | Values / Default | Meaning |
|-------|--------|-------------------|---------|
| `--variant` | `@build_config//:variant` | `srr7e` \| `flr7` \| `srr7p` *(default)* \| `flr7v3` | Sensor variant — REQUIRED for `//:gen7` |
| `--board` | `//software/m7/mcal:boardVersion` | e.g. `A1` | Board revision |
| `--micro_revision` | `@build_config//:micro_revision` | `ES1` \| `ES2` *(default)* | SAF85xx silicon stepping |
| `--rot` | `@appl_inclusion_dep//:rot_fusion` | `none` *(default — satellite)* \| `master` \| `slave` \| `standalone` | Tracker inclusion mode (`string_flag`) |
| `--rot_gen` | `@appl_inclusion_dep//:rot_gen` | **`string_flag`**: `rot_gen7v1` \| `rot_gen7v2` *(default)* \| `rot_gen8` | Tracker codebase generation |
| `--enable_cdc` | `//software/bbe32:enable_cdc` | `True` *(default)* / `False` | Enable CDC (Compressed Detection Cube) |
| `--enable_cdc_crc` | `//software/bbe32:enable_cdc_crc` | `True` *(default)* / `False` | CDC CRC validation |
| `--enable_nm` | `//software/m7/autosar/swc/TKEY_SWC/SWC_TKEY_CDD_NMHandler:enable_nm` | `True` *(default)* / `False` | CAN Network Management |
| `--enable_secoc` | `//software/m7:enable_secoc` | `False` *(default)* / `True` | SecOC Tx/Rx |
| `--enable_vlan` | `//software/m7/autosar/config/Appl/GenData:enable_vlan` | `False` *(default)* / `True` | EthIf VLAN |
| `--disable_rtm` | `//:DISABLE_RTM_MEASUREMENT` | bool (default False) | Disable Run Time Measurement |
| `--enable_startup_measurement` | `//:STARTUP_MEASUREMENT` | bool (default False) | Measure startup time |
| `--nvm_crc_disable` | `//:NVM_CRC_DISABLE` | **Inverted logic — see note** | NvM CRC gating |
| `--disable_int_wdg` | `//:INT_WDG_DISABLE` | bool (default False) | Disable internal watchdog |
| `--enable_stream_generation` | `//:stream_generation_enabled` | `False` *(default)* / `True` | RESIM-compatible stream regeneration |
| `--enable_sqt_testcases` / `--enable_sit_testcases` | `//:SQT_Test_Cases` / `//:SIT_Test_Cases` | bool (default False) | SQT/SIT integration tests |
| `--Integration_Testing` / `--Anglefinding_IT` | `//:Integration_Testing` / `//:Anglefinding_IT` | bool (default False) | IT hooks |
| `--enable_id_testing` / `--enable_bench_testing` / `--align_det_test` | `@appl_inclusion_dep//:enable_id_testing` / `:enable_bench_testing` / `:align_det_test` *(bench_testing & align_det also live at `//:...`)* | bool (default False) | ID / alignment bench testing |
| `--enable_parking_mode` | `//:Enable_UWB_Mode` | bool (default False) | UWB / parking mode |
| `--enable_XCP_FAULT_INJECTION_testcases` | `//:XCP_FAULT_INJECTION_Test_Cases` | bool **(default True)** | XCP fault injection |
| `--enable_RFE_COPYBACK_CONFIG_Debug` | `//:RFE_COPYBACK_CONFIG_Debug` | bool (default False) | RFE copyback debug |
| `--enable_XCP_JTAG` | `//:enable_XCP_JTAG` | `True` *(default)* | XCP/JTAG |
| `--enable_sra_old_algo` | `//:enable_sra_old_algo` | `True` *(default)* | Static-alignment old algo |
| `--define_disable_RTE_Enhanced_Locks` | `//:disable_Enhanced_RTE_Locks` | bool **(default True)** | Disable enhanced RTE locks |
| `--Aptiv_Official_Release` | `//:Aptiv_Official_Release` | bool (default False) | Filter DET errors for official release |
| `--alignment_nvm_test` | `@appl_inclusion_dep//:alignment_nvm_test` | bool (default False) | Alignment NVM test |
| `--jenkins` | `@build_config//:jenkins` | bool (default False, CI only) | Jenkins-specific flags |
| `--rc_fi` | `@rcbb//module/capability/RC_fault_injection:enable_rc_fault_injection` | bool | Radar Capability fault injection |
| `--gitBranch` / `--swVersion` | `//tools/python/memoryStats:gitBranch` / `:swVersion` | strings | Stamp metadata |
| `--psp_sil_config` | `@appl_inclusion_dep//:enable_psp_sil` | bool (default False) | PSP SIL |
| `--valgrind` | `@build_config//:valgrind` | bool (default False) | Enables Valgrind test config (alias lives in `tools/bazel/config/sanitizers.bazelrc`, activated by `--config=valgrind`) |
| `--regression` | `//tools/mayhem:regression` | bool | Mayhem regression mode (from `mayhem.bazelrc`) |
| `--wait` | `//tools/mayhem:wait` | bool | Wait for Mayhem run to finish (from `mayhem.bazelrc`) |
| `--nightly` | `//tools/coverity:nightly` | bool | Coverity nightly mode (from `coverity.bazelrc`) |

> **⚠️ Inverted-logic flag — `--nvm_crc_disable`**: `//:NVM_CRC_DISABLE` has `build_setting_default = True`, but the matching `config_setting` `BUILD_NVM_CRC_DISABLE` triggers on `"False"`. The `config_setting` fires only when the user explicitly passes `--nvm_crc_disable=False`. The README's plain-English description conflicts with this; the BUILD file is authoritative — verify empirically for any safety-relevant change.

### Direct flag usage (no alias)

Any flag can be passed long-form: `--//software/bbe32:enable_doppler_module_timing=True` (Doppler module profiling, documented in README).

### Named `.bazelrc` configs (`--config=<name>`)

| `--config=X` | Source | Effect |
|--------------|--------|--------|
| `--config=asan` | `sanitizers.bazelrc` | AddressSanitizer + LeakSanitizer + pointer-compare/subtract + rich `ASAN_OPTIONS` |
| `--config=ubsan` | `sanitizers.bazelrc` | UBSan (`-fsanitize=undefined`, no `vptr`) |
| `--config=valgrind` | `sanitizers.bazelrc` | Sets `--valgrind`; Valgrind wiring via `@bazel_platform//tools/valgrind:valgrind.bzl` |
| `--config=mayhem` | `mayhem.bazelrc` | `-c dbg`, `host_compiler=clang`, `cc_engine=libfuzzer`, `libfuzzer` instrumentation, `--java_runtime_version=remotejdk_11`, `--enable_runfiles` on Windows |
| `--config=cov_commit` | `coverity.bazelrc` | Disables remote cache (`--noremote_accept_cached`) and sets `--nightly=true` for committing defects to Coverity Connect |
| `--config=CAN_Tx_Dummy` | root `.bazelrc` | `--define=Build_CAN_TX_Dummy_Data=true` → triggers `config_setting(define_values=...)` — shortcut for Dummy CAN Tx data |
| `--config=windows` / `--config=linux` | auto (via `--enable_platform_specific_config`) | Host-OS-specific lines (hermetic PATH) |

## 4. Canonical Build Commands

```bash
# Production images (four variants)
./bazelisk build //:gen7 --variant=srr7p  --board=A1 --micro_revision=ES2
./bazelisk build //:gen7 --variant=srr7e  --board=A1 --micro_revision=ES2
./bazelisk build //:gen7 --variant=flr7   --board=A1 --micro_revision=ES2
./bazelisk build //:gen7 --variant=flr7v3 --board=A1 --micro_revision=ES2

# Standalone (tracker) build
./bazelisk build //:gen7 --variant=srr7p --rot=standalone

# Doppler module profiling
./bazelisk build //:gen7 --variant=srr7p --//software/bbe32:enable_doppler_module_timing=True

# Unit tests — full / per-domain / single target
./bazelisk test //:all_unit_tests
./bazelisk test //:tests_bbe  --variant=srr7p
./bazelisk test //:tests_mmic
./bazelisk test //software/common/versions/test:unit_tests

# Coverage
./bazelisk test //coverage:report      --variant=srr7p
./bazelisk test //coverage:bbe_report  --variant=srr7p

# Sanitizers (Linux / WSL only)
./run_dynamic.sh asan srr7p            # or ubsan / valgrind; variant or "all"
./bazelisk test //:all_unit_tests --config=asan
./bazelisk test //:all_unit_tests --config=ubsan
./bazelisk test //:all_unit_tests --config=valgrind

# Coverity (build per core + commit defects)
./bazelisk build //software/m7:windriver_v7_cov    --variant srr7p
./bazelisk build //software/a53:windriver_v7_cov   --variant srr7p
./bazelisk build //software/bbe32:xtensa_bbe32_cov --variant srr7p
./bazelisk run   //software/m7:commit_defects      --variant srr7p -- --auth-key-file="<path>/coverity.auth"

# Fuzz testing (Mayhem) — note: run, not build
./bazelisk run :fuzz --config mayhem
./bazelisk run //software/common/app_chksum/test:dd_app_chksum_fuzz --config mayhem
./bazelisk run :fuzz --config mayhem --regression
./bazelisk build //coverage:fuzz_report --config mayhem          # un-fuzzed file report

# TICS compile_commands.json
./bazelisk run //:compiledb

# Stream bandwidth report
./bazelisk build //tools/python/streamGenerator:generate_bandwidth
```

Final artifacts land in `bazel-bin/outputs/<variant>/` via `copy_to_dir` rules in the root [BUILD](../../../BUILD): `.s19` / `.ptp` flash images, memory stats, stream definitions, SMC/USC calibration PTPs, PBL/SBL/SFT/HSE_FW boot files, SPC, Quickflash, Wireshark Lua/configs.

## 5. The `@build_config` Repository and `@appl_inclusion_dep`

### 5.1 `@build_config` — [tools/bazel/config/BUILD](../../../tools/bazel/config/BUILD)

Declared in `MODULE.bazel` as `local_repository(name="build_config", path="tools/bazel/config")` (bzlmod-style: `local_repository = use_repo_rule("@bazel_tools//tools/build_defs/repo:local.bzl", "local_repository")`). Everything `@build_config//:...`:

- `string_flag` **`variant`** (values `srr7e`/`flr7`/`srr7p`/`flr7v3`, default `srr7p`) + four matching `config_setting`s.
- `string_flag` **`micro_revision`** (`ES1`/`ES2`, default `ES2`) + matching `config_setting`s.
- `bool_flag` **`jenkins`** (→ `:jenkinsBuild`), **`valgrind`** (→ `:valgrindBuild`).
- `platform` **`winbbe32`** / **`linuxbbe32`** with `constraint_values = ["//cpu:bbe32", "@platforms//os:{windows,linux}"]`.
- `constraint_value` **`//cpu:bbe32`** (in [tools/bazel/config/cpu/BUILD](../../../tools/bazel/config/cpu/BUILD)) with `constraint_setting = "@platforms//cpu:cpu"` — used directly in `deps` selects to pick embedded vs SIL libs:
  ```starlark
  deps = [...] + select({
      "@build_config//cpu:bbe32": ["@spbb//modules/helpers/imp:profiling_helpers_lib"],    # Embedded
      "@platforms//cpu:x86_64":   ["@spbb//modules/helpers/api/mocks:profiling_helpers_mock_lib",
                                    "@spbb//common/cstub_helpers/mocks:xtensa_core_macros_mock_lib"],  # SIL
      "//conditions:default":     [],
  })
  ```
- `string_flag` **`host_compiler`** (values `gcc`/`clang`, default `gcc`) + per-value `config_setting`s — Mayhem forces `clang` via `mayhem.bazelrc`.
- **`selects.config_setting_group`** composites — only **`srr7p`** and **`flr7`** have satellite/rot variants exposed; `flr7v3` and `srr7e` do NOT:
  - `:srr7p_satellite` = `match_all=[:srr7p, @appl_inclusion_dep//:none]`
  - `:srr7p_emb_tracker_tmp`   = `match_any=[@appl_inclusion_dep//:master, :standalone, :slave]` *(private)*
  - `:srr7p_emb_tracker`       = `match_all=[:srr7p, :srr7p_emb_tracker]`
  - `:flr7_satellite`, `:flr7_emb_tracker_tmp`, `:flr7_emb_tracker` follow the same pattern

### 5.2 `@appl_inclusion_dep` — [software/building_block/common/BUILD](../../../software/building_block/common/BUILD)

The application-side contract that BBs (especially the tracker) consume. Key declarations:

- `string_flag` **`rot_fusion`** (values `none`/`master`/`slave`/`standalone`, default `none`) with four `config_setting`s `@appl_inclusion_dep//:none`, `:master`, `:slave`, `:standalone`.
- `string_flag` **`rot_gen`** (values `rot_gen7v1`/`rot_gen7v2` *(default)*/`rot_gen8`) with `config_setting`s `:ROT_GEN7V1`, `:ROT_GEN7V2`, etc.
- `string_flag` **`variant`** (duplicates `@build_config//:variant` — consumers can reference whichever apparent repo they have in scope). Has its own `srr7p`/`srr7e`/`flr7`/`flr7v3` `config_setting`s, each keyed on `@appl_inclusion_dep//:variant`.
- `bool_flag`s: `enable_bench_testing`, `align_det_test`, `enable_id_testing`, `enable_psp_sil`, `id_enable_v2` (default True), `rbin_scaling_enabled` (default True), `alignment_nvm_test`.
- `cc_library` header overlays: `afbb_include_h`, `idbb_include_h`, `sil_idbb_include_h`, `dabb_include_h`, `rcbb_include_h`, `hilbb_include_h` — each provides the `*_include.h` contract the BB archive includes.

Other key `local_repository` overlays declared in `MODULE.bazel`:

| Name | Path | Purpose |
|------|------|---------|
| `build_config` | `tools/bazel/config` | Variant/platform flags (above) |
| `appl_inclusion_dep` | `software/building_block/common` | Application-provided BB deps (`rot_fusion`, `enable_id_testing`, etc.) |
| `SAF85xx_MCAL_Config` | `software/m7/mcal/app/output` | Generated MCAL code |
| `spbb_cfg` | `software/bbe32/bb_cfg` | SPBB macro config |
| `spbb_include` | `software/bbe32/bb_include` | SPBB type headers |
| `appl_calib_dep` | `software/common/calibrations` | Calibration archive BUILD files (`smc:archive.BUILD`, `usc:archive.BUILD`) |
| `angle_finding_module_cfg` | `software/building_block/anglefinding` | AFBB application config |
| `Static_alignment_module_interface` | `software/a53/static_alignment` | SABB interface |
| `Dyn_alignment_module_interface` | `software/a53/dyn_alignment` | DABB interface |
| `radar_capability_module_interface` | `software/a53/capability` | RCBB interface |
| `interference_detection_cfg` | `software/a53/id` | IDBB config |
| `calib_cfg` | `software/m7/calib_cfg` | M7 calibration config |
| `hil_module_cfg` | `software/building_block/hil_process` | HILBB config |
| `TOI_Char_Quality_Configuration` | `software/a53/TOI_char_quality_determination/.../TOI_Char_Quality_Configuration` | TOI config |

## 6. Toolchains

Registered in `MODULE.bazel` via `register_toolchains(...)`. `.bazelrc` enforces `--incompatible_enable_cc_toolchain_resolution` (platform-based) and sets `BAZEL_DO_NOT_DETECT_CPP_TOOLCHAIN=1`.

| Toolchain | Target | Source |
|-----------|--------|--------|
| `//tools/bazel/toolchains/windriver_m7:windriver_m7_toolchain[_linux]` | M7 Cortex-M7 | Wind River Diab |
| `//tools/bazel/toolchains/windriver_a53:windriver_a53_toolchain[_linux]` | A53 Cortex-A53 | Wind River Diab |
| `//tools/bazel/toolchains/bbe32:bbe_toolchain_{windows,linux}` | BBE32 DSP | Xtensa RI-2021.7 |
| `//tools/bazel/toolchains/mingw:mingw_windows_toolchain` | Host (Windows) | MinGW-w64 |
| `//tools/bazel/toolchains/gcc:gcc_linux_toolchain` | Host (Linux) | GCC |
| `//tools/bazel/toolchains/clang:clang_{linux,windows}_toolchain` | Sanitizers | Clang |
| `@coverity_toolchain//:all` | Coverity SCA | Registered by `coverity` module extension in `extensions.bzl` |
| `@bazel_platform//toolchains/python3:py_{linux,windows}_toolchain` | Python | RapidBuild platform |

**Python pinned**: `common --@rules_python//python/config_settings:python_version=3.10`.

**Hermetic PATH**:
- Windows: `common:windows --action_env=PATH=;` (semicolon workaround for [bazel#15364](https://github.com/bazelbuild/bazel/issues/15364))
- Linux: `common:linux --action_env=PATH="/bin:/usr/bin"`

## 7. Root `BUILD` Anatomy

### 7.1 Final deliverable

```
filegroup //:gen7
├── :aptivAppOutputs      → outputs/<variant>/{app.s19, app_sb.s19, app.ptp, app_sb.ptp,
│                                              rfe_enc_*.s19, app_crc_filled.s19,
│                                              app_vectorboot.s19, smc_cal.ptp, usc_cal.ptp,...}
├── :boot                 → outputs/{HSE_FW, pbl, sbl, sft}
├── :generate_lua_script  → outputs/<variant>/streamdefs/wiresharkDissector.lua
├── :generate_stream_defs → outputs/<variant>/streamdefs/*.yaml
├── :generate_wireshark_configs → outputs/<variant>/someip/*
├── :quickflash / :quickflash_cfg
├── :streamGeneratorTool  → outputs/tools/streamGeneratorTool/
├── :stream_bandwidth_report → outputs/<variant>/streamdefs/Stream_Bandwidth_Details.csv
├── :spc                  → outputs/spc/{spc_1, ..., spc_5}
├── :warnings_report      → outputs/<variant>/warnings
└── :generate_stream_files  (only when --enable_stream_generation)
```

### 7.2 Flash image assembly ([software/BUILD](../../../software/BUILD))

Pipeline for the primary `app.s19` deliverable:

1. `create_flash_image //software:app_no_rfe_no_crc.s19` — merges `//software/a53:a53App.s19`, `//software/m7:m7App.s19`, `//software/bbe32:bbe32App.s19` at `entry_address=0x340C4000`, `start_address=0x00110000`, pattern `APP`.
2. `patch_s19_crc //software:app_no_rfe.s19` — patches M7 code-section CRC32 at `0x0037FED0` using `__M7_code_start_c0` / `__M7_code_end_c0` from `m7App.elf.map`.
3. `create_header_image //software:apphdr.s19` — bootloader header at `0x0037E000`, magic `0xECE9654A`, length `0x0026FFFC`.
4. `srec_cat //software:app_no_crc.s19` — merges app + RFE firmware (`@build_config//:ES1` → `ES1/bin:rfe.s19`, `@build_config//:ES2` → `ES2/bin:rfe.s19`) + apphdr (offset `0x00040000`).
5. `patch_s19_crc //software:app.s19` — final CRC patch.

Parallel `app_sb.s19` path uses encrypted RFE (`rfe_enc.s19`) for ES2. Additional outputs: `rfe_enc_filled.s19`, `rfe_enc_crc.s19`, `app_crc_filled.s19`, `app_vectorboot.s19`.

### 7.3 Reports

- `save_build_config //:save_build_config` — writes plain-text `build-cfg.txt` listing the active values of a whitelist of flags: `@build_config//:variant`, `:micro_revision`, `@appl_inclusion_dep//:rot_gen`, `:rot_fusion`, `//software/m7/mcal:boardVersion`, `//software/bbe32:enable_cdc`, `:enable_cdc_crc`, `//software/m7:enable_secoc`, `//software/m7/autosar/config/Appl/GenData:enable_vlan`, `//:enable_sra_old_algo`. Reads each flag via `BuildSettingInfo` provider (see [save_build_config.bzl](../../../tools/bazel/scripts/save_build_config.bzl)).
- `generate_memory_stats //:generate_memory_stats` — aggregated M7/A53/BBE32 memory usage from the three `cc_windriver_binaries`.
- `stack_analysis //:stack_analysis_bbe32` — runs Xtensa `xt-stack-usage` (from `@bbe_windows` or `@bbe_linux` under `tools/RI-2021.7-{win32,linux}/XtensaTools/bin/`) against `//software/bbe32:bbe32`.
- `flash_memory_stats //:flash_memory_stats.txt` — occupancy of the 4 MB flash for regions `NVM`, `SYS_IMG`, `SYS_IMG_BCK`, `HSE_FW`. Reads `//software/common/linker:common_ld_preprocessed` (a linker script run through the `ld_preprocess` rule, which uses `@cygwin_windows//:cmd/clang.exe` as default preprocessor). Inputs are `app.s19`, `@boot//:pbl`, `@smc_srr7p//:smc_cal_ptp`, `@usc_srr7p//:usc_cal_ptp`.
- `compiler_warnings_report //:warnings` — from `@bazel_platform//quality/compiler_warnings`, gated by `compiler_warnings_allowlist.yml`.
- `refresh_compile_commands //:compiledb` — from `@bazel_platform//utils/compilation_database`, generates `compile_commands.json` for TICS and IDE/clangd.

### 7.4 Test infrastructure

Three Python lists in [BUILD](../../../BUILD):
- **`CORE_TESTS`** — ~60 M7/A53/common unit-test targets (AUTOSAR SWCs, drivers, IPC, MMIC, RTE, Diag, FaultMgr, UDS, NvM, SecOC, etc.).
- **`SAFE_BBE_TESTS`** — BBE-safe tests: `software/bbe32/...`, `software/m7/dsp_setup/test:all_tests`, `@spbb//:tests`, AFBB (down_selection, fascia_compensation, radar_math), anglefinding interface.
- **`UNSAFE_BBE_TESTS`** — `@afbb//module:anglefinding_unit_test` (too large for Valgrind).

Composed into `test_suite`s:
- `//:all_unit_tests` = CORE + SAFE_BBE + UNSAFE_BBE
- `//:tests_bbe` = SAFE_BBE + UNSAFE_BBE
- `//:tests_mmic` = explicit MMIC subset
- `//:valgrind_all_unit_tests` = CORE + SAFE_BBE under `valgrind --tool=memcheck --leak-check=full --track-origins=yes`

**Adding a test**: append to the right list. If it fails at `-Og`, mark with `# This test fails with -Og` (pattern already present). Tests default to `-O0` via `.bazelrc`.

### 7.5 Fuzz targets

- `fuzz_test_runner //:fuzz` — currently: `app_chksum`, `crc_calc`, `versions`.
- `filegroup //:fuzz_harnesses` — exported binaries for coverage.
- **When adding a fuzz test, update BOTH `:fuzz` and `:fuzz_harnesses` AND `//coverage:fuzz` (in [coverage/BUILD](../../../coverage/BUILD)).**

### 7.6 Local `bool_flag` / `config_setting` pairs (~25 in root BUILD)

Most follow the `bool_flag` + `config_setting(flag_values={":X": "true"})` pattern:

```starlark
bool_flag(name = "X",           build_setting_default = <True|False>, visibility = ["//visibility:public"])
config_setting(name = "Build_X", flag_values = {":X": "true"},         visibility = ["//visibility:public"])
```
Notable: `SQT_Test_Cases`, `SIT_Test_Cases`, `Integration_Testing`, `Anglefinding_IT`, `Enable_UWB_Mode`, `disable_Enhanced_RTE_Locks` **(default True)**, `DISABLE_RTM_MEASUREMENT`, `INT_WDG_DISABLE`, `STARTUP_MEASUREMENT`, `NVM_CRC_DISABLE` *(inverted)*, `enable_XCP_JTAG` (default True), `MMIC_DATA_ACQ_DELAY_ENABLE`, `enable_sra_old_algo` (default True), `stream_generation_enabled`, `XCP_FAULT_INJECTION_Test_Cases` **(default True)**, `RFE_COPYBACK_CONFIG_Debug`, `Aptiv_Official_Release`.

**Exception — the third way**: `Build_CAN_TX_Dummy_Data` uses the legacy native-define mechanism (`define_values={"Build_CAN_TX_Dummy_Data": "true"}`), triggered by `--define=Build_CAN_TX_Dummy_Data=true` (exposed conveniently as `--config=CAN_Tx_Dummy`). This is the **third** way to write a `config_setting`, alongside `values=` (native flags like `--cpu`) and `flag_values=` (Starlark flags).

## 8. Core Patterns Cheat-Sheet

### 8.0 Custom rules you'll see everywhere

Most of these come from `@bazel_platform//` (the RapidBuild platform) or `//tools/`:

| Rule / macro | Source | Purpose |
|--------------|--------|---------|
| `copy_to_dir` | [tools/bazel/scripts/copy_to_dir.bzl](../../../tools/bazel/scripts/copy_to_dir.bzl) | Copies files to `outputs/<variant>/...`. Supports `preserve_directories` + `strip_prefix`. Uses `cmd.exe` on Windows, `cp` on Linux. |
| `save_build_config` | [tools/bazel/scripts/save_build_config.bzl](../../../tools/bazel/scripts/save_build_config.bzl) | Dumps flag values to `build-cfg.txt` using `BuildSettingInfo`. |
| `ld_preprocess` | [tools/bazel/scripts/ld_preprocess.bzl](../../../tools/bazel/scripts/ld_preprocess.bzl) | Runs `clang -E -P -x c` over a `.ld` linker script (default preprocessor `@cygwin_windows//:cmd/clang.exe`). |
| `generate_stream_def` | [tools/bazel/scripts/generate_stream_def.bzl](../../../tools/bazel/scripts/generate_stream_def.bzl) | Runs stream-def tool; formats file names `streamdef_src<NNN>_str<NNN>_ver<NNN>.txt` with zero padding; special SMC/USC sub-stream handling for stream `014`. |
| `create_flash_image` | `//tools/python/create_flash_image` | Merges per-core `.s19` files at given `entry_address` / `start_address`; produces `app_ram.s19` side-output. |
| `patch_s19_crc` | `//tools/python/crc32` | Reads `__M7_code_start_c0`/`__M7_code_end_c0` from the M7 `.elf.map`, computes CRC32, patches it at `crc_address`. |
| `create_header_image` | `//tools/tools_tkey/create_header_image` | Builds the bootloader auth header (magic `0xECE9654A`). |
| `srec_cat` | `@bazel_platform//tools/srec_cat` | Thin wrapper around the `srec_cat` utility (supports `{INPUT_FILE0}` templating, `-offset`, `-CRC32_Big_Endian`, `-fill`, `-crop`, `-disable=data-count`, etc.). |
| `valgrind` | `@bazel_platform//tools/valgrind:valgrind.bzl` | Wraps `cc_test` with Valgrind. |
| `coverage_report` | `@bazel_platform//quality/ut:coverage_report.bzl` | Aggregates gcov output using `GLOBAL_EXCLUDES` / `TOPLEVEL_EXCLUDES` / `GLOBAL_COVERAGE_EXCLUDES` regex lists (see [coverage/BUILD](../../../coverage/BUILD)). |
| `compiler_warnings_report` | `@bazel_platform//quality/compiler_warnings` | Compares warnings against `compiler_warnings_allowlist.yml`. |
| `refresh_compile_commands` | `@bazel_platform//utils/compilation_database` | Emits `compile_commands.json` for `//:compiledb`. |
| `fuzz_test_runner` | `//tools/mayhem` | Runs Mayhem/LibFuzzer harnesses. |
| `libfuzzer_coverage` / `libfuzzer_aggregate_coverage` / `mayhem_testsuite` | `//tools/mayhem` | Coverage for fuzz harnesses. |
| `generate_memory_stats` / `stack_analysis` / `flash_memory_stats` | `//tools/python/...` | Memory, stack, flash analytics. |
| `generate_stream_files` | `//tools/python/streamGenerator:stream_generator.bzl` | Per-variant stream regeneration when `--enable_stream_generation=True`. |
| `Stream_Bandwidth_Info` | `//tools/python/streamBandwidth:stream_bandwidth_rule.bzl` | Emits `Stream_Bandwidth_Details.csv`; per-variant `variant_id` (`srr7e=076`, `flr7=071`, `srr7p=079`, `flr7v3=071`). |
| `arxml_parser_wrapper` | `//tools/python/arxmlParser` | Parses AUTOSAR ARXML. |
| `platforms_transition` | `@bazel_platform//platforms:platforms_transition.bzl` | 1:1 Starlark transition to swap `--platforms`. |
| `cc_binary_bbe32` | `@bazel_platform//toolchains/xtensa:cc_binary_bbe32.bzl` | `cc_binary` specialized for the Xtensa BBE32 toolchain. |

### 8.1 Variant-specific `select()` for calibrations

```starlark
srcs = select({
    "@build_config//:srr7e":           ["@smc_srr7e//:smc_cal_ptp",             "@usc_srr7e//:usc_cal_ptp"],
    "@build_config//:flr7_satellite":  ["@smc_flr7//:smc_cal_ptp",              "@usc_flr7//:usc_cal_ptp"],
    "@build_config//:flr7_emb_tracker":        ["@smc_flr7_standalone//:smc_cal_ptp",   "@usc_flr7//:usc_cal_ptp"],
    "@build_config//:srr7p_satellite": ["@smc_srr7p//:smc_cal_ptp",             "@usc_srr7p//:usc_cal_ptp"],
    "@build_config//:srr7p_emb_tracker":       ["@smc_srr7p_standalone//:smc_cal_ptp",  "@usc_srr7p//:usc_cal_ptp"],
    "@build_config//:flr7v3":          ["@smc_flr7v3//:smc_cal_ptp",            "@usc_flr7v3//:usc_cal_ptp"],
})
```
The `<variant>_{satellite,rot}` keys are `selects.config_setting_group`s composing `:variant` AND `@appl_inclusion_dep//:rot_fusion`.

### 8.2 Variant-specific `outputDir`

```starlark
outputDir = select({
    "@build_config//:srr7e":  "outputs/srr7e",
    "@build_config//:flr7":   "outputs/flr7",
    "@build_config//:srr7p":  "outputs/srr7p",
    "@build_config//:flr7v3": "outputs/flr7v3",
})
```
Every `copy_to_dir` that lands under `outputs/<variant>/` uses this exact select — adding a new variant means updating *every* such `select` in the repo.

### 8.3 Host-OS `select`

```starlark
srcs = select({
    "@bazel_tools//src/conditions:host_windows": ["@StreamHeader_Gen//:LE_Defined_StreamGeneratorWindows"],
    "//conditions:default":                      ["@StreamHeader_Gen//:StreamGeneratorLinux"],
})
```

### 8.4 Micro-revision `select` (RFE firmware)

```starlark
select({
    "@build_config//:ES1": {"//software/m7/mmic/drivers/RFE_driver/ES1/bin:rfe.s19":     "rfe.s19"},
    "@build_config//:ES2": {"//software/m7/mmic/drivers/RFE_driver/ES2/bin:rfe_enc.s19": "rfe_enc.s19"},
})
```

### 8.5 Adding a new `bool_flag` + alias

```starlark
load("@bazel_skylib//rules:common_settings.bzl", "bool_flag")

bool_flag(
    name = "MY_FEATURE",
    build_setting_default = False,
    visibility = ["//visibility:public"],
)
config_setting(
    name = "Build_MY_FEATURE",
    flag_values = {":MY_FEATURE": "true"},
    visibility = ["//visibility:public"],
)
```
Then in `.bazelrc`:
```
common --flag_alias=my_feature=//:MY_FEATURE
```
And document in [README.md](../../../README.md) under "Conditional Builds".

### 8.6 Platform transition for BBE32 DSP

From [software/bbe32/BUILD](../../../software/bbe32/BUILD):
```starlark
load("@bazel_platform//platforms:platforms_transition.bzl", "platforms_transition")
load("@bazel_platform//toolchains/xtensa:cc_binary_bbe32.bzl", "cc_binary_bbe32")

cc_binary_bbe32(name = "bbe32App", srcs = ["main.c"], ...)

platforms_transition(
    name = "bbe32",
    actual_target = ":bbe32App",
    set_platform = select({
        "@platforms//os:windows": "@build_config//:winbbe32",
        "//conditions:default":   "@build_config//:linuxbbe32",
    }),
)
```
`platforms_transition` is a 1:1 Starlark transition that switches `--platforms` for the target's subgraph, driving toolchain resolution to Xtensa.

### 8.7 Local BB override

In [user.bazelrc](../../../user.bazelrc) (never committed):
```
common --override_repository=afbb=C:/Projects/Core_Radar_Gen7_SAF85xx_Angle_Finding
common --override_repository=spbb=/home/me/spbb
```
The `<name>` is the `http_archive(name=...)` apparent repo name from `bb.MODULE.bazel`. Equivalent Bzlmod form inside `MODULE.bazel` (root-module only):
```starlark
local_path_override(module_name = "afbb", path = "C:/Projects/...")
```

### 8.8 Updating a BB version

In `bb.MODULE.bazel`, update the matching `http_archive`:
```starlark
http_archive(
    name   = "afbb",
    sha256 = "<new-64-hex>",                           # OR integrity = "sha256-<base64>="
    urls   = ["https://jfrog.asux.aptiv.com/..."],
)
```
Steps:
1. Copy the "URL to file" and "SHA-256" from JFrog Artifactory (General pane).
2. Rebuild all four variants to confirm nothing breaks.
3. If the BB has migrated to Bzlmod, prefer `bazel_dep(name="...", version="x.y.z", repo_name="...")` — already done for `quickflash`, `core_radar_swc_plt_appl_diag` (→ `@SWC_PLT_Appl_Diag`), `core_radar_swc_plt_appl_faultmgr` (→ `@SWC_PLT_Appl_FaultMgr`), `core_radar_log_struct_gen` (→ `@StreamHeader_Gen`).

> **🔗 Maintainer pairing rules** (from [bb.MODULE.bazel](../../../bb.MODULE.bazel) comments):
> - **`smc_srr7p_standalone` must be updated alongside `smc_srr7p`** — the two SMCs form a paired satellite/standalone set for the same release.
> - **`smc_flr7v3` / `usc_flr7v3` are placeholders** currently tracking the latest `flr7` calibrations until `flr7v3` bring-up finishes (explicit `TODO` comments).
> - **`Core_Radar_Reuse`** (`@reuse`) uses `sha256=` while newer BBs use `integrity=sha256-...=` (Subresource Integrity). Both are valid; pick whichever JFrog shows.

## 9. `.bazelrc` Highlights

- `common --jobs="HOST_CPUS*.5"` — use half host CPUs.
- `common --enable_platform_specific_config` — auto-applies `:windows` / `:linux` sections.
- `common --enable_bzlmod` + `--lockfile_mode=off`.
- `common --spawn_strategy=local` — no sandboxing (Wind River needs this).
- `common --experimental_cc_implementation_deps` — enables `implementation_deps` on `cc_library`.
- `common --host_crosstool_top=@bazel_tools//tools/cpp:toolchain` — host build tools use default toolchain.
- `common --incompatible_no_implicit_file_export` + `--incompatible_enable_cc_toolchain_resolution` — modern-Bazel hygiene.
- `common --repo_env=BAZEL_DO_NOT_DETECT_CPP_TOOLCHAIN=1` — forces our toolchains.
- `build --stamp` + `workspace_status_command=software/common/versions/linkstamp/gen_workspace_status.{bat,sh}` — embed git/version info into binaries via linkstamp.
- `build --experimental_platform_in_output_dir` — readable output-dir names (e.g. `bazel-out/winm7-fastbuild/`).
- `build:CAN_Tx_Dummy --define=Build_CAN_TX_Dummy_Data=true` — shortcut config.
- Unit-test defaults: `--compilation_mode=dbg`, `--test_output=errors`, `-O0`, `-fpermissive`, `-DOS_STATIC_CODE_ANALYSIS`, `-fno-exceptions`, `--@spbb//common:use_bbe_cstub_simulator=True`. Warning suppressions: `-Wno-unknown-pragmas`, `-Wno-unused-function`, `-Wno-unused-variable`, `-Wno-uninitialized`, `-Wno-missing-braces` (needed for AUTOSAR-generated code).
- Windows-only test: `--@bazel_platform//quality/ut:tmp_dir=C:/tmp` — avoids long-path breakage in coverage.
- Imports: `sanitizers.bazelrc`, `coverity.bazelrc`, `mayhem.bazelrc`, `arb_cache.bazelrc`, `sil/emb_lib/.bazelrc`, `try-import user.bazelrc`.
- `--flag_alias=valgrind=...` is defined only in `sanitizers.bazelrc` (activated by `--config=valgrind`); the root `.bazelrc` does not re-alias it.

### 9.1 Sanitizers ([sanitizers.bazelrc](../../../tools/bazel/config/sanitizers.bazelrc))

- `--config=asan` — `-fsanitize=address,leak,pointer-compare,pointer-subtract`, `-fsanitize-address-use-after-scope`, `-fno-common`, `--linkopt=-static-libasan`, rich `ASAN_OPTIONS` env.
- `--config=ubsan` — `-fsanitize=undefined`, `-fno-sanitize=vptr`, `-fno-omit-frame-pointer`.
- `--config=valgrind` — sets the `--valgrind` flag; Valgrind wiring lives in `@bazel_platform//tools/valgrind:valgrind.bzl`.

### 9.2 RapidBuild cache + BES ([arb_cache.bazelrc](../../../tools/bazel/config/arb_cache.bazelrc))

```
build --bes_backend=grpcs://rapidbuild.grpc.asux.aptiv.com:8001/
build --bes_header=x-arb-project-id=gpo/core_radar_gen7_saf85xx
build --bes_results_url=https://rapidbuild.asux.aptiv.com/organizations/gpo/projects/core_radar_gen7_saf85xx/invocations/
build --bes_upload_mode=fully_async
build --remote_cache=grpcs://rapidbuild.grpc.asux.aptiv.com:8000/
build --remote_header=x-arb-project-id=gpo/core_radar_gen7_saf85xx
build --remote_download_all
build --experimental_guard_against_concurrent_changes
build --experimental_remote_cache_eviction_retries=1
```
JFrog access needs a working `~/.netrc` — provision via `python repo_init.py`.

## 10. Common Troubleshooting

| Symptom | Likely Cause / Fix |
|---------|---------------------|
| `no matching toolchains found for target platform` | Missing `platforms_transition`, wrong `--variant`, or constraint mismatch. Inspect `tools/bazel/config/BUILD` platforms + `//cpu` constraints. Try `--toolchain_resolution_debug=.*`. |
| `error: no such package '@spbb//...'` | Bad `.netrc` (run `python repo_init.py`) or incorrect `sha256`/`integrity` in `bb.MODULE.bazel`. Verify the JFrog URL is reachable. |
| Wrong `outputs/<variant>` directory | `copy_to_dir.outputDir` or `aptivAppOutputs.srcs` select is missing a variant arm. Every variant-select must list all four variants. |
| "configurable attribute doesn't match this configuration" / no matching conditions | Add a missing variant/rot key or `"//conditions:default": []`. `srr7p_emb_tracker` requires `@appl_inclusion_dep//:rot_fusion` ∈ {`master`,`standalone`,`slave`}. Use `bazel cquery` + `bazel config <hash>` to see the active config. |
| Unit test fails ONLY with `-Og` | Tests default to `-O0`. Add a `# This test fails with -Og` comment in `CORE_TESTS` (pattern already used). |
| Windows long-path errors during coverage | In a user-level `.bazelrc`: `startup --output_user_root=C:/bzl`, and `test:windows --@bazel_platform//quality/ut:tmp_dir=C:/tmp`. Use the same drive as the workspace. |
| Local BB changes not picked up | The override name is the `http_archive(name=...)`, not the folder name. `--override_repository=...` goes on `common`, not `build`/`test`. |
| `MODULE.bazel.lock` conflicts | `.bazelrc` sets `--lockfile_mode=off`; delete any committed lockfile. |
| "Multiple conditions match" in `select()` | Conditions must be unambiguous (identical values or strict specialization). Use `selects.config_setting_group` to combine, and ensure composite keys are mutually exclusive. |
| BES/cache upload warnings on fast builds | Harmless: `bes_upload_mode=fully_async` prints a warning when the 5-second upload window elapses. |
| `--config=mayhem` tests don't appear on the server | Mayhem uses `bazelisk run :fuzz --config mayhem` (not `build`). Check `.netrc` and follow the console Run URL. |
| Circular BB dep between `appl_inclusion_dep` and a BB | The BB archive expects `@appl_inclusion_dep//...`, and the overlay at `software/building_block/common` re-exports types. Never remove the `local_repository("appl_inclusion_dep", ...)` entry. |

## 11. Where to Look First

- Unknown CLI flag → `grep -n 'flag_alias=<name>=' .bazelrc` (also check `mayhem.bazelrc`, `coverity.bazelrc`, `sanitizers.bazelrc`).
- Unknown external repo (`@foo//...`) → `bb.MODULE.bazel` for `http_archive(name="foo")`, OR `MODULE.bazel` for `local_repository(name="foo")` / `bazel_dep(..., repo_name="foo")`.
- Unknown top-level target `//:X` → [BUILD](../../../BUILD).
- Variant / platform / rot conditions → [tools/bazel/config/BUILD](../../../tools/bazel/config/BUILD) and [software/building_block/common/BUILD](../../../software/building_block/common/BUILD).
- Per-core main wiring → `software/{m7,a53,bbe32}/BUILD`.
- Shared headers / streams → [software/common/BUILD](../../../software/common/BUILD).
- Flash image / S19 surgery → [software/BUILD](../../../software/BUILD).
- Toolchain internals → `tools/bazel/toolchains/<name>/BUILD.bazel` and `compilers.MODULE.bazel`.
- Module extensions → [extensions.bzl](../../../extensions.bzl) (Coverity), `@rules_mayhem//mayhem:extensions.bzl`.
- Custom rule implementations → [tools/bazel/scripts/](../../../tools/bazel/scripts/) (`copy_to_dir.bzl`, `save_build_config.bzl`, `ld_preprocess.bzl`, `generate_stream_def.bzl`).
- Coverage exclude regexes → [coverage/BUILD](../../../coverage/BUILD) (`GLOBAL_EXCLUDES`, `GLOBAL_COVERAGE_EXCLUDES`, `TOPLEVEL_EXCLUDES`, `UNTESTED_GRAPH_ONLY_EXCLUDES`).

### Bzlmod external path scheme (critical for coverage/regex excludes)

With Bzlmod, repos declared via `use_repo_rule` get a canonical name like `_main~_repo_rules~afbb` (not just `afbb`). The `external/` layout is:

| Repo type | Path pattern |
|-----------|--------------|
| `bazel_dep` (module, MVS-resolved) | `external/<name>~<version>/` or `external/<name>~/` |
| `use_repo_rule` `http_archive` in MODULE.bazel | `external/_main~_repo_rules~<name>/` |
| `use_extension` repo | `external/_main~<ext_name>~<repo_name>/` |
| `local_repository` (overlay) | `external/<name>~/` |

Use `.*` after `external/` when writing regex filters (the repo root matches any of the variants above). Examples from [coverage/BUILD](../../../coverage/BUILD): `external/.*SAF85xx_MCAL/.*`, `external/.*afbb/module/_common/radar_math/_src/...`.

## 12. Do-Not List

- **Never** invoke `bazel` directly — always `./bazelisk` / `bazelisk`. Wrong Bazel version breaks toolchain resolution.
- **Never** commit `user.bazelrc` or local `--override_repository=...` paths.
- **Never** edit generated MCAL/AUTOSAR code under `software/m7/mcal/app/output/` or `software/m7/autosar/config/Appl/GenData/` — regenerate via the relevant tool.
- **Never** add a `--flag_alias=` without a matching `README.md` entry (Gerrit CI can reject).
- **Never** omit `sha256`/`integrity` from a new `http_archive` — hermeticity and supply-chain requirement.
- **Never** run `bazelisk clean` as a first debugging step; builds are incremental and reliable.
- **Never** hard-code system paths; always go through a registered toolchain.
- **Never** close Dynamic-Analysis bypass JIRA tickets before the fix is merged to `/dev` — doing so blocks all merges.
- **Never** commit a `MODULE.bazel.lock`.
- **Never** use `bind()`; it's deprecated. Use `alias()` with a `select()` on `actual` for configurable dispatch.
- **Never** `select()` directly on a `platform` label — `select` on `constraint_values` or `config_setting`+`constraint_values` instead ([Bazel FAQ](https://bazel.build/docs/configurable-attributes#faq)).
- **Never** update `smc_srr7p` without also updating `smc_srr7p_standalone` (and vice-versa) — they must stay in lockstep.
- **Never** assume `flr7v3` has the same satellite/rot composite `config_setting_group`s as `srr7p`/`flr7` — it does NOT; only those two variants define `:<variant>_satellite` / `:<variant>_rot` in `@build_config`.
- **Never** hand-edit `bazel-out/`, `bazel-bin/`, or any `external/` directory — all artifacts are regenerated from sources + pinned hashes.
- **Never** use the old `external/<repo>/...` regex in coverage/exclude lists — under Bzlmod the canonical name is `external/_main~_repo_rules~<repo>/`; use `external/.*<repo>/.*`.

## 13. Expert-Level Correctness Notes & Known Footguns in THIS Repo

These are subtle issues a strong Bazel engineer should spot. They come from actually reading the BUILD files in this workspace, not from generic docs.

### 13.1 `@appl_inclusion_dep//:variant` is an orphan `string_flag` (latent bug)

Two independent `string_flag`s both named `variant` exist:
1. [`@build_config//:variant`](../../../tools/bazel/config/BUILD) — aliased by `--variant=<X>` in root `.bazelrc`.
2. [`@appl_inclusion_dep//:variant`](../../../software/building_block/common/BUILD) — **NOT** aliased anywhere.

So `@appl_inclusion_dep//:variant` is **never** updated by the `--variant=<X>` CLI flag and permanently stays at its `build_setting_default = "srr7p"`.

`config_setting`s like `@appl_inclusion_dep//:srr7p`, `:flr7`, `:srr7e`, `:flr7v3` key off this frozen flag. They are used by real targets, e.g. `//software/a53/id:interference_detection_algo_feature_lib` (`local_defines = select({ "@appl_inclusion_dep//:srr7p": ["ID_ENABLE"], "@appl_inclusion_dep//:srr7e": [], ... })`).

**Effect**: With `--variant=srr7e` or `--variant=flr7v3`, those id-library selects still match the `srr7p` arm (because `@appl_inclusion_dep//:variant` is stuck on `srr7p`) and define `ID_ENABLE`. The srr7e arm is dead code.

**Correct fixes** (pick ONE):
- Alias: `common --flag_alias=variant=@appl_inclusion_dep//:variant` — but you can only alias a single target once, so you'd drop the `@build_config` alias and re-point every consumer.
- Better: delete `@appl_inclusion_dep//:variant` and re-point its `config_setting`s to `flag_values = {"@build_config//:variant": "srr7p"}` (one source of truth).
- Or: add a user-level transition that forwards `@build_config//:variant` → `@appl_inclusion_dep//:variant`.

Always check for this duplicate-flag pattern before modifying id/alignment selects.

### 13.2 Duplicate `bool_flag` declarations (dead code)

`enable_bench_testing` and `align_det_test` are declared in **two** packages:
- Root [BUILD](../../../BUILD) (`//:enable_bench_testing`, `//:align_det_test`)
- [software/building_block/common/BUILD](../../../software/building_block/common/BUILD) (`@appl_inclusion_dep//:enable_bench_testing`, `:align_det_test`)

The `--enable_bench_testing` / `--align_det_test` flag aliases in `.bazelrc` point to `@appl_inclusion_dep//:...`, and all consumers `select()` on the `@appl_inclusion_dep` versions. **The root-BUILD copies are dead code** and should eventually be removed. Don't add new consumers of the `//:` copies.

### 13.3 `flag_values` string semantics

- **`bool_flag`**: Bazel canonicalises `"True"`, `"true"`, `"1"` → true and `"False"`, `"false"`, `"0"` → false. So `flag_values = {":X": "true"}` and `{":X": "True"}` are equivalent. This repo mixes both casings (see root `BUILD` lines 247, 393, 520, etc.) — pick one convention when editing for consistency.
- **`string_flag`**: case-sensitive. `"srr7p"` ≠ `"SRR7P"`. The `values=[...]` allowlist is also case-sensitive.
- **Native-flag `config_setting`** (`values=`): always the literal CLI string (`values = {"compilation_mode": "dbg"}`).
- **Legacy `--define`** (`define_values=`): CLI form is `--define=KEY=VALUE`; match with `define_values = {"KEY": "VALUE"}`. Prefer `bool_flag`/`string_flag` for anything new.

### 13.4 `select()` ambiguity rules (from Bazel spec)

When multiple `config_setting`s match the current configuration, Bazel requires either:
- **Identical values** across the matching keys (then any one wins, safely), OR
- **Strict specialization**: one matching `config_setting`'s `flag_values`/`constraint_values`/`define_values` must be a strict superset of every other matching one.

Otherwise Bazel emits `configurable attribute "X" doesn't match this configuration` with a "multiple conditions match" error. Use `selects.config_setting_group(match_all=[...])` to build specialized composites.

**Anti-pattern**: relying on `//conditions:default` to "cover" a missing variant. Prefer exhaustive per-variant arms so a new variant forces a compile-time failure — Bazel's `select()` is your type-system.

### 13.5 `defines` vs `local_defines` vs `copts`

- **`defines`** — exported to all reverse-deps (propagated). Use sparingly; think of them as part of the `cc_library`'s ABI.
- **`local_defines`** — applied only to the library's own compilation, not propagated. Prefer this for implementation-only switches (e.g. `"ID_ENABLE"` in `//software/a53/id:interference_detection_algo_feature_lib`).
- **`copts`** — raw compiler flags for this target's compilation.
- **`conlyopts` / `cxxopts`** — C-only / C++-only variants.

### 13.6 `implementation_deps` (private deps)

`common --experimental_cc_implementation_deps` is enabled in `.bazelrc`. This unlocks `cc_library(implementation_deps=[...])` — deps that are used internally but NOT re-exported in the `CcInfo` headers. Use it for helper libraries you don't want to leak into the public include graph of a BB.

### 13.7 `alias()` instead of `bind()`

`bind()` is deprecated. For a configurable label, write:
```starlark
alias(
    name = "cal_ptp",
    actual = select({
        "@build_config//:srr7e":  "@smc_srr7e//:smc_cal_ptp",
        "@build_config//:flr7":   "@smc_flr7//:smc_cal_ptp",
        "@build_config//:srr7p":  "@smc_srr7p//:smc_cal_ptp",
        "@build_config//:flr7v3": "@smc_flr7v3//:smc_cal_ptp",
    }),
)
```

### 13.8 Platform / constraint pitfalls

- Don't `select()` on a `platform` label; `select()` on `constraint_values` (e.g. `@platforms//os:windows`) or a `config_setting` with `constraint_values = [...]`.
- `platforms_transition` is a **1:1 Starlark transition** — the target (e.g. BBE32) and its entire subgraph build under the new `--platforms`, but the parent configuration is unchanged.
- `constraint_value(name="bbe32", constraint_setting="@platforms//cpu:cpu")` in [tools/bazel/config/cpu/BUILD](../../../tools/bazel/config/cpu/BUILD) is the canonical way to gate embedded-only code:
  ```starlark
  deps = [...] + select({
      "@build_config//cpu:bbe32": [<embedded lib>],
      "@platforms//cpu:x86_64":   [<SIL mock>],
      "//conditions:default":     [],
  })
  ```

### 13.9 Visibility hygiene

Almost every target in this repo uses `visibility = ["//visibility:public"]`. That is convenient but disables Bazel's dependency-layering enforcement. When adding new infrastructure, prefer narrow visibility lists (`["//software/a53/...", "//software/common/..."]`) to catch cross-layer violations at build time.

### 13.10 Hermeticity checklist for new `http_archive`s

- Exactly one of `sha256="<64 hex>"` or `integrity="sha256-<base64>="`. Missing = supply-chain violation.
- Prefer `urls=[<primary>, <mirror>]` over a single `url=` — JFrog outages have bitten this repo before.
- Set `strip_prefix` if the archive wraps a single top-level dir (common with GitHub tarballs).
- Overlay a BUILD via `build_file` or `build_file_content` for archives that don't ship one (pattern used for SMC/USC PTPs).

### 13.11 Starlark hygiene

- `load("@bazel_skylib//rules:common_settings.bzl", "bool_flag", "string_flag")` — always load from `@bazel_skylib`, never declare your own.
- `load("@bazel_skylib//lib:selects.bzl", "selects")` for `config_setting_group`.
- Keep `bool_flag` + matching `config_setting` pairs in the **same** BUILD file — easier to grep and impossible to accidentally delete the flag while leaving the `config_setting` orphaned.
- Don't use `glob([...])` for source lists on embedded targets; enumerate files so incremental builds don't silently pick up new sources.

## 14. Debugging Selects & Toolchains

When a `select()` resolves to the wrong arm or a toolchain fails to resolve:

```bash
# See the full configuration hash of a target
./bazelisk cquery //:gen7 --variant=srr7p --output=build

# Dump every config_setting match evaluation
./bazelisk cquery //:gen7 --variant=srr7p --output=config

# Inspect one specific configuration hash (taken from cquery output)
./bazelisk config <hash>

# Show why toolchain resolution picked what it picked
./bazelisk build //software/bbe32:bbe32 --toolchain_resolution_debug=.* 2>&1 | head -80

# Print the action that was actually spawned (full cmdline + inputs)
./bazelisk aquery //software/bbe32:bbe32App --variant=srr7p

# Show the analysis-phase dependency graph for a select
./bazelisk query 'deps(//software/a53/id:interference_detection_algo_feature_lib)' --output=label_kind

# Find every target that reads a flag
./bazelisk cquery 'attr("flag_values", ".*@appl_inclusion_dep//:variant.*", //...)' --variant=srr7p
```

The killer combo is `cquery --output=config` + `config <hash>` — it prints the exact `build_setting` values and constraint assignments Bazel used, which is how you prove bugs like §13.1.
