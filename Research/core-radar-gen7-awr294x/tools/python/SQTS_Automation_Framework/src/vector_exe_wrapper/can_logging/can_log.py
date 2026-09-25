"""
Module to wrap the multiple parsers for can logs in the library python-can and word with the parsed contents.
"""
from dataclasses import dataclass, field
from typing import Optional
import can
import logging

logger = logging.getLogger(__name__)


@dataclass
class CANLog:
    """
    Class that represents a general CAN log.

    Attributes:
       messages: A list of messages recorded in the log.
    """
    messages: list[can.Message] = field(default_factory=list)

    @staticmethod
    def from_file(filename: str) -> 'CANLog':
        """
        Function to get a CANLog by passing the filename.

        Args:
           filename: The name/path of the log file.

        Returns:
            The corresponding CANLog object.
        """
        logger.debug(f'Opening log file {filename}...')
        return CANLog(list(can.LogReader(filename)))

    def _find_timestamp_lowest(self, timestamp: float) -> Optional[int]:
        """
        Method to find the index of the first message that has a timestamp
        greater than or equal to the passed timestamp using binary search.

        Args:
           timestamp: The timestamp to compare against.

        Returns:
            The index in the messages list of the first message with a timestamp >= to the one passed in the argument.
        """
        # Check that there are messages
        if len(self.messages) == 0:
            return None

        # Use binary search
        lowest_index = 0
        greatest_index = len(self.messages)-1
        search_interval_size = greatest_index - lowest_index
        current_index = lowest_index + search_interval_size//2
        while search_interval_size > 1:
            if timestamp <= self.messages[current_index].timestamp:
                greatest_index = current_index
            else:
                lowest_index = current_index
            search_interval_size = greatest_index - lowest_index
            current_index = lowest_index + search_interval_size//2

        # Check that the timestamp of the message is not less than the passed argument
        if timestamp > self.messages[current_index].timestamp:
            # Try the next one
            if len(self.messages) >= current_index:
                current_index += 1
            else:
                return None

        # If it's less than the passed argument
        if timestamp > self.messages[current_index].timestamp:
            # Not found
            return None

        return current_index

    def _find_timestamp_greatest(self, timestamp: float) -> Optional[int]:
        """
        Method to find the index of the last message that has a timestamp
        lesser than or equal to the passed timestamp using binary search.

        Args:
           timestamp: The timestamp to compare against.

        Returns:
            The index in the messages list of the last message with a timestamp <= to the one passed in the argument.
        """
        # Check that there are messages
        if len(self.messages) == 0:
            return None

        # Binary search
        lowest_index = 0
        greatest_index = len(self.messages)-1
        search_interval_size = greatest_index - lowest_index
        current_index = lowest_index + search_interval_size//2
        while search_interval_size > 1:
            if timestamp < self.messages[current_index].timestamp:
                greatest_index = current_index
            else:
                lowest_index = current_index
            search_interval_size = greatest_index - lowest_index
            current_index = lowest_index + search_interval_size//2

        # Check that the timestamp of the message is not less than the passed argument
        if timestamp < self.messages[current_index].timestamp:
            # Try the previous one
            if current_index != 0:
                current_index -= 1
            else:
                return None

        # If it's less than the passed argument
        if timestamp < self.messages[current_index].timestamp:
            # Not found
            return None

        return current_index

    def get_filtered(self, *, start_time: float = None, end_time: float = None, messages: list[int] = None) -> list[can.Message]:
        """
        Method to get a list with filtered messages from the log contents.

        Args:
           start_time: The filtered messages will all have a timestamp greater than or equal to this argument, if it's provided.
           end_time: The filtered messages will all have a timestamp less than or equal to this argument, if it's provided.
           messages: The filtered messages will all have an ID present in this list, if it's provided.

        Returns:
            The filtered list of CAN messages.
        """
        lower_index = 0 if start_time is None else self._find_timestamp_lowest(start_time)
        upper_index = len(self.messages) if end_time is None else self._find_timestamp_greatest(end_time)
        if lower_index is None or upper_index is None or upper_index < lower_index:
            logger.warning(f'Could not get filtered messages from log, invalid start_time ({start_time}) or end_time ({end_time}).')
            return []

        filtered_messages = self.messages[lower_index:upper_index+1]

        start_time = 0.0 if start_time is None else start_time
        end_time = filtered_messages[-1].timestamp if end_time is None else end_time

        if messages is None:
            logger.debug(f'Returning filtered messages from {start_time}s to {end_time}s')
            return filtered_messages

        filtered_messages = [message for message in filtered_messages if message.arbitration_id in messages]

        logger.debug(f'Returning filtered messages with IDs: {messages}, from {start_time}s to {end_time}s')
        return filtered_messages

    def __getitem__(self, key: int | list | slice) -> list[can.Message]:
        """
        Operator overloading of indexing and slicing []. It calls the method get_filtered.

        Args:
           key: The index or slice to get.

        Returns:
            A list with the requested items.
        """
        match key:
            case int(id):  # can_log[0x9CA]
                return self.get_filtered(messages=[id])
            case list(ids):  # can_log[[0x9CA, 0x8B1]]
                return self.get_filtered(messages=ids)
            # can_log[0x9CA, 0x8B1] or can_log[0.5:1.5, 0x9CA] or can_log[0.5:1.5, [0x9CA, 0x8B1]]
            case tuple(ids):
                message_ids = [i for i in ids if isinstance(i, int)]
                for i in ids:
                    if isinstance(i, list):
                        message_ids += i
                sl = slice(None, None)
                for i in ids:
                    if isinstance(i, slice):
                        sl = i
                        break
                return self.get_filtered(start_time=sl.start, end_time=sl.stop, messages=message_ids)
            case time_range if isinstance(key, slice):  # can_log[a:b]
                return self.get_filtered(start_time=time_range.start, end_time=time_range.stop)

    def __str__(self) -> str:
        """
        Operator overloading of __str__ to represent a CANLog object.

        Returns:
            The string representation of the CANLog object.
        """
        return f'CAN_LOG: {len(self.messages)} messages, duration of {self.messages[-1].timestamp-self.messages[0].timestamp if self.messages else 0} s.'

    def __repr__(self) -> str:
        """
        Operator overloading of __repr__ to represent a CANLog object.

        Returns:
            The string representation of the CANLog object.
        """
        return str(self)


###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 03/21/2023  ABPA       FKU-796   Initial creation
# 05/11/2023  ABPA       FKU-736   Added trace logging.
