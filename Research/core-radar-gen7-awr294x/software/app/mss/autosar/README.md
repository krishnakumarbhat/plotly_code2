# AUTOSAR Configuration

[toc]

## Getting Started
### Getting the SIP
AUTOSAR code generation requires the SIP generators to be in a known locations.
Including them in this repository would explode the size of the repository for all users, when only a small subset needs it.
It also adds significant time to clone commands for Jenkins, which is not necessary.

Bazel is used to download the files required to build the C implementaiton for non-AUTOSAR developers.

WThe SIP is hosted in its own repository, which is included in this project via a git submodule for AUTOSAR development.
This submodule is not downloaded automatically when this repository is cloned, because only those generating the AUTOSAR will need it.
The below command should be run from the **root** of the repository, or the path listed below can be altered to match the from the command line to the submodule.

Therefore, anyone who will generate AUTOSAR code will need to initialize (clone) this submodule. This can be done by running

    git submodule update --init --progress -- software/app/mss/autosar/sip

This step will be required for every workspace you have that you plan to to generate AUTOSAR code.

If you wish, you can develop right from this submodule once cloned. To run git commands, you must be in the submodule directory though, otherwise they run on the parent project.
Otherwise, the SIP repository operates similar to this repo.

Please see the SIP repository's README for more information on committing and releasing cpde from the SIP repository for use by Bazel.

### Updating the SIP package
To update the commit that the SIP submodule points to, git is used. From the command line, navigate to the sip folder, then run

    git checkout <Tag>

This will move the pointer in the integration repository to the new tag, which appears as a change to software\app\mss\autosar\sip.
When diffed from the integration repo, you will see the commit hash for the tag you selected is now in this "file."

Lastly, commit this change in the integration repo, using the normal review process. This will now update the pointer in this repository to the new tag for everyone else.

After this is done, everyone who uses the SIP submodule will need to run

    git submodule update --init --progress -- software/app/mss/autosar/sip

again to get the latest commit.
Therefore, it is reccomended that you run the above command each time you plan to make changes to the AUTOSAR configuration.
