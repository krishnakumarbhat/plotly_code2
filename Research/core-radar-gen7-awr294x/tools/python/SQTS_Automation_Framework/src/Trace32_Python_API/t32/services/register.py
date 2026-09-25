"""
This module contains classes to read from and write to registers with Trace32.
"""
from lauterbach.trace32 import rcl
import logging
from typing import Literal, Optional

from .common import Trace32Logger
from .error import RegisterError

logger = logging.getLogger(__name__)

UnitType = Literal['CPU', 'FPU', 'VPU']


class Register:
    """
    Class that contains the data of a register. Wrapper class for rcl.Register.

    Attributes:
        name: The name of the register.
        core: The core where the register is from.
        unit: The unit of the register (CPU, FPU or VPU).
        value: The integer value contained in the register.
        fvalue: The float value contained in the register, if interpreted as a float.
    """

    def __init__(self, register: rcl.Register, _logger: logging.LoggerAdapter) -> None:
        """
        Init method for Register class.

        Args:
            register: The rcl.Register object to wrap.
            _logger: A logger adapter to use for logging purposes.
        """
        self.__register = register
        self.logger = _logger

    def __eq__(self, other: 'Register') -> bool:
        """
        Operator overloading for equality comparisons.

        Args:
            other: The other Register object to compare with.

        Returns:
            Whether both registers are equal or not.
        """
        return self.__register == other

    def __str__(self) -> str:
        """
        Operator overloading for str.

        Returns:
            The string representation of a Register.
        """
        return str(self.__register)

    def __repr__(self) -> str:
        """
        Operator overloading for repr.

        Returns:
            The string representation of a Register.
        """
        return str(self)

    @property
    def name(self) -> str:
        """
        The name of the register.
        """
        return self.__register.name

    @property
    def core(self) -> int:
        """
        The core where the register is from.
        """
        return self.__register.core

    @property
    def unit(self) -> UnitType:
        """
        The unit of the register (CPU, FPU or VPU).
        """
        return self.__register.unit

    @property
    def value(self) -> int:
        """
        The integer value contained in the register.
        """
        return self.__register.value

    @value.setter
    def value(self, value: int):
        """
        Setter for value attribute.
        """
        self.__register.value = value

    @property
    def fvalue(self) -> float | None:
        """
        The float value contained in the register, if interpreted as a float.
        """
        return self.__register.fvalue

    @fvalue.setter
    def fvalue(self, fvalue: float) -> None:
        """
        Setter for fvalue attribute.
        """
        self.__register.fvalue = fvalue

    def read(self) -> None:
        """
        Method to update the register values by re-reading them from the debugger.
        """
        try:
            self.logger.debug(f'Reading register {self}...')
            self.__register.read()
            self.logger.debug(f'Updated register values: {self}.')
        except Exception as exc:
            error_msg = f'Could not read values from register {self.name}.'
            self.logger.error(error_msg)
            raise RegisterError(error_msg) from exc

    def write(self) -> None:
        """
        Method to write the current register values to the debugger.
        """
        try:
            self.logger.debug(f'Updating register {self} with current values...')
            self.__register.write()
        except Exception as exc:
            error_msg = f'Could not update values for register {self.name}.'
            self.logger.error(error_msg)
            raise RegisterError(error_msg) from exc

    @property
    def _internal_obj(self) -> rcl.Register:
        """
        Wrapped rcl.Register object.
        """
        return self.__register


class RegisterService:
    """
    Class to read from and write to registers using Trace32.
    Wrapper class for rcl.RegisterService.
    """

    def __init__(self, register_service: rcl.RegisterService, _logger: logging.LoggerAdapter):
        """
        Init method for RegisterService class.

        Args:
            register_service: The rcl.RegisterService instance to wrap.
            _logger: A logger adapter from which to extract the extra info
                to log with this module's logger.
        """
        self.__register_service = register_service
        self.logger = Trace32Logger(logger, extra=_logger.extra)

    def read(self, name: str, core: Optional[int] = None, unit: Optional[UnitType] = None) -> Register:
        """
        Method to read a single Register.

        Args:
            name: Name of register
            core: The core of the register.
            unit: The unit of the register.

        Returns:
            The read register.
        """
        try:
            self.logger.debug(f'Reading register with name {name}, core {core} and unit {unit}.')
            register = self.__register_service.read(name, core=core, unit=unit)
            reg = Register(register, self.logger)
            self.logger.debug(f'Read register {reg}.')
            return reg
        except Exception as exc:
            error_msg = f'Could not read register {name}.'
            self.logger.error(error_msg)
            raise RegisterError(error_msg) from exc

    def read_by_name(self, name: str) -> Register:
        """
        Method to read a single Register by name.

        Args:
            name: Name of the register to read.

        Returns:
            The read register.
        """
        try:
            self.logger.debug(f'Reading register with name {name}')
            register = self.__register_service.read_by_name(name)
            reg = Register(register, self.logger)
            self.logger.debug(f'Read register {reg}.')
            return reg
        except Exception as exc:
            error_msg = f'Could not read register {name}.'
            self.logger.error(error_msg)
            raise RegisterError(error_msg) from exc

    def read_by_names(self, names: list[str]) -> list[Register]:
        """
        Method to read registers specified by a list of names.

        Args:
            names: Names of registers to read.

        Returns:
            A list with the read registers.
        """
        try:
            self.logger.debug(f'Reading registers with name {names}')
            registers = self.__register_service.read_by_names(names)
            regs = [Register(register, self.logger) for register in registers]
            self.logger.debug(f'Read registers {regs}.')
            return regs
        except Exception as exc:
            error_msg = f'Could not read registers {names}.'
            self.logger.error(error_msg)
            raise RegisterError(error_msg) from exc

    def read_all(self, *, core: Optional[int] = None, unit: Optional[UnitType] = None) -> list[Register]:
        """
        Method to read all the Registers.

        Args:
            core: core from which to read.
            unit: Type that the Registers should have (CPU, FPU, VPU).

        Returns:
            A list of the read registers.
        """
        try:
            self.logger.debug(f'Reading registers for core {core} with unit {unit}')
            registers = self.__register_service.read_all(core=core, unit=unit)
            regs = [Register(register, self.logger) for register in registers]
            self.logger.debug(f'Read registers {regs}.')
            return regs
        except Exception as exc:
            error_msg = f'Could not read registers for core {core} and unit {unit}'
            self.logger.error(error_msg)
            raise RegisterError(error_msg) from exc

    def read_list(self, regs: list[Register]) -> list[Register]:
        """
        Method to update a list of Register objects with the current values.

        Args:
            regs: Registers to read.

        Returns:
            The read registers.
        """
        try:
            self.logger.debug(f'Reading registers {regs}')
            read_registers = self.__register_service.read_list([register._internal_obj for register in regs])
            regs = [Register(register, self.logger) for register in read_registers]
            self.logger.debug(f'Read registers {regs}.')
            return regs
        except Exception as exc:
            error_msg = f'Could not read registers {regs}.'
            self.logger.error(error_msg)
            raise RegisterError(error_msg) from exc

    def write(self, name: str, value: int | float, **kwargs) -> Register:
        """
        Method to write to a single Register.

        Args:
            name: Name of register on which to write.
            value: Value to write

        Returns:
            A register object with the written values.
        """
        try:
            self.logger.debug(f'Writing {value} to register {name}.')
            register = self.__register_service.write(name, value, **kwargs)
            return Register(register, self.logger)
        except Exception as exc:
            error_msg = f'Could not write {value} to register {name}.'
            self.logger.error(error_msg)
            raise RegisterError(error_msg) from exc

    def write_by_name(self, name: str, value: int | float, **kwargs) -> Register:
        """
        Method to write a value to a register specified by name.

        Args:
            name: Name of register on which to write.
            value: Value to write

        Returns:
            A register object with the written values.
        """
        try:
            self.logger.debug(f'Writing {value} to register {name}.')
            register = self.__register_service.write_by_name(name, value, **kwargs)
            return Register(register, self.logger)
        except Exception as exc:
            error_msg = f'Could not write {value} to register {name}.'
            self.logger.error(error_msg)
            raise RegisterError(error_msg) from exc

    def write_by_names(self, names: list[str], values: list[int | float], **kwargs) -> list[Register]:
        """
        Method to write a list of specified values to registers specified by a list of names.

        Args:
            names: Names of registers on which to write.
            values: Values to write.

        Returns:
            A list of registers with the written values.
        """
        try:
            self.logger.debug(f'Writing {values} to registers {names}.')
            registers = self.__register_service.write_by_names(names, values, **kwargs)
            return [Register(register, self.logger) for register in registers]
        except Exception as exc:
            error_msg = f'Could not write {values} to registers {names}.'
            self.logger.error(error_msg)
            raise RegisterError(error_msg) from exc

    def write_list(self, regs: list[Register]) -> list[Register]:
        """
        Method to write a list of Register Objects.

        Args:
            regs: Registers to write.

        Returns:
            A list of registers with the written values.
        """
        try:
            self.logger.debug(f'Writing registers {regs}.')
            registers = self.__register_service.write_list([register._internal_obj for register in regs])
            return [Register(register, self.logger) for register in registers]
        except Exception as exc:
            error_msg = f'Could not write registers {regs}.'
            self.logger.error(error_msg)
            raise RegisterError(error_msg) from exc

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 07/27/2023  ABPA       FKU-897   Initial creation
