"""
Simple module used to install pre-commit on a cloned git repository.

This module simply automates running python -m pre-commit install in an OS agnostic fashion.
A .pre-commit-config.yaml file is required at the root of the repository.

See https://pre-commit.com/ for more details on pre-commit
"""

import sys
import subprocess


def installPreCommit(verbose=False):
    """
    Install the pre-commit plug-in on this repository to run standard pre-commit checks during git actions.
    """
    cmdList = [sys.executable, "-m", "pre_commit", "install"]
    P = subprocess.run(cmdList, capture_output=True)

    if P.returncode:
        print("ERROR! Installing Pre-Commit Features Failed! Try running this manually via:")
        print("python -m pre_commit install")
        if verbose:
            print("Failure Debugging Info:")
            print("cmdList = " + " ".join(map(str, cmdList)))
            print("stdout = " + P.stdout.decode("utf-8"))
            print("stderr = " + P.stderr.decode("utf-8"))
        return

    print("Pre-commit installed successfully!")
    if verbose:
        print()


if __name__ == "__main__":
    installPreCommit(verbose=False)
