"""
This module contains the main errors that can occur when using the methods of
t32 classes.
"""


class T32Error(Exception):
    """Base exception class for Trace32 errors"""


class AddressError(T32Error):
    ...


class BreakpointError(T32Error):
    ...


class CommandError(T32Error):
    ...


class FunctionError(T32Error):
    ...


class MemoryError(T32Error):
    ...


class PracticeError(T32Error):
    ...


class RegisterError(T32Error):
    ...


class SymbolError(T32Error):
    ...


class VariableError(T32Error):
    ...

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 07/27/2023  ABPA       FKU-897   Initial creation
