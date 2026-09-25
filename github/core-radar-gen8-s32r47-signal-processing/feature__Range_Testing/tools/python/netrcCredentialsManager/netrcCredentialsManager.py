"""
Package used to create a .netrc file as well as check if the credentials are valid for a given URL.

A JSON file is required to specify the credential requirements. This can either be stored alongside this script,
which is used by default, or a path can be provided to another location.

The JSON shold have the following format:
{
    "hosts": [{
        "name": ".netrc name here ie. gitgerrit.asux.aptiv.com",
        "authCheckUrl": "URL to request to which the user must have access"
    },
    {
        "name": ".netrc name 2 here",
        "authCheckUrl": "URL to request to which the user must have access"
    }],
    ...
}
"""

from multiprocessing import AuthenticationError
from tinynetrc import Netrc
from os import path
import requests
from requests.auth import HTTPBasicAuth
import json
import getpass

BASE_PATH = path.dirname(path.abspath(__file__))


class netrcCredentialsManager:
    """
    Create or update a .netrc file with some required credentials, specified in JSON format.
    """

    def __init__(self, verbose=False):
        """
        Create the object to run methods from.

        Args:
            verbose: Default - False, set to true to output more debugging information
        """
        self.verbose = verbose
        fi = path.join(path.expanduser("~"), ".netrc")
        if not path.exists(fi):
            fp = open(fi, "x")
            fp.close()
        self.localNetrc = Netrc()

    def checkAndUpdateCredentials(self, requiredCredentialsJson=None):
        """
        Check if required .netrc credentials are able to access the provided URL and request an update if not.

        Args:
             requiredCredentialsJson: Path to a JSON file which has all the required urls
                If not specified, it will use requiredCredentials.json in the folder of this script
                JSON should have the format of:
                {
                    "hosts": [{
                        "name": ".netrc name here ie. gitgerrit.asux.aptiv.com",
                        "authCheckUrl": "URL to request to which the user must have access"
                    },
                    {
                        "name": ".netrc name 2 here",
                        "authCheckUrl": "URL to request to which the user must have access"
                    }]
                }

        """
        if not requiredCredentialsJson:
            requiredCredentialsJson = path.join(BASE_PATH, "requiredCredentials.json")

        with open(requiredCredentialsJson) as readFile:
            jsonData = json.loads(readFile.read())
        for host in jsonData["hosts"]:
            authSuccessful = False
            retryCounter = 0
            while not authSuccessful and retryCounter < 5:
                try:
                    # Check that the host from the JSON file exists in the self.localNetrc
                    # The next line will cause a KeyError if it doesn't.
                    # Use this error to trigger adding it to the netrc.
                    self.localNetrc.hosts[host["name"]]
                except KeyError:
                    if self.verbose:
                        print(host["name"] + " not found. We now need to add it")
                    self._updateNetrcCredentials(host["name"])

                try:
                    user, _, password = self.localNetrc.authenticators(host["name"])
                    authSuccessful = self._checkAuthentication(
                        authCheckUrl=host["authCheckUrl"], user=user, password=password
                    )
                except AuthenticationError:
                    print(
                        "Authentication Failed! Maybe your password has changed or you typed it incorrectly?"
                    )
                    print("Let's update it and try again.")
                    self._updateNetrcCredentials(host["name"])
                    pass
                except requests.exceptions.ConnectionError:
                    if self.verbose:
                        print("Connection Failed!")
                        print(
                            "If download from "
                            + host["name"]
                            + " is required for your build, it could fail"
                        )
                    break
                if not authSuccessful:
                    retryCounter = retryCounter + 1
                else:
                    print("Authentication for " + host["name"] + " successful!")
            if retryCounter == 5:
                print("Could not verify authentication for " + host["name"])
                print(
                    "If download from "
                    + host["name"]
                    + " is required to build, the build could fail"
                )

    def _checkAuthentication(self, authCheckUrl, user, password):
        response = requests.get(authCheckUrl, auth=HTTPBasicAuth(user, password))
        if response.status_code != 200:
            raise AuthenticationError
        return True

    def _updateNetrcCredentials(self, hostName):
        print()
        print("Login information is needed to use Bazel to download dependencies.")
        print()
        print(
            "It is recommended to use an API key instead of your password otherwise your raw password will be stored in the \
            .netrc file on this machine."
        )
        print("API keys can be generated from the Web GUIs of each individual tool.")
        print(
            "See the Adv Active Safety SW/SYS Git Gerrit Wiki for instructions on generating API keys for the various tools."
        )
        print()
        print("Your password will not appear in the prompt below when typing or copy pasting.")
        print("To paste, simply right click and hit enter (Ctrl+v will not work")
        print()

        self.localNetrc[hostName] = {
            "login": input("Username for " + hostName + ": "),
            "password": getpass.getpass("API Key / password (No text will show): "),
        }
        print()
        self.localNetrc.save()


netrcCredentialsManager()
