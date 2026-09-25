# `.github/` — CI/CD for AWR294X (Gen7 Core Radar)

This directory replaces the Jenkins pipelines under `tools/Jenkins/`. It is structured in three tiers:

1. **Composite actions** (`.github/actions/`) — reusable steps for tooling/auth setup
2. **Reusable workflows** (`.github/workflows/*.yml` with `on: workflow_call`) — the
   verbs of the pipeline (build, unit-tests, coverity, dynamic-analysis, smoke-test,
   wrsd-integration, gerrit-sync)
3. **Trigger workflows** — bind the reusable workflows to GitHub events
   (PR, push, schedule, dispatch)

## Folder Map

```
.github/
├── actions/
│   ├── configure-bazel-cache/      # writes ARB API key header into user.bazelrc
│   ├── configure-netrc/            # writes ~/.netrc for JFrog/Gerrit access
│   ├── pr-status-comment/          # upserts a sticky PR comment with job status
│   ├── setup-bazelisk/             # validates ./bazelisk or PATH bazelisk
│   └── setup-coverity-auth/        # base64-decodes Coverity committer credentials
├── workflows/
│   ├── build.yml                   # reusable matrix Bazel build
│   ├── unit-tests.yml              # reusable MSS/DSS unit tests (+ coverage mode)
│   ├── coverity.yml                # reusable per-stream Coverity run
│   ├── dynamic-analysis.yml        # reusable asan/ubsan/valgrind run
│   ├── smoke-test.yml              # reusable HW bench skeleton (gated)
│   ├── wrsd-integration.yml        # JFrog upload + optional WRSD trigger
│   ├── gerrit-sync.yml             # bi-directional Gerrit mirror
│   ├── build-flavors.yml           # orchestrator (production/standalone/eco)
│   ├── pr-checks.yml               # on: pull_request
│   ├── nightly.yml                 # on: schedule (02:00 UTC daily)
│   ├── weekly.yml                  # on: schedule (Sun 22:00 UTC)
│   ├── post-merge.yml              # on: push to dev
│   ├── ondemand-build-production.yml
│   ├── ondemand-build-standalone.yml
│   ├── ondemand-build-eco.yml
│   ├── ondemand-build-custom.yml
│   ├── ondemand-swe5.yml           # bench, gated
│   └── ondemand-swe6.yml           # bench, gated
├── Dependency.md                   # secrets, runners, services, omissions
└── README.md                       # this file
```

## Jenkins → GitHub Actions mapping

| Jenkins job (`tools/Jenkins/`)     | GitHub Actions equivalent                                                       |
|------------------------------------|---------------------------------------------------------------------------------|
| `verifiedJob/Jenkinsfile`          | [pr-checks.yml](workflows/pr-checks.yml)                                        |
| `buildJob/Jenkinsfile`             | [build.yml](workflows/build.yml) (via [build-flavors.yml](workflows/build-flavors.yml)) |
| `unitTestJob/Jenkinsfile`          | [unit-tests.yml](workflows/unit-tests.yml)                                      |
| `coverityJob/Jenkinsfile`          | [coverity.yml](workflows/coverity.yml)                                          |
| `dynamicAnalysisJob/Jenkinsfile`   | [dynamic-analysis.yml](workflows/dynamic-analysis.yml)                          |
| `formatterJob/Jenkinsfile`         | `precommit` job in [pr-checks.yml](workflows/pr-checks.yml)                     |
| `nightlyJob/Jenkinsfile`           | [nightly.yml](workflows/nightly.yml)                                            |
| `weeklyJob/Jenkinsfile`            | [weekly.yml](workflows/weekly.yml)                                              |
| `integrationJob/Jenkinsfile`       | [smoke-test.yml](workflows/smoke-test.yml) (gated `if: false`)                  |
| `SWE5Job/Jenkinsfile`              | [ondemand-swe5.yml](workflows/ondemand-swe5.yml) (gated `if: false`)            |
| `SWE6Job/Jenkinsfile`              | [ondemand-swe6.yml](workflows/ondemand-swe6.yml) (gated `if: false`)            |
| (Jenkins JFrog publish tail)       | [wrsd-integration.yml](workflows/wrsd-integration.yml)                          |
| (Gerrit replication, hook-driven)  | [gerrit-sync.yml](workflows/gerrit-sync.yml)                                    |

## Build flavors

Three flavors are supported by [build-flavors.yml](workflows/build-flavors.yml):

| Flavor       | Variants supported | Extra Bazel flags                       |
|--------------|--------------------|-----------------------------------------|
| `production` | `srr7p`, `flr7`    | (none)                                  |
| `standalone` | `srr7p` only       | `--rot=standalone`                      |
| `eco`        | `srr7p`, `flr7`    | `--awr_target_variant=awr2944eco`       |

`srr7hd` exists in the WORKSPACE/BUILD as a third variant but is **not** built by
the Jenkins verifiedJob/nightlyJob today, so the default matrices ship only
`srr7p` + `flr7`. Add it explicitly via `inputs.variants` if needed.

## Coverity streams

PR checks and Nightly both run the full 8-stream matrix:
`ADVRADAR_{SRR7p,FLR7}_{Mss,Dss}` and `ADVRADAR_{SRR7p,FLR7}_ECO_{Mss,Dss}`.
Post-merge only commits defects against the 4 non-ECO streams.

## See also

- [Dependency.md](Dependency.md) — secrets / runners / services / omissions
