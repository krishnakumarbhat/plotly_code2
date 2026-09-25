# ADVRADAR_Gen7_Exec_Spec

## Getting Started
### Getting the ADVRADAR_Gen7_Exec_Spec
Integration Testing Framework(ITF) requires ADVRADAR_Gen7_Exec_Spec for getting expected results.
Including them in this repository would explode the size of the repository for all users, when only a small subset needs it.
It also adds significant time to clone commands for Jenkins, which is not necessary for all users.

The ADVRADAR_Gen7_Exec_Spec is hosted in its own repository, which is included in this project via a git submodule for ITF development.
Repository: https://aptv.ghe.com/GPO/core-radar-gen7-exec-spec.git
This submodule is not downloaded automatically when this repository is cloned, because only those running ITF will need it.
The below command should be run from the **root** of the repository, or the path listed below can be altered to match the from the command line to the submodule.

Therefore, anyone who will need ITF will need to initialize (clone) this submodule. This can be done by running

    git submodule update --init -- documentation/ExecutableSpecs/ADVRADAR_Gen7_Exec_Spec

This step will be required for every workspace you have that you plan to run ITF.

If you wish, you can develop right from this submodule once cloned. To run git commands, you must be in the submodule directory though, otherwise they run on the parent project.
Otherwise, the ADVRADAR_Gen7_Exec_Spec repository operates similar to this repo.

### Updating the ADVRADAR_Gen7_Exec_Spec package
To update the commit that the ADVRADAR_Gen7_Exec_Spec submodule points to, git is used. From the command line, navigate to the ADVRADAR_Gen7_Exec_Spec folder, then run

    git checkout <Tag>

This will move the pointer in the integration repository to the new tag, which appears as a change to documentation\ExecutableSpecs\ADVRADAR_Gen7_Exec_Spec.
When diffed from the integration repo, you will see the commit hash for the tag you selected is now in this "file."

Lastly, commit this change in the integration repo, using the normal review process. This will now update the pointer in this repository to the new tag for everyone else.

After this is done, everyone who uses the ADVRADAR_Gen7_Exec_Spec submodule will need to run

    git submodule update --init --progress -- documentation/ExecutableSpecs/ADVRADAR_Gen7_Exec_Spec

again to get the latest commit.
Therefore, it is recomended that you run the above command each time you plan to make changes to the ADVRADAR_Gen7_Exec_Spec.
