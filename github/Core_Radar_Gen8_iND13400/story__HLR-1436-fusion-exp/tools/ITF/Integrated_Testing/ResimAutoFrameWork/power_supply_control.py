"""
This module contains classes to control programmable power supplies from the KORAD and TENMA brands.
"""
# Import python base installed modules here
import sys
import time
import glob
import logging
from typing import Optional
from dataclasses import dataclass

# Import pip installed modules here
import serial

logger = logging.getLogger(__name__)

# Global Variables
SERIAL_DELAY_S = 0.02
READ_ATTEMPTS = 2
COMMAND_RETRY_ATTEMPTS = 3


@dataclass
class PowerSupplyStatus:
    """
    Class to store information about the power supply status.
    """

    CH1_mode: Optional[str] = None
    CH2_mode: Optional[str] = None
    tracking_mode: Optional[str] = None
    beep: Optional[str] = None
    control_lock: Optional[str] = None
    output_status: Optional[str] = None


class PowerCtrl:
    """
    Class to interact with a programmable power supply.

    More information can be found here:
        https://sigrok.org/wiki/Korad_KAxxxxP_series
    """

    SUPPORTED_POWER_SUPPLIES = {
        "TENMA": {"72-2710", "72-13330", "72-2540"},
        "KORAD": {"KA3005P", "KD3005P", "KA3005D", "KD3005D"},
    }

    def __init__(self) -> None:
        """
        Init method for PowerCtrl.
        """
        self.com_port: str
        self._end_character = ""

        port_list = self.list_serial_ports()
        for com_port in port_list:
            self.serialPort = self.serial_config(com_port)
            try:
                for end_character in ["", "\n"]:
                    id_var = self.get_ID(end_character=end_character)
                    if id_var is not None:
                        for brand, models in self.SUPPORTED_POWER_SUPPLIES.items():
                            if brand in id_var and any(model in id_var for model in models):
                                self.com_port = com_port
                                self._end_character = end_character
                                return
            except TimeoutError:
                continue

        self.com_port = ""
        logger.error("No supported Power Supply Found")
        return

    def list_serial_ports(self) -> list[str]:
        """
        Returns the list of names of available ports.

        Returns:
            A list of available serial ports.

        Raises:
            EnvironmentError: On unsupported or unknown platforms.
        """
        logger.debug("Finding Serial Ports...")
        platform = sys.platform
        if platform.startswith("win"):
            ports = ["COM%s" % (i + 1) for i in range(256)]
        elif platform.startswith("linux") or sys.platform.startswith("cygwin"):
            # this excludes your current terminal "/dev/tty"
            ports = glob.glob("/dev/tty[A-Za-z]*")
        elif platform.startswith("darwin"):
            ports = glob.glob("/dev/tty.*")
        else:
            logger.error(f"Unsupported platform: {platform}.")
            raise EnvironmentError(f"Unsupported platform: {platform}.")

        result = []
        for port in ports:
            try:
                s = serial.Serial(port)
                s.close()
                result.append(port)
            except (OSError, serial.SerialException):
                pass
        logger.debug(f"Done Searching Serial Ports. Ports found: {result}.")
        return result

    def serial_config(self, com_port: str, timeout_s: float = 0.1) -> serial.Serial:
        """
        Method to get the serial port com_port and configure it.

        Args:
            com_port: The name of the serial port to get and configure.
            timeout_s: The timeout that the port will have for receiving messages.

        Returns:
            The configured serial port.
        """
        logger.debug(f"Configuring port {com_port}...")
        serialPort = serial.Serial(
            baudrate=9600,
            bytesize=8,
            timeout=timeout_s,
            write_timeout=timeout_s,
            stopbits=serial.STOPBITS_ONE,
        )
        serialPort.port = com_port
        return serialPort

    def serial_write(self, message: str) -> None:
        """
        Method to send a message to the current self.serialPort port.

        Args:
            message: The message to send.
        """
        with self.serialPort as ser:
            logger.debug(f"Writing {message} to serial port...")
            ser.write((message + self._end_character).encode())

    def serial_read(self, decode_result: bool = True) -> bytes | str:
        """
        Method to read from the current self.serialPort port.

        Args:
            decode_result: Whether to decode the received bytes into a string.

        Returns:
            The received bytes, either decoded or as they were received.
        """
        # Read data out of the buffer until a carraige return / new line is found
        with self.serialPort as ser:
            for _ in range(READ_ATTEMPTS):
                serialString = ser.readline()
                logger.debug(f"Received {serialString} from serial port.")

                if serialString:
                    break
                else:
                    logger.debug("Received empty string.")

        # Print the contents of the serial data
        try:
            if decode_result:
                logger.debug("Decoding received bytes...")
                return serialString.decode().strip()
            else:
                logger.debug("Returning bytes without decoding them...")
                return serialString
        except UnicodeDecodeError:
            logger.warning(f"Could not decode received bytes: {serialString}")
            return str(serialString)

    def get_ID(self, end_character: str) -> Optional[str]:
        """
        Method to get the ID of the power supply.

        Args:
            end_character: Character to append at the end of the IDN command.

        Returns:
            The power supply ID if one was received.
        """
        logger.debug("Trying to get power supply ID...")
        try:
            self.serial_write("*IDN?" + end_character)
            response = self.serial_read()
            logger.debug(f"Got response: {response}.")
        except Exception:
            response = None
            logger.debug("No ID received from port.")
        return response

    def send_command(self, command: str) -> None:
        """
        Method to send a power supply command.

        Args:
            command: The command to send.
        """
        self.serial_write(command)

    def read_from_command(
        self, command: str, attempts: int = COMMAND_RETRY_ATTEMPTS
    ) -> float | str:
        """
        Method to send a command and read the value returned by the power supply.

        Args:
            command: The command to send to get a response from the power supply.
            attempts: The number of attempts to retry if the power supply does not respond.

        Returns:
            The power supply response converted to a float.
        """
        for _ in range(attempts):
            self.send_command(command)
            result = self.serial_read()
            if result:
                try:
                    return float(result)
                except ValueError:
                    return result

        error_msg = f"Got no response from the power supply for command {command}."
        logger.error(error_msg)
        raise Exception(error_msg)

    def set_output_voltage(self, setting: float, channel: int = 1) -> None:
        """
        Method to set the output voltage of the power supply.

        Args:
            setting: The voltage value to set.
            channel: The power supply channel to set.
        """
        logger.debug(f"Setting output voltage in channel {channel} to {setting}...")
        self.send_command(f"VSET{channel}:{setting:.2f}")

    def set_output_current(self, setting: float, channel: int = 1) -> None:
        """
        Method to set the output current of the power supply.

        Args:
            setting: The current value to set.
            channel: The power supply channel to set.
        """
        logger.debug(f"Setting output current in channel {channel} to {setting}...")
        self.send_command(f"ISET{channel}:{setting:.3f}")

    def get_output_voltage_setting(self, channel: int = 1) -> float:
        """
        Method to get the current setting for the output voltage of a channel.

        Args:
            channel: The channel of the power supply for which the voltage setting will be queried.

        Returns:
            The current output voltage setting for the specified channel.
        """
        logger.debug(f"Getting output voltage setting for channel {channel}...")
        return self.read_from_command(f"VSET{channel}?")

    def get_output_current_setting(self, channel: int = 1) -> float:
        """
        Method to get the current setting for the output current of a channel.

        Args:
            channel: The channel of the power supply for which the current setting will be queried.

        Returns:
            The current output current setting for the specified channel.
        """
        logger.debug(f"Getting output current setting for channel {channel}...")
        return self.read_from_command(f"ISET{channel}?")

    def get_output_voltage(self, channel: int = 1) -> float:
        """
        Method to get the current output voltage of a power supply channel.

        Args:
            channel: The power supply channel to read the voltage from.

        Returns:
            The current output voltage for the specified channel.
        """
        logger.debug(f"Getting output voltage for channel {channel}...")
        return self.read_from_command(f"VOUT{channel}?")

    def get_output_current(self, channel: int = 1) -> float:
        """
        Method to get the current output current of a power supply channel.

        Args:
            channel: The power supply channel to read the voltage from.

        Returns:
            The current output voltage for the specified channel.
        """
        logger.debug(f"Getting output current for channel {channel}...")
        return self.read_from_command(f"IOUT{channel}?")

    def switch_off(self) -> None:
        """
        Method to switch off the power supply.
        """
        logger.debug("Turning power supply OFF...")
        self.send_command("OUT0")

    def switch_on(self) -> None:
        """
        Method to switch on the power supply.
        """
        logger.debug("Turning power supply ON...")
        self.send_command("OUT1")
        logger.debug(f"Reading Ouptut Voltage after switching ON: {self.get_output_voltage()}")

    def power_cycle(self, sleep_time: int = 3) -> None:
        """
        Method to perform a power cycle on power supply.

        Args:
            sleep_time: The time to wait after turning off power supply to turn it on. Default value is 3s.
        Returns:
            None
        """
        logger.debug("Performing a Power Cycle...")
        self.switch_off()

        time.sleep(sleep_time)

        self.switch_on()

    def get_status(self) -> PowerSupplyStatus:
        """
        Method to get the current status of the power supply.

        Returns:
            The current status.
        """
        logger.debug("Getting power supply status...")

        status = PowerSupplyStatus()

        received_status = self.read_from_command("STATUS?")
        if received_status == "":
            logger.warning("Got no status response from power supply.")
            return status

        result = ord(received_status)
        time.sleep(SERIAL_DELAY_S)

        if 0x01 & result:
            status.CH1_mode = "CV Mode"
        else:
            status.CH1_mode = "CC Mode"

        if 0x02 & result:
            status.CH2_mode = "CV Mode"
        else:
            status.CH2_mode = "CC Mode"

        match (0b1100 & result) >> 2:
            case 0b11:
                status.tracking_mode = "Parallel Mode"
            case 0b10:
                status.tracking_mode = "Unknown Mode"
            case 0b01:
                status.tracking_mode = "Series Mode"
            case 0b00:
                status.tracking_mode = "Independent Mode"

        if 0x10 & result:
            status.beep = "Beep ON"
        else:
            status.beep = "Beep OFF"

        if 0x20 & result:
            status.control_lock = "Unlocked"
        else:
            status.control_lock = "Locked"

        if 0x40 & result:
            status.output_status = "Output ON"
        else:
            status.output_status = "Output OFF"

        logger.debug(f"Power Supply Status: {status}")

        return status
