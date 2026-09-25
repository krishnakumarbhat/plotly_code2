"""

This script queries Bazel to see which files are built, which files are tested, and calculates the difference between them.

"""

import subprocess
import sys

IGNORE_LIST = [
    "@saf85xx_autosar_sip",
    "//software/m7/autosar/config/Appl/GenData:",
]


class targetQuery:
    """
    Query Bazel about a given build target.
    """

    def __init__(self, target, flags):
        """
        Perform the initial query and save the data to internal class variables.
        """
        try:
            query = subprocess.run(
                [
                    "bazel",
                    "cquery",
                    "--noimplicit_deps",
                    'kind("source", filter(".*\\.(c|cpp|h)$", deps("{0}")))'.format(target),
                ]
                + flags,
                capture_output=True,
                check=True,
            )
            self.fileList = query.stdout.decode(sys.stdout.encoding).splitlines(keepends=True)
            self.fileCount = len(self.fileList)
        except subprocess.CalledProcessError as e:
            print("Bazel query did not run successfully")
            print("Error: {}".format(e))
            print("stderr: {}".format(e.stderr))
            print("stdout: {}".format(e.stdout))
            raise e


def analyzeTestingCoverage(build_target, build_flags, ut_target, ut_flags, name):
    """
    Perform the query, analysis, and write the outputs to a file.
    """
    tq = targetQuery(build_target, build_flags)
    ut = targetQuery(ut_target, ut_flags)
    # compare their contents to make sure that everything in the build output exists in the UT file (there will be extras in the UT file)
    missedList = []
    for file in tq.fileList:
        skip = False
        for i in IGNORE_LIST:
            if i in file:
                skip = True
                break
        if file not in ut.fileList and not skip:
            missedList.append(file)
    missedList.sort()
    # Generate and report metrics on missing files.
    print("{0} files not tested out of {1} total files".format(len(missedList), tq.fileCount))
    with open("{0}_misses.txt".format(name), "w") as f:
        f.writelines(missedList)
    return missedList


if __name__ == "__main__":
    """
    Analyse the primary build targets and UT targets to determine if we're covering all of our files with UT cases.
    """
    # invoke bazel to find all the files it builds, have it pipe the output to a file
    gen7_misses = analyzeTestingCoverage(
        "//:gen7",
        ["--config=srr7e"],
        "//:all_unit_tests",
        ["--config=bbe_ut", "--variant=srr7e"],
        "gen7_srr7e",
    )
    gen7_misses = analyzeTestingCoverage(
        "//:gen7",
        ["--config=srr7p"],
        "//:all_unit_tests",
        ["--config=bbe_ut", "--variant=srr7p"],
        "gen7_srr7p",
    )
    gen7_misses = analyzeTestingCoverage(
        "//:gen7",
        ["--config=flr7"],
        "//:all_unit_tests",
        ["--config=bbe_ut", "--variant=flr7"],
        "gen7_flr7",
    )

    print("Analysis complete")
