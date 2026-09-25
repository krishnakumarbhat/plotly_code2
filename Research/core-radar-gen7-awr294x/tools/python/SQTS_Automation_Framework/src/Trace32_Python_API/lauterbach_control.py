"""
date: 3/19/2021

author: Jackson Brennecke

description:    This module is designed to control the lauterbach unit for
                erasing and flashing the CADM ECUs through the JTAG debugger
                port. This module contains functions and methods that can
                be imported to enable the automated continuous testing
                features. This module can also be run independently to test
                the connection and functionality of the lauterbach control.
"""

import os
import subprocess
import time
import enum
from typing import Optional, Literal
from pathlib import Path
import logging

from .hsm_wrapper import HSMWrapper
from .hsm_wrapper.project_specific_hsm_wrappers import IFV600_HSM
from . import t32

logger = logging.getLogger(__name__)

THIS_DIR = Path(__file__).parent


class RunState(enum.IntEnum):
    """
    This Class is used as enum for TRACE32 RunState conditions
    0 : Debug System is Down
    1 : Debug System is Halted, CPU Makes no Cycles (no access)
    2 : Target Execution is Stopped (Break)
    3 : Target Execution is Running (Go)
    """
    DOWN = 0
    HALTED = 1
    STOPPED = 2
    RUNNING = 3


class Trace32:
    """
    Class to interact with the Trace32 python RCL interface.

    Attributes:
        _trace_exe_path: Path to the trace32 executable.
        dbg: RCL interface to control Trace32.
        address: Trace32 address service.
        breakpoint: Trace32 breakpoint service.
        cmd: Trace32 command service.
        fnc: Trace32 function service.
        memory: Trace32 memory service.
        practice: Trace32 practice service.
        register: Trace32 register service.
        symbol: Trace32 symbol service.
        variable: Trace32 variable service.
        _t32_process: The Trace32 subprocess handle.
        tc397x: Handle to interact with the HSM script.
    """

    class exceptions:
        """
        Class to expose internal exceptions.
        """
        AddressError = t32.AddressError
        BreakpointError = t32.BreakpointError
        CommandError = t32.CommandError
        FunctionError = t32.FunctionError
        MemoryError = t32.MemoryError
        PracticeError = t32.PracticeError
        RegisterError = t32.RegisterError
        SymbolError = t32.SymbolError
        VariableError = t32.VariableError

    class types:
        """
        Class to expose internal classes.
        """
        RunState = RunState
        Symbol = t32.Symbol

        BreakpointAction = t32.Breakpoint.Action
        BreakpointImpl = t32.Breakpoint.Impl
        BreakpointType = t32.Breakpoint.Type

    def __init__(
            self,
            trace32_exe_path: str
            ) -> None:
        """
        Init method for Trace32 class.

        Args:
            trace32_exe_path: Path to the trace32 executable.
        """
        self._trace_exe_path: Path = Path(trace32_exe_path)

        if not self._trace_exe_path.exists():
            logger.error(f'File specified for trace32 executable does not exist: {self._trace_exe_path}.')
            raise FileNotFoundError(f'File specified for trace32 executable does not exist: {self._trace_exe_path}.')

        self.logger = t32.Trace32Logger(logger, extra={'instance': self._trace_exe_path.stem})

        self.dbg: t32.Debugger = None
        self.address: t32.AddressService = None
        self.breakpoint: t32.BreakpointService = None
        self.cmd: t32.CommandService = None
        self.fnc: t32.FunctionService = None
        self.memory: t32.MemoryService = None
        self.practice: t32.PracticeService = None
        self.register: t32.RegisterService = None
        self.symbol: t32.SymbolService = None
        self.variable: t32.VariableService = None

        self._t32_process: subprocess.Popen = None

        self.tc397x: HSMWrapper = None

    def start_exe(self, t32_config: str, t32_config_arguments: list[str] = [], timeout: float = 2) -> None:
        """
        Method to start the Trace32 application.

        Args:
            t32_config: The path to the .t32 configuration file for Trace32.
            t32_config_arguments: A list of arguments to pass to the .t32 configuration file.
            timeout: Time to sleep after starting Trace32.
        """
        # Launch TRACE32 as a sub process - this prevents our script
        # from blocking
        self.logger.debug(f'Starting {self._trace_exe_path.name} with configuration {t32_config}...')
        t32_config_path = Path(t32_config).absolute()
        if not t32_config_path.exists():
            self.logger.error(f'Specified config file for Trace32 not found: {t32_config_path}.')
            raise FileNotFoundError(f'Specified config file for Trace32 not found: {t32_config_path}.')

        self._t32_process = subprocess.Popen([self._trace_exe_path, '-c', t32_config] + t32_config_arguments, creationflags=subprocess.CREATE_NEW_PROCESS_GROUP)

        time.sleep(timeout)

        self.logger.debug('Trace32 successfully started.')

    def exit_exe(self) -> None:
        """
        Method to close the Trace32 application.
        """
        self.logger.debug('Exiting Trace32...')
        if self.cmd is not None:
            self.cmd('QUIT')
            time.sleep(3)  # Wait a bit and then send the taskkill just to be sure

        os.system(f"taskkill /im {self._trace_exe_path.name}")

    def connect(
            self,
            connection_port: int = 20000,
            connection_protocol: Literal['TCP', 'UDP'] = "TCP",
            connection_timeout_s: float = 10.0
            ) -> None:
        """
        Method to connect to Trace32 with the RCL interface.

        Args:
            connection_port: The UDP/TCP port to use for the connection with Trace32.
                It must match the one defined in the .t32 configuration that was used when opening Trace32.
            connection_protocol: The protocol to use for the connection with Trace32. TCP is recommended.
            connection_timeout_s: Timeout to wait for the connection with Trace32 to be established.

        NOTE:
            start_exe must be called first.
        """
        self.logger.debug(f'Starting connection with Trace32 through port {connection_port} with procotol {connection_protocol}...')
        self.dbg = t32.connect(
            node='localhost',
            port=connection_port,
            protocol=connection_protocol,
            timeout=connection_timeout_s,
            _logger=self.logger)
        self.address = self.dbg.address
        self.breakpoint = self.dbg.breakpoint
        self.cmd = self.dbg.cmd
        self.fnc = self.dbg.fnc
        self.memory = self.dbg.memory
        self.practice = self.dbg.practice
        self.register = self.dbg.register
        self.symbol = self.dbg.symbol
        self.variable = self.dbg.variable
        self.logger.debug('Connection with Trace32 successfully created.')

    def set_hsm(
            self,
            *,
            tc397x_dir: str = '',
            tc397x_config: dict = {},
            project: Optional[Literal['IFV600']] = None
            ) -> None:
        """
        Method to set the configuration parameters for the HSM handle.

        Args:
            tc397x_dir: The path to the directory where the HSM_TC39x.cmm script is located.
            tc397x_config: A dictionary with the configuration for the HSM handle. It must have
                the following entries: flashing_files, source_code_dirs and options. Check the
                init method for HSMWrapper for more details about what these entries mean.
            project: The project to which the HSM script to be used belongs to. Required only if
                some flashing functions are needed, for example: programming CertStore and programming
                HSM + BM.
        """
        self.logger.debug(f'Setting up HSM wrapper for project {project} with configuration {tc397x_config}...')
        match project:
            case None:
                hsm_class = HSMWrapper

            case 'IFV600':
                hsm_class = IFV600_HSM

            case _:
                self.logger.error(f'Project {project} not recognized.')
                raise Exception(f'Unrecognized hsm type: {project}')

        if not tc397x_dir:
            tc397x_dir = THIS_DIR / 'cmm_scripts/CADM_LO'

        self.tc397x = hsm_class(
            t32=self,
            cmm_dir=tc397x_dir,
            **tc397x_config
        )

    def run_cmm(self, cmm: str | Path, *cmm_arguments, timeout_s: Optional[float] = None) -> None:
        """
        Method to execute a Trace32 PRACTICE (cmm) script.

        Args:
            cmm: The path to the PRACTICE script.
            *cmm_arguments: Arguments to pass to the PRACTICE script
            timeout_s: Timeout in seconds.
                Special values:
                - None: Wait indefinitely for the cmm script to finish.
                - 0: Don't poll for script to finish (non-blocking)
        """
        self.logger.debug(f'Running cmm {cmm} with arguments {cmm_arguments}...')
        command = str(cmm)
        cmm_path = command.strip('"')
        cwd = Path(self.fnc('OS.PresentWorkingDirectory()'))
        if not Path(cmm_path).exists() and not (cwd / cmm_path).exists():
            self.logger.error(f'cmm script does not exist: {cmm_path}.')
            raise FileNotFoundError(f'cmm script does not exist: {cmm_path}.')

        if cmm_arguments:
            command += ' ' + ' '.join(map(str, cmm_arguments))

        last_message = self.dbg.get_message()
        if last_message:
            self.logger.debug(f'Trace32 print area: {last_message}')

        stack_depth_pre = self.fnc("PRACTICE.SD()")
        start_time = time.perf_counter()
        try:
            self.cmd(f"DO {command}")
        except t32.CommandError as e:
            self.logger.error(f'Practice error: {e}.')
            raise t32.PracticeError(str(e)) from None

        if timeout_s is None or timeout_s > 0:
            while True:
                stack_depth = self.fnc("PRACTICE.SD()")
                if stack_depth < stack_depth_pre:
                    self.logger.error(f"Practice stack depth error running {command}")
                    raise t32.PracticeError(f"Practice stack depth error running {command}")
                elif stack_depth == stack_depth_pre:
                    break
                if timeout_s is not None:
                    if time.perf_counter() - start_time > timeout_s:
                        self.logger.error(f'Timeout expired for execution of practice script {command}')
                        raise TimeoutError(f'Timeout expired for execution of practice script {command}')
                current_message = self.dbg.get_message()
                if current_message != last_message:
                    last_message = current_message
                    self.logger.debug(f'Trace32 print area: {last_message}')

                time.sleep(0.01)

    def run_command(self, command: str) -> None:
        """
        Method to run a Trace32 command. Retaining redundant method
        for backward-compatibility reasons

        Args:
            command: The command to run.
        """
        self.cmd(command)

    def execute_cmd(self, command: str) -> None:
        """
        Method to run a Trace32 command. Retaining redundant method
        for backward-compatibility reasons.

        Args:
            command: The command to run.
        """
        self.cmd(command)

    def set_processor_type(self, processor_type: str = "TC397XP") -> None:
        """
        Method to set the processor type.

        Args:
            processor_type: The processor type.
        """
        self.cmd(f'SYStem.CPU {processor_type}')

    def power_on(self) -> None:
        """
        Method to start the processor.
        """
        if self.get_run_state() != RunState.RUNNING:
            self.dbg.go()

    def reset_symbol_files(self) -> None:
        """
        Method to reset the symbol files.
        """
        self.cmd('SYMBOL.RESET')

    def load_symbol_file(self, file_path: str) -> None:
        """
        Method to load a symbols file.

        Args:
            file_path: The path to the symbols file.
        """
        self.cmd(f"Data.LOAD.auto {file_path} /NoCODE /NoCLear")

    def set_src_code_path(self, src_path: str) -> None:
        """
        Method to set the source code path.

        Args:
            src_path: The path to the source code.
        """
        # TODO: Check if subsequent calls overwrite the source code path or just add more paths
        self.cmd(f"SYMBOL.SOURCEPATH.SETRECURSEDIR {src_path}")

    def add_var_watch(self, var_name: str) -> None:
        """
        Method to add a variable to the watch list.

        Args:
            var_name: The name of the variable to add.
        """
        self.cmd(f"Var.AddWatch {var_name}")

    def del_var_watch(self, var_name: str) -> None:
        """
        Method to remove a variable from the watch list.

        Args:
            var_name: The name of the variable to remove.
        """
        self.cmd(f"Var.DelWatch {var_name}")

    def show_breakpoints(self) -> None:
        """
        Method to show the breakpoints in Trace32.
            The command sent to Trace32.
        """
        self.cmd("Break.List")

    def delete_all_breakpoints(self) -> None:
        """
        Method to delete all set breakpoints.
        """
        self.cmd("Break.Delete /ALL")

    def in_target_reset(self) -> None:
        """
        Method to do an in-target reset.
        """
        self.cmd("SYStem.RESetTarget")

    def get_symbol_addr(self, symbol_name: str) -> t32.Symbol:
        """
        Method to get details about a symbol.

        Args:
            symbol_name: The name of the symbol.

        Returns:
            The symbol object containing information about the requested symbol.
        """
        return self.symbol.query_by_name(symbol_name)

    def read_from_symbol(
            self,
            symbol: t32.Symbol,
            endianness: Literal['little', 'big'] = 'little',
            need_raw: bool = False
            ) -> int | bytes:
        """
        Method to read a value from a symbol.

        Args:
            symbol: The symbol object containing information about the symbol in memory.
            endianness: The endianness to use when converting the symbol bytes into an int.
            need_raw: Whether to convert the bytes into an int or return them in an array.

        Returns:
            An int constructed by interpreting the bytes of the symbol, or an array of bytes of the symbol
            if need_raw is True.
        """
        if isinstance(symbol, str):  # Added check for backwards-compatibility reasons
            return self.variable.read(symbol).value

        value = self.memory.read(address=symbol.address, length=symbol.size)

        if not need_raw:
            value = int.from_bytes(value, byteorder=endianness)

        return value

    def get_run_state(self) -> RunState:
        """
        Method to get the current debugger state.

        Returns:
            The state of the debugger. See RunState class for a description
            of the possible states.
        """
        state = self.dbg.get_state()
        return RunState(int.from_bytes(state, byteorder='little'))

    def set_breakpoint(self, bp_location: str, read_write: bool = False) -> None:  # TODO: return breakpoint object
        """
        Method to set a breakpoint.

        Args:
            bp_location: The location to set the breakpoint at, can be one of the following:
                * Address in hexadecimal format, e.g. '0xA34F'
                * Function name, e.g. 'func1'
                * Function name with address offset, e.g. 'func2+0x1c'
                * Function name with line offset (line in compiled program), e.g. r'func3\17'
            read_write: If False the breakpoint will have type Program,
                if True it will have type ReadWrite.

        NOTE:
            For this method to work, the source code files must have been already loaded in Trace32.
        """
        command = f"Break.Set {bp_location}"
        if read_write:
            command = f"Break.Set {bp_location} /ReadWrite "
        self.cmd(command)

    def set_m_breakpoint(self, Function_name: str, read_write: bool = False) -> None:  # TODO: return breakpoint object
        """t32_W2.cmd('break.set MMIC_Diagnostics /Program /Onchip')"""
        command = f"break.Set {Function_name}"
        if read_write:
            command = f"break.Set {Function_name} "
        self.cmd(command)

    def set_breakpoint_end(self, Function_name: str, read_write: bool = False) -> None:  # TODO: return breakpoint object
        """t32_W2.cmd('break.set MMIC_Diagnostics /Program /Onchip')"""
        command=f"Break.set sYmbol.END({Function_name})"
        if read_write:
            command =f"Break.set sYmbol.END({Function_name}) /Program /Onchip"
        self.cmd(command)

    def set_breakpoint_exit(self, Function_name: str, read_write: bool = False) -> None:
        command=f"Break.set sYmbol.EXIT({Function_name})"
        if read_write:
            command=f"Break.set sYmbol.EXIT({Function_name})"
        self.run_command(command)
        return command

    def set_breakpoint_at_line(self, filename: str, line_number: int, read_write: bool = False) -> None:  # TODO: Return breakpoint object
        """
        Method to set a breakpoint at a specific line in a source code file.

        Args:
            filename: The name of the source code file without any extensions, or
                the full path to the source code file.
            line_number: The number of the line in the source code file to set the breakpoint at.
            read_write: If False the breakpoint will have type Program,
                if True it will have type ReadWrite.
        """
        if {'.', '\\', '/'} & set(filename):  # If '.', '\' or '/' in filename, use absolute path
            filename = f'"{Path(filename).absolute()}"'

        breakpoint_location = fr'\{filename}\{line_number}'
        self.set_breakpoint(breakpoint_location, read_write=read_write)

    def set_breakpoint_at_end(self, function_name: str) -> None:
        """
        Method to set a breakpoint at the end of a function.

        Args:
            function_name: The name of the function to set the breakpoint at.
        """
        self.set_breakpoint(f'sYmbol.Exit({function_name})')

    def read_var(self, var_name: str) -> int | float:
        """
        Method to read the value of a variable.

        Args:
            var_name: The name of the variable to read.

        Returns:
            The value of the variable.
        """
        state = self.get_run_state()
        if state == RunState.RUNNING:
            self.dbg.break_()

        var = self.variable.read(var_name)

        if state == RunState.RUNNING:
            self.dbg.go()

        return var.value

    def read_var_m(self, var_name: str) -> int | float:
        """
        Method to read the value of a variable.

        Args:
            var_name: The name of the variable to read.

        Returns:
            The value of the variable.
        """
        var = self.variable.read(var_name)

        return var.value

    def write_var(self, var_name: str, value: int | float) -> None:
        """
        Method to write a value to a variable.

        Args:
            var_name: Name of the variable to write.
            value: The value to be assigned to the variable.
        """
        # state = self.get_run_state()
        # if state == RunState.RUNNING:
            # self.dbg.break_()
        self.print(f'Assigning {var_name} value as {value}.')
        self.variable.write(var_name, value)
        # if state == RunState.RUNNING:
            # self.dbg.go()

    def wait_for_breakpoint(self, timeout: float = 20.0) -> None:
        """
        Method to wait until a breakpoint is reached.

        Args:
            timeout: Time limit to wait for the breakpoint.

        Raises:
            TimeoutError: The time limit was reached and the ECU was still running.
            Exception: The time limit was reached and the ECU was not in the running or the stopped states.
        """
        start = time.time()
        state = self.get_run_state()
        while time.time() - start < timeout:
            if state == RunState.STOPPED:
                break

            time.sleep(0.5)
            state = self.get_run_state()
        else:  # Timeout expired
            if state != RunState.STOPPED:
                self.logger.error(f'Timeout expired while waiting for breakpoint, last state was {state.name}.')
                raise TimeoutError(f'Timeout expired while waiting for breakpoint, last state was {state.name}.')

    def reset_to_code_start(self) -> None:
        """
        Method to reset the code to the start.
        """
        self.logger.debug('Resetting code to start...')
        self.run_cmm(THIS_DIR / "cmm_scripts/CADM_LO/TC39x/reset_to_code_start.cmm")

    def get_message(self) -> str:
        """
        Method to read the contents of the message line and AREA window of Trace32.

        Returns:
            The read message.
        """
        message = self.dbg.get_message()  # TODO: log message
        self.logger.debug(f'Got message of type {message.type}.')
        return message.text

    def print(self, message: str) -> None:
        """
        Method to print something in Trace32.

        Args:
            message: The message to print.
        """
        self.logger.debug(f'Printing message {message} to Trace32...')
        self.dbg.print(message)

    def NoDebug(self) -> None:
        """
        Method to make the T32 instance to No Debug Mode.
        """
        time.sleep(2)
        self.cmd('SYStem.Mode NoDebug')
        time.sleep(2)
        self.cmd('PRINT "Entered into No Debug Mode"')
        return

    def T32_Radar_Status(self):
        """
        True when Radar status is Successful.
        False in all other states.
        """
        try:
            Var = self.read_var("Radar_Ctl_Data.init_status")
            if Var == 3 :
                return True
            return False
        except Exception as VariableError:
            return False

    def T32_Check_Looks(self) -> bool:
        """
        True if Looks are properly Switching.
        False if Looks are not Switching properly.
        """
        Var = self.read_var("Radar_Ctl_Data.this_look")
        Var1 = self.read_var("Radar_Ctl_Data.next_look")
        if Var != Var1 :
            return True
        return False

    def T32_Looks_Switch(self) -> bool:
        """
        True if Looks are properly Switching for Three checks.
        False if Looks are not Switching properly for Three checks.
        """
        if(self.T32_Check_Looks()):
            if(self.T32_Check_Looks()):
                if(self.T32_Check_Looks()):
                    return True
                return False
            return False
        return False

    def T32_Counters_Check(self) -> bool:
        """
        True if Scan Index, Radar_Ctl_Data.look_index and Radar_50ms_Counter is Incrementing.
        False if scan Index, Radar_Ctl_Data.look_index and Radar_50ms_Counter is not Incrementing.
        """
        Var = self.read_var("Radar_Ctl_Data.scan_index")
        Cons = self.read_var("Radar_Ctl_Data.look_index")
        time.sleep(3)
        Var1 = self.read_var("Radar_Ctl_Data.scan_index")
        Cons1 = self.read_var("Radar_Ctl_Data.look_index")
        if ((Var1 > Var) and (Cons1 > Cons)):
            return True
        return False

    def T32_Radiate_Enable(self) -> bool:
        """
        True when Radar is Radiating.
        False when Radar is Not Radiating.
        """
        Var = self.read_var("Radar_Ctl_Data.radiate_enable")
        if Var == 1:
            return True
        return False

    def T32_Reset_and_Go(self) -> bool:
        """
        True when it is able to perform Reset and Go operation Properly on Core State otherwise returns False.
        """
        try:
            self.cmd("SYStem.mode.Up")
        except BaseException:
            print("An exception occurred.")
        time.sleep(5)
        GoAttempts = 1
        while self.get_run_state() != 3:
            self.cmd("SYStem.Attach")
            GoAttempts += 1
            time.sleep(6)
            while self.get_run_state() != 3:
                try:
                    self.cmd("SYStem.mode.Up")
                except BaseException:
                    print("An exception occurred.")
                time.sleep(2)
                GoAttempts += 1
                time.sleep(6)
                self.print(f'-------- State: {self.get_run_state()} --------')
                if (GoAttempts > 3) :
                    assert False, "Core state is not Fine for Testing and It is Not Stable, Please go for Re-flashing"
            print("===> COre State is stable to State 3. Core is running Now<===")

    def Clean_and_Reset(self) -> None:
        """
        Default Window Setup for R5f Core.
        """
        self.delete_all_breakpoints()
        self.cmd("WinCLEAR")
        time.sleep(2)
        try:
            # DIR = Path.cwd()
            # print(f'Working DIR Right now: {DIR} <====\nTrying to Run default_win_r5f.cmm Script.')
            self.run_cmm("../../../instrumentation/Lauterbach/default_win_r5f.cmm")
        except FileNotFoundError:
            # print("Exception Occured Need to change the Working Dir to Lauterbach Folder.")
            try:
                self.run_cmm("./default_win_r5f.cmm")
            except FileNotFoundError:
                try:
                    self.run_cmm("../../../instrumentation/Lauterbach/default_win_r5f.cmm")
                except FileNotFoundError:
                    None
            # self.cmd("cd ../../../instrumentation/Lauterbach/")
            # print("Working Dir got changed to Lauterbach Folder now.")
            # DIR = Path.cwd()
            # print(f'Working DIR Right now: {DIR} <====\nTrying to send CMD as "DO default_win_r5f.cmm".')
            # self.cmd("DO default_win_r5f.cmm")
        time.sleep(2)
        return

    def Check_and_Run(self) -> bool:
        """
        Code which checks the status of Radar, Basic working of Radar and make it up and running.
        """
        self.Clean_and_Reset()
        GoAttempts = 1
        self.T32_Reset_and_Go()
        self.print(f'-------- Attempt Number: {GoAttempts} --------')
        while (not(self.T32_Radar_Status() and self.T32_Looks_Switch() and self.T32_Counters_Check() and self.T32_Radiate_Enable())):
            GoAttempts += 1
            self.T32_Reset_and_Go()
            time.sleep(3)
            self.print(f'-------- Attempt Number: {GoAttempts} --------')
            if (GoAttempts > 8) :
                assert False, "Test case is not executed because, Radar Basic Tests got Failed, Please go for Re-flashing"
        print(f"--------> Attempt Number: {GoAttempts} is Successful and")
        self.print(f"--------> Attempt Number: {GoAttempts} is Successful and")
        self.print("CoRe is Up and Running for this Test Script")
        print("CoRe is Up and Running for this Test Script")
        print()
        print("===> Radar State is stable and Successful. Good To Start Testing <===")
        print()
        print("============> Entering into Testing Mode")
        print()
        return True

    def SETup_for_C66(self) -> None:
        """
        Basic setup for C66 Core.
        """
        self.delete_all_breakpoints()
        self.NoDebug()
        try:
            self.cmd("SYStem.mode.Attach")
        except BaseException:
            print("An Exception Occurred in C66")
        time.sleep(3)
        return

    def read_and_Del_File(self, file_path):
        """
        Takes the TXT file name as input.
        Consider the data in the file is 5B/n5D/n81/n6C/n.
        This function will delete the TXT file and Before that It returns the Value as 0x6C815D5B.
        """
        # print(file_path)
        # DIR = Path.cwd()
        # print('STEP-2 ', DIR)
        try:
            with open(file_path,'r') as file:
                data = file.read().strip().split()
                hex_value = "0x" + ''.join(f'{int(x, 16):02x}' for x in reversed(data)).upper()
                return hex_value
        except FileNotFoundError:
            print("File Not Found in the given Path")
        finally:
            try:
                os.remove(file_path)
                try:
                    os.remove("read.bak")
                except FileNotFoundError:
                    None
            except FileNotFoundError:
                print("Unable to Delete the File.\nThe File Path is not Correct/File not Found.")

    def Peripherals_Read_Via_Address(self, Start_Address):
        """
        Reads the Data of the Peripherals Address.
        Returns the Data Value as a String.
        """
        HELP_VAR = int(Start_Address, 16)
        Help_Address = hex(HELP_VAR + 3)
        End_Address = "0x" + Help_Address[2:].upper()
        Send_CMD_To_Save_File = f"Data.SAVE.Ascii read.txt SD:{Start_Address}--{End_Address} /Hex "
        try:
            self.cmd(Send_CMD_To_Save_File)
        except BaseException:
            print("Unable to Save the File./nDebug Port Fail./nSet the Trace32 in Required Condition for Reading Peripherals Address.")
        finally:
            time.sleep(6)
            File_Name = "../../../instrumentation/Lauterbach/read.txt"
            # DIR = Path.cwd()
            # print('STEP-1 ', DIR)
            Value = self.read_and_Del_File(File_Name)
            return Value
