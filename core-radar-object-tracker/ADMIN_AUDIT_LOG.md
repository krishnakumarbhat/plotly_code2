## 2026-08-31 06:47:58 UTC - Repository Configuration Updated

- **Action**: Manage Repository Configuration
- **Setting**: Replace Status Checks
- **New Value**: ['verify / Check Documentation Changes', 'verify / Validate PR JIRA Tickets / pr-validation', 'verify / Prepare Variant Matrix', 'verify / Precommit / Pre-commit', "verify / Unit Tests (${{ (((matrix.variant != '') && matrix.variant) || 'default') }})", "verify / Build (${{ (((matrix.variant != '') && matrix.variant) || 'default') }})", 'verify / Verification Summary']
- **Ruleset Name**: Repo-Level Standard Governance Ruleset - Branch
- **Reason**: Add these validations as required rule-set checks to ensure code quality, proper traceability, successful builds, adequate test coverage, and complete documentation before PRs can be merged, reducing the risk of defects and improving release reliability.
- **Target Repositories**: GPO/core-radar-gen7-nxp-signal-processing, GPO/core-radar-xcp, GPO/core-radar-gen8-s32r47-sip, GPO/core-radar-reuse, GPO/core-radar-gen8-s32r47-boot, GPO/core-radar-toi-char-quality-determination, GPO/core-radar-static-alignment, GPO/core-radar-sensor-position, GPO/core-radar-gen8-ind13400-sdk, GPO/Core_Radar_Stream_Handler, GPO/core-radar-gen7-saf85xx-python-framework, GPO/core-radar-interference-detection, GPO/core-radar-object-tracker, GPO/core-radar-hil, GPO/core-radar-gen8-ind13400-sip, GPO/core-radar-gen8-s32r47-signal-processing
- **Requested By**: @Jonnalagadda-Sai-Dheeraj
- **Approved By**: @Jonnalagadda-Sai-Dheeraj
- **Issue**: [#2672](https://aptv.ghe.com/Aptiv/repository-admin-service/issues/2672)
- **Status**: ✅ Success


---

## 2026-08-31 06:47:22 UTC - Repository Configuration Updated

- **Action**: Manage Repository Configuration
- **Setting**: Update Ruleset Target Branches
- **New Value**: ['dev', 'release/**']
- **Ruleset Name**: Repo-Level Standard Governance Ruleset - Branch
- **Reason**: We are updating our branching strategy from main and release/* to dev and release/**. Accordingly, the branch-level rulesets must be updated to align with the new branch structure and maintain consistent branch governance. Hence, this change request.
- **Target Repositories**: GPO/core-radar-gen7-nxp-signal-processing, GPO/core-radar-xcp, GPO/core-radar-gen8-s32r47-sip, GPO/core-radar-reuse, GPO/core-radar-gen8-s32r47-boot, GPO/core-radar-toi-char-quality-determination, GPO/core-radar-static-alignment, GPO/core-radar-sensor-position, GPO/core-radar-gen8-ind13400-sdk, GPO/Core_Radar_Stream_Handler, GPO/core-radar-gen7-saf85xx-python-framework, GPO/core-radar-interference-detection, GPO/core-radar-object-tracker, GPO/core-radar-hil, GPO/core-radar-gen8-ind13400-sip, GPO/core-radar-gen8-s32r47-signal-processing
- **Requested By**: @Jonnalagadda-Sai-Dheeraj
- **Approved By**: @Jonnalagadda-Sai-Dheeraj
- **Issue**: [#2671](https://aptv.ghe.com/Aptiv/repository-admin-service/issues/2671)
- **Status**: ✅ Success


---

# Admin Action Audit Log

## 2026-08-31 06:46:30 UTC - Team Access Managed

- **Action**: grant Team Access
- **Operation**: grant
- **Repositories**: GPO/core-radar-gen7-nxp-signal-processing, GPO/core-radar-xcp, GPO/core-radar-gen8-s32r47-sip, GPO/core-radar-reuse, GPO/core-radar-gen8-s32r47-boot, GPO/core-radar-toi-char-quality-determination, GPO/core-radar-static-alignment, GPO/core-radar-sensor-position, GPO/core-radar-gen8-ind13400-sdk, GPO/Core_Radar_Stream_Handler, GPO/core-radar-gen7-saf85xx-python-framework, GPO/core-radar-interference-detection, GPO/core-radar-object-tracker, GPO/core-radar-hil, GPO/core-radar-gen8-ind13400-sip, GPO/core-radar-gen8-s32r47-signal-processing
- **Team**: CORERADARS-DEVOPS
- **Access Level**: maintain
- **Reason**: Maintain access is required to support DevOps activities such as repository maintenance, CI/CD management, and configuration updates. Admin access cannot be granted to our role and Maintain is the appropriate access level available to fulfill these responsibilities.
- **Requested By**: @Jonnalagadda-Sai-Dheeraj
- **Approved By**: @Jonnalagadda-Sai-Dheeraj
- **Issue**: [#2670](https://aptv.ghe.com/Aptiv/repository-admin-service/issues/2670)
- **Status**: ✅ Success


---
