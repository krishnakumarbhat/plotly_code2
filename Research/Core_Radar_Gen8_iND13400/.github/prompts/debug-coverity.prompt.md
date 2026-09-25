---
mode: agent
description: "Fix Coverity static analysis defects"
---

# Debug Coverity Defects

## Input
- Paste the Coverity defect report or CI failure output
- Which core: R52 or BBE32
- Which variant: flr8 or srr8p

## Build Coverity Locally

### R52 Analysis
```bash
bazelisk build //software/r52:windriver_r52_cov --config=flr8
```

### BBE32 Analysis
```bash
bazelisk build //software/bbe32:xtensa_bbe32_cov --config=flr8
```

### Commit/Check Defects (requires auth key)
```bash
# Generate auth key first from https://coverity.asux.aptiv.com/
bazelisk run //software/r52:commit_defects --config=flr8 -- --auth-key-file="$HOME/coverity.auth"
```

## CI Failure Strategy

CI fails on:
- **New Medium/High impact defects** (`fail-on-new-medium-high`)
- **New MISRA Mandatory defects** (`fail-on-new-mandatory`)

Nightly builds commit ALL results to server (no failure on defects).

## Coverity Streams

| Variant | Core | Stream |
|---------|------|--------|
| FLR8 | R52 | `10034606_Core_Radar_Gen8_IND13400_FLR8_R52` |
| FLR8 | BBE32 | `10034606_Core_Radar_Gen8_IND13400_FLR8_BBE32` |
| SRR8P | R52 | `10034606_Core_Radar_Gen8_IND13400_SRR8p_R52` |
| SRR8P | BBE32 | `10034606_Core_Radar_Gen8_IND13400_SRR8p_BBE32` |

## Common MISRA C:2012 Fixes

| Rule | Issue | Fix |
|------|-------|-----|
| Rule 10.1 | Implicit type conversion | Use explicit cast: `(uint32_t)x` |
| Rule 10.3 | Narrowing conversion | Cast to target type |
| Rule 10.4 | Mismatched arithmetic types | Ensure same type on both sides |
| Rule 11.3 | Cast between pointer types | Use intermediate `void*` or redesign |
| Rule 12.1 | Operator precedence | Add explicit parentheses |
| Rule 14.3 | Dead code (always true/false) | Remove unreachable branch |
| Rule 15.7 | Missing else clause | Add `else { /* no action */ }` |
| Rule 17.7 | Unused return value | `(void)function_call();` |
| Rule 21.6 | Use of banned functions | Replace `printf` → use logging system |

## Common High-Impact Defect Fixes

| Checker | Issue | Fix |
|---------|-------|-----|
| UNINIT | Uninitialized variable | Initialize at declaration |
| NULL_RETURNS | Null pointer dereference | Add NULL check before use |
| OVERRUN | Buffer overflow | Validate index bounds |
| RESOURCE_LEAK | Unclosed resource | Add cleanup path |
| DEADCODE | Unreachable code | Remove or fix logic |
| USE_AFTER_FREE | Dangling pointer | Clear pointer after free |
| CHECKED_RETURN | Ignoring error return | Check return value |

## Skipped Files (NOT analyzed)

Always skipped:
- `external/.*iND13400_autosar_sip/.*`
- `external/.*iND13400_sdk/.*`

Non-nightly additionally skips:
- `software/r52/autosar/config/Appl/GenData/.*`

## Fix Procedure
1. Identify defect type (MISRA rule or checker name)
2. Look up fix in tables above
3. Apply minimal fix (don't over-engineer)
4. Rebuild Coverity target to verify fix clears:
   ```bash
   bazelisk build //software/r52:windriver_r52_cov --config=flr8
   ```
5. Also verify normal build still passes:
   ```bash
   bazelisk build //:gen8 --config=flr8
   ```

## Config Files

| File | Purpose |
|------|---------|
| `tools/bazel/config/coverity.bazelrc` | Coverity flag aliases |
| `tools/bazel/config/coverity.bzl` | Shared constants (CODING_STANDARD_CONFIG, etc.) |
| `tools/coverity/BUILD` | `nightly` bool_flag + `coverityNightly` config_setting |
| `tools/coverity/coding-standards/misrac2012/` | 8 MISRA config files |
| `tools/coverity/cov-cli.toml` | Legacy CLI config |
