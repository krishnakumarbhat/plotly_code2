---
name: gerrit-api
description: Skill for interacting with the Gerrit API. Use this skill when you need to query Gerrit for information about changes, comments, or other data related to Gerrit reviews. This skill can be used in conjunction with the debug-wrsd-failure skill when investigating failing WRSD pipelines that are linked from Gerrit reviews.
---

To query the Gerrit API, here are some rules to follow.

1. Establish access to Gerrit up front so you do not need to ask again later in the investigation.
        - default to https://gitgerrit.asux.aptiv.com/ unless the user provides a different base URL. You can query change messages using the REST API (for example, `/changes/<change>/messages`) and authenticate via `.netrc` or an API token.
        - The full API can be viewed here: https://gerrit-review.googlesource.com/Documentation/rest-api-changes.html
        - Command templates commonly used during investigation:
		   - Gerrit (messages): `curl -sS -n "https://gitgerrit.asux.aptiv.com/a/changes/<change>/messages"`
			   - Note: strip the Gerrit XSSI prefix `)]}'` before JSON parsing.
                           - Note: use `curl.exe` in Windows Powershell environments to avoid SSL issues with the built-in `curl` alias.
	- Do not request or store credentials in plain text. If access fails, ask the user to confirm that `.netrc` (for Gerrit) and studio-cli credentials are configured for the active environment.
