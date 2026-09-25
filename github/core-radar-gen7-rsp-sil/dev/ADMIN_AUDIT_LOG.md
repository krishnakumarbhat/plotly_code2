## 2026-08-27 11:36:43 UTC - Repository Configuration Updated

- **Action**: Manage Repository Configuration
- **Setting**: Replace Status Checks
- **New Value**: ['verify / Check Documentation Changes', 'verify / Validate PR JIRA Tickets / pr-validation', 'verify / Prepare Variant Matrix', 'verify / Precommit / Pre-commit', "verify / Unit Tests (${{ (((matrix.variant != '') && matrix.variant) || 'default') }})", "verify / Build (${{ (((matrix.variant != '') && matrix.variant) || 'default') }})", 'verify / Verification Summary']
- **Ruleset Name**: Repo-Level Standard Governance Ruleset - Branch
- **Reason**: Add these validations as required rule-set checks to ensure code quality, proper traceability, successful builds, adequate test coverage, and complete documentation before PRs can be merged, reducing the risk of defects and improving release reliability.
- **Target Repositories**: GPO/core-radar-gen7-rsp-sil, GPO/core-radar-gen7-awr294x-autosar-sip
- **Requested By**: @hrithik-singh
- **Approved By**: @hrithik-singh
- **Issue**: [#2628](https://aptv.ghe.com/Aptiv/repository-admin-service/issues/2628)
- **Status**: ✅ Success


---

## 2026-08-27 11:36:33 UTC - Repository Configuration Updated

- **Action**: Manage Repository Configuration
- **Setting**: Update Ruleset Target Branches
- **New Value**: ['dev', 'release/**']
- **Ruleset Name**: Repo-Level Standard Governance Ruleset - Branch
- **Reason**: We are updating our branching strategy from main and release/* to dev and release/**. Accordingly, the branch-level rulesets must be updated to align with the new branch structure and maintain consistent branch governance. Hence, this change request.
- **Target Repositories**: GPO/core-radar-gen7-rsp-sil, GPO/core-radar-gen7-awr294x-autosar-sip
- **Requested By**: @hrithik-singh
- **Approved By**: @hrithik-singh
- **Issue**: [#2627](https://aptv.ghe.com/Aptiv/repository-admin-service/issues/2627)
- **Status**: ✅ Success


---

# Admin Action Audit Log

## 2026-08-26 09:59:02 UTC - Team Access Managed

- **Action**: grant Team Access
- **Operation**: grant
- **Repositories**: GPO/core-radar-gen7-awr294x-autosar-sip, GPO/core-radar-gen7-awr294x-ti-sdk, GPO/core-radar-gen7-rsp-sil, GPO/core-radar-gen7-exec-spec, GPO/core-resim-radar-emb-library
- **Team**: CORERADARS-DEVOPS
- **Access Level**: maintain
- **Reason**: Maintain access is required to support DevOps activities such as repository maintenance, CI/CD management, and configuration updates. Admin access cannot be granted to our role, and Maintain is the appropriate access level available to fulfill these responsibilities.
- **Requested By**: @Jonnalagadda-Sai-Dheeraj
- **Approved By**: @Jonnalagadda-Sai-Dheeraj
- **Issue**: [#2565](https://aptv.ghe.com/Aptiv/repository-admin-service/issues/2565)
- **Status**: ✅ Success


---

