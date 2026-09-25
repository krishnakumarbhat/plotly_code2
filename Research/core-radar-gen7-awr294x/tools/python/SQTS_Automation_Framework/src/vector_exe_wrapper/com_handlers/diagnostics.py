"""
This module contains classes to handle diagnostic requests through CANoe.
"""
from typing import TypeVar, Optional, Callable
import time
from enum import IntEnum, auto
from win32com.client import WithEvents
from functools import cached_property
import logging

logger = logging.getLogger(__name__)

IDiagnostic = TypeVar('IDiagnostic')
IDiagnosticResponse = TypeVar('IDiagnosticResponse')


class DiagnosticRequestError(Exception):
    """An error occurred while making a diagnostic request"""


class DiagnosticRequestStatus(IntEnum):
    """
    Status of a diagnostic request.
    """
    CREATED = auto()
    COMPLETED_SUCCESSFULLY = auto()
    SENDING_SUCCESSFUL = auto()
    RESPONSE_RECEIVED = auto()
    SEND_FAILURE = auto()
    NO_RESPONSE = auto()


class DiagnosticRequestResponse:
    """
    Wrapper class for a response to a diagnostic request.

    Attributes:
        positive: Whether the response is positive.
        negative: Whether the response is negative.
        response_code: The error code provided by the ECU.
        sender: The identifier of the ECU that sent the response.
        data: The bytes of the diagnostic response.
    """

    def __init__(self, com_response: IDiagnosticResponse) -> None:
        """
        Init method for DiagnosticRequestResponse.

        Args:
            com_response: COM interface to interact with the received response.
        """
        self.__com_response = com_response

    @cached_property
    def positive(self) -> bool:
        """Whether the response is positive."""
        return self.__com_response.Positive

    @cached_property
    def negative(self) -> bool:
        """Whether the response is negative."""
        return not self.positive

    @cached_property
    def response_code(self) -> int:
        """The error code provided by the ECU. This value is only meaningful as long as the response is negative."""
        return self.__com_response.ResponseCode

    @cached_property
    def sender(self) -> str:
        """The identifier of the ECU that sent the response."""
        return self.__com_response.Sender

    @cached_property
    def data(self) -> bytes:
        """The bytes of the diagnostic response."""
        return self.__com_response.Stream.tobytes()

    def __str__(self) -> str:
        """
        Operator overloading for str dunder method.
        """
        response_type = 'Positive' if self.positive else 'Negative'
        return f'{response_type} response from {self.sender} with response code {self.response_code} and data {self.data.hex(" ")}'


class DiagnosticsDevice:
    """
    Wrapper class for a diagnostics device. Used to make diagnostic requests through CANoe.

    Attributes:
        network: The name of the network of the diagnostics device.
        ecu_qualifier: The ecu qualifier for the diagnostics device.
    """

    def __init__(self, com_diag_device: IDiagnostic, network: str, ecu_qualifier: str):
        """
        Init method for DiagnosticsDevice.

        Args:
            com_diag_device: The COM interface to interact with the Diagnostics Device in CANoe.
            network: The name of the network of the diagnostics device.
            ecu_qualifier: The ecu qualifier for the diagnostics device.
        """
        self.__com_diag_device = com_diag_device
        self.__network = network
        self.__ecu_qualifier = ecu_qualifier

    @property
    def com_interface(self) -> IDiagnostic:
        """COM interface to interact with the Diagnostics Device in CANoe."""
        return self.__com_diag_device

    @property
    def network(self) -> str:
        """The name of the network of the diagnostics device."""
        return self.__network

    @property
    def ecu_qualifier(self) -> str:
        """The ecu qualifier for the diagnostics device."""
        return self.__ecu_qualifier

    @property
    def tester_present(self) -> bool:
        """Whether the Tester Present requests to the ECU are being sent through this diagnostics device."""
        tester_present = self.com_interface.TesterPresentStatus
        logger.debug(f'Tester present: {tester_present}.')
        return tester_present

    def send_request(
            self,
            request_info: bytes | list[int] | str,
            *,
            timeout_s: float = 10,
            callback: Optional[Callable[[], None]] = None
    ) -> list[DiagnosticRequestResponse]:
        """
        Method for sending a diagnostic request.

        Args:
            request_info: Bytes or primitive path for the diagnostic request.
            timeout_s: Timeout to wait for the diagnostic request AND the callback to finish.
                If the callback takes longer to run than this value, a TimeoutError exception
                will be raised.
            callback: A function to call right after sending the diagnostic request.

        Returns:
            A list of the received responses.

        Raises:
            TimeoutError: The timeout expired while waiting for the diagnostics request to
                be received or for the callback to finish.
            DiagnosticRequestError: An error occurred while sending the diagnostic request or receiving the response.
        """
        match request_info:
            case bytes(info) | list(info) | bytearray(info):
                req_bytes = bytes(info)  # Convert to bytes just in case a list of ints was passed.
                logger.debug(f'Sending diagnostic request with hex data {req_bytes.hex(" ")} through network {self.network} and ecu qualifier {self.ecu_qualifier}...')
                logger.debug('Creating request...')
                diag_req = self.com_interface.CreateRequestFromStream(req_bytes)

            case str(info):
                logger.debug(f'Sending diagnostic request with primitive path {info} through network {self.network} and ecu qualifier {self.ecu_qualifier}...')
                logger.debug('Creating request...')
                diag_req = self.com_interface.CreateRequest(info)

        diagnostic_request_status = DiagnosticRequestStatus.CREATED

        class DiagnosticRequestEvents:
            def OnCompletion(_):
                nonlocal diagnostic_request_status
                diagnostic_request_status = DiagnosticRequestStatus.COMPLETED_SUCCESSFULLY
                logger.debug(f'Got last response of diagnostic request {request_info}.')

            def OnConfirmation(_):
                nonlocal diagnostic_request_status
                diagnostic_request_status = DiagnosticRequestStatus.SENDING_SUCCESSFUL
                logger.debug(f'Successfully sent diagnostic request {request_info}.')

            def OnResponse(_, _response):
                nonlocal diagnostic_request_status
                diagnostic_request_status = DiagnosticRequestStatus.RESPONSE_RECEIVED
                logger.debug(f'Got response for diagnostic request {request_info}.')

            def OnTimeout(_):
                nonlocal diagnostic_request_status
                match diagnostic_request_status:
                    case DiagnosticRequestStatus.CREATED:
                        diagnostic_request_status = DiagnosticRequestStatus.SEND_FAILURE
                        logger.error(f'Failed to send diagnostic request {request_info}.')

                    case DiagnosticRequestStatus.SENDING_SUCCESSFUL:
                        diagnostic_request_status = DiagnosticRequestStatus.NO_RESPONSE
                        logger.error(f'Got no response for diagnostic request {request_info}.')

        WithEvents(diag_req, DiagnosticRequestEvents)

        logger.debug(f"Sending request {diag_req}...")
        diag_req.Send()

        start = time.perf_counter()

        if callback is not None:
            logger.debug('Calling callback function...')
            callback()

        logger.debug("Waiting for Diagnostic Response...")
        while time.perf_counter() - start < timeout_s:
            if not diag_req.Pending:
                break
            time.sleep(0.05)
        else:
            error_msg = f'Timeout expired waiting for response to diagnostic request {request_info}'
            logger.error(error_msg)
            raise TimeoutError(error_msg)

        logger.debug(f'Diagnostic request completed after {time.perf_counter()-start}s.')

        if diagnostic_request_status != DiagnosticRequestStatus.COMPLETED_SUCCESSFULLY:
            raise DiagnosticRequestError(f'An error occured sending diagnostic request {request_info}: {diagnostic_request_status.name}')

        return [DiagnosticRequestResponse(response) for response in diag_req.Responses]

    def start_tester_present(self) -> None:
        """
        Method to start sending autonomous/cyclical Tester Present requests to the ECU.
        The TesterPresent remains active only as long as the COM script is running.
        When the COM script is finished, the diagnostic channel is closed and the Tester Present process is switched off.
        """
        logger.debug('Starting tester present...')
        self.com_interface.DiagStartTesterPresent()

    def stop_tester_present(self) -> None:
        """
        Method to stop sending autonomous/cyclical Tester Present requests to the ECU.
        """
        logger.debug('Stopping tester present...')
        self.com_interface.DiagStopTesterPresent()


###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 06/28/2023  ABPA       FKU-975   Initial creation
