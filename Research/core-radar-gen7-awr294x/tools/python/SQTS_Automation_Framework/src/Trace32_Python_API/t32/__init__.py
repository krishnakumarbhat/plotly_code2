from .rcl import Debugger, connect
from .services import (
    Address,
    AddressService,
    Breakpoint,
    BreakpointService,
    CommandService,
    FunctionService,
    MemoryService,
    PracticeMacro,
    PracticeService,
    Register,
    RegisterService,
    Symbol,
    SymbolService,
    Variable,
    VariableService,
    Trace32Logger
)

from .services.error import (
    AddressError,
    BreakpointError,
    CommandError,
    FunctionError,
    MemoryError,
    PracticeError,
    RegisterError,
    SymbolError,
    VariableError
)

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 07/27/2023  ABPA       FKU-897   Initial creation
