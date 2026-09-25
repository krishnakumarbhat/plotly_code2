"""
Provides standard APIs to simplify some repetative printing.
"""


import inspect
from colorama import Fore, Style

__author__ = "Devin K. Jaenicke"
__version__ = "1.1.0"
__email__ = "devin.k.jaenicke@aptiv.com"
__copyright__ = "Copyright 2017 Aptiv, All Rights Reserved."


class Trace:
    """
    The Trace class is used to host all the print methods available from this package.
    """

    @staticmethod
    def print_error(e_message, noColor=False, exit_on_complete=False):
        """
        Print an error message using a standardized message format.

        Can be used to exit the program by setting exit_on_complete to True

        Args:
            e_message: error message to print on the console
            noColor: Default - False: Set to true if you do not want colored outputs
            exit_on_complete: Default - False: Set to true if you want the program to exit when this error is triggered
        """
        frm = inspect.stack()[1]
        mod = inspect.getmodule(frm[0])

        # Only add the module name if not called from base script
        if mod and mod.__name__ != "__main__":
            if not noColor:
                errMsg = (
                    Fore.RED
                    + Style.BRIGHT
                    + "Error: "
                    + Fore.YELLOW
                    + Style.NORMAL
                    + mod.__name__
                    + ".py"
                    + Style.RESET_ALL
                    + " -> "
                    + e_message
                )
            else:
                errMsg = "Error: " + mod.__name__ + ".py -> " + e_message
        else:
            if not noColor:
                errMsg = Fore.RED + Style.BRIGHT + "Error: " + Style.RESET_ALL + e_message
            else:
                errMsg = "Error: " + e_message

        print(errMsg)
        print(Style.RESET_ALL)

        if exit_on_complete:
            exit(1)

    @staticmethod
    def print_error_header(e_message, noColor=False, exit_on_complete=False):
        """
        Print an error message with a header using a standardized message format.

        Can be used to exit the program by setting exit_on_complete to True

        Args:
            e_message: error message to print on the console
            noColor: Default - False: Set to true if you do not want colored outputs
            exit_on_complete: Default - False: Set to true if you want the program to exit when this error is triggered
        """
        frm = inspect.stack()[1]
        mod = inspect.getmodule(frm[0])

        # Only add the module name if not called from base script
        if mod and mod.__name__ != "__main__":
            if not noColor:
                temp_string = (
                    Fore.RED
                    + Style.BRIGHT
                    + " ERROR: "
                    + mod.__name__
                    + ".py  -> "
                    + e_message
                    + " "
                    + Style.RESET_ALL
                )
            else:
                temp_string = " ERROR: " + mod.__name__ + ".py  -> " + e_message + " "
        else:
            if not noColor:
                temp_string = (
                    Fore.RED + Style.BRIGHT + " ERROR: " + e_message + " " + Style.RESET_ALL
                )
            else:
                temp_string = " ERROR: " + e_message + " "

        print()
        print(
            "********************************************************************************************"
        )
        print((temp_string.center(105, "*")))
        print(
            "********************************************************************************************"
        )
        print()
        print(Style.RESET_ALL)

        if exit_on_complete:
            exit(1)

    @staticmethod
    def print_header(message):
        """
        Print a header using a standardized message format.

        Args:
            message: message to print in the header format on the console
        """
        temp_string = " " + message + " "
        print()
        print(
            "********************************************************************************************"
        )
        print((temp_string.center(92, "*")))
        print(
            "********************************************************************************************"
        )
        print()
        print(Style.RESET_ALL)

    @staticmethod
    def print_info(i_message, noColor=False):
        """
        Print an information message using a standardized message format.

        Args:
            i_message: message to print in the information format on the console
            noColor: Default - False: Set to true if you do not want colored outputs
        """
        frm = inspect.stack()[1]
        mod = inspect.getmodule(frm[0])

        # Only add the module name if not called from base script
        if mod and mod.__name__ != "__main__":
            if not noColor:
                infoMsg = (
                    Fore.GREEN
                    + Style.BRIGHT
                    + "Info: "
                    + Style.RESET_ALL
                    + "%s.py -> %s" % (mod.__name__, i_message)
                )
            else:
                infoMsg = "Info: " + "%s.py -> %s" % (mod.__name__, i_message)
        else:
            if not noColor:
                infoMsg = Fore.GREEN + Style.BRIGHT + "Info: " + Style.RESET_ALL + i_message
            else:
                infoMsg = "Info: " + i_message

        print(infoMsg)
        print(Style.RESET_ALL)

    @staticmethod
    def print_warning(w_message, noColor=False):
        """
        Print a warning message using a standardized message format.

        Args:
            w_message: message to print in the warning format on the console
            noColor: Default - False: Set to true if you do not want colored outputs
        """
        frm = inspect.stack()[1]
        mod = inspect.getmodule(frm[0])

        # Only add the module name if not called from base script
        if mod and mod.__name__ != "__main__":
            if not noColor:
                warnMsg = (
                    Fore.MAGENTA
                    + Style.BRIGHT
                    + "Warning: "
                    + Style.RESET_ALL
                    + "%s.py -> %s" % (mod.__name__, w_message)
                )
            else:
                warnMsg = "Warning: %s.py -> %s" % (mod.__name__, w_message)
        else:
            if not noColor:
                warnMsg = Fore.MAGENTA + Style.BRIGHT + "Warning: " + Style.RESET_ALL + w_message
            else:
                warnMsg = "Warning: " + w_message
        print(warnMsg)
        print(Style.RESET_ALL)


###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/       JIRA AAA-#### for release x.x.x.
#             Initials    Explanation of changes done here.
#    Date        By             Description
# ----------  ---------   -----------------------
# 12/18/2017  Devin J.    APS-0603  Initial creation
# 12/18/2017  Devin J.    APS-0694  Added calling module to print statements
# 02/06/2018  Devin J.    APS-2366  Replaced references to delphi with aptiv
# 11/20/2018  Tim B.      APS-14367 Ensure 'mod' exists before calling it.
# 08/22/2019  Jan S.      APS-56064 Use Python3 style prints
#
###############################################################################
