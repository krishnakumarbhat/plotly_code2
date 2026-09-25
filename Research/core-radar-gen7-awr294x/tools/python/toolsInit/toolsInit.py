"""
Download and initialize some common tools used by this repository.

This repository has some large tools that should not be stored in the repo due to size concerns.
This module can be used to download them from a static URL (such as Artifactory or SharePoint) and
initialize them in tool specific ways.
"""
# Import Statements
from genericpath import isdir
import os
from os import makedirs, path
import stat
from shutil import rmtree
import subprocess
import argparse
import zipfile
from artifactory import ArtifactoryPath
from requests.exceptions import HTTPError


def remove_readonly(func, path, _):
    """Clear the readonly bit and reattempt the removal."""
    os.chmod(path, stat.S_IWRITE)
    func(path)


class tool:
    """
    Create a tool object that can be used to download and initialize the tool.
    """

    def __init__(
        self,
        name,
        downloadUrl,
        installDir,
        filename,
        downloadPath=None,
        unzipFile=False,
        runInitCmd=False,
        initCmd=None,
    ):
        """
        Create a tool object that can be downloaded, unzipped, and/or installed.

        Args:
            name: Name of the tool
            downloadUrl: URL from where the tool can be downloaded from
            installDir: The directory where the tool should be installed or unzipped at
            filename: The filename of the resulting download
            downloadPath: The path to download the tool to - Default: None - uses a temp directory in the folder of this module
            unzipFile: Whether the downloaded file needs to be unzipped - Default: False
            runInitCmd: Whether to run an init command on the downloaded file. This would happen after unzip, if both are True. Default: False
            initCmd: A list of strings that should be run to initialized the tool when runInitCmd is True Default: None
        """
        self.name = name
        self.downloadUrl = downloadUrl
        self.filename = filename
        self.downloadPath = downloadPath
        self.installDir = installDir
        self.unzipFile = unzipFile
        self.runInitCmd = runInitCmd
        if initCmd is None:
            self.initCmd = []
        else:
            assert isinstance(initCmd, list)
            self.initCmd = initCmd

    def initTool(self):
        """
        Init the tool by downloading it and unzipping it and/or running a init command.
        """
        print("Attempting to initialize " + self.name + "...")
        download = downloadFromArtifactory(self.downloadUrl, self.filename)
        downloadPath = download.downloadFile()

        if self.unzipFile:
            if not path.isdir(self.installDir):
                makedirs(self.installDir)
            self.unzipZipFile(downloadPath, self.installDir)

        if self.runInitCmd and self.initCmd:
            cmd = [downloadPath] + self.installerCommand
            subprocess.run(cmd)

        download.deleteDownloadFile()
        print(self.name + " initialized!")

    def unzipZipFile(self, zipLocation, unzipPath):
        """
        Unzip a file into a provided location.

        Args:
            zipLocation: Location of a zip file to unzip
            unzipPath: Path to unzip the zip file to
        """
        with zipfile.ZipFile(zipLocation, "r") as zip_ref:
            zip_ref.extractall(unzipPath)


class downloadFromArtifactory:
    """
    Download a file from artifactory from a provided link and authentication.

    Can also delete the downloaded file if needed once the downloaded file is used in another way.
    """

    def __init__(self, downloadUrl, fileName, pathToDownloadDir=None):
        """
        Define where to download the file from, where to download it to, and the authentication to use.

        Args:
            downloadUrl: The URL to download from
            fileName: The filename to save the download to
            pathToDownloadDir: Where to store the downloaded file. Default: None - downloads to a temp folder alongside this module
        """
        self.downloadUrl = downloadUrl
        self.fileName = fileName
        if pathToDownloadDir:
            self.pathToDownloadDir = path.join(pathToDownloadDir, fileName)
        else:
            self.pathToDownloadDir = os.path.join(
                os.path.dirname(__file__), "artifactoryDownloads", self.fileName
            )

        if not os.path.isdir(os.path.dirname(self.pathToDownloadDir)):
            os.makedirs(os.path.dirname(self.pathToDownloadDir))

        self.__checkAuthentication()

    # download and use custom print function
    def __print_download_status(self, bytes_now, total):
        """
        Print the current download progress.

        Custom function that accepts first two arguments as [int, int] in its signature.
        """
        # print("Downloaded ", bytes_now / 1024, " / ", round(total / 1024, 1), " KB", end='\r')
        print("Downloaded ", bytes_now, " / ", total, " Bytes", end="\r")

    def downloadFile(self):
        """
        Download a file from the web using curl.
        """
        urlPath = ArtifactoryPath(self.downloadUrl)
        urlPath.writeto(
            out=self.pathToDownloadDir,
            chunk_size=1048576 * 10,  # Download 10Mb at at time
            progress_func=lambda x, y: self.__print_download_status(x, y),
        )
        print("\nDownload Complete!")
        return self.pathToDownloadDir

    def deleteDownloadFile(self):
        """
        Delete the file that was downloaded.
        """
        if os.path.isfile(self.pathToDownloadDir):
            os.remove(self.pathToDownloadDir)

    def __checkAuthentication(self):
        """
        Check that a .netrc file exists can log in to JFrog.
        """
        try:
            url = self.downloadUrl[: self.downloadUrl.rfind("/")]
            urlPath = ArtifactoryPath(url)
            urlPath.touch()
        except HTTPError:
            raise ValueError(
                "Authentication not successful! Please run repo_init.py at the root of the repo"
            )


# Function Definitions
class toolsInit:
    """
    Initialize all added tools in a common way.

    Tool specific initializations can be added as needed, if basic steps are not enough.

    Tools supported nativley are:
        - Tresos
        - Trace32
    """

    def __init__(self, repoRoot):
        """
        Init the toolsInit class by providing authentication and a path to the root of the repo.

        Args:
            repoRoot: Path to the root of the repo
        """
        self.repoRootPath = (
            repoRoot  # This is the relative path from the pwd to the repository root
        )
        self.pathToToolsDir = path.join(self.repoRootPath, "tools")

        self.tools = []

    def addTool(self, toolAddition):
        """
        Add a tool to the toolInit object to download and initialize.
        """
        if isinstance(toolAddition, tool):
            self.tools.append(toolAddition)
        else:
            print("Input was not of the tool class type and was not added!")

    def resetTools(self):
        """
        Reset tools to [], if for some reason you need it.
        """
        self.tools = []

    def initTools(self):
        """
        Init all tools that were added to this object.
        """
        for toolInit in self.tools:
            toolInit.initTool()

    def updatePathToRepoRoot(self, pathToRepoRoot):
        """
        Update the path to the root of the repository, if required.

        When the class is initialized, it assumes the class is run from the location
        that the script is stored. If the class is being exectued from another directory,
        the path to the repository root needs to be updated.

        The path can either be a relative path from the pwd to the repository root or it
        can be an absolute path to the repository root.
        """
        self.repoRootPath = pathToRepoRoot

    def addTresosTool(self, build_type=None):
        """
        Add the Tresos tool to this repository.

        This is a tool specific funciton and makes some assumptions specific to the ADP Gen 7 repo.
        Use with caution outside of the ADP Gen 7 repo.

        Args:
            build_type: The build type ('eco' or 'default'). If None, the user will be prompted.
        """
        # Define Tresos download URLs for different build types
        tresos_urls = {
            "default": "https://jfrog.asux.aptiv.com/artifactory/gen7-aptiv-advradar-3rd_party-generic-local/AWR294x/tresos/2.0.5/tresos-02_00_05_03.zip",  # noqa: E501
            "eco": "https://jfrog.asux.aptiv.com/artifactory/gen7-aptiv-advradar-3rd_party-generic-local/AWR2x44p/tresos/2.2.6/tresos-02_02_06_00.zip",  # noqa: E501
        }

        # Prompt user if build_type is not provided
        if build_type is None:
            print("\nSelect Tresos build type:")
            print("1. Default (AWR294x - v2.0.5)")
            print("2. ECO (AWR2x44p - v2.2.6)")

            choice = None
            while choice is None:
                user_input = input("Enter your choice (1/2): ").strip()
                if user_input == "1" or user_input.lower() == "default":
                    choice = "default"
                elif user_input == "2" or user_input.lower() == "eco":
                    choice = "eco"
                else:
                    print("Invalid input! Please enter 1 or 2.")
            build_type = choice

        # Validate build_type and get the appropriate URL
        if build_type.lower() not in tresos_urls:
            print(f"Invalid build type: {build_type}. Using 'default'.")
            build_type = "default"

        download_url = tresos_urls[build_type.lower()]
        print(f"\nDownloading Tresos for {build_type.upper()} build...")

        tresosInstallDir = path.join(self.pathToToolsDir, "tresos")
        tresosTool = tool(
            "tresos",
            download_url,
            self.pathToToolsDir,
            "tresos.zip",
            unzipFile=True,
        )
        self.addTool(tresosTool)
        if isdir(tresosInstallDir):
            rmtree(tresosInstallDir)

    def addTrace32Tool(self):
        """
        Installs the Trace tool to the C drive.

        This is a tool specific funciton and makes some assumptions specific to the ADP Gen 7 repo.
        Use with caution outside of the ADP Gen 7 repo.
        """
        t32InstallDir = "C:/T32"
        trace32Tool = tool(
            "trace32",
            "https://jfrog.asux.aptiv.com/artifactory/core_radar-aptiv-00000000-common_3rd_party-local/trace32/T32_2023_02_159199.zip",  # noqa: E501
            t32InstallDir,
            "t32.zip",
            unzipFile=True,
        )

        if isdir(t32InstallDir):
            print(
                "{} will be removed before initializing Trace32, removing any previous installation of T32!".format(
                    t32InstallDir
                )
            )
            addT32 = None
            while addT32 is None:
                choice = input("Continue? (y/n): ").lower()
                if choice == "y" or choice == "yes":
                    addT32 = True
                elif choice == "n" or choice == "no":
                    addT32 = False
                else:
                    print("Unknown input of {}. Please input y or n!".format(choice))
            if addT32:
                try:
                    rmtree(t32InstallDir, onerror=remove_readonly)
                except PermissionError:
                    print(
                        "Could not remove old Trace32 Installation at {}! Please remove this manually and run again.".format(
                            t32InstallDir
                        )
                    )
                self.addTool(trace32Tool)
        else:
            self.addTool(trace32Tool)

    def addPathsToGitignore(self, pathsList):
        """
        Add paths to .gitignore in an automated fashion with a provided list of paths.

        Args:
            pathsList: A List of paths to add to .gitignore
        """
        # Read current .gitignore paths
        with open(self.repoRootPath + ".gitignore") as fi:
            gitignoreLines = fi.readlines()
            gitignoreLines = [line.rstrip() for line in gitignoreLines]

        with open(self.repoRootPath + ".gitignore", "a") as fi:
            for repoPath in pathsList:
                # Remove relative path to the repository root
                if repoPath.startswith(self.repoRootPath):
                    repoPath = repoPath[len(self.repoRootPath) :]
                if repoPath not in gitignoreLines:
                    # Write each new path on a new line
                    fi.write("\n" + repoPath)
                    print(repoPath + " added to .gitignore")


# Current implemenatation relies on relative paths.
# Future improvement could be to set up a path manager and use
# absolute paths which could prevent the need to switch directories.
if __name__ == "__main__":
    origPwd = os.getcwd()
    os.chdir(os.path.dirname(__file__))
    fileDir = path.dirname(os.path.dirname(__file__))

    rootDir = path.normpath(
        path.join(
            fileDir,
            "..",
            "..",
        )
    )

    parser = argparse.ArgumentParser(
        description="Provide inputs to they Python script on which to. \n \
        If no tool is provided, all tools in the script will be initialized."
    )
    parser.add_argument(
        "--tresos", action="store_true", help="Tells the script to initialize the Tresos tool"
    )
    parser.add_argument(
        "--trace32", action="store_true", help="Tells the script to initialize the Trace32 tool"
    )
    parser.add_argument(
        "--build_type",
        choices=["default", "eco"],
        help="Specify the build type for Tresos: 'default' (AWR294x) or 'eco' (AWR2x44p). If not specified, you will be prompted.",
    )

    args = parser.parse_args()

    # If no specific tool is provided, download both tools.
    if not args.tresos and not args.trace32:
        args.tresos = True
        args.trace32 = True

    toolsInitObj = toolsInit(rootDir)

    if args.tresos:
        toolsInitObj.addTresosTool(build_type=args.build_type)
    if args.trace32:
        toolsInitObj.addTrace32Tool()

    if toolsInitObj.tools:
        toolsInitObj.initTools()

    os.chdir(origPwd)
