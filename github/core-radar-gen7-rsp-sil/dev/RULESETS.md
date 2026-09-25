# Repository Rulesets Documentation

**Repository**: GPO/core-radar-gen7-rsp-sil
**Generated**: 2026-08-27 11:36:48 UTC
**Total Rulesets**: 7

This document contains comprehensive information about all repository rulesets configured for this repository.

---

## 1. Codex Validation

### Basic Information

- **Name**: Codex Validation
- **ID**: 1083472
- **Source Type**: Enterprise
- **Enforcement**: active
- **Target**: branch
- **Created**: 2026-05-28T13:11:55.070Z
- **Last Updated**: 2026-07-28T14:47:51.281Z

### Targeting Conditions

**Included Branches/Tags**:
- `~DEFAULT_BRANCH`

### Rules Applied

#### 1. Workflows

- **Required Workflows**:
  - Path: `.github/workflows/codex-validation.yml` (Ref: refs/heads/main) (Repository ID: 3291782)

### Bypass Actors

*No bypass actors configured.*

---

## 2. Deletions/transfers restriction

### Basic Information

- **Name**: Deletions/transfers restriction
- **ID**: 98742
- **Source Type**: Enterprise
- **Enforcement**: active
- **Target**: repository
- **Created**: 2025-10-06T18:23:41.379Z
- **Last Updated**: 2025-10-06T18:23:41.379Z

### Rules Applied

#### 1. Repository Transfer

*No specific parameters configured.*

#### 2. Repository Delete

*No specific parameters configured.*

### Bypass Actors

*No bypass actors configured.*

---

## 3. Visibility Restriction

### Basic Information

- **Name**: Visibility Restriction
- **ID**: 78527
- **Source Type**: Enterprise
- **Enforcement**: active
- **Target**: repository
- **Created**: 2025-09-18T15:34:02.824Z
- **Last Updated**: 2025-10-31T14:50:09.827Z

### Rules Applied

#### 1. Repository Visibility

- **Parameters**:
  - `internal`: False
  - `private`: True

### Bypass Actors

*No bypass actors configured.*

---

## 4. Organization-Level Governance Ruleset - Branch

### Basic Information

- **Name**: Organization-Level Governance Ruleset - Branch
- **ID**: 822687
- **Source Type**: Organization
- **Enforcement**: active
- **Target**: branch
- **Created**: 2026-04-15T10:27:31.506Z
- **Last Updated**: 2026-08-12T07:27:16.030Z

### Targeting Conditions

**Included Branches/Tags**:
- `~DEFAULT_BRANCH`
- `refs/heads/main`
- `refs/heads/includes/**`

### Rules Applied

#### 1. Deletion

*No specific parameters configured.*

#### 2. Non Fast Forward

*No specific parameters configured.*

#### 3. Pull Request

- **Dismiss Stale Reviews**: No
- **Require Last Push Approval**: Yes
- **Required Approving Review Count**: 1
- **Required Review Thread Resolution**: No
- **Code Owner Reviews**: Yes

### Bypass Actors

*The following users, teams, or apps can bypass this ruleset:*

- **Repositoryrole** (ID: 11) - *always*
- **Integration** (ID: 11407) - *always*
- **Businessteam** (ID: 6045262) - *always*

---

## 5. Organization-Level Governance Ruleset - Tags

### Basic Information

- **Name**: Organization-Level Governance Ruleset - Tags
- **ID**: 822692
- **Source Type**: Organization
- **Enforcement**: active
- **Target**: tag
- **Created**: 2026-04-15T10:27:31.993Z
- **Last Updated**: 2026-08-12T07:27:42.150Z

### Targeting Conditions

**Included Branches/Tags**:
- `~ALL`

### Rules Applied

#### 1. Deletion

*No specific parameters configured.*

#### 2. Non Fast Forward

*No specific parameters configured.*

#### 3. Creation

*No specific parameters configured.*

#### 4. Update

*No specific parameters configured.*

### Bypass Actors

*The following users, teams, or apps can bypass this ruleset:*

- **Repositoryrole** (ID: 11) - *always*
- **Repositoryrole** (ID: 21) - *always*
- **Integration** (ID: 11407) - *always*
- **Businessteam** (ID: 6045262) - *always*

---

## 6. Repo-Level Standard Governance Ruleset - Branch

### Basic Information

- **Name**: Repo-Level Standard Governance Ruleset - Branch
- **ID**: 1452972
- **Source Type**: Repository
- **Enforcement**: active
- **Target**: branch
- **Created**: 2026-08-24T10:01:00.722Z
- **Last Updated**: 2026-08-27T11:36:34.424Z

### Targeting Conditions

**Included Branches/Tags**:
- `refs/heads/dev`
- `refs/heads/release/**`

### Rules Applied

#### 1. Deletion

*No specific parameters configured.*

#### 2. Non Fast Forward

*No specific parameters configured.*

#### 3. Required Status Checks

- **Strict Policy**: No
- **Required Status Checks**:
  - `build-analyze`
  - `unit-tests`

### Bypass Actors

*The following users, teams, or apps can bypass this ruleset:*

- **Organizationadmin** (ID: None) - *always*

---

## 7. Repo-Level Standard Governance Ruleset - Tags

### Basic Information

- **Name**: Repo-Level Standard Governance Ruleset - Tags
- **ID**: 1452977
- **Source Type**: Repository
- **Enforcement**: active
- **Target**: tag
- **Created**: 2026-08-24T10:01:01.032Z
- **Last Updated**: 2026-08-24T10:01:01.071Z

### Targeting Conditions

**Included Branches/Tags**:
- `~ALL`

### Rules Applied

#### 1. Deletion

*No specific parameters configured.*

#### 2. Non Fast Forward

*No specific parameters configured.*

#### 3. Creation

*No specific parameters configured.*

#### 4. Update

*No specific parameters configured.*

### Bypass Actors

*The following users, teams, or apps can bypass this ruleset:*

- **Organizationadmin** (ID: None) - *always*
- **Repositoryrole** (ID: 21) - *always*

---

