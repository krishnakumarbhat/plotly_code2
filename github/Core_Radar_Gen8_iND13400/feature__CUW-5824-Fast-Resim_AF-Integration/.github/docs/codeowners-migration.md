# Gerrit → GitHub Code Owners Migration

## Branch Protection Setup

Required settings on `main` (or `dev`) branch:

### Via GitHub UI: Settings → Branches → Branch protection rules

1. **Add rule** for branch `main` (or pattern `dev`):

| Setting | Value |
|---------|-------|
| Require a pull request before merging | ✅ |
| Required approving reviews | 1+ (minimum) |
| **Require review from Code Owners** | ✅ |
| Dismiss stale pull request approvals | ✅ |
| Require approval of most recent push | ✅ (prevents self-approve) |
| Require status checks to pass | ✅ |
| Status checks required | `strict-codeowners-check` |
| Require branches to be up to date | ✅ |

### Via GitHub CLI (gh):

```bash
gh api repos/{owner}/{repo}/branches/main/protection -X PUT --input - <<'EOF'
{
  "required_status_checks": {
    "strict": true,
    "contexts": ["strict-codeowners-check"]
  },
  "enforce_admins": true,
  "required_pull_request_reviews": {
    "dismiss_stale_reviews": true,
    "require_code_owner_reviews": true,
    "required_approving_review_count": 1,
    "require_last_push_approval": true
  },
  "restrictions": null
}
EOF
```

### Via Ruleset (recommended for GitHub Enterprise):

```bash
gh api repos/{owner}/{repo}/rulesets -X POST --input - <<'EOF'
{
  "name": "Code Owners Enforcement",
  "target": "branch",
  "enforcement": "active",
  "conditions": {
    "ref_name": { "include": ["refs/heads/main", "refs/heads/dev"] }
  },
  "rules": [
    {
      "type": "pull_request",
      "parameters": {
        "required_approving_review_count": 1,
        "dismiss_stale_reviews_on_push": true,
        "require_code_owner_review": true,
        "require_last_push_approval": true
      }
    },
    {
      "type": "required_status_checks",
      "parameters": {
        "strict_required_status_checks_policy": true,
        "required_status_checks": [
          { "context": "strict-codeowners-check" }
        ]
      }
    }
  ]
}
EOF
```

## How It Works

```
┌─────────────────────────────────────────────────────────────────┐
│ PR Opened / New Commits Pushed / Review Submitted               │
├─────────────────────────────────────────────────────────────────┤
│ 1. GitHub native CODEOWNERS → requests reviewers from teams     │
│    (standard GitHub behavior)                                   │
│                                                                 │
│ 2. strict-codeowners.yml:                                       │
│    a. Get changed files via PR API                              │
│    b. Match each file → owner groups from .github/CODEOWNERS    │
│    c. Collect union of all required groups                      │
│    d. Filter reviews: only HEAD commit reviews count            │
│       (approvals from previous commits → stale, ignored)        │
│    e. Resolve each team — two-attempt strategy:                 │
│       • Attempt 1: listMembersInOrg (needs read:org)            │
│       • Attempt 2: getMembershipForUserInOrg per approver       │
│         (works on many GHE orgs without read:org)               │
│       • If both fail → "unresolvable" → skip + warn             │
│    f. Check: each resolved group has ≥1 approver on HEAD        │
│    g. FAIL if any resolved group is missing an approval         │
│       WARN if any group could not be resolved (not blocking)    │
│                                                                 │
│ 3. Branch protection blocks merge until:                        │
│    - strict-codeowners-check ✅                                 │
│    - Required reviewers approved                                │
└─────────────────────────────────────────────────────────────────┘
```

### Stale Approval Behaviour

After new commits are pushed to a PR, **all previous approvals are ignored** — only reviews whose `commit_id` matches the current HEAD SHA count. Reviewers must re-approve after each push. This mirrors GitHub's *"Dismiss stale pull request approvals"* branch-protection setting and is enforced entirely within the workflow (no branch-protection dependency).

### Team Resolution — No Config Files or Extra Secrets Required

The workflow resolves team membership dynamically via two API attempts. If both fail (e.g. `GITHUB_TOKEN` lacks `read:org` and the org's GHE policy blocks per-user checks), the team is marked **unresolvable** and enforcement is skipped with a warning. The PR is **not permanently blocked** — native GitHub CODEOWNERS branch protection remains the safety net.

| Situation | Outcome |
|-----------|---------|
| Team resolved, ≥1 member approved HEAD | ✅ PASSED |
| Team resolved, no member approved HEAD | ❌ FAILED — re-approve needed |
| Team unresolvable, approvals exist | ⚠️ PASSED WITH WARNINGS |
| Team unresolvable, no approvals on HEAD | ❌ FAILED — someone must approve |

## Gerrit → GitHub Behavior Mapping

| Gerrit Behavior | GitHub Equivalent |
|----------------|-------------------|
| Directory OWNERS (all files in dir) | `/path/to/dir/` pattern in CODEOWNERS |
| `per-file glob=owner` | `/path/to/dir/glob` pattern in CODEOWNERS |
| `include OtherRepo:/PATH` | Map to `@org/team` in CODEOWNERS |
| All owners must approve | `strict-codeowners.yml` action (not native) |
| Inheritance (parent OWNERS apply) | `*` fallback + explicit patterns |
| File renames | Action uses `--diff-filter=ACMRT` (includes renames) |

## File Inventory

| File | Purpose |
|------|---------|
| `.github/CODEOWNERS` | Owner rules (native GitHub integration) |
| `.github/workflows/strict-codeowners.yml` | Strict ALL-owners enforcement |
| `tools/python/convert_owners_to_codeowners.py` | Migration converter script |
| `tools/python/owners_mapping.json` | Gerrit ref → GitHub team mapping |

## Running the Converter

```bash
python tools/python/convert_owners_to_codeowners.py \
  --repo-root . \
  --output .github/CODEOWNERS \
  --mapping tools/python/owners_mapping.json \
  --fallback "@AptivRadar/core-radar-maintainers"
```

## Team Setup Required

Create these GitHub teams before enabling:

1. `@GPO/coreradars-devops` — CI/build infrastructure + fallback/default owners
2. `@GPO/core-radar-afbb-owners` — Angle Finding Building Blocks
3. `@GPO/core-radar-mcal-owners` — MCAL drivers
4. `@GPO/core-radar-sdmf-owners` — SDMF/Logging
5. `@GPO/core-radar-autosar-owners` — AUTOSAR SWCs
6. `@GPO/core-radar-boot-owners` — Boot/Startup
7. `@GPO/core-radar-memory-owners` — Memory/Linker

Add Gerrit owners as members of corresponding teams.

> **Important:** Teams must be **"closed" visibility** (the GitHub default), not "secret".
> Secret teams return 404 for all membership API calls, causing them to appear unresolvable.
> If you require secret teams, store an org-scoped PAT (`read:org`) as repository secret
> `ORG_READ_TOKEN` and replace `github-token: ${{ secrets.GITHUB_TOKEN }}` with
> `github-token: ${{ secrets.ORG_READ_TOKEN }}` in the "Verify" step of `strict-codeowners.yml`.
