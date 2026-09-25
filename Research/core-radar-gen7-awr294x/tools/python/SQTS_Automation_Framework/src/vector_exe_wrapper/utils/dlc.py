"""
This module provides utility functions to translate between a message's DLC and data length.
"""

DLC_TO_DLF = {
    0: 0,
    1: 1,
    2: 2,
    3: 3,
    4: 4,
    5: 5,
    6: 6,
    7: 7,
    8: 8,
    9: 12,
    10: 16,
    11: 20,
    12: 24,
    13: 32,
    14: 48,
    15: 64
}

DLF_TO_DLC = {
    0: 0,
    1: 1,
    2: 2,
    3: 3,
    4: 4,
    5: 5,
    6: 6,
    7: 7,
    8: 8,
    9: 9,
    10: 9,
    11: 9,
    12: 9,
    13: 10,
    14: 10,
    15: 10,
    16: 10,
    17: 11,
    18: 11,
    19: 11,
    20: 11,
    21: 12,
    22: 12,
    23: 12,
    24: 12,
    25: 13,
    26: 13,
    27: 13,
    28: 13,
    29: 13,
    30: 13,
    31: 13,
    32: 13,
    33: 14,
    34: 14,
    35: 14,
    36: 14,
    37: 14,
    38: 14,
    39: 14,
    40: 14,
    41: 14,
    42: 14,
    43: 14,
    44: 14,
    45: 14,
    46: 14,
    47: 14,
    48: 14,
    49: 15,
    50: 15,
    51: 15,
    52: 15,
    53: 15,
    54: 15,
    55: 15,
    56: 15,
    57: 15,
    58: 15,
    59: 15,
    60: 15,
    61: 15,
    62: 15,
    63: 15,
    64: 15
}


def dlc_to_data_length(dlc: int) -> int:
    """
    Utility function to translate from message DLC to message data length.

    Args:
        dlc: The DLC to translate.

    Returns:
        The data length of the message.
    """
    return DLC_TO_DLF[dlc]


def data_length_to_dlc(data_length: int) -> int:
    """
    Utility function to translate from message data length to message DLC.

    Args:
        data_length: The data length to translate.

    Returns:
        The DLC of the message.
    """
    return DLF_TO_DLC[data_length]


###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 05/05/2023  ABPA       FKU-883   Initial creation
