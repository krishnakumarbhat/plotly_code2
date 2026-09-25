---
name: debug-wrsd-failure
description: Guide for debugging failing WRSD pipelines. Use this when asked to debug failing WRSD pipelines or when asked why a CI verfication job is failing.
---

To debug failing WRSD pipelines in a Gerrit review, follow this process. Additional documentation that may be helpful is linked in the steps below; all documentation can be found in the .github\skills\debug-wrsd-failure folder.  This skill pairs well with the gerrit-api skill, as you will likely need to query Gerrit for information about the change and the linked WRSD pipeline.

1. Ensure the user provided a link to a WRSD pipeline or to their Gerrit review. If not, ask them to provide one so you can investigate the failure.
2. Establish access to Gerrit and WRSD Pipeline Manager up front so you do not need to ask again later in the investigation.
	- Gerrit: use the gerrit-api skill to establish access and query Gerrit for change messages.
	- WRSD Pipeline Manager:
        - default to https://389177498442.awsaptiv.com/plm unless the user provides a different endpoint. Use `studio-cli` and confirm it can reach the WRSD server. Confirm the user has credentials configured for studio-cli. Derive the pipeline name and run number from the link if provided, or from the Gerrit change messages if not.
        - Refer to the studio-cli documentation for authentication and usage details: studio_command_line_interface_v2505.md
        - Refer to the WRSD YAML language reference for understanding pipeline definitions: studio_pipeline_manager_yaml_language_reference_2505.md
        - Command templates commonly used during investigation:
		    - WRSD run metadata: `studio-cli plm run get -n <pipeline> -r <run> --output json`
		    - WRSD step logs: `studio-cli plm run log -n <pipeline> -r <run> -t <task> -s <step> --quiet`
	- Do not request or store credentials in plain text. If access fails, ask the user to confirm that `.netrc` (for Gerrit) and studio-cli credentials are configured for the active environment.

3. If the user provided a Gerrit link, look through the change messages to see if there are any details about the failing pipeline. There will almost always be a link to a WRSD pipeline in the comments.  If not, then perhaps the -trigger-verificatin job is not configured with a trigger for the branch they're trying to merge into, or perhaps the job did not trigger properly. If they provided a WRSD pipeline link, you can skip this step.
3. Use studio-cli (Pipeline Manager) to determine which pipeline is failing and what the failure message is. This will give you a starting point for your investigation. If they gave you a pipeline link, you can go to that directly. If the pipeline link is to a trigger pipeline (verification/nightly/weekly/post-merge), you'll probably need to identify which child run failed. It is possible that the top-level job failed, but it is unlikely. You can get details about runs from studio-cli.
4. Look through the WRSD pipeline stages to see which stage(s), then which step(s) underneath are failing. Look at the logs for those steps to see if you can determine the cause of the failure. If the logs are not helpful, you may need to look at the code for the failing step to understand what it is doing and why it might be failing. Code for the pipelines is stored in this repository under the tools/CI/WRSD folder. If that pipeline refers to a step that isn't listed, then that step is likely defined in the ADVRADAR_CICD repo inside the WRSD/Tasks folder, or possibly the WRSD/Jobs folder. If it is not there, then you'll have to query the WRSD server directly to see how the task is defined.  You can also pull exactly the pipeline definition that ran from the WRSD server, which will show you the exact code that was executed for each step, including any parameters that were passed in. You can do this with `studio-cli plm run get -n <pipeline> -r <run> --output json` and look at the `pipelineSpec` section of the output, but this doesn't tell you where to save any modifications you made to the code for a fix, so you may need to cross reference the pipeline definition with the code in the repos to determine where to make your fix.
5. Determine whether the failure is due to WRSD infrastructure issues, pipeline errors, or code issues.  If the failure is due to infrastructure issues, replaying the job might work or you may need to escalate the issue to the CI team, and stop here.  If the failure is due to pipeline errors, you may need to fix the pipeline, or improve it to handle a new corner case behavior of the tests.  If the failure is due to code issues, you will need to fix the code to resolve the failure.
6. If it is a pipeline or code failure, try to reproduce the failure yourself in your own environment. Reproducing the issue may not be possible if the user is running in Windows, as the CI system runs on Linux. Check if those environment differences could be the cause of the failure. If you can reproduce the failure, then you can iterate more quickly on potential fixes. Don't forget to make sure you're using the same commit as the job which failed. The commit will be one of the parameters of the pipeline.
7. Fix the failing pipeline. Do not disable any CI checks or skip steps to do so.  If you were able to reproduce the failure yourself, make sure it is fixed before handing control back over to the user to verify and commit the changes.

Notes and lessons learned:
- Trigger pipelines often fail due to child pipelines. Use `studio-cli plm run get -n <trigger> -r <run>` to locate child pipeline links and their statuses first.
- Tekton timeouts like "PipelineRun failed to finish within" typically indicate infra or scheduling issues (bench reservation, queueing). Treat these as transient unless they repeat.
- An empty step log paired with a non-zero exit often indicates a download or service failure (for example, JFrog fetches). Retry or check artifact availability before changing code.
- When logs are large, pull the tail (`Select-Object -Last 200`) or filter on error keywords. If no error appears, request the full tail to confirm the real failure line.
- If `--jq` quoting is tricky in PowerShell, fall back to `studio-cli plm run get ... --output json | Select-String -Pattern "FailureCauseStep|failed to finish within|<task-name>" -Context 0,2` to surface the error payload quickly.
