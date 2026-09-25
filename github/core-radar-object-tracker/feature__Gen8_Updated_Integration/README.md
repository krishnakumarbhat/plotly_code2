# Core Radar Object Tracker (rotbb)

[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://github.com/pre-commit/pre-commit)

This repository stores ROT algorithm code as used by core radar projects for the embedded tracker.

[TOC]

## First Steps (repo_init.py)

### Overview

A script called repo_init.py is stored at the base of this repository.
It should be called each time this repository is cloned.

### What it does

 1. Verifies a compliant version of Python is used & downloads required python packages
 2. Installs required pre-commit hooks for this repository. These are verified via CI
 3. .netrc - creates/updates a .netrc with required credentials for building the project (See [netrc Credentials](#netrc-credentials))
 4. Tresos - Downloads and initializes Tresos. These files are stored in Artifactory, but are ignored in this repository to reduce its overall size.

### Requirements

Python v3.7 or greater should be supported.  v3.10 is recommended since this is what the scripts are tested with.

### netrc Credentials

This script *may* ask you to provide your username and API Key / password to populate a .netrc file. This file is stored locally in your machine's Home folder.

It is recommended to use an API key instead of your password otherwise your raw password will be stored in the .netrc file on this machine.
API keys can be generated from the Web GUIs of each individual tool.
See the [Adv Active Safety SW/SYS Git Gerrit Wiki](https://tinyurl.com/advSysSwGitGerritWikiApi) for instructions on generating API keys for the various tools.
**You will also need to run this script whenever your password updates (Once per user per machine) unless you use API keys**

### How to Run

To run the script, open a command prompt and run:

    python repo_init.py

#### Committing changes to the Gerrit Repository

Our Gerrit repository is guarded against changes that may break the build.
You will not be able to submit your changes if any build is broken by them.

It is also guarded against changes that do not match the formatting standards identified by a team of your peers, guided by Aptiv's coding standards.
When commiting your changes, a set of scripts will run (installed by running repo_init.py above) which should automatically format your code, and flag some potential problems.
These checks will then be run again as part of the "Verified" Jenkins job to ensure that you have the pre-commit checks in place.
This is to remove the unnecessary burden on developers to format their code in a standard way, and to make sure all our code is fomratted in the same method.

If your change fails the "Verified" check, there are 3 potential issues:

1. Your code did not build
2. Your code did not pass all the pre-commit checks
3. An unknown error occured and the CICD team needs to check what happened

If it is one of the first 2 options, it is your job to fix the issues.
If you have code that you think should *not* be required to follow the autoformatting standards, please reach out to the CICD team.
They can determine if it is a valid request and assist in excluding the files.

*Note: C Code formatters are disabled in this repository, since all code is 3rd-Party code.*

#### Testing changes locally

An integration repository may be used in conjunction with this repository.
Local development can be done using the --override_repository flag in the integration repository.

    --override_repository={rotbb_name_in_integration_repo}={my local path}

can be included on the command line when building or a user.bazelrc file to compile this package

### Releasing New rotbb for Integration

Releasing new rotbb versions is completely automated. Simply update version.yaml and complete a code review on Gerrit.
CICD scripts on dev branch must be updated for this process to work for other branches.

Below listed workspaces need to be created in integration repo where ROT building block is integrated.
Please update local repository path as per the folder structure of integration repo.

local_repository(
    name = "emb_tracker_wrapper",
    path = "software/bbe32/emb_tracker",
)

**Any update of the version.yaml file that is submitted to the dev branch automatically triggers tagging the commit and uploading a scaled-down version of this repository to Artifactory.**

Once the upload to Artifactory is complete, the commiter will get an email with details of how to use this new upload in a Bazel WORKSPACE or bb.MODULE.bazel file.

### ROT Build Options
See relevant integration repo for the relevant options.

## How does tagging and versioning works?
Refer: https://confluence.asux.aptiv.com/display/JI/Building+Blocks+Tagging+and+Versioning
