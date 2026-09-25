# Coverity
** TODO: UPDATE THIS FOR GEN8 iND13400 **

Coverity is a proprietary static code analysis tool from Synopsys. This product enables engineers and security teams to find and fix software defects.

## Configuration

### coverity_config.xml

Prior to running Coverity, configuration files need to be created for each specific compiler using the "cov-configure" command.

The commands for each supported compiler can be found here:
[Aptiv Compiler Configurations for Coverity](https://spo.aptiv.com/sites/0309-GlobalSoftwareServices/SitePages/Coverity/Coverity.aspx#aptiv-compiler-configurations-for-coverity)

The output of this command is "coverity_config.xml" and folders for each compiler with the format "template-xxx-config-0". They will be generated to C:\Coverity\cov-analysis-win64-2023.6.0\config\. Copy these files/folders to this repo at .\tools\coverity\

Below are the commands used for this AWR294x project:

    cov-configure --compiler tiarmclang --comptype clangcc --template
    cov-configure --compiler cl6x --comptype ti:cl6x --template


### cov-bazel.toml

This cov-bazel.toml file is used by the cov-cli commands.
It contains all the settings for the coverity server, coverity checkers, and targets that are to be built and analyzed.



## Coding Standard

The MISRA C and MISRA C++ standards are a set of coding guidelines for the C and C++ programing languages that promote safety, security, and reliability in embedded system software. This project is using MISRA C:2012.

### Coverity Checkers

In the cov-bazel.toml, the following checkers are used to fail a Coverity job.

All High and Medium impact issue will fail the CICD Jenkins job.

    default = "fail-on-new-medium-high-impact"


The following list of MISRA Mandatory Rules will fail the CICD Jenkins job.

    MISRA C-2012 Rule 9.1
    MISRA C-2012 Rule 12.5
    MISRA C-2012 Rule 13.6
    MISRA C-2012 Rule 17.3
    MISRA C-2012 Rule 17.4
    MISRA C-2012 Rule 17.6
    MISRA C-2012 Rule 19.1
    MISRA C-2012 Rule 21.13
    MISRA C-2012 Rule 21.17
    MISRA C-2012 Rule 21.18
    MISRA C-2012 Rule 21.19
    MISRA C-2012 Rule 21.20
    MISRA C-2012 Rule 22.2
    MISRA C-2012 Rule 22.4
    MISRA C-2012 Rule 22.5
    MISRA C-2012 Rule 22.6


The following list of MISRA Required Rules will fail the CICD Jenkins job.

    MISRA C-2012 Directive 1.1
    MISRA C-2012 Directive 2.1
    MISRA C-2012 Directive 4.1
    MISRA C-2012 Directive 4.3
    MISRA C-2012 Directive 4.10
    MISRA C-2012 Directive 4.11
    MISRA C-2012 Rule 1.1
    MISRA C-2012 Rule 1.3
    MISRA C-2012 Rule 2.1
    MISRA C-2012 Rule 2.2
    MISRA C-2012 Rule 3.1
    MISRA C-2012 Rule 3.2
    MISRA C-2012 Rule 4.1
    MISRA C-2012 Rule 5.1
    MISRA C-2012 Rule 5.2
    MISRA C-2012 Rule 5.3
    MISRA C-2012 Rule 5.4
    MISRA C-2012 Rule 5.5
    MISRA C-2012 Rule 5.8
    MISRA C-2012 Rule 6.1
    MISRA C-2012 Rule 6.2
    MISRA C-2012 Rule 7.1
    MISRA C-2012 Rule 7.2
    MISRA C-2012 Rule 7.3
    MISRA C-2012 Rule 7.4
    MISRA C-2012 Rule 8.1
    MISRA C-2012 Rule 8.2
    MISRA C-2012 Rule 8.3
    MISRA C-2012 Rule 8.4
    MISRA C-2012 Rule 8.5
    MISRA C-2012 Rule 8.6
    MISRA C-2012 Rule 8.8
    MISRA C-2012 Rule 8.10
    MISRA C-2012 Rule 8.12
    MISRA C-2012 Rule 8.14
    MISRA C-2012 Rule 9.2
    MISRA C-2012 Rule 9.3
    MISRA C-2012 Rule 9.4
    MISRA C-2012 Rule 9.5
    MISRA C-2012 Rule 11.1
    MISRA C-2012 Rule 11.2
    MISRA C-2012 Rule 11.3
    MISRA C-2012 Rule 11.6
    MISRA C-2012 Rule 11.7
    MISRA C-2012 Rule 11.8
    MISRA C-2012 Rule 11.9
    MISRA C-2012 Rule 12.2
    MISRA C-2012 Rule 13.1
    MISRA C-2012 Rule 13.2
    MISRA C-2012 Rule 13.5
    MISRA C-2012 Rule 14.1
    MISRA C-2012 Rule 14.2
    MISRA C-2012 Rule 14.4
    MISRA C-2012 Rule 15.2
    MISRA C-2012 Rule 15.3
    MISRA C-2012 Rule 15.6
    MISRA C-2012 Rule 15.7
    MISRA C-2012 Rule 16.1
    MISRA C-2012 Rule 16.2
    MISRA C-2012 Rule 16.3
    MISRA C-2012 Rule 16.4
    MISRA C-2012 Rule 16.5
    MISRA C-2012 Rule 16.6
    MISRA C-2012 Rule 16.7
    MISRA C-2012 Rule 17.2
    MISRA C-2012 Rule 18.1
    MISRA C-2012 Rule 18.2
    MISRA C-2012 Rule 18.3
    MISRA C-2012 Rule 18.6
    MISRA C-2012 Rule 18.7
    MISRA C-2012 Rule 18.8
    MISRA C-2012 Rule 20.2
    MISRA C-2012 Rule 20.3
    MISRA C-2012 Rule 20.4
    MISRA C-2012 Rule 20.6
    MISRA C-2012 Rule 20.7
    MISRA C-2012 Rule 20.8
    MISRA C-2012 Rule 20.9
    MISRA C-2012 Rule 20.11
    MISRA C-2012 Rule 20.13
    MISRA C-2012 Rule 20.14
    MISRA C-2012 Rule 21.1
    MISRA C-2012 Rule 21.2
    MISRA C-2012 Rule 21.4
    MISRA C-2012 Rule 21.7
    MISRA C-2012 Rule 21.9
    MISRA C-2012 Rule 21.11
    MISRA C-2012 Rule 21.14
    MISRA C-2012 Rule 21.15
    MISRA C-2012 Rule 21.16
    MISRA C-2012 Rule 21.21
    MISRA C-2012 Rule 22.1
    MISRA C-2012 Rule 22.3
    MISRA C-2012 Rule 22.7
    MISRA C-2012 Rule 22.8
    MISRA C-2012 Rule 22.9
    MISRA C-2012 Rule 22.10


* The above lists of MISRA Mandatory and MISRA Required rules are a full list of all the Mandatory and Required rules with the some removed per the Aptiv MISRA Standards committee. Not all deviations were used from this IDI config, as they do not apply to this project.
[GSS - Coverity - MISRA configuration files](https://spo.aptiv.com/sites/0309-GlobalSoftwareServices/SitePages/Coverity/Coverity.aspx#idi-misra-configuration-files)


## Run Coverity Locally

To run Coverity locally, open a command prompt from the \tools\coverity\ directory and run:

    python RunCoverity.py --variant srr7p --image mss

Change the variant and image as needed.

A report of found issues will be saved to \covdir\\(variant)\_misrac\Coverity_Report\_(variant)_misrac.csv


## Run Coverity in CICD

As part of the CICD environment, a Coverity build for each variant and image will be built automatically.

Result will show in the the Gerrit review and reports will be saved to Artifactory.

If any new High/Medium/Mandatory/Required issues are found, it will block the merge from being possible until the issue is resolved and the Coverity job is re-run.


## Links

[GSS - Coverity](https://spo.aptiv.com/sites/0309-GlobalSoftwareServices/SitePages/Coverity/Coverity.aspx)
