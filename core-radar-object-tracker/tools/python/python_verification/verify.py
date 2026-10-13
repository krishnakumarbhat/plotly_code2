"""
Verify Python 3.8 64-bit or greater is used and install pip packages.

This is a simple script to check the version of Python to ensure that it is
  both 64 bit and at least version 3.8. It also is used to update any python packages
  via Pip.
"""
import os
import sys
import subprocess
from getpass import getpass


def verifyPython():
    """
    Verify Python 3.8 64-bit or greater is being used.
    """
    if sys.maxsize + 1 == 2**31:
        bitArch = 32
    elif sys.maxsize + 1 == 2**63:
        bitArch = 64
    else:
        bitArch = "Unknown"
    if bitArch != 64 or sys.version_info < (3, 8, 0):
        sys.stderr.write(
            """
###############################################################################

        You're running this script with python {} {}-bit.

        You need at least python 3.8.x 64-bit to run this script.

        Python 3.9.7 or greater is recommended.

        Windows 7 only works with Python 3.8.

        Python can be downloaded here:
        https://www.python.org/downloads/

###############################################################################
 """.format(
                ".".join([str(x) for x in sys.version_info]), bitArch
            )
        )
        exit(1)


def updatePythonPackages(pathToRequirementsTxt=None):
    """
    Update and install Python packages using pip from a requirements.txt file.

    Args:
        pathToRequirementsTxt: Path to a requirements.txt file. Default=None
            If none is provided, the module uses the one located in the folder with the module
    """
    if pathToRequirementsTxt is None:
        pathToRequirementsTxt = os.path.join(os.path.dirname(__file__), "requirements.txt")

    try:
        subprocess.run(
            [
                sys.executable,
                "-m",
                "pip",
                "--isolated",
                "install",
                "--trusted-host",
                "pypi.org",
                "--trusted-host",
                "files.pythonhosted.org",
                "--disable-pip-version-check",
                "--no-warn-script-location",
                "-q",
                "-r",
                pathToRequirementsTxt,
            ],
            check=True,
        )
    except subprocess.CalledProcessError:
        try:
            print("\nPip install failed. Let's try again with proxy settings.")
            print("netID and Password are required, but will not be stored.\n")
            retryAttempts = 0
            maxRetries = 3
            while retryAttempts < maxRetries:
                try:
                    retryAttempts = retryAttempts + 1
                    user = input("Enter your netID: ")
                    password = getpass()
                    proxyUrl = (
                        "--proxy=http://"
                        + user
                        + ":"
                        + password
                        + "@autoproxy-amer.aptiv.com:8080"
                    )
                    subprocess.run(
                        [
                            sys.executable,
                            "-m",
                            "pip",
                            "--isolated",
                            "install",
                            proxyUrl,
                            "--trusted-host",
                            "pypi.org",
                            "--trusted-host",
                            "files.pythonhosted.org",
                            "--disable-pip-version-check",
                            "--no-warn-script-location",
                            "-q",
                            "-r",
                            pathToRequirementsTxt,
                        ],
                        check=True,
                    )
                except subprocess.CalledProcessError as e:
                    if retryAttempts < maxRetries:
                        print(
                            "PIP failed on retry attempt",
                            retryAttempts,
                            "of",
                            maxRetries,
                            ". Make sure you typed your password correctly!",
                        )
                    else:
                        raise (e)
        except subprocess.CalledProcessError as e:
            print(
                "\n\nPIP auto installer failed. Verify pip is installed via python -m pip --version."
            )
            print("Then check your password is correct.")
            print(
                "If issue persists, check if a proxy is defined in the global pip.ini file and remove it."
            )
            print(
                "If issue still persists try debugging the issue, install the packages from",
                pathToRequirementsTxt,
                "manually or contact the build team.\n\n",
            )
            raise (e)


if __name__ == "__main__":
    verifyPython()

    pathToRequirementsTxt = os.path.join(os.path.dirname(__file__), "requirements.txt")
    updatePythonPackages(pathToRequirementsTxt)

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/       JIRA AAA-####
#             Initials    Explanation of changes done here.
#    Date        By             Description
# ----------  ---------   -----------------------
# 11/13/2018  Tim B.      APS-14041  Initial creation
# 07/28/2021  Bohdan V.   EQQ-57 Enforce at least python 3.8
