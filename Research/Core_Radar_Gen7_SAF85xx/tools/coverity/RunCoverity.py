"""
Run Coverity Build and Analyze to produce Coverity results.

This Python module performs the following:
 - It runs the build and analyze commands to get Coverity results

Requirements:
 - It assumes that Coverity is installed at C:/Coverity/xxx
 - It assumes that this same path C:/Coverity/xxx/bin is in the PATH Enviornment variable
 - It assumes that the path to Bazel is in the PATH Enviornment variable
   - C:/bazel/downloads/bazelbuild/bazel-5.1.1-windows-x86_64/bin
 - Cov-cli library will need to be installed. This script will atempt to do that.

Inputs:
  - variant: srr7e, srr7hd
  - image: m7, a53
Output:
  - covdir/(variant)_(image)_misrac/Coverity_Report_(variant)_(image)_misrac.csv

Example usage:
 -> Python RunCoverity.py -v srr7p -i m7

  pip install cov-cli==1.16.0
  pip install cov-cli>=2
"""

import os

# from importlib.metadata import distributions
import re
import argparse
import time
import subprocess
import sys


class CovBazel:
    """
    Run Coverity and provide results with the CovBazel class.
    """

    def __init__(self, targetName, variant, username=None, jobs=12):
        """
        Create all targets for the created variant, and setup required authentication.

        Args:
            targetName: m7 & a53 are supported
            variant: variant to build for (ie. srr7e, srr7hd, etc.)
            username: Coverity username to use. Default=None
                Default gets system username
            jobs: Number of concurrent processes for Coverity to spawn. Default=12
        """
        self.workspace = os.getcwd()
        if username is None:
            self.username = os.getlogin()
        else:
            self.username = username
        self.jobs = jobs

        self.targetName = targetName.lower()
        if self.targetName == "m7":
            self.target = variant + "_m7_misrac"
            self.buildTarget = "//software/m7:m7 --config=" + variant
        elif self.targetName == "a53":
            self.target = variant + "_a53_misrac"
            self.buildTarget = "//software/a53:a53 --config=" + variant
        elif self.targetName == "bbe32":
            self.target = variant + "_bbe32_misrac"
            self.buildTarget = "//software/bbe32:bbe32"
            # self.buildTarget = "bazel //software/bbe32:bbe32 --config=" + variant
            # self.buildTarget = "bazelisk build //software/bbe32:bbe32" # --config=" + variant
        else:
            print("Unknown Target: " + targetName)

    def login(self):
        """
        Authenticate cov-cli.
        """
        os.system("cov-cli login --user {0}".format(self.username))

    def build(self):
        """
        Build target with cov-cli.
        """
        print("--------------------------")
        print("cov-cli build {0} --dir=covdir/{1}".format(self.buildTarget, self.target))
        print("--------------------------")

        os.system("cov-cli build {0} --dir=covdir/{1}".format(self.buildTarget, self.target))

    def clean(self):
        """
        Clean workspace with cov-cli.
        """
        os.system("cov-cli clean --expunge")

    def analyze(self):
        """
        Run Coverity analysis with cov-cli.
        """
        if not os.path.exists("covdir"):
            os.makedirs("covdir")

        os.system(
            "cov-cli analyze -j {0} -t {1} --dir=covdir/{1} --allow-unmerged-emits".format(
                self.jobs, self.target
            )
        )
        os.system(
            "cov-cli defects-save -t {0} covdir/{0}/Coverity_Report_{0}.csv --dir=covdir/{0}".format(
                self.target
            )
        )

    def setup(self):
        """
        Check that cov-cli is correctly configured to run Coverity.

        1. Check that Coverity is installed
        2. Check that cov-cli is installed. If not, install using pip
        3. Check if a coverity.auth file exists. If not, create it.
        """
        print("\n*******************************")
        # check for coverity
        if os.path.isdir("C:\\Coverity"):
            PATH = os.getenv("PATH")
            if PATH != "":
                for i in PATH.split(";"):
                    if "Coverity" in i:
                        print("Coverity version: " + re.search(r"Coverity\\(.*)\\bin", i).group(1))
                        break
        else:
            print("*** Coverity is not installed ***")
            return 0

        # check for cov-cli library
        required = {"cov_cli"}
        installed = self.__get_pip_list()

        missing = required - installed

        if missing:
            # "Downloading and installing cov-cli..."
            cov = '"git+https://gitgerrit.asux.aptiv.com/cov-cli.git@master"'
            os.system(
                "python -m pip install --trusted-host pypi.org --trusted-host files.pythonhosted.org -U {0}".format(
                    cov
                )
            )
        else:
            os.system("pip freeze | findstr cov-cli")

        # Bazel version
        # os.system("bazelisk version | findstr Bazelisk")
        # os.system("bazelisk version | findstr label")
        print("*******************************\n")

        if not os.path.exists("C:\\Users\\{}\\coverity.auth".format(self.username)):
            self.login()

        return 1

    def __get_pip_list(self):
        """Retrieve a set of installed package names using pip list."""
        result = subprocess.run(
            [sys.executable, "-m", "pip", "list"], stdout=subprocess.PIPE, text=True
        )
        lines = result.stdout.split("\n")
        packages = {line.split()[0] for line in lines if line and not line.startswith("Package")}
        return packages


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Provide inputs for Coverity analysis")
    parser.add_argument(
        "-v",
        "--variant",
        dest="variant",
        action="store",
        required=True,
        help="Provide the variant to be analyzed (srr7p, srr7hd)",
    )
    parser.add_argument(
        "-i",
        "--image",
        dest="image",
        action="store",
        required=True,
        help="Provide the image to be analyzed (m7,a53)",
    )

    args = parser.parse_args()

    cov_bazel = CovBazel(args.image, args.variant)
    if cov_bazel.setup():
        start_time = time.time()
        cov_bazel.clean()
        cov_bazel.build()
        cov_bazel.analyze()
        end_time = time.time()

        print("\n-------------------------------------")
        print(
            time.strftime(
                "Coverity Build & Analyze took %Mm%Ss",
                time.gmtime(end_time - start_time),
            )
        )
        print("-------------------------------------")
