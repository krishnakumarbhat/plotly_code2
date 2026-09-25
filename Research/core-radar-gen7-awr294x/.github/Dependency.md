# AWR294X CI/CD — Dependencies, Setup & Omissions

This document tracks everything the `.github/` GitHub Actions migration depends on
that is **not** in this repository. Provision these before enabling the workflows.

---

## 1. Required GitHub Secrets

Configure under **Repo Settings → Secrets and variables → Actions → Repository secrets**.

| Secret                              | Used by                                            | Purpose                                                                                          |
|-------------------------------------|----------------------------------------------------|--------------------------------------------------------------------------------------------------|
| `ADVRADAR_GHCR_USER`                | every container-based job                          | Username for pulling the build/coverity/gut container images. For this repo the images live on JFrog (`jfrog.asux.aptiv.com`), so set this to a JFrog SID/service account allowed to pull from `core_radar-aptiv-00000000-docker-local`. |
| `ADVRADAR_GHCR_TOKEN`               | every container-based job                          | Password / identity-token paired with `ADVRADAR_GHCR_USER`.                                      |
| `ADVRADAR_NETRC_CONTENT`            | every job that touches Bazel / Gerrit              | Full `~/.netrc` body (multi-line). Must include entries for `jfrog.asux.aptiv.com` and `gitgerrit.asux.aptiv.com`. |
| `ADVRADAR_ARB_TOKEN`                | every Bazel job                                    | Aptiv RapidBuild API key. Injected as `x-arb-api-key` for BES upload and the remote cache.       |
| `ADVRADAR_COVERITY_COMMITTER_AUTH`  | [coverity.yml](workflows/coverity.yml)             | Base64-encoded Coverity `auth.key` file used by `cov-commit-defects`.                            |
| `ADVRADAR_SID_USERNAME`             | [wrsd-integration.yml](workflows/wrsd-integration.yml) | JFrog username for artifact upload via JFrog CLI.                                            |
| `ADVRADAR_JFROG_API_KEY`            | [wrsd-integration.yml](workflows/wrsd-integration.yml) | JFrog API key / identity token for artifact upload.                                          |
| `ADVRADAR_WRSD_TRIGGER_TOKEN`       | [wrsd-integration.yml](workflows/wrsd-integration.yml) | **Optional.** Bearer token for the WRSD pipeline trigger endpoint. If unset, the workflow only uploads to JFrog. |
| `GITHUB_TOKEN` (built-in)           | PR comment, labels, branch update                  | Provided automatically. Workflows request the minimum permissions they need.                     |

> **Why the names start with `ADVRADAR_`** — follows the master prompt naming
> convention and avoids collisions with other Aptiv repos that may share a
> GitHub Actions org but need distinct credentials.

---

## 2. Required Self-Hosted Runners

| Label set                                                  | Used by                                                       | OS      | Notes                                                                                                     |
|------------------------------------------------------------|---------------------------------------------------------------|---------|-----------------------------------------------------------------------------------------------------------|
| `gh-injected-gpo-linux-medium`                             | build / unit-tests / coverity / dynamic-analysis / PR jobs    | Linux   | Generic Linux runner that can pull the JFrog-hosted container images. Default for every reusable workflow. |
| `[self-hosted, windows, SmokeTest, Gen7, AWR294x]`         | [smoke-test.yml](workflows/smoke-test.yml)                    | Windows | Replaces the Jenkins `GEN7_CICD` bench (Ethernet flash + power-cycle test). Currently gated `if: false` by callers. |
| `[self-hosted, windows, GEN7_ITF]`                         | [ondemand-swe5.yml](workflows/ondemand-swe5.yml), [ondemand-swe6.yml](workflows/ondemand-swe6.yml) | Windows | Replaces the Jenkins `GEN7_ITF` Windows-bench-with-instruments. Currently gated `if: false`. |

Once a Windows bench joins with the matching labels, flip `if: false` to a real
gate (e.g. `if: github.event.pull_request.draft == false`).

---

## 3. External Services

| Service                                  | Used for                                                           | Auth via                                              |
|------------------------------------------|--------------------------------------------------------------------|-------------------------------------------------------|
| **JFrog Artifactory** (`jfrog.asux.aptiv.com`) | Bazel remote cache reads, dependency downloads, container images, release artifact upload | `ADVRADAR_NETRC_CONTENT` + `ADVRADAR_GHCR_USER/TOKEN` + `ADVRADAR_SID_USERNAME` / `ADVRADAR_JFROG_API_KEY` |
| **Aptiv RapidBuild (ARB)**               | Build event streaming and remote cache headers                     | `ADVRADAR_ARB_TOKEN`                                  |
| **Coverity Connect**                     | Static analysis defect commit (8 streams, see README)              | `ADVRADAR_COVERITY_COMMITTER_AUTH`                    |
| **Gerrit** (`gitgerrit.asux.aptiv.com/ADVRADAR_AWR294X`) | Upstream source-of-truth; bi-directional mirror               | `ADVRADAR_NETRC_CONTENT`                              |
| **WRSD pipeline** (optional)             | Triggering downstream release flow                                 | `ADVRADAR_WRSD_TRIGGER_TOKEN`                         |

### JFrog repositories referenced

- `gen7-aptiv-advradar-releases-generic-local` — default upload target for nightly/weekly/ad-hoc artifacts
- `gen7-aptiv-advradar-swe.4_swe.5-generic-local` — release upload target when `pipeline=release`
- `core_radar-aptiv-00000000-docker-local` — container images
  (`build-format-ready:1.6`, `gut-ready:1.5`, `coverity-build-ready:1.7`)

### Coverity streams

| Stream                       | Variant | Core | Notes                                                       |
|------------------------------|---------|------|-------------------------------------------------------------|
| `ADVRADAR_SRR7p_Mss`         | srr7p   | mss  | PR + nightly + post-merge                                   |
| `ADVRADAR_SRR7p_Dss`         | srr7p   | dss  | PR + nightly + post-merge                                   |
| `ADVRADAR_FLR7_Mss`          | flr7    | mss  | PR + nightly + post-merge                                   |
| `ADVRADAR_FLR7_Dss`          | flr7    | dss  | PR + nightly + post-merge                                   |
| `ADVRADAR_SRR7p_ECO_Mss`     | srr7p   | mss  | + `--awr_target_variant=awr2944eco`. PR + nightly only.     |
| `ADVRADAR_SRR7p_ECO_Dss`     | srr7p   | dss  | + `--awr_target_variant=awr2944eco`. PR + nightly only.     |
| `ADVRADAR_FLR7_ECO_Mss`      | flr7    | mss  | + `--awr_target_variant=awr2944eco`. PR + nightly only.     |
| `ADVRADAR_FLR7_ECO_Dss`      | flr7    | dss  | + `--awr_target_variant=awr2944eco`. PR + nightly only.     |

---

## 4. Omissions (intentionally NOT migrated)

The migration is faithful to what's in this workspace. The following Jenkins
features were intentionally left out and need a separate decision before they
are added:

1. **Jenkins shared libraries** — `ADVRADAR_CICD` and `YGGDRASIL_Libs` are not
   vendored in this repository. Their public surface (the calls visible from
   `tools/Jenkins/**/Jenkinsfile`) has been reimplemented step-by-step in the
   reusable workflows, but private helpers (advanced Gerrit voting helpers,
   internal report aggregators, BlackDuck/TICS/SquishCoco integrations) are
   **not** ported. Treat them as a black box; revisit once they are open-sourced
   or vendored.
2. **TICS** — there is no `ticsJob` Jenkinsfile in this repo, so no TICS
   workflow was added.
3. **SquishCoco MCDC** — referenced but commented-out in `verifiedJob/Jenkinsfile`.
4. **MATLAB Integration Test Framework** (Matlab_Automated_Testing) — requires
   a MATLAB R2023a license and the bench instruments; not migrated.
5. **Platform Health / Datadog publishers** — Jenkins SWE5/SWE6 jobs publish to
   these dashboards. The workflows generate report artifacts but do not push
   them anywhere.
6. **BlackDuck** — no BlackDuck Jenkinsfile in this repo.
7. **HW bench execution** — workflows are written and self-consistent, but every
   caller currently has `if: false` (smoke / SWE5 / SWE6) because no Windows
   bench has been registered with GitHub Actions yet. See §2 above.
8. **`tools/python/unitTestTrueCount`** — does not exist in this repo (it exists
   in some sibling repos). The True-Count post-processor was omitted from
   [unit-tests.yml](workflows/unit-tests.yml).

---

## 5. Setup checklist

Before the first PR opens against the GitHub mirror:

- [ ] Create all secrets in §1 at the repo (or org) level.
- [ ] Register a Linux runner with the `gh-injected-gpo-linux-medium` label.
- [ ] (Optional) register the Windows bench runners with the labels in §2 and
      remove the `if: false` from `smoke-test.yml` callers / `ondemand-swe5.yml` /
      `ondemand-swe6.yml`.
- [ ] Confirm the JFrog repos in §3 exist and the service account has read
      (cache) + write (release) permissions.
- [ ] Confirm Gerrit replication into the GitHub mirror has been set up
      (push-events + repository_dispatch from Gerrit hooks). If not, the cron
      in `gerrit-sync.yml` will still catch up every 15 minutes.
- [ ] Run a manual `On-Demand · Build Production` once to validate the toolchain
      before letting `pr-checks.yml` fire on a real PR.
