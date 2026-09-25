"""
This module contains the PeriodicMessage class to specify the properties of a periodic message.
"""

from dataclasses import dataclass


@dataclass(kw_only=True)
class PeriodicMessage:
    """
    Class that specifies the properties of a periodic message.

    Attributes:
    id: The id or name of the message.
    start_delay_ms: How many ms should CANoe wait to send the message the first time after the
        measurement starts.
    period_ms: The periodicity of the message in ms.
    data: Tha bytes of the message that will be sent periodically.
    channel: Through with channel the message will be sent.
    """
    id: int | str
    start_delay_ms: int
    period_ms: int
    data: bytearray
    channel: int

    def __post_init__(self) -> None:
        """
        Post-init method. Used to convert the passed data to bytearray, in case a list of numbers or a bytes object
        was passed instead of a bytearray.
        """
        self.data = bytearray(self.data)

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 04/10/2023  ABPA       FKU-848   Initial creation
