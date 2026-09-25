# Core_Radar_Gen7_SAF85xx

## Getting Started


### MAIN Step :-  Clone the Gen7v2 repo and Checkout to the required Commit

Clone the GEN7_V2 Repo ( https://gitgerrit.asux.aptiv.com/admin/repos/Core_Radar_Gen7_SAF85xx )

### Step 1 :- Initialize the repo with gitgerrit and Jfrog credentials

Run the Batch File : STEP1_REPO_INIT.bat
							OR
					Run the "Python repo_init.py" in Root_Directory Location(./Core_Radar_Gen7_SAF85xx/) of GEN7_V2 DIR via CMD


### Step 2 :- Build the Code and get the Flashing output files

Give the build according to required variant:

Note: In present Dir Location give a Git-Bash and use following Build cmd's :-
./../../bazelisk.exe build //:gen7 --config=srr7e --micro_revision=ES2 --board=A1
./../../bazelisk.exe build //:gen7 --config=flr7 --micro_revision=ES2 --board=A1
./../../bazelisk.exe build //:gen7 --config=srr7p --micro_revision=ES2 --board=A1
						OR
Run the Batch File : STEP2_BUILD_OUTPUT_FILES.bat


### Step 3 :- Pull the submodule repo from Core_Radar_Gen7_SAF85xx_Python_Framework

Note: Run the following Git cmd in present location by giving "Open Git Bash Here" in Present Dir

Git-Bash submodule  CMD :-

git submodule update --init -- Core_Radar_Gen7_SAF85xx_Python_Framework/
							OR
Run the Batch File : STEP3_Initiate_Python_Automation_Repo.bat


### Step 4 :- Trigger the batch form following location

Run the Batch File : STEP4_Python_FM_Dependency_Installer.bat
						OR
<<<Root_Directory>>>\Core_Radar_Gen7_SAF85xx\tools\python\-->
\Core_Radar_Gen7_SAF85xx_Python_Framework\installation\Dependency_Installer.bat


### Step 5 :- Trigger the batch for below location to Run the framework SWE5/6

Run the Batch File : STEP5_RUN_TEST_CASES.bat
					OR
<<<Root_Directory>>>\Core_Radar_Gen7_SAF85xx\tools\python\-->
\Core_Radar_Gen7_SAF85xx_Python_Framework\Run_Test_Cases.bat
