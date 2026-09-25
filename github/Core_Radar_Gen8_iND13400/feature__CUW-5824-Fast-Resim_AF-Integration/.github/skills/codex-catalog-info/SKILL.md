---
name: codex-catalog-info
description: >
  How to create and update catalog-info.yaml for Aptiv CodeX onboarding (Backstage.io).
  USE FOR: creating a new catalog-info.yaml from scratch; updating any field (owner, domain,
  lifecycle, projectID, jiraProject, requirements, sbom, repository, tags, annotations, etc.);
  adding or correcting version/release entries; filling in or updating scorecard data;
  setting up TechDocs (mkdocs.yml + docs/); understanding which fields are auto-managed vs.
  manually maintained; fixing validation errors reported by the CodeX portal; understanding
  the Aptiv-specific schema (aptiv.com/v1alpha1) and how it differs from standard Backstage.io.
  DO NOT USE FOR: general Backstage.io questions unrelated to Aptiv's schema; CI/CD pipeline
  setup; GitHub Actions workflow authoring; onboarding to systems other than CodeX.
---

# catalog-info.yaml — CodeX Onboarding Skill

> **Schema source of truth:** [`codex_template.yaml`](https://aptv.ghe.com/DevSecOps/CODEX-process/blob/main/config-yaml/codex_template.yaml) — keep this SKILL in sync when the template changes.

## Overview

`catalog-info.yaml` is the CodeX (Aptiv Software Asset Catalog) registration file for a GitHub repository.
It follows the Backstage.io Component schema at `apiVersion: aptiv.com/v1alpha1`.

Place the file in the **repository root**. CodeX discovers it automatically via GitHub App integration.

---

## Full File Structure

```yaml
apiVersion: aptiv.com/v1alpha1  # fixed — do not change
kind: Component                 # fixed — do not change

metadata:
  name: <repo-name>             # [mandatory] must match GitHub repo name exactly
  description: >
    <Human-readable description shown in the CodeX portal>
  domain: <domain>              # [mandatory] one of the allowed values: UX | ADAS | GPO
  tags:                         # Each tag: [a-z0-9+#] chars only, separated by [-] (NO underscores), max 63 chars
    - <tag1>                    # ✅ core-radars  ❌ core_radars (underscore not allowed)
    - <tag2>
  annotations:
    backstage.io/techdocs-ref: dir:.  # required for TechDocs; dir:. = mkdocs.yml at repo root

  subcomponentof: []   # auto-updated by CodeX promotion — do not edit manually
  dependOn: []         # auto-updated by GitHub Workflow during CI — do not edit manually

spec:
  type: <library|application|service|website|cli>  # [mandatory]
  lifecycle: <active|production|deprecated|experimental>  # [mandatory]
  owner: <netid>                      # [mandatory] use plain netid (e.g. 'fz1nfx') — NOT 'user:netid' format (with prefix); CodeX raises 'entity not found' with the 'user:' prefix
  ownerEmail: <email>                   # [mandatory]
  projectID: '<Aptiv-roster-number>'    # quote as string to preserve leading zeros
  jiraProject: <JIRA-key>
  requirements: <URL>     # URL field — omit this line when no URL exists; never use '' or 'N/A' (CodeX rejects both)
  sbom: <URL>             # URL field — omit this line when no URL exists; never use '' or 'N/A' (CodeX rejects both)
  repository: https://aptv.ghe.com/<Org>/<repo>  # [mandatory]

  versions:            # auto-populated on GitHub Release by Workflow — manual updates only for initial setup
  - name: <version>    # e.g. 1.0.0 — must be ≥5 characters; use semver X.Y.Z (always ≥5 chars); avoid short tags like v1.0 (4 chars)
    location: <release-URL>
    released: <ISO-8601>   # e.g. 2026-04-20T08:46:35Z
    scorecard:
      misra_violations: 0
      ut_coverage: 0
      compiler_warnings: 0
      docs_present: false
      quality_scan_passed: false
      security_findings: 0
      sbom_present: false
      cve_critical: 0
      cve_high: 0
      cve_medium: 0
      techdocs_valid: false
      v-cycle_maturity: 0
```

---

## Step-by-Step: Creating catalog-info.yaml

### 1. Auto-Detect Context Before Asking Anything

> **CRITICAL — DO NOT ASK FOR REPO NAME OR URL UNDER ANY CIRCUMSTANCES.**
> The repository name and org are always available in the VS Code workspace attachment
> (fields `Repository name` and `Owner`). Read them directly — never prompt the user for them.

**How to read the workspace attachment:**
The chat context includes a block like:
```
Repository name: CODEX-process
Owner: DevSecOps
```
Use `metadata.name = <Repository name>` and construct `spec.repository = https://aptv.ghe.com/<Owner>/<Repository name>` directly from these values. If the attachment is absent, fall back to `git remote get-url origin`.

> **IMPORTANT:** Detect ALL values in this table silently before showing the user any questions. Never ask for values that can be determined from the workspace or git context.

| Value | How to auto-detect |
|---|---|
| `metadata.name` | **Read from workspace attachment** (`Repository name` field) or `git remote get-url origin`. **NEVER ask — always detect.** |
| `spec.repository` | Construct as `https://aptv.ghe.com/<Owner>/<Repository name>` from the workspace attachment — **NEVER ask** |
| `spec.owner` | **Ask the user** — do not auto-detect (see Step 2) |
| `spec.ownerEmail` | **Ask the user** — do not auto-detect (see Step 2) |
| `metadata.description` | **Generate** a one- or two-sentence description by reading `README.md`, the repo description on GitHub, and any existing documentation files (see §1a below) |
| `metadata.tags` | **Generate** a list of 4–8 lowercase tags derived from language files, folder names, and domain keywords (see §1b below) |
| Existing releases | Query GitHub Releases API; list as suggestions for `versions` entries |

#### 1a. Generating the Description

1. Read `README.md` (first 30 lines), any top-level `*.md` files, and the GitHub repo description field.
2. Write a concise one- or two-sentence summary (≤ 160 characters per sentence) that describes what the component **does**, not what files it contains.
3. Present it to the user with: *"I suggest this description — confirm, edit, or replace:"*
4. Wait for approval or a revised version before proceeding.

#### 1b. Generating Tags

Derive tags from the following sources (pick the most relevant 4–8, all lowercase, hyphen-separated):

> **Tag format rule (enforced by CodeX/Backstage):** Each tag must contain only `[a-z0-9+#]` characters, separated by `-` (hyphens). Underscores (`_`) are **not allowed** and will fail the portal policy check with: `"tags.N" is not valid; expected a string that is sequences of [a-z0-9+#] separated by [-]`. Maximum 63 characters per tag. Convert any underscore to a hyphen when generating or correcting tags (e.g. `core_radars` → `core-radars`).

- **Programming languages / file types** detected in the repo (e.g. `python`, `typescript`, `bash`, `yaml`)
- **Framework or tooling keywords** from `package.json`, `requirements.txt`, `CMakeLists.txt`, `*.csproj`, etc.
- **Folder / module names** that represent significant areas of functionality
- **Domain keywords** inferred from the repo name and description (e.g. `license-management`, `ci-cd`, `adas`, `codex`)
- **Component type** if it is distinctive (e.g. `library`, `cli`)

Present the generated list to the user with: *"I suggest these tags — confirm, or add/remove/change:"*
Wait for approval or corrections before proceeding.

### 2. Ask Only What Cannot Be Auto-Detected

Present a **single, grouped question set** for the remaining fields. Pre-fill every answer with the best suggestion so the user only needs to confirm or correct. Mark mandatory fields clearly.

| Question | Suggested default | Notes |
|---|---|---|
| Domain | *(none — must choose)* | Options: `UX` \| `ADAS` \| `GPO` |
| Component type | `service` if unclear; `library` for pure-code repos | Options: `library` \| `application` \| `service` \| `website` \| `cli` |
| Lifecycle | `experimental` | Options: `experimental` \| `active` \| `production` \| `deprecated` |
| Owner (netid) | *(no default — must ask)* | Plain netid — **no** `user:` prefix |
| Owner email | *(no default — must ask)* | Aptiv email address, ex.: `firstname.lastname@aptiv.com` |
| Project ID | `(unknown)` — ask user to fill later | Quote as string; omit leading zeros risk |
| JIRA project key | *(none)* — skip if unknown | ex.: `IKD`, `DIN`, `CES` |
| Requirements URL | *(omit)* | Ask only if the user mentions it; omit rather than `N/A` |
| SBOM URL | *(omit)* | Ask only if the user mentions it; omit rather than `N/A` |
| TechDocs | `dir:.` if `mkdocs.yml` exists at root; `dir:docs` if `docs/` folder detected | Show detected option as recommended |

**NEVER ask for:**
- Repository name (`metadata.name`) — read from the workspace attachment `Repository name` field; this is always present
- Repository URL (`spec.repository`) — construct from workspace attachment `Owner` + `Repository name`; this is always deterministic
- GitHub org — read from workspace attachment `Owner` field (e.g. `DevSecOps`)

### 3. Create the File

Copy the template above, fill in all mandatory fields (`[mandatory]`).
For optional fields without known values, use `[]` for arrays and omit string/URL fields entirely (or comment them out). **Never use `N/A` or an empty string `''` for URL fields** — CodeX validates URL field values and rejects both.

### 4. Add Existing Releases

For each GitHub Release, add a `versions` entry:
- `name`: a display name for the release — **must be ≥ 5 characters** (CodeX enforces this). Use semver format `X.Y.Z` (e.g. `1.0.0`) — always ≥5 chars and avoids validation errors. Short tags like `v1.0` (4 chars) will fail. The `location` field still uses the actual GitHub tag URL.
- `location`: the release URL (always use the actual GitHub tag URL, e.g. `.../releases/tag/v1.0`)
- `released`: ISO 8601 timestamp from GitHub (copy from the release page or API response)
- `scorecard`: fill `docs_present: true` if documentation exists, `techdocs_valid: true` if TechDocs is set up; leave numeric fields as `0` and booleans as `false` for unknown

List releases in **descending order** (newest first).

### 5. Create TechDocs Files (mandatory)

TechDocs support is **required** for every new `catalog-info.yaml`. Always create `mkdocs.yml` and the entry-point `index.md` as part of this workflow — do not skip.

**Required files:**
- `backstage.io/techdocs-ref: dir:<path>` annotation in `catalog-info.yaml`
- `mkdocs.yml` at the path matched by the annotation
- `index.md` in the same directory as `mkdocs.yml` (or in the `docs_dir` it references)

**Action — determine `mkdocs.yml` location:**

1. Scan the repository root for any existing documentation folder: `doc`, `docs`, `documents`, `documentation`, or any similarly named directory.
2. **Ask the user** where they want `mkdocs.yml` placed, presenting the detected folder(s) as the recommended option alongside the repo root. Example prompt:
   > "A `docs/` folder already exists. Should I place `mkdocs.yml` inside `docs/` (`dir:docs`) or at the repo root (`dir:.`)?"
3. Set `backstage.io/techdocs-ref` in `catalog-info.yaml` to match the chosen location:
   - `dir:.` → repo root (`mkdocs.yml`)
   - `dir:docs` → `docs/mkdocs.yml`
   - `dir:sub/folder` → `sub/folder/mkdocs.yml`

**Never default silently to `dir:.`** when a documentation folder exists — always ask first.

**Action — create `mkdocs.yml` at the path from the annotation** (if exists, update and ask user to confirm):
```yaml
site_name: <Repo Name>
site_description: <One-line description matching catalog-info.yaml description>
docs_dir: docs

plugins:
  - techdocs-core

nav:
  - Home: index.md
```

Add more nav entries for each `.md` file in `docs/`. Example:
```yaml
nav:
  - Overview: index.md
  - Server: server.md
  - Client: client.md
```

**Action — create `docs/index.md`** (if it doesn't exist):
```markdown
--8<-- "README.md"
```
This snippet include avoids duplication by rendering the repo's `README.md` as the TechDocs home page.

When TechDocs is correctly set up, set `techdocs_valid: true` in the latest version's scorecard.

---

## Updating catalog-info.yaml

### Changing Any Field

Edit the relevant field directly in `catalog-info.yaml`. All fields can be updated at any time except the auto-managed ones (see **Auto-Managed Fields** section below).

**Common updates and where to find each field:**

| What to change | Field(s) to edit |
|---|---|
| Owner | `spec.owner`, `spec.ownerEmail` |
| Domain | `metadata.domain` (`UX`, `ADAS`, or `GPO`) |
| Lifecycle | `spec.lifecycle` (`active`, `production`, `deprecated`, `experimental`) |
| Description | `metadata.description` |
| Tags | `metadata.tags` (list of strings — each must match `[a-z0-9+#]` separated by `-`; **no underscores**; max 63 chars) |
| TechDocs | `metadata.annotations.backstage.io/techdocs-ref` |
| Component type | `spec.type` (`library`, `application`, `service`, `website`, `cli`) |
| Project ID | `spec.projectID` (quote as string to preserve leading zeros) |
| JIRA project | `spec.jiraProject` |
| Requirements URL | `spec.requirements` (omit the field when not applicable — both `''` and `N/A` are rejected by CodeX) |
| SBOM URL | `spec.sbom` (omit the field when not applicable — both `''` and `N/A` are rejected by CodeX) |
| Repository URL | `spec.repository` |

### Adding a New Release

> **Reminder:** Whenever you create a new GitHub Release, also add a corresponding entry to `catalog-info.yaml`. The GitHub Workflow auto-updates `versions` only after initial setup — for new releases created via the skill or manually, update `catalog-info.yaml` at the same time.

Add a new entry at the **top** of the `versions` list (newest first):

```yaml
  versions:
  - name: 1.2.0                  # ← new entry here
    location: https://aptv.ghe.com/Aptiv/<repo>/releases/tag/v1.2.0
    released: 2026-05-06T10:00:00Z
    scorecard:
      ...
  - name: 1.1.0                  # ← previous entry stays below
    ...
```

### Updating Scorecard for a Release

Edit the `scorecard` block under the relevant `versions` entry.

**Scorecard field guide:**

| Field | Type | Promotion criteria? | Notes |
|---|---|---|---|
| `misra_violations` | int | ✅ | Static analysis violations |
| `ut_coverage` | int (%) | ✅ | Unit test coverage percentage |
| `compiler_warnings` | int | ✅ | Build warnings count |
| `docs_present` | bool | ✅ | Any documentation exists |
| `quality_scan_passed` | bool | ✅ | Static analysis gate passed |
| `security_findings` | int | display only | CyberSecurity team data |
| `sbom_present` | bool | display only | SBOM artifact available |
| `cve_critical` | int | display only | Critical CVE count |
| `cve_high` | int | display only | High CVE count |
| `cve_medium` | int | display only | Medium CVE count |
| `techdocs_valid` | bool | display only | TechDocs renders in portal |
| `v-cycle_maturity` | int | display only | Based on PHI PF |

---

## Auto-Managed Fields

| Field | Managed by |
|---|---|
| `metadata.subcomponentof` | CodeX promotion workflow (cross-referenced from other components) |
| `metadata.dependOn` | GitHub Workflow during CI run |
| `spec.versions` | GitHub Release event → GitHub Workflow (after initial manual setup) |

---

---

## Known Validation Errors

| Portal error message | Cause | Fix |
|---|---|---|
| `"tags.N" is not valid; expected a string that is sequences of [a-z0-9+#] separated by [-]` | A tag contains an invalid character (most commonly an underscore `_`) or is too long (>63 chars) | Replace underscores with hyphens (`core_radars` → `core-radars`); ensure every tag uses only `[a-z0-9+#-]` |
| `owner ... entity not found` | `spec.owner` was set with the `user:` prefix (e.g. `user:fz1nfx`) | Remove the prefix — use plain netid only: `fz1nfx` |
| `"versions[N].name" must be at least 5 characters` | A version name is too short (e.g. `v1.0` = 4 chars) | Use full semver `1.0.0` or prefix to reach ≥5 chars |
| `spec.requirements` / `spec.sbom` URL rejected | Field is set to `''` or `N/A` | Omit the field entirely when no URL exists |

---

## Real Example

See [catalog-info.yaml](https://aptv.ghe.com/DevSecOps/CODEX-process/blob/main/config-yaml/catalog-info.yaml) in the CODEX-process repository for a complete working example
