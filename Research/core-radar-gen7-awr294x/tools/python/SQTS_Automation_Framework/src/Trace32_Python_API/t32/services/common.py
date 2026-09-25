"""
This module contains utility classes used by multiple t32 modules.
"""
import logging


class Trace32Logger(logging.LoggerAdapter):
    """
    Class to log Trace32 messages specifying from which instance they came.
    """

    def process(self, msg: str, kwargs):
        instance_name = self.extra.get('instance', '_')
        return f'[{instance_name}] {msg}', kwargs

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 07/27/2023  ABPA       FKU-897   Initial creation
