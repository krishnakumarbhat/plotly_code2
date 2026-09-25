# Reusable GitHub Workflows for Bazel-based Projects

This directory contains modular, reusable GitHub Actions workflows designed for Bazel-based projects. These workflows can be extracted to a shared repository and used across multiple projects.

## Workflows

### 1. `build.yml` - Bazel Build Workflow

Reusable workflow for building Bazel targets with optional matrix fan-out, summary reporting, artifact upload, and optional nightly metrics publish.

**Usage Example:**
```yaml
jobs:
  build:
    uses: your-org/shared-workflows/.github/workflows/build.yml@main
    with:
      target: //:your_target
      config: flr8
      upload-artifact-name: build-output.zip
      upload-artifact-path: bazel-bin/outputs
    secrets:
      GHCR_USER: ${{ secrets.GHCR_USER }}
      GHCR_TOKEN: ${{ secrets.GHCR_TOKEN }}
      NETRC_CONTENT: ${{ secrets.NETRC_CONTENT }}
      ARB_TOKEN: ${{ secrets.ARB_TOKEN }}
```

**Inputs:**
- `target` (required): Bazel build target, for example `//:gen8`
- `config` (optional, default `flr8`): Bazel config name
- `matrix-json` (optional): JSON matrix include array for internal fan-out
- `pr-number` (optional): PR number for failure-label management
- `nightly` (optional, default `false`): Enables metrics generation/publish path
- `container-image` (optional): Runner container image
- `upload-artifact-name` (optional): Uploaded artifact name
- `upload-artifact-path` (optional): Artifact path(s), semicolon-separated
- `runner` (optional): Runner label
- `bazel-extra-flags` (optional): Additional Bazel flags

**Secrets:**
- Required: `GHCR_USER`, `GHCR_TOKEN`, `NETRC_CONTENT`, `ARB_TOKEN`
- Optional (nightly metrics DB publish): `METRICS_DB_USER`, `METRICS_DB_PASSWORD`, `METRICS_DB_HOST`, `METRICS_DB_PORT`, `METRICS_DB_NAME`

---

### 2. `unit-tests.yml` - Unit Test Workflow

Reusable workflow for Bazel unit tests with retry-once behavior, coverage parsing, true-count execution, summary generation, artifact upload, and optional nightly metrics publish.

**Usage Example:**
```yaml
jobs:
  unit-test:
    uses: your-org/shared-workflows/.github/workflows/unit-tests.yml@main
    with:
      test-target: //coverage:report
      config: flr8
      nightly: true
      force-db-publish: false
      upload-artifact-name: unit-test-output.zip
    secrets:
      GHCR_USER: ${{ secrets.GHCR_USER }}
      GHCR_TOKEN: ${{ secrets.GHCR_TOKEN }}
      NETRC_CONTENT: ${{ secrets.NETRC_CONTENT }}
      ARB_TOKEN: ${{ secrets.ARB_TOKEN }}
      METRICS_DB_USER: ${{ secrets.METRICS_DB_USER }}
      METRICS_DB_PASSWORD: ${{ secrets.METRICS_DB_PASSWORD }}
      METRICS_DB_HOST: ${{ secrets.METRICS_DB_HOST }}
      METRICS_DB_PORT: ${{ secrets.METRICS_DB_PORT }}
      METRICS_DB_NAME: ${{ secrets.METRICS_DB_NAME }}
```

**Inputs:**
- `test-target` (required): Bazel test target
- `config` (optional, default `flr8`): Bazel config name
- `matrix-json` (optional): JSON matrix include array for internal fan-out
- `pr-number` (optional): PR number for failure-label management
- `nightly` (optional, default `false`): Enables metrics generation/publish path
- `force-db-publish` (optional, default `false`): Bypass branch/event gating for DB publish attempt
- `container-image` (optional): Runner container image
- `upload-artifact-name` (optional): Uploaded artifact name
- `upload-artifact-paths` (optional): Artifact paths, newline-separated
- `runner` (optional): Runner label
- `bazel-extra-flags` (optional): Additional Bazel flags
- `artifact-if-no-files-found` (optional): `error`, `warn`, or `ignore`

**Secrets:**
- Required: `GHCR_USER`, `GHCR_TOKEN`, `NETRC_CONTENT`, `ARB_TOKEN`
- Optional (nightly metrics DB publish): `METRICS_DB_USER`, `METRICS_DB_PASSWORD`, `METRICS_DB_HOST`, `METRICS_DB_PORT`, `METRICS_DB_NAME`

---

### 3. `coverity.yml` - Coverity Analysis Workflow

Reusable workflow for matrix Coverity analysis, summary extraction, optional coverity-summary generation, artifact upload, and optional nightly metrics publish.

**Usage Example:**
```yaml
jobs:
  coverity:
    uses: your-org/shared-workflows/.github/workflows/coverity.yml@main
    with:
      matrix-json: |
        [
          {
            "variant": "flr8",
            "core": "r52",
            "coverity_target": "//software/r52:commit_defects",
            "coverity_stream": "YOUR_STREAM_ID_R52"
          },
          {
            "variant": "flr8",
            "core": "bbe32",
            "coverity_target": "//software/bbe32:commit_defects",
            "coverity_stream": "YOUR_STREAM_ID_BBE32"
          }
        ]
    secrets:
      GHCR_USER: ${{ secrets.GHCR_USER }}
      GHCR_TOKEN: ${{ secrets.GHCR_TOKEN }}
      NETRC_CONTENT: ${{ secrets.NETRC_CONTENT }}
      ARB_TOKEN: ${{ secrets.ARB_TOKEN }}
      COVERITY_COMMITTER_AUTH: ${{ secrets.COVERITY_AUTH }}
```

**Inputs:**
- `matrix-json` (required): JSON array of matrix configurations
  - Required fields per matrix item:
    - `variant`: Build variant (e.g., `flr8`, `srr8p`)
    - `core`: Processor core (e.g., `r52`, `bbe32`)
    - `coverity_target`: Bazel target for Coverity analysis
    - `coverity_stream`: Coverity stream ID
- `pr-number` (optional): PR number for failure-label management
- `container-image` (optional): Docker image
- `runner` (optional): Runner label
- `bazel-extra-flags` (optional): Additional Bazel flags
- `nightly` (optional, default `false`): Adds nightly flags and enables metrics path
- `upload-artifact-paths` (optional): Paths to upload

**Secrets:**
- Required: `GHCR_USER`, `GHCR_TOKEN`, `NETRC_CONTENT`, `ARB_TOKEN`, `COVERITY_COMMITTER_AUTH`
- Optional (nightly metrics DB publish): `METRICS_DB_USER`, `METRICS_DB_PASSWORD`, `METRICS_DB_HOST`, `METRICS_DB_PORT`, `METRICS_DB_NAME`

---

### 4. `smoke-test.yml` - Hardware Smoke Test Workflow

Reusable workflow for hardware smoke tests on self-hosted runners with artifact download, quickflash, smoke execution, summary report parsing, optional report upload, and optional nightly metrics publish.

**Usage Example:**
```yaml
jobs:
  smoke-test:
    needs: build
    uses: your-org/shared-workflows/.github/workflows/smoke-test.yml@main
    with:
      matrix-json: |
        [
          {
            "variant": "flr8",
            "artifact_name": "build-flr8.zip",
            "quickflash_variant": "flr8_satellite_can",
            "smoke_test_report_path": "/path/to/flr8_Smoke_Test.html, /path/to/flr8_dashboard.json"
          }
        ]
      runner-labels: '["self-hosted","linux","SmokeTest"]'
      timeout-minutes: 45
      commit-sha: ${{ github.sha }}
```

**Inputs:**
- `artifact-name` (optional): Downloaded artifact name (overridden by `matrix-json` values)
- `artifact-path` (optional): Artifact extract path
- `quickflash-script-path` (optional): Path to `RunQuickFlash.sh` within artifact
- `quickflash-variant` (optional): Quickflash variant (overridden by `matrix-json` values)
- `smoke-test-report-path` (optional): Comma-separated expected report paths (overridden by `matrix-json` values)
- `runner-labels` (optional): JSON array string for self-hosted labels
- `timeout-minutes` (optional): Job timeout in minutes
- `matrix-json` (optional): JSON matrix include array for internal fan-out
- `pr-number` (optional): PR number for failure-label management
- `nightly` (optional, default `false`): Enables metrics generation/publish path
- `commit-sha` (optional): Commit SHA propagated to bench clone/setup

**Secrets:**
- No required runtime secrets for quickflash/smoke execution
- Optional (nightly metrics DB publish): `METRICS_DB_USER`, `METRICS_DB_PASSWORD`, `METRICS_DB_HOST`, `METRICS_DB_PORT`, `METRICS_DB_NAME`

**Special Notes:**
- Requires `needs: build` in the calling workflow
- Runs on self-hosted runners only
- Use `if: ${{ always() }}` to run even if prior jobs fail
- Can be used with matrix strategies for multi-variant testing

---

### 5. `strict-codeowners.yml` — Strict CODEOWNERS Enforcement

Enforces that **at least one member from every required owner group** approves a PR — stricter than GitHub's native CODEOWNERS which only requires one owner from any single matching rule.

**Why this exists:** GitHub's built-in CODEOWNERS requires only one approval from *one* of the listed groups. This workflow ensures *each* distinct group (e.g. `@GPO/coreradars-devops` AND `@GPO/core-radar-autosar-owners`) has signed off before merging.

**Triggers:**

| Event | When |
|-------|------|
| `pull_request: opened / synchronize / reopened` | PR created or new commits pushed |
| `pull_request_review: submitted / dismissed` | Review submitted or dismissed |

**How team membership is resolved (no secrets or config files needed):**

| Attempt | API call | Requires |
|---------|----------|---------|
| 1 | `listMembersInOrg` — full member list | `read:org` scope (may fail for `GITHUB_TOKEN`) |
| 2 | `getMembershipForUserInOrg` per approver — checks only actual approvers | Works on many GHE orgs even without `read:org` |
| Fallback | Team marked **unresolvable** — enforcement skipped with warning | Native branch-protection CODEOWNERS is the safety net |

**Stale approval handling:**

Only reviews whose `commit_id` matches the current PR `head.sha` are counted. Approvals from previous commits are automatically treated as stale and ignored — equivalent to enabling *"Dismiss stale pull request approvals when new commits are pushed"* in branch protection, but enforced in-workflow.

**Result states:**

| State | Meaning |
|-------|---------|
| ✅ PASSED | Every required group has at least one approver on the current HEAD |
| ⚠️ PASSED WITH WARNINGS | No missing approvals, but one or more teams could not be verified via API |
| ❌ FAILED | At least one group has no verified approver on the current HEAD |

**Label management:** Automatically adds `codeowners-approval-missing` label to failing PRs and removes it once all groups are satisfied.

**Required permissions (no extra secrets):**

```yaml
permissions:
  contents: read       # read CODEOWNERS file
  issues: write        # add/remove label
  pull-requests: write # read PR reviews
```

**Job Summary output includes:**
- Current approvers (on HEAD commit only)
- Which groups still need approval and exactly who can approve
- Per-file ownership table with approval status
- Clear instructions when a team cannot be resolved (team doesn't exist or is secret)

**Branch protection setup (recommended):**

Enable these settings on your protected branch so this workflow is the enforcement gate:

| Setting | Value |
|---------|-------|
| Require status checks to pass | ✅ — add `strict-codeowners-check` |
| Require review from Code Owners | ✅ |
| Dismiss stale pull request approvals | ✅ (belt-and-suspenders with in-workflow stale logic) |
| Require approval of most recent push | ✅ |

**Fixing "team unresolvable" permanently:**

If `listMembersInOrg` and `getMembershipForUserInOrg` both fail (team is secret, or org policy blocks GITHUB_TOKEN), store a fine-grained PAT or GitHub App token with `read:org` as a repo secret (e.g. `ORG_READ_TOKEN`) and replace `github-token: ${{ secrets.GITHUB_TOKEN }}` with `github-token: ${{ secrets.ORG_READ_TOKEN }}` in the "Verify" step.

---

### Step 1: Create a Shared Workflows Repository

1. Create a new repository: `https://github.com/your-org/shared-workflows`
2. Copy `build.yml`, `unit-tests.yml`, `coverity.yml`, and `smoke-test.yml` to `.github/workflows/`

### Step 2: Configure Your Project Repository

In your project's `.github/workflows/your-ci.yml`:

```yaml
name: CI

on:
  pull_request:
    branches:
      - main

jobs:
  build:
    uses: your-org/shared-workflows/.github/workflows/build.yml@main
    with:
      target: //:your_target
      config: your_config
    secrets:
      GHCR_USER: ${{ secrets.GHCR_USER }}
      GHCR_TOKEN: ${{ secrets.GHCR_TOKEN }}
      NETRC_CONTENT: ${{ secrets.NETRC_CONTENT }}
      ARB_TOKEN: ${{ secrets.ARB_TOKEN }}

  unit_test:
    uses: your-org/shared-workflows/.github/workflows/unit-tests.yml@main
    with:
      test-target: //coverage:report
      config: your_config
      nightly: false
    secrets:
      GHCR_USER: ${{ secrets.GHCR_USER }}
      GHCR_TOKEN: ${{ secrets.GHCR_TOKEN }}
      NETRC_CONTENT: ${{ secrets.NETRC_CONTENT }}
      ARB_TOKEN: ${{ secrets.ARB_TOKEN }}

  coverity:
    uses: your-org/shared-workflows/.github/workflows/coverity.yml@main
    with:
      matrix-json: |
        [
          {
            "variant": "config_a",
            "core": "core_1",
            "coverity_target": "//src:coverity_a",
            "coverity_stream": "YOUR_STREAM_ID"
          }
        ]
    secrets:
      GHCR_USER: ${{ secrets.GHCR_USER }}
      GHCR_TOKEN: ${{ secrets.GHCR_TOKEN }}
      NETRC_CONTENT: ${{ secrets.NETRC_CONTENT }}
      ARB_TOKEN: ${{ secrets.ARB_TOKEN }}
      COVERITY_COMMITTER_AUTH: ${{ secrets.COVERITY_AUTH }}

  smoke_test:
    needs: build
    if: ${{ always() }}
    uses: your-org/shared-workflows/.github/workflows/smoke-test.yml@main
    with:
      matrix-json: |
        [
          {
            "variant": "config_a",
            "artifact_name": "build-config_a.zip",
            "quickflash_variant": "config_a_satellite_can",
            "smoke_test_report_path": "/path/to/reports/config_a.html"
          },
          {
            "variant": "config_b",
            "artifact_name": "build-config_b.zip",
            "quickflash_variant": "config_b_satellite_can",
            "smoke_test_report_path": "/path/to/reports/config_b.html"
          }
        ]
      runner-labels: '["self-hosted","linux","SmokeTest"]'
      commit-sha: ${{ github.sha }}
```

### Step 3: Set Required Secrets

In your repository settings (`Settings -> Secrets and variables -> Actions`), add:

- `GHCR_USER` - GitHub Container Registry username
- `GHCR_TOKEN` - GitHub Container Registry token
- `NETRC_CONTENT` - Raw `.netrc` payload content
- `ARB_TOKEN` - Artifactory Remote Build API token
- `COVERITY_AUTH` - Coverity authentication key (for Coverity workflow only)

Optional for nightly metrics DB publish:

- `METRICS_DB_USER`
- `METRICS_DB_PASSWORD`
- `METRICS_DB_HOST`
- `METRICS_DB_PORT`
- `METRICS_DB_NAME`

---

## Key Features

✅ **Modular & Reusable** - Use across multiple projects
✅ **Parameterized** - Customize for different build configurations
✅ **Container-based** - Consistent execution environment
✅ **Artifact Handling** - Automatic artifact upload and retention
✅ **Error Handling** - Built-in error checking and reporting
✅ **Logging** - Coverity results and build logs automatically captured
✅ **GitHub Summary Reports** - Results posted to PR workflow summaries

---

## GitHub Summary Reports

Each workflow automatically generates a summary that appears in the GitHub PR workflow summary. This includes:

### Build Summary
- ✅/❌ Build status
- Target and config information
- Overall outcome

### Unit Test Summary
- ✅/❌ Test status
- Test target and config
- Total tests, untested source files, and true-count coverage (when available)
- Coverage summary (if available)

### Coverity Summary
- ✅/❌ Defect detection status
- Variant and core information
- Coverity target and stream
- Build log excerpt (on failures)

### Smoke Test Summary
- ✅/❌ QuickFlash and test results
- Variant information
- Passing/failing test counters parsed from smoke log
- Test report location

---

## Migration Notes

### From Previous Setup

If you're migrating from an older workflow setup:

1. **Review your build/test targets** - Update the `target` and `test-target` inputs
2. **Update Bazel configs** - Ensure `config` values match your setup
3. **Matrix customization** - For Coverity and smoke, update `matrix-json` entries
4. **Artifact paths** - Verify build uses semicolon-separated `upload-artifact-path`, and unit tests use newline-separated `upload-artifact-paths`

### Troubleshooting

- **Missing secrets**: Ensure all required secrets are configured in repository settings
- **Build failures**: Check container image tag is accessible and valid
- **Artifact not found**: Verify the artifact path matches your Bazel build output directory
- **Coverity errors**: Confirm Coverity auth key is valid and stream IDs are correct

---

## Contributing

To update these workflows:

1. Test changes in your project repository
2. Create a PR in the shared-workflows repository
3. Update the version reference (`@main` → `@v1.0.0` etc.) when merging
4. Notify teams to update their workflow references

---

## Example: Core_Radar_Gen8_iND13400 Project

The original `pr-checks.yml` in this project demonstrates the full integration:

```yaml
jobs:
  build:
    uses: ./.github/workflows/build.yml
    with:
      target: //:gen8
      config: ${{ matrix.variant }}
    secrets: # ...

  unit_test:
    uses: ./.github/workflows/unit-tests.yml
    with:
      test-target: //coverage:report
      config: ${{ matrix.variant }}
    secrets: # ...

  coverity:
    uses: ./.github/workflows/coverity.yml
    with:
      matrix-json: # [flr8-r52, flr8-bbe32, srr8p-r52, srr8p-bbe32]
    secrets: # ...

  hw_test:
    needs: build
    if: ${{ always() }}
    uses: ./.github/workflows/smoke-test.yml
    with:
      matrix-json: # [{variant, artifact_name, quickflash_variant, smoke_test_report_path}, ...]
      runner-labels: '["self-hosted","linux","SmokeTest"]'
      commit-sha: ${{ github.sha }}
```

This same pattern can be applied to any Bazel-based project with hardware testing capabilities.
