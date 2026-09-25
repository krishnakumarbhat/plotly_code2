# Core_Radar_Gen8_iND13400

## Getting Started

### STEP 0 :- Install the Python 3.10.6.exe from the same folder.

### MAIN Step :-  Clone the Generation Specific repo and Checkout to the required Commit

Clone the Main Repo: https://aptv.ghe.com/GPO/Core_Radar_Gen8_iND13400

### Step 1 :- Initialize the repo with GitHub Enterprise and Jfrog credentials

Run the Batch File : STEP1_REPO_INIT.bat
							OR
					Run the "Python repo_init.py" in Root_Directory Location(./MAIN_REPO/) of GENERATION DIR via CMD


### Step 2 :- Build the Code and get the Flashing output files

Give the build according to required variant:

Note: In present Dir Location give a Git-Bash and use following Build cmd's :-
./../../bazelisk.exe build //:gen8 --config=flr8
./../../bazelisk.exe build //:gen8 --config=srr8p
						OR
Run the Batch File : STEP2_BUILD_OUTPUT_FILES.bat


### Step 3 :- Pull the submodule repo core-radar-python-framework

Note: Run the following Git cmd in present location by giving "Open Git Bash Here" in Present Dir

Git-Bash submodule  CMD :-

Repo Link: https://aptv.ghe.com/GPO/core-radar-python-framework

git submodule update --init -- Core_Radar_Python_Framework/
							OR
Run the Batch File : STEP3_Initiate_Python_Automation_Repo.bat


### Step 4 :- Trigger the batch form following location

Run the Batch File : STEP4_Python_FM_Dependency_Installer.bat
						OR
<<<Root_Directory>>>\MAIN_REPO\tools\python\-->
\Core_Radar_Python_Framework\installation\Dependency_Installer.bat


### Step 5 :- Trigger the batch for below location to Run the framework SWE5/6

Run the Batch File : STEP5_LAUNCH.bat
					OR
<<<Root_Directory>>>\MAIN_REPO\tools\python\-->
\Core_Radar_Python_Framework\Run_Test_Cases.bat
