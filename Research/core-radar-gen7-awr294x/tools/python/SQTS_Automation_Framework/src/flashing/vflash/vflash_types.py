"""
This module contains enums that represent results and status of operations carried out through the
vFlash C API.
"""
from enum import IntEnum


class FlashStatus(IntEnum):
    """
    Enum that represents the possible flashing status.
    """
    SUCCESS = 0  # Reprogramming successful
    LICENSE_IS_NOT_AVAILABLE = 9  # vFlash License is not available

    # Errors - call 'GetLastFlashError(...)' for details
    ABORTED = 10  # The launched action was aborted (e.g. flash process was aborted by the user)
    UNKNOWN_ERROR = 11  # Unknown error occurred
    PROCESSING_FLASHWARE_FAILED = 15  # Processing Flashware failed (e.g. due invalid format of Intel-Hex, Motorola-S files, ODX files)
    INITIALIZING_COMMUNICATION_FAILED = 16  # Initializing communication failed (e.g. due to incorrect channel assignment, incorrect hardware setup)
    STOP_REPROGRAMMING_FAILED = 17  # Reprogramming could not be aborted successfully
    PROJECT_DATA_INVALID = 18  # Invalid project data detected (e.g. invalid communication parameters, missing Flashware, missing SeedKey.dll)
    INITIALIZING_FLASH_LIBRARY_FAILED = 19  # Not in use
    SCRIPT_VERSION_MISMATCH = 20  # The version of vFlash has to be at least as current as the version of the vFlash Template
    LOADING_PROJECT_FAILED = 21  # The project could not be loaded (e.g. referenced file missing, invalid project content)
    FLASHWARE_CHANGED = 22  # The flashware changed (e.g. the referenced flashware changed during reprogramming)
    READING_EXPECTED_IDENTS_FAILED = 23  # The ODX-F ExpectedIdent data could not be read (e.g. since required ODX-D is missing)
    FORCE_BOOT_MODE_FAILED = 24  # Force Boot Mode could not be executed
    SWITCH_BAUDRATE_FAILED = 25  # Baudrate switch failed (e.g. since other application has �Init Rights� on hardware channel)
    SEND_WAKE_UP_PATTERN_FAILED = 26  # Wakeup pattern could not be send (e.g. transmitting wakeup pattern failed)

    GENERAL_SCRIPT_FAILURE = 40  # Executing reprogramming failed (e.g. transferring data failed)
    SEED_KEY_HANDLING_FAILED = 41  # SeedKey.dll could not be loaded (e.g. invalid or missing SeedKey.dll)
    SECURITY_ACCESS_FAILED = 42  # Security Access failed (e.g. unlocking ECU failed)
    COMMUNICATION_FAILED = 43  # Starting communication failed (e.g. incomplete hardware configuration setup)
    DIAGNOSTIC_TRANSACTION_FAILED = 44  # Processing Diagnostic transaction failed (e.g. transmitting diagnostic message failed, response missing)
    SOFTWARE_INTEGRITY_CHECK_FAILED = 45  # Software integrity check failed (e.g. invalid checksum)
    NEGATIVE_RESPONSE_RECEIVED = 46  # ECU send a negative response although a positive response was required
    ERASE_MEMORY_FAILED = 47  # Erase memory failed (e.g. ECU responded negatively to Erase-Routine request)
    SOFTWARE_AUTHENTICITY_CHECK_FAILED = 48  # Software authenticity check failed (e.g. invalid signature)
    SOFTWARE_COMPATIBILITY_CHECK_FAILED = 49  # Software compatibility check failed (e.g. invalid application data selected for already programmed application)
    FINGERPRINT_CHECK_FAILED = 50  # Fingerprint check failed (e.g. invalid TesterSerialNumber configured)
    HARDWARE_COMPATIBILITY_CHECK_FAILED = 51  # Hardware compatibility check failed (e.g. invalid software selected for hardware platform)
    PROGRAMMING_PRECONDITIONS_CHECK_FAILED = 52  # Preconditions are not fulfilled (e.g. since the user tried to reprogram an engine ECU while the engine is running)
    INVALID_PARAMETER_DETECTED = 53  # The value of a FlashAttribute is missing or invalid

    GENERAL_CUSTOM_ACTION_FAILURE = 60  # Custom Action could not be executed
    CUSTOM_ACTION_ATTRIBUTE_CONVERSION_FAILED = 61  # Custom Action Attribute could not be converted (e.g. string can not be converted into an integer)

    REPORTING_FAILURE = 80  # Reporting cannot be execute (e.g. could not create defined report file)


class FlashResult(IntEnum):
    """
    Enum that represents the possible results of executing vFlash operations.
    """
    SUCCESS = 0  # Okay result

    INITIALIZATION_ERROR = 10  # Project could not be initialized (e.g. vFlash is not installed on the machine
    DEINITIALIZATION_ERROR = 11  # deprecated / not in use
    INVALID_PROJECT_HANDLE = 12  # Project handle is not valid.
    INVALID_LICENSE = 13  # License check failed. (e.g. vFlash license is not valid)

    STOP_FAILED = 20  # Stop failed.
    PROJECT_IN_PROCESSING = 21  # deprecated / not in use

    NO_PROJECT_LOADED = 30  # Project is not loaded.
    PROJECT_PATH_INVALID = 31  # Path to the project file is invalid (e.g. the project file does not exist )
    INCONSISTENT_PROJECT = 32  # The project could not be loaded (e.g. referenced file missing, invalid project content, invalid project path)
    INVALID_CALLBACKS = 33  # Not in use
    PROCESSING_FLASHWARE_ERROR = 34  # Flashware (e.g. Intel-Hex, Motorola-S files, ODX files) cannot be processed
    CHANNEL_ALREADY_IN_USE = 35  # Hardware channel already in use (e.g. incorrect hardware configuration setup)
    NETWORK_ACTIVATION_FAILED = 36  # Activate Network failed (e.g. incorrect hardware configuration setup)
    CHANNEL_INVALID = 37  # Hardware channel is not assigned (e.g. incorrect hardware configuration setu
    NO_CAN_DRIVER_AVAILABLE = 40  # CAN Driver could not be found (e.g. check whether vector driver is installed)

    INVALID_COMMAND_ORDER = 50  # Command order is incorrect (e.g. initialization is required before loading a project)
    FILE_NOT_FOUND = 51  # The project file or files referenced in a project could not be loaded (e.g. referenced file missing, invalid project content, invalid project path)
    PROJECT_LOAD_FAILED = 52  # The project could not be loaded (e.g. referenced file missing, invalid project content)
    PROJECT_UNLOAD_FAILED = 53  # The project could not be unloaded
    MAX_PROJECT_NUMBER_EXCEEDED = 54  # Limit of project number exceeded (e.g. more than 8 projects can not be loaded)

    ATTRIBUTE_NOT_FOUND = 55  # Flash attribute could not be found (e.g. incorrect attribute name)
    ATTRIBUTE_CONVERSION_FAILED = 56  # Flash attribute could not be converted (e.g. string can not be converted into an integer)

    TEMPLATE_NOT_FOUND = 57  # Accessing the template failed (e.g. when trying to read the template version)
    TEMPLATE_PROPERTY_NOT_FOUND = 58  # Accessing template property (e.g. template name) failed

    DATABLOCK_NOT_FOUND = 59  # Accessing a datablock (e.g. when trying to read a datablock file path) failed
    DATABLOCK_PROPERTY_NOT_FOUND = 60  # Accessing a datablock property (e.g. TYPE) failed

    INVALID_USE_CASE = 70  # A native-only operation is called with a ODX-F vFlashpack.
    INVALID_COM_PARAM = 71  # A communication parameter which is invalid for the current bus system is accessed (e.g. trying to get or set P2/P2* for DoIP, get or set P6/P6* for CAN, FlexRay, LIN)
    INVALID_BUS_TYPE = 72  # No valid bus type found (Expected: CAN, LIN, DoIP, FlexRay).

    UNKNOWN_ERROR = 100


class FlashReportingOptions(IntEnum):
    """
    Options for the flashing report.
    """
    # By default the following settings apply
    # Flash scripts and Custom Action scripts may write into the report.
    # When the same report file path is used, the report is overwritten.
    # The script may change the report path.
    DEFAULT = 0
    DISABLE_FLASH_SCRIPT_REPORTING = 1  # Calls to reporting methods in Flash scripts are ignored.
    DISABLE_CUSTOM_ACTION_REPORTING = 2  # Calls to reporting methods in Custom Action scripts are ignored.
    DENY_OVERWRITING_REPORT_PATH_IN_SCRIPT = 4  # Calls to overwrite the report path in scripts are ignored.
    APPEND_TO_EXISTING_REPORT = 8  # If the report file already exists, the report is not overwritten but appended.


###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 05/09/2023  ABPA       FKU-888   Initial creation
