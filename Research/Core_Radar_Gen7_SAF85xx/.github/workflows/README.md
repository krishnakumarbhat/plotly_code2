# GitHub Actions Workflow Suite — Core_Radar_Gen7_SAF85xx

This `.github/` tree mirrors the production-ready Gen8 (`Core_Radar_Gen8_iND13400`)
layout, adapted for Gen7 SAF85xx (variants `srr7e`, `srr7p`, `flr7`, `flr7v3`;
cores `m7`, `a53`, `bbe32`; Bazel target `//:gen7`).

## Folder structure

```
.github/
├── actions/                          # Composite actions (reusable steps)
│   ├── configure-bazel-cache/        # Writes ARB remote-cache headers
│   ├── configure-netrc/              # Writes ~/.netrc for Gerrit/JFrog/Coverity
│   ├── pr-status-comment/            # Upserts the sticky PR status comment
│   ├── setup-bazelisk/               # Validates ./bazelisk and exports its path
│   └── setup-coverity-auth/          # Materialises COVERITY_AUTH_FILE
└── workflows/
    ├── build.yml                     # Reusable Bazel build engine (workflow_call)
    ├── build-flavors.yml             # Reusable flavor catalog (satellite/sil/iso/vlan/sqt/sit/sit-core0/alignment)
    ├── unit-tests.yml                # Reusable unit-tests/coverage (workflow_call)
    ├── coverity.yml                  # Reusable Coverity analysis (workflow_call)
    ├── smoke-test.yml                # Reusable HW smoke (workflow_call, self-hosted)
    ├── pr-checks.yml                 # Caller: PR validation orchestrator (satellite only)
    ├── nightly.yml                   # Caller: scheduled full-fan-out CI (all 8 flavors)
    ├── weekly.yml                    # Caller: weekly long-soak HW tests (4h per bench)
    ├── post-merge.yml                # Caller: push-to-dev coverity commit + grafana
    ├── gerrit-sync.yml               # GitHub <-> Gerrit two-way mirror
    ├── wrsd-integration.yml          # JFrog upload + WRSD pipeline trigger
    ├── radar-test-reusable.yml       # Reusable radar test engine (workflow_call) — generic across job-type + family
    ├── swe6-flr7.yml                 # Caller: daily SWE6 FLR7 (18:30 UTC)
    ├── swe6-srr7p.yml                # Caller: daily SWE6 SRR7p (19:30 UTC)
    ├── swe6-alignment.yml            # Weekly SWE6 alignment (Sun 20:30 UTC, FLR7+SRR7p)
    ├── swe6-sqt-macro-flr7.yml       # Caller: SWE6 SQT_MACRO FLR7 (manual; folder=SWE6_TestScript_SQT_MACRO)
    ├── swe5-flr7.yml                 # Caller: SWE5 FLR7 (manual; folder=SWE5_Testing)
    ├── swe5-srr7p.yml                # Caller: SWE5 SRR7p (manual; folder=SWE5_Testing)
    ├── swe5-sit-macro-flr7.yml       # Caller: SWE5 SIT_MACRO FLR7 (manual; folder=SWE5_TestScript_SIT_MACRO)
    ├── swe-test-runner.yml           # Manual launcher with workflow_dispatch dropdowns (family/variant/xcp/folder/...)
    ├── swe-test-regression-flr7.yml  # Manual demo: markers + flags + extra-args
    ├── swe-test-stress-flr7.yml      # Manual demo: bench-attributes + spc-file + custom report glob + 12h timeout
    ├── swe-test-wi-smoke-srr7p.yml   # Manual demo: bracket WI markers + project/team overrides
    ├── swe-test-pyfilter-flr7.yml    # Manual demo: python-files + python-functions + no-reports dev loop
    ├── ondemand-swe6-flr7.yml        # On-demand SWE6 FLR7 re-run (manual)
    └── ondemand-swe6-srr7p.yml       # On-demand SWE6 SRR7p re-run (manual)
```

## Workflows

| File | Trigger | Purpose |
|---|---|---|
| `build.yml` | `workflow_call` | Low-level Bazel build engine. Per-matrix-item overrides for target, flags, output dir, artifact name. |
| `build-flavors.yml` | `workflow_call`, `workflow_dispatch` | Mirrors the 8 WRSD build flavors (satellite, sil, iso, vlan, sqt, sit, sit-core0, alignment). Inputs: `flavors` + `variants` JSON arrays. Used by pr-checks/nightly/weekly. |
| `unit-tests.yml` | `workflow_call` | Bazel `test` / `coverage` runner with per-variant matrix |
| `coverity.yml` | `workflow_call` | Per-(variant,core) Coverity intermediate dir build + commit_defects |
| `smoke-test.yml` | `workflow_call` | Self-hosted bench QuickFlash + smoke suite |
| `pr-checks.yml` | `pull_request: dev`, `workflow_dispatch` | Orchestrates pre-commit, dependency scan, satellite build, UT, Coverity, HW smoke; posts sticky PR comment |
| `nightly.yml` | `schedule: 02:00 UTC`, `workflow_dispatch` | Full nightly fan-out (all 8 flavors × flr7+srr7p) + unit-tests + coverity; hands off to WRSD. Mirrors WRSD `trigger-nightly` with `run-all-builds=true`. |
| `weekly.yml` | `schedule: Sun 22:00 UTC`, `workflow_dispatch` | Mirrors WRSD `trigger-weekly`: all flavors + 4h HW soak per bench, 13h overall budget. |
| `post-merge.yml` | `push: dev`, `workflow_dispatch` | Mirrors WRSD `trigger-post-merge`: auto-rebase open changes, run full Coverity sweep against merged commit, publish grafana metrics. |
| `gerrit-sync.yml` | `push` (mirror to Gerrit), `schedule: */15`, `repository_dispatch: gerrit-replication`, `workflow_dispatch` | Two-way Gerrit ↔ GitHub mirroring; mirrors `refs/changes/*` as `gerrit/<num>` branches so PR-checks can validate Gerrit patchsets |
| `wrsd-integration.yml` | `workflow_call` | Bundles per-variant build artifacts, uploads to JFrog, fires the WRSD pipeline trigger |
| `radar-test-reusable.yml` | `workflow_call` | **Generic** reusable engine for SWE6 / SWE6_Alignment / future SWE5 / SQT / SIT runs. Typed inputs (`markers`, `flags`, `bench-attributes`, `python-files`, `python-functions`, `spc-file`, `no-reports`, `gui-html`, `extra-args`, plus legacy `test-cmd-args` escape hatch) compose the `Run_Test_Cases.bat` command at runtime. Pulls the nightly zip from JFrog, optionally updates a pyproject TOML, runs the framework, uploads the GUI HTML report (common + static-latest), pushes results to Platform-Health/Grafana, posts a single Teams + Outlook notification via Power Automate, and cleans runner credentials. Concurrency is keyed on `(job-type, family)`. Ported from Gen8 `radar-test-reusable.yml` and adapted to Gen7's in-tree `Core_Radar_Gen7_SAF85xx_Python_Framework`. |
| `swe6-flr7.yml` | `schedule: 30 18 * * *` (00:00 IST), `workflow_dispatch` | Family wrapper for FLR7 (`FLR7_SQTp_pyproject_CICD.toml`, `output_flr7.zip`). |
| `swe6-srr7p.yml` | `schedule: 30 19 * * *` (01:00 IST), `workflow_dispatch` | Family wrapper for SRR7p (`SRR7p_SQTp_pyproject.toml`, `output_srr7p.zip`). |
| `swe6-alignment.yml` | `schedule: 30 20 * * 0` (Sunday 02:00 IST Monday), `workflow_dispatch` | **Weekly** alignment regression — calls the reusable twice (FLR7 + SRR7p) with `job-type: SWE6_Alignment`, `markers: Module=ALIGNMENT_Module`, `bench-attributes: Variant=<family>_V2`, `ph-mode: alignment`. Single combined Teams/Email after both finish. |
| `ondemand-swe6-flr7.yml` | `workflow_dispatch` | Manual FLR7 SWE6 re-run with optional JFrog path + recipients override. |
| `ondemand-swe6-srr7p.yml` | `workflow_dispatch` | Manual SRR7p SWE6 re-run with optional JFrog path + recipients override. |
| `swe5-flr7.yml` / `swe5-srr7p.yml` | `workflow_dispatch` (skeleton; cron commented out) | SWE5 nightly skeletons. Same engine, `test-folder: SWE5_Testing`, `report-subdir: Reports/SWE5`. Uncomment the `workflow_run` block to chain after the matching SWE6 nightly. |
| `swe6-sqt-macro-flr7.yml` | `workflow_dispatch` | SQT_MACRO suite (folder `SWE6_TestScript_SQT_MACRO` → `Reports/SWE6_SQT_MACRO`). Matches `launcher_config.toml` `folder_map`. |
| `swe5-sit-macro-flr7.yml` | `workflow_dispatch` | SIT_MACRO suite (folder `SWE5_TestScript_SIT_MACRO` → `Reports/SWE5_SIT_MACRO`). |
| `swe-test-runner.yml` | `workflow_dispatch` | **Picker UI** — the friendliest manual launcher. Dropdowns for family, variant (`FLR7_V2`/`SRR7p_V2`/`SRR7e`), XCP (`CAN`/`Eth`/`Eth_Direct`), test folder, test-type, SPC file; plus free-text fields for modules / WI IDs / SRS IDs / extra markers / flags / extra bench attributes. A tiny `prepare` job (ubuntu-latest) composes the markers / bench-attributes / subdir / TOML / JFrog path before delegating to `radar-test-reusable.yml`. Use this whenever you don't want to write a wrapper. |
| `swe-test-regression-flr7.yml` | `workflow_dispatch` | Demo wrapper — shows how to use `markers: TestType=Regression` + `flags: 'power_check log_verbose'` + `extra-args`. Copy + tweak for your own regression slice. |
| `swe-test-stress-flr7.yml` | `workflow_dispatch` | Demo wrapper — long-running stress run. Shows multi-attribute `bench-attributes`, `spc-file`, custom `report-name-glob`, and `timeout-minutes: 720`. |
| `swe-test-wi-smoke-srr7p.yml` | `workflow_dispatch` | Demo wrapper — bracket-notation WI markers (`WI=[WI-266881, WI-266882]`), `project-name` / `team-name` overrides, isolated `report-subdir`. |
| `swe-test-pyfilter-flr7.yml` | `workflow_dispatch` (with `python_files` + `python_functions` inputs) | Demo wrapper — fast dev loop. Demonstrates `python-files` + `python-functions` to override the TOML's collection patterns, `no-reports: true` to skip HTML upload. |

## Required secrets

Set these at the **repo** (or **org**) level. All are referenced by name from
the workflows above.

| Secret | Scope | Used by |
|---|---|---|
| `ADVRADAR_GHCR_USER` | Build container pull | `build`, `unit-tests`, `coverity` |
| `ADVRADAR_GHCR_TOKEN` | Build container pull | `build`, `unit-tests`, `coverity` |
| `ADVRADAR_NETRC_CONTENT` | Full `~/.netrc` payload — must contain machine entries for `gitgerrit.asux.aptiv.com`, `jfrog.asux.aptiv.com`, Coverity Connect | `build`, `unit-tests`, `coverity`, `gerrit-sync` |
| `ADVRADAR_ARB_TOKEN` | Aptiv RapidBuild (BES) remote-cache key | `build`, `unit-tests`, `coverity` |
| `ADVRADAR_COVERITY_COMMITTER_AUTH` | Coverity Connect auth-key JSON | `coverity` |
| `ADVRADAR_SID_USERNAME` | JFrog username for WRSD upload | `wrsd-integration` |
| `ADVRADAR_JFROG_API_KEY` | JFrog API key for WRSD upload | `wrsd-integration` |
| `ADVRADAR_WRSD_TRIGGER_TOKEN` | Bearer token for the WRSD trigger REST endpoint | `wrsd-integration` |
| `GITHUB_TOKEN` | Auto-issued — used for labels, PR comments, branch pushes | all callers |

## Setup checklist

1. Add the secrets above (Settings → Secrets and variables → Actions).
2. Configure the Gerrit `replication.config` to fire a `repository_dispatch`
   event of type `gerrit-replication` against this repo whenever a ref
   changes — that drives the `pull-from-gerrit` half of `gerrit-sync.yml`
   without waiting for the 15-min poll.
3. Register the self-hosted Gen7 bench runner(s) with labels
   `self-hosted, linux, SmokeTest, Gen7`.
4. Provision Coverity streams named `Core_Radar_Gen7_SAF85xx_<VARIANT>_<CORE>`
   and remove the `if: false` guard on the `coverity` job in `pr-checks.yml`.
5. Confirm `containers.aptv.ghe.com/devsecops/core-radar/build-format-ready:1861593`
   is the image you want; bump via the `container-image` input on each call.

## Extending

* New variant: add a row to each `matrix-json` in `pr-checks.yml` and
  `nightly.yml`. No reusable workflow change needed.
* New nightly stage: copy `nightly.yml` to `nightly-<family>.yml` and pin a
  family-specific cron, the same way Gen8 uses `swe6-flr8.yml` / `swe6-srr8p.yml`.
* New shared step: add a composite action under `.github/actions/<name>/action.yml`
  and `uses: ./.github/actions/<name>` from any workflow.
