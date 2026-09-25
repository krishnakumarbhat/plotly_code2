# AUTOSAR Configuration

[toc]

## Getting Started
### Getting the SIP
AUTOSAR code generation requires the SIP generators to be in a known locations.
Including them in this repository would explode the size of the repository for all users, when only a small subset needs it.
It also adds significant time to clone commands for Jenkins, which is not necessary.

Bazel is used to download the files required to build the C implementaiton for non-AUTOSAR developers.

The source code (*.h, *.c) from the SIP is hosted in its own repository, which is included in this project via a git submodule for AUTOSAR development.
All other SIP files such as generatores, DaVinci binaries, documentation and demo folders are uploaded to Artifactory.

This submodule and corresponding generators/binaries are not downloaded automatically when this repository is cloned, because only those generating the AUTOSAR will need it.

If autosar development is not the intent and only want to debug/access the Vector's SIP source code, one can execute batch file "init_sip_submodule.bat" from the same place it reside in repo (software/m7/autosar) or initilize SIP sub-module as described in step 1 below.

To populate the SIP folder in single step for autosar development, batch file "init_sip_and_download_generators.bat" can be executed from the same place (software/m7/autosar). Running this batch file will basically do following two steps internally.

#### 1- Initialize the submodule
Run below command from the **root** of the repository

    git submodule update --init --progress -- software/m7/autosar/sip

#### 2- Download SIP Generators/Binaries
Download the current version of the SIP Generators (includes Generators, DaVinci binaries, Documentation, Demo etc.) from the artifactory and unzip the package directly on top of the sip folder (no nested folders).

After download, the folder tree would look like:
--sip
----| Components
----| DaVinciConfigurator
----| ...
Everything that is part of SIP generator/binaries should be added to .gitignore of the SIP repo so that it does not appear as change to git.

#### Note:
Above step will be required for every workspace you have that you plan to to generate AUTOSAR code.

If you wish, you can develop right from this submodule once cloned. To run git commands, you must be in the submodule directory though, otherwise they run on the parent project.
Otherwise, the SIP repository operates similar to this repo.

Please see the SIP repository's README for more information on committing and releasing code from the SIP repository for use by Bazel.

### Updating the SIP package
To update the commit that the SIP submodule points to, git is used. From the command line, navigate to the sip folder, then run

    git checkout <Tag>

This will move the pointer in the integration repository to the new tag, which appears as a change to software\m7\autosar\sip.
When diffed from the integration repo, you will see the commit hash for the tag you selected is now in this "file."

Lastly, commit this change in the integration repo, using the normal review process. This will now update the pointer in this repository to the new tag for everyone else.

After this is done, everyone who uses the SIP submodule will need to run

    git submodule update --init --progress -- software/m7/autosar/sip

again to get the latest commit.
Therefore, it is reccomended that you run the above command each time you plan to make changes to the AUTOSAR configuration.
