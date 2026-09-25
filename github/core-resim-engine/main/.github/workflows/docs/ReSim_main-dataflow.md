# core-resim-engine Data Flow

This document contains sequence diagrams that illustrate the data flow and processes in the core-resim-sensor-model CI/CD workflow.

## Project Overview

**Repository:** `GPO\core-resim-sensor-model`

**Team:** AZ-GTE-GPO-RESIM-DEVELOPER (TODO: confirm team name)

**Platform:** linux (Cmake build system with MSVC compiler)

**Grafana Project Key:** `NIL`(TODO: confirm team name)

**Gerrit Project:** https://gitgerrit.asux.aptiv.com/admin/repos/Core_RESIM_Sensor_Model,general

**Confluence link:** https://confluence.asux.aptiv.com/spaces/CInD/pages/757183955/RESIM+-+WR+Integration+Plan


**Components:**
- SM2 Sensor Model (core codebase)
- SM2 FMU (Functional Mock-up Unit package)
- SM2 DLL (dynamic library build)
- SM Interface executable (SMiface.exe)
- CMake-based build system
- Visual Studio 2019 Build Tools (MSVC toolchain)
- Dependent libraries:
   - OpenSimulationInterface v3.5.0
   - Protobuf v3.6.1
   - Eigen v3

## Dependencies / Tools

| Category | Dependency / Tool | Version |
|---|---|---|
| Base OS | Ubuntu | 18.04.6 LTS (Bionic Beaver)"|
| Core Utilities | curl | Latest |
| Core Utilities | wget | Latest |
| Core Utilities | git | Latest |
| Core Utilities | git-lfs | Latest |
| Build Toolchain | MSVC |
| Python Toolchain | pyenv | 3.1.1 |
| Python Toolchain | Python | 3.6.9 |
| Python Toolchain | pip | Enterprise configured |
| Python Toolchain | wheel | Latest |
| Python Toolchain | py7zr | Latest |
| Artifact Repos | pip config (JFrog) | Custom endpoints |
| Bazel Tooling | cmake | 3.28.4 |
| Credentials | git-credential-netrc | Custom |
| Credentials | .netrc | Custom |

> Note: Jobs run directly on self-hosted runners. No job containers / container images are used.

## Overall Flow (High-Level)
This diagram shows the main phases of the  workflow:

```mermaid
sequenceDiagram
    participant Dev as Developer
    participant PR as Pull Request
    participant WF as Workflow
    participant Validate as PR Validation
    participant Build as Build  
    participant Metrics as Metrics Publish

    Note over Dev,Metrics: Phase 1: PR Creation & Validation
    Dev->>PR: Create/Update PR to master
    PR->>WF: Trigger workflow (opened/reopened/synchronize)
    WF->>Validate: Run pr-validation (reusable workflow)

    alt Validation passes
        Validate-->>WF: ✓ Gate passed

        Note over Dev,Metrics: Phase 2: Parallel Build/Test
        par Build & Analyze
        WF->>Build: Execute build job
        Build->>Build: cmake build, Coverity scan
        Build->>Build: Create artifacts and Unit Test
        end

        Note over Dev,Metrics: Phase 3: Consolidated Report & Metrics
        WF->>CReport: Run consolidated-report (reusable workflow)
        WF->>Metrics: Run publish-metrics (reusable workflow)

        WF-->>Dev: ✅ Workflow complete
    else Validation fails
        Validate-->>PR: Post failure details
        WF-->>Dev: ❌ Workflow failed (fix JIRA tickets)
    end
```

---

## Detailed Technical Flow

This diagram shows a more complete technical view (jobs, decision points, and external integrations):

```mermaid
sequenceDiagram
    participant Dev as Developer
    participant GH as GitHub
    participant Runner as Runner
    participant JIRA as JIRA API
    participant Cov as Coverity Service

    Dev->>GH: Create/Update PR
    GH->>Runner: Run pr-validation (reusable workflow)

    Note over Dev,Grafana: Reusable workflow 1: PR Validation (JIRA)
    Runner->>GH: Checkout code (with Git LFS)
    Runner->>JIRA: Query ticket status (IAX)
    JIRA-->>Runner: Ticket status

    alt Tickets valid (WIP/In Progress/Done)
        Runner-->>GH: ✓ Validation passed
    else Tickets missing/invalid
        Runner-->>GH: ✗ Validation failed
        Runner-->>GH: Post PR comment with details
        Note over Runner: Workflow stops (gate)
    end

    Note over Dev,Grafana: Reusable workflows 2-4: Parallel jobs after validation gate
    par build-analyze
        GH->>Runner: Run build-analyze (reusable workflow)
        Runner->>GH: Checkout code (with Git LFS)
        Runner->>Runner: bazel build ... (iar compiler)
        Note over Runner: bazel bazel build //:all --config=iar
        Runner->>Runner: cov-cli build + analyze
        Note over Runner: cov-cli build "bazel build //:all --config=iar"
        Runner->>Cov: Upload Coverity results
        Cov-->>Runner: Analysis complete
        Runner-->>GH: Upload artifacts/logs (GitHub Actions artifacts)
    and unittest
        GH->>Runner: Run unittest (reusable workflow)
        Runner->>GH: Checkout code (with Git LFS)
        Runner->>Runner: bazel test ... --coverage
        Runner->>Runner: Generate coverage report (gcovr)
        Runner-->>GH: Upload coverage artifacts/logs (GitHub Actions artifacts)
    and fuzz-test (optional)
        GH->>Runner: Run fuzz-test (nightly/full-build or manual option)
        Runner->>GH: Checkout code (with Git LFS)
        Runner->>Mayhem: mayhem login + run_fuzz_targets.ps1 -RunAll
        Mayhem-->>Runner: Fuzz target execution status
        Runner-->>GH: Upload fuzz artifacts/logs (fuzz-test.zip, output/*)
    end

    Note over Dev,Grafana: Reusable workflow 4: Consolidated Report
    GH->>Runner: Run consolidated-report (reusable workflow)
    Runner->>Runner: Consolidate statuses (pr-validation/build-analyze/unittest/fuzz-test)
    Runner->>Runner: Generate a single PR comment / summary artifact
    Runner-->>GH: Upload consolidated report logs/artifacts (GitHub Actions artifacts)

    Note over Dev,Grafana: Reusable workflow 6: Metrics publish
    GH->>Runner: Run publish-metrics (reusable workflow)
    Runner->>Grafana: Publish workflow metrics
    Grafana-->>Runner: Metrics stored

```

---

## Workflow Triggers

This diagram shows the different trigger mechanisms and their behavior:

```mermaid
sequenceDiagram
    participant Dev as Developer
    participant PR as Pull Request
    participant Queue as Merge Queue
    participant Branch as develop Branch
    participant Manual as Workflow Dispatch
    participant Workflow as Workflow

    Note over Dev,Workflow: Trigger 1: Pull Request
    Dev->>PR: Create/update PR to develop
    PR->>Workflow: Trigger (opened, reopened, synchronize)
    Workflow->>Workflow: Run all reusable workflows (except blackduck)

    Note over Dev,Workflow: Trigger 2: Merge Queue
    Queue->>Workflow: PR enters merge queue
    Workflow->>Workflow: Run all reusable workflows (except blackduck)
    Note over Dev,Workflow: Trigger 3: Push to develop
    Dev->>Branch: Push to develop (after merge)
    Branch->>Workflow: Trigger on push
    Workflow->>Workflow: Run all reusable workflows (except blackduck)

    Note over Dev,Workflow: Trigger 4: Manual Workflow Dispatch
    Dev->>Manual: Select workflow_dispatch
    Manual->>Dev: Prompt for job selection

    alt Select 'all'
        Dev->>Workflow: Run all reusable workflows
    else Select 'build'
    end
```

## Artifact Flow

This diagram shows how artifacts are created, uploaded, and consumed:

```mermaid
sequenceDiagram
    participant Build as build
    participant Storage as GitHub Artifacts
    participant Report as consolidate-report
    participant User as Developer

    Note over Build,User: Build artifacts
    Build->>Build: Create bazel-out-artifacts.tar.gz
    Build->>Build: Create bazel-bin-artifacts.tar.gz
    Build->>Storage: Upload 10058898_PAG_SACID_PRO-build-analyze

    Build->>Storage: Upload build tar.gz files

    Note over Build,User: Test artifacts  
    Test->>Test: Create unittests.tar.gz
    Test->>Storage: Upload 10058898_PAG_SACID_PRO-unit-test
    Test->>Storage: Upload unittests.tar.gz

    Note over Build,User: Fuzz artifacts
    Fuzz->>Fuzz: Create fuzz-test.zip and output/*
    Fuzz->>Storage: Upload fuzz-test artifacts/logs

    Note over Build,User: Artifact consumption
    Report->>Storage: Download 10058898_PAG_SACID_PRO-build-analyze
    Storage-->>Report: Build artifacts
    Report->>Storage: Download unit-test artifacts (pattern: *)
    Storage-->>Report: Test artifacts
    Report->>Storage: Download fuzz-test artifacts (pattern: *)
    Storage-->>Report: Fuzz artifacts

    Report->>Report: Parse and consolidate
    Report->>Report: Generate report

    Note over Build,User: Artifact retention
    Note over Storage: Artifacts stored per GitHub retention policy
    User->>Storage: Can download artifacts manually
    Storage-->>User: Download zip files
```

Key behavior from run traton-srr6plust-asp3-973:

- Execution gate: runs for pipeline-type=full-build with trigger-event=nightly (or empty/default) and pipeline-option including all or fuzz-test.
- Runtime: mayhem login succeeds, then run_fuzz_targets.ps1 -RunAll executes all configured fuzz targets.
- Observed targets from logs: Alignment and DataSetManager harness packages.
- Artifacts: task uploads fuzz-test.zip plus /workspace/repo/output/* in post-actions.
        
                       
    

### Trigger Events
- **Pull Request Events**: `opened`, `reopened`, `synchronize`
- **Target Branches**: `master`
- **Push Events**: Direct pushes to `master` *(no jobs run)*

### Runner & Environment
- **Primary Compute**: `linux-2-cpu-8g` (2 CPU, 8GB RAM)
- **Container**: linux containers with resource limits
- **Execution Model**: Direct execution on self-hosted runners (no job containers/images)
- **Workspace**: `C:\workspace`
- **Thread Count**: Configurable per task (build: 16, test: 4)
- **Build Cache**: Aptiv Rapid Build multi-region shared cache

### Quality Gates
1. **JIRA Validation**: Tickets must exist and be in valid status *(WIP/In Progress/Done)*
2. **Build Success**: `build` must complete successfully
3. **Code Coverage**: Minimum 80% threshold
4. **Static Analysis**: Coverity must pass
5. **Compiler Warnings**: Analyzed and reported

### Secrets & Authentication
- **JIRA_TOKEN**: JIRA API authentication for ticket validation
- **JFROG_USERNAME/PASSWORD**: JFrog Artifactory access (dependencies only; no artifact publishing)
- **APP_ID/APP_PRIVATE_KEY**: GitHub App authentication for enhanced API access and permissions

### Compute Resources

| Resource Name | Specification | Usage |
|---|---|---|
| linux-2-cpu-8g | 2 CPU, 8GB RAM | Primary build/analysis tasks |

### Container Configurations

| Container Type | Resource Specification | Purpose |
|---|---|---|
| linux-2-cpu-8g | 2 CPU, 8GB RAM, 150GB disk | Default container |
| asp-container-2c8g50g | 2 CPU, 8GB RAM, 50GB disk | Small Linux tasks |

---

### Bazel Cache Comparison (Before vs After Rapid Build)--------

| Process | Before Rapid Build (No Bazel Cache Optimization) | After Rapid Build (Bazel Cache Enabled) | Notes |
|---|---|---|---|
| Build process | ~48 min to 1 hr 15 min | ~15 min to 25 min | Pod allocation time (~15 min to 50 min) excluded from these ranges |
| Coverity build process | More than 2 hr (includes pod allocation) | ~1 hr 15 min to 2 hr | Statistics are based on build and Coverity process analysis |

**Test pipeline reference:** https://389177498442.awsaptiv.com/plm/view/sacid-pro-asp3-test-build/workflow

## Pain Point Comparison

## Gerrit vs GitHub — Pain Point Comparison

| Area | Gerrit (Pain Points) | GitHub (Strengths) |
|------|---------------------|-------------------|
| Workflow flexibility | Rigid review‑before‑submit enforced | Configurable via branch protections |
| Code review UX | Dated, dense UI | Modern, clean interface |
| Review semantics | Label-based (+1, +2) | Approve / Request changes |
| Change updates | Multiple patch sets per change | Single evolving PR |
| Comment context | Often lost across patch sets | Automatically preserved |
| Reviewer effort | High for iterative changes | Lower and in-built Github copilot review system |
| CI/CD integration | External systems required | Built-in GitHub Actions |
| CI feedback | Indirect and fragmented | PR integration |
| Security & Compliance Features | Weak integration of codequality and secret scannig | In built  Dependabot, Secret scanning, CodeQL |
| Permission model | Very granular but complex | Simple, predictable roles |
| Cross-team collaboration | Friction-heavy | Seamless |
| Build Summary | Not Native | Native summary via GitHub Actions job view | 
| Quality metrics summary | Requires custom plugins or bots | readable tabular summaries in PR checks |
| Test results visibility | External (CI UI or manual links) | Native test summary in PR |
| Ease of Use | Intricate & complex  | Intuitive & straight forward |

## Contact and Support

**Repository:** `Core_RESIM_Sensor_Model`



**CI/CD Support:** *(TODO: add owning team / DevSecOps contact)*





---

**Document Version:** 1.0
**Last Updated:** June 25, 2026
