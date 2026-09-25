# Dependencies — Gen7 SAF85xx GitHub Actions

Everything you need to wire up before the workflows under
[.github/workflows/](workflows/) and the composite actions under
[.github/actions/](actions/) will run successfully.

> Scope: covers every YAML in this directory tree — reusable engines,
> orchestrators, on-demand wrappers, SWE6 family runners, and the WRSD bridge.

---

## 1. Repository secrets

Set at **Settings → Secrets and variables → Actions → New repository secret**
(or at the org level if shared across Gen7 repos).

> **Naming convention** — every project-owned secret is prefixed `ADVRADAR_`
> so they sort together in the GitHub org-level secret list (alongside
> `ADVRADAR_SID_USERNAME`, `ADVRADAR_JFROG_API_KEY`, etc.). The only
> exception is `GITHUB_TOKEN`, which is auto-issued by the Actions runtime
> and cannot be renamed. When adding a new secret, keep the prefix.

### 1.1 Core build / test secrets (required for almost every workflow)

| Secret | What it holds | Consumed by |
|---|---|---|
| `ADVRADAR_GHCR_USER` | Username for `containers.aptv.ghe.com` registry pull | build.yml, build-flavors.yml, unit-tests.yml, coverity.yml, pr-checks.yml (precommit container), post-merge.yml (grafana), all ondemand-build-*.yml |
| `ADVRADAR_GHCR_TOKEN` | PAT / password for the GHCR user | same as above |
| `ADVRADAR_NETRC_CONTENT` | Full `~/.netrc` body. **Must** contain `machine` entries for: `gitgerrit.asux.aptiv.com`, `jfrog.asux.aptiv.com`, Coverity Connect host | build.yml, unit-tests.yml, coverity.yml, gerrit-sync.yml, post-merge.yml, radar-test-reusable.yml family, weekly.yml |
| `ADVRADAR_ARB_TOKEN` | Aptiv RapidBuild (BES) remote cache bearer token | build.yml, unit-tests.yml, coverity.yml (via `configure-bazel-cache` action) |

### 1.2 Coverity-specific

| Secret | What it holds | Consumed by |
|---|---|---|
| `ADVRADAR_COVERITY_COMMITTER_AUTH` | JSON auth-key downloaded from Coverity Connect (single user, commit-permission) | coverity.yml, nightly.yml, post-merge.yml |

### 1.3 WRSD / JFrog handoff

| Secret | What it holds | Consumed by |
|---|---|---|
| `ADVRADAR_SID_USERNAME` | JFrog upload account username | wrsd-integration.yml, radar-test-reusable.yml (+ swe6-*/ondemand-swe6-* callers), nightly.yml, weekly.yml |
| `ADVRADAR_JFROG_API_KEY` | JFrog API key for that account | wrsd-integration.yml, radar-test-reusable.yml (+ swe6-*/ondemand-swe6-* callers) |
| `ADVRADAR_WRSD_TRIGGER_TOKEN` | Bearer token for the WRSD pipeline trigger REST endpoint | wrsd-integration.yml |

### 1.4 Gerrit sync (only if you mirror to Gerrit)

| Secret | What it holds | Consumed by |
|---|---|---|
| `ADVRADAR_GERRIT_USERNAME` | Gerrit HTTP user (typically a service account) | gerrit-sync.yml, post-merge.yml (auto-rebase) |
| `ADVRADAR_GERRIT_HTTP_PASSWORD` | Gerrit HTTP password from Gerrit user settings | gerrit-sync.yml, post-merge.yml |

### 1.5 SWE6 / Power Automate notifications

| Secret | What it holds | Consumed by |
|---|---|---|
| `ADVRADAR_SWE6_TEAMS_WEBHOOK_URL` | HTTP trigger URL for the single Power Automate flow that posts to Teams **and** sends the Outlook email (one webhook handles both). Kept SWE6-prefixed for backwards compatibility — reused for SWE6_Alignment and any future job-type. | radar-test-reusable.yml + every `swe6-*`, `swe5-*`, `ondemand-swe6-*`, `swe-test-*` caller |
| `ADVRADAR_PLATFORM_HEALTH_TOKEN` | Token for posting results into Platform-Health / Grafana | radar-test-reusable.yml |

### 1.6 Auto-issued (no action required)

| Secret | Where it comes from | Used by |
|---|---|---|
| `GITHUB_TOKEN` | GitHub Actions runtime | pr-checks.yml, pr-status-comment action, build.yml (label management), gerrit-sync.yml |

---

## 2. Repository variables (non-secret)

Set at **Settings → Secrets and variables → Actions → Variables**. None are
strictly required, but these let you adjust runtime without code changes.

| Variable | Default if unset | Purpose |
|---|---|---|
| `CONTAINER_IMAGE` | `containers.aptv.ghe.com/devsecops/core-radar/build-format-ready:1861593` | Build/test container — bump to new SHA when the image is republished |
| `JFROG_BASE_URL` | `https://jfrog.asux.aptiv.com` | JFrog instance for nightly/weekly uploads |
| `JFROG_REPO` | `core_radar-aptiv-00000000-gen7_saf85xx-local` | JFrog repo for build artifacts |
| `GERRIT_URL` | `https://gitgerrit.asux.aptiv.com` | Gerrit base URL |
| `GERRIT_PROJECT` | `Core_Radar_Gen7_SAF85xx` | Gerrit project name |

---

## 3. Self-hosted runners

| Runner label set | Used by | Notes |
|---|---|---|
| `gh-injected-gpo-linux-medium` (or your linux-medium pool) | All build/test workflows | Standard hosted Linux. Adjust label in each `runs-on:` if your fleet uses a different name. |
| `self-hosted, linux, SmokeTest, Gen7` | smoke-test.yml (called by pr-checks/weekly) | Linux bench runner with QuickFlash, JTAG, and bench HW attached |
| `self-hosted, Windows, GEN7_V2_FRAMEWORK` | radar-test-reusable.yml + every `swe6-*`, `swe5-*`, `ondemand-swe6-*`, `swe-test-*` caller | **Windows** runner with the Gen7 Python Framework checked out at `tools\python\Core_Radar_Gen7_SAF85xx_Python_Framework` and the STEP1/3/4 batch files reachable |

Register runners via **Settings → Actions → Runners** with the labels above.

---

## 4. External services that must be reachable from runners

| Service | Used for | Notes |
|---|---|---|
| `containers.aptv.ghe.com` | Pull build container | ADVRADAR_GHCR_USER/ADVRADAR_GHCR_TOKEN credentials |
| `gitgerrit.asux.aptiv.com` | Source checkout, mirroring, auto-rebase | Inside Aptiv network only |
| `jfrog.asux.aptiv.com` | Artifact upload (nightly/weekly/swe6) and download (swe6 pulls `output_<family>.zip`) | `.netrc` credentials |
| Coverity Connect host (in `.netrc`) | `commit_defects` upload | Streams must be provisioned — see §6 |
| Aptiv RapidBuild BES | Remote Bazel cache | `ADVRADAR_ARB_TOKEN` |
| Power Automate flow endpoints | Teams + email notifications | URLs stored as secrets |
| Platform-Health / Grafana dashboard repo `00000000_wrsd/metrics` | post-merge metrics publish, swe6 result push | Repo write access for the bot user |
| WRSD pipeline trigger endpoint | wrsd-integration.yml handoff | Bearer token `ADVRADAR_WRSD_TRIGGER_TOKEN` |

---

## 5. Coverity streams to provision

Create these streams in Coverity Connect before enabling the `coverity` job
(currently guarded by `if: false` in [pr-checks.yml](workflows/pr-checks.yml#L138)):

```
Core_Radar_Gen7_SAF85xx_FLR7_M7
Core_Radar_Gen7_SAF85xx_FLR7_A53
Core_Radar_Gen7_SAF85xx_FLR7_BBE32
Core_Radar_Gen7_SAF85xx_SRR7P_M7
Core_Radar_Gen7_SAF85xx_SRR7P_A53
Core_Radar_Gen7_SAF85xx_SRR7P_BBE32
```

Once provisioned, remove the `if: false` from the `coverity` job in
[pr-checks.yml](workflows/pr-checks.yml).

---

## 6. Per-workflow dependency matrix

Quick reference — pick a workflow, see exactly what it needs.

### Reusable engines (called by orchestrators, never run directly)

| Workflow | Secrets | Vars | Runner | Other |
|---|---|---|---|---|
| [build.yml](workflows/build.yml) | ADVRADAR_GHCR_USER, ADVRADAR_GHCR_TOKEN, ADVRADAR_NETRC_CONTENT, ADVRADAR_ARB_TOKEN | CONTAINER_IMAGE | linux-medium | Bazel cache reachable |
| [build-flavors.yml](workflows/build-flavors.yml) | inherits from build.yml | — | linux-medium | — |
| [unit-tests.yml](workflows/unit-tests.yml) | ADVRADAR_GHCR_USER, ADVRADAR_GHCR_TOKEN, ADVRADAR_NETRC_CONTENT, ADVRADAR_ARB_TOKEN | CONTAINER_IMAGE | linux-medium | — |
| [coverity.yml](workflows/coverity.yml) | ADVRADAR_GHCR_USER, ADVRADAR_GHCR_TOKEN, ADVRADAR_NETRC_CONTENT, ADVRADAR_ARB_TOKEN, ADVRADAR_COVERITY_COMMITTER_AUTH | CONTAINER_IMAGE | linux-medium | Streams from §5 |
| [smoke-test.yml](workflows/smoke-test.yml) | (none — runs on bench) | — | self-hosted SmokeTest Gen7 | Bench HW + QuickFlash |
| [wrsd-integration.yml](workflows/wrsd-integration.yml) | ADVRADAR_SID_USERNAME, ADVRADAR_JFROG_API_KEY, ADVRADAR_WRSD_TRIGGER_TOKEN | JFROG_BASE_URL, JFROG_REPO | linux-medium | JFrog + WRSD reachable |
| [radar-test-reusable.yml](workflows/radar-test-reusable.yml) | ADVRADAR_NETRC_CONTENT, ADVRADAR_SID_USERNAME, ADVRADAR_JFROG_API_KEY, ADVRADAR_SWE6_TEAMS_WEBHOOK_URL, ADVRADAR_PLATFORM_HEALTH_TOKEN | JFROG_BASE_URL, JFROG_REPO | self-hosted GEN7_V2_FRAMEWORK | Python Framework checkout |

### Orchestrators (scheduled / event-driven — auto-run)

| Workflow | Trigger | Calls | Extra secrets vs. callees |
|---|---|---|---|
| [pr-checks.yml](workflows/pr-checks.yml) | `pull_request: dev`, manual | build-flavors, unit-tests, coverity (gated), smoke-test | GITHUB_TOKEN (auto) |
| [nightly.yml](workflows/nightly.yml) | `schedule: 02:00 UTC`, manual | build-flavors (all 8), unit-tests, coverity, wrsd-integration | — |
| [weekly.yml](workflows/weekly.yml) | `schedule: Sun 22:00 UTC`, manual | build-flavors (all 8), smoke-test (4h), wrsd-integration | — |
| [post-merge.yml](workflows/post-merge.yml) | `push: dev`, manual | coverity | ADVRADAR_GERRIT_USERNAME, ADVRADAR_GERRIT_HTTP_PASSWORD |
| [gerrit-sync.yml](workflows/gerrit-sync.yml) | `push`, `schedule: */15`, `repository_dispatch: gerrit-replication`, manual | — | ADVRADAR_GERRIT_USERNAME, ADVRADAR_GERRIT_HTTP_PASSWORD, ADVRADAR_NETRC_CONTENT |
| [swe6-flr7.yml](workflows/swe6-flr7.yml) | `schedule: 30 18 * * *`, manual | radar-test-reusable.yml | inherits radar-test-reusable set |
| [swe6-srr7p.yml](workflows/swe6-srr7p.yml) | `schedule: 30 19 * * *`, manual | radar-test-reusable.yml | inherits radar-test-reusable set |
| [swe6-alignment.yml](workflows/swe6-alignment.yml) | `schedule: 30 20 * * 0`, manual | radar-test-reusable.yml (×2 — FLR7 + SRR7p) | inherits radar-test-reusable set |
| [ondemand-swe6-flr7.yml](workflows/ondemand-swe6-flr7.yml) | manual | radar-test-reusable.yml | inherits radar-test-reusable set |
| [ondemand-swe6-srr7p.yml](workflows/ondemand-swe6-srr7p.yml) | manual | radar-test-reusable.yml | inherits radar-test-reusable set |
| [swe5-flr7.yml](workflows/swe5-flr7.yml) | manual (cron commented out) | radar-test-reusable.yml | inherits radar-test-reusable set |
| [swe5-srr7p.yml](workflows/swe5-srr7p.yml) | manual (cron commented out) | radar-test-reusable.yml | inherits radar-test-reusable set |
| [swe6-sqt-macro-flr7.yml](workflows/swe6-sqt-macro-flr7.yml) | manual | radar-test-reusable.yml | inherits radar-test-reusable set |
| [swe5-sit-macro-flr7.yml](workflows/swe5-sit-macro-flr7.yml) | manual | radar-test-reusable.yml | inherits radar-test-reusable set |
| [swe-test-runner.yml](workflows/swe-test-runner.yml) | manual (picker UI) | radar-test-reusable.yml | inherits radar-test-reusable set; `prepare` job runs on `ubuntu-latest` |
| [swe-test-regression-flr7.yml](workflows/swe-test-regression-flr7.yml) | manual | radar-test-reusable.yml | inherits radar-test-reusable set |
| [swe-test-stress-flr7.yml](workflows/swe-test-stress-flr7.yml) | manual | radar-test-reusable.yml | inherits radar-test-reusable set |
| [swe-test-wi-smoke-srr7p.yml](workflows/swe-test-wi-smoke-srr7p.yml) | manual | radar-test-reusable.yml | inherits radar-test-reusable set |
| [swe-test-pyfilter-flr7.yml](workflows/swe-test-pyfilter-flr7.yml) | manual (with `python_files`/`python_functions` inputs) | radar-test-reusable.yml | inherits radar-test-reusable set |

### On-demand wrappers (manual only — never auto-run)

All require the same secrets as [build-flavors.yml](workflows/build-flavors.yml)
(ADVRADAR_GHCR_USER, ADVRADAR_GHCR_TOKEN, ADVRADAR_NETRC_CONTENT, ADVRADAR_ARB_TOKEN) and run on `linux-medium`.

| Workflow | Bazel flavor | Trigger |
|---|---|---|
| [ondemand-build-satellite.yml](workflows/ondemand-build-satellite.yml) | `//:gen7 --jenkins` | `workflow_dispatch` |
| [ondemand-build-sil.yml](workflows/ondemand-build-sil.yml) | RDD SIL target | `workflow_dispatch` |
| [ondemand-build-iso.yml](workflows/ondemand-build-iso.yml) | `--rot=standalone --enable_cdc=False` | `workflow_dispatch` |
| [ondemand-build-vlan.yml](workflows/ondemand-build-vlan.yml) | `--enable_vlan=True` | `workflow_dispatch` |
| [ondemand-build-sqt.yml](workflows/ondemand-build-sqt.yml) | `--enable_sqt_testcases` | `workflow_dispatch` |
| [ondemand-build-sit.yml](workflows/ondemand-build-sit.yml) | `--Integration_Testing=true --Anglefinding_IT=true` | `workflow_dispatch` |
| [ondemand-build-sit-core0.yml](workflows/ondemand-build-sit-core0.yml) | `--enable_sit_testcases` | `workflow_dispatch` |
| [ondemand-build-alignment.yml](workflows/ondemand-build-alignment.yml) | `--enable_bench_testing=true` | `workflow_dispatch` |
| [ondemand-build-custom.yml](workflows/ondemand-build-custom.yml) | Any subset of the 8 (checkboxes) | `workflow_dispatch` |

### Composite actions (consumed by the workflows above)

| Action | Inputs | External deps |
|---|---|---|
| [configure-bazel-cache](actions/configure-bazel-cache/) | `arb-token` | ARB BES reachable |
| [configure-netrc](actions/configure-netrc/) | `netrc-content` | — |
| [setup-bazelisk](actions/setup-bazelisk/) | — | `./bazelisk` in repo root |
| [setup-coverity-auth](actions/setup-coverity-auth/) | `auth-key` | Coverity Connect reachable |
| [pr-status-comment](actions/pr-status-comment/) | `github-token`, job-* | — |

---

## 7. Setup checklist (one-time)

1. **Add the secrets** listed in §1.1–§1.5 at repo or org level.
2. **Add the variables** in §2 if you want to override defaults; otherwise the
   inline defaults in the workflows are used.
3. **Register self-hosted runners** with the labels in §3.
4. **Provision Coverity streams** from §5, then remove the `if: false` guard
   on the `coverity` job in [pr-checks.yml](workflows/pr-checks.yml).
5. **Configure Gerrit `replication.config`** to fire a `repository_dispatch`
   event of type `gerrit-replication` against this repo so
   [gerrit-sync.yml](workflows/gerrit-sync.yml) doesn't wait for the 15-min poll.
6. **Set up Power Automate flows** for Teams + email notifications and paste
   their HTTP trigger URLs into the SWE6 secrets in §1.5.
7. **Verify network reachability** from each runner to every service in §4.
8. **Smoke test**: kick [ondemand-build-satellite.yml](workflows/ondemand-build-satellite.yml)
   manually against your branch with `variants=["flr7"]`. If it builds and
   uploads, your secrets / cache / container are wired correctly.

---

## 8. Troubleshooting quick reference

| Symptom | Likely missing dependency |
|---|---|
| `unauthorized` pulling container image | `ADVRADAR_GHCR_USER` / `ADVRADAR_GHCR_TOKEN` not set or expired |
| Bazel `403` on remote cache | `ADVRADAR_ARB_TOKEN` missing or rotated |
| `.netrc` errors / 401 from JFrog or Gerrit | `ADVRADAR_NETRC_CONTENT` missing the right `machine` entry |
| `Coverity stream not found` | Stream from §5 not provisioned |
| `WRSD trigger 401/403` | `ADVRADAR_WRSD_TRIGGER_TOKEN` missing/expired |
| Smoke test "no runner matched labels" | Bench runner not registered with `self-hosted, linux, SmokeTest, Gen7` |
| SWE6 "STEP1.bat not found" | Windows runner missing `GEN7_V2_FRAMEWORK` label or Python Framework checkout |
| Auto-rebase no-op in post-merge | `ADVRADAR_GERRIT_USERNAME` / `ADVRADAR_GERRIT_HTTP_PASSWORD` not set |
| Teams/email notifications silent | `ADVRADAR_SWE6_TEAMS_WEBHOOK_URL` not set, or the Power Automate flow behind it is disabled |
