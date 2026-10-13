# flake8: noqa: E402
from os import chdir, getcwd, path
import sys

BASE_PATH = path.dirname(path.abspath(__file__))
REPO_ROOT = path.abspath(path.join(BASE_PATH))
PYTHON_SCRIPTS_DIR = path.abspath(path.join(REPO_ROOT, "tools", "python"))

sys.path.insert(0, path.abspath(path.join(PYTHON_SCRIPTS_DIR, "python_verification")))

# Check Python version and pip packages before importing anything else
import verify

verify.verifyPython()
verify.updatePythonPackages()

import time
import argparse
import colorama
from colorama import Fore, Style

#####################################
# CUSTOM LIBRARIES
#####################################
# Add buildPaths to the scripts
sys.path.insert(0, path.abspath(path.join(PYTHON_SCRIPTS_DIR, "netrcCredentialsManager")))
sys.path.insert(0, path.abspath(path.join(PYTHON_SCRIPTS_DIR, "installPreCommit")))

# Import the custom libaraires
from installPreCommit import installPreCommit
from netrcCredentialsManager import netrcCredentialsManager


class elapsedTime:
    def __init__(self, noColor):
        self.startTime = time.time()
        self.noColor = noColor

    def printElapsedTime(self):
        if not self.noColor:
            elapsedTimeMsg = (
                Fore.CYAN
                + Style.BRIGHT
                + "Time Elapsed: "
                + Style.RESET_ALL
                + str(round(time.time() - self.startTime))
                + " Seconds"
            )
        else:
            elapsedTimeMsg = (
                "Time Elapsed: " + str(round(time.time() - self.startTime)) + " Seconds"
            )
        print(elapsedTimeMsg)


#####################################
# Script begins here
#####################################
if __name__ == "__main__":
    colorama.init(autoreset=True)

    parser = argparse.ArgumentParser(formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument(
        "--verbose",
        action="store_true",
        default=False,
        dest="verbose",
        help="Print more verbose statements durning the build",
    )
    parser.add_argument(
        "--nocolor",
        action="store_true",
        default=False,
        dest="nocolor",
        help="Turn off coloring rules on the command line",
    )
    cmdLineArgs = parser.parse_args()

    elapsedTimeObj = elapsedTime(cmdLineArgs.nocolor)
    startDir = getcwd()

    scriptPath = path.realpath(__file__)
    scriptDir = path.dirname(scriptPath)

    chdir(scriptDir)
    # Install the pre-commit plug-in on this git repo
    # in order to run standard pre-commit checks.
    if cmdLineArgs.verbose:
        print("Installing Pre-Commit Hooks...")
    installPreCommit(verbose=cmdLineArgs.verbose)

    # Check that .netrc has the required credentials for the build
    if cmdLineArgs.verbose:
        print("Checking for .netrc file...")
    credManager = netrcCredentialsManager(verbose=cmdLineArgs.verbose)
    credManager.checkAndUpdateCredentials()
    if cmdLineArgs.verbose:
        print(".netrc file found and verified!\n")

    chdir(scriptDir)

    elapsedTimeObj.printElapsedTime()
