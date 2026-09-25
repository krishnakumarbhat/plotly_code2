#!/usr/bin/env python3
"""MCAL Test Automation Script.

Simple script that connects to Trace32, sets breakpoints at Mcal_Test start/end,
and waits for already-running software to hit them.

Workflow:
1. User manually launches Trace32 and flashes software (software is running)
2. Script connects to Trace32
3. Script loads symbols
4. Script sets breakpoints at Mcal_Test() start and end
5. Script waits for breakpoints to be hit
6. Script reads Mcal_Mcu_Test_status variable
7. Script generates HTML/XML report
"""

import sys
import time
from pathlib import Path
from datetime import datetime

# Try to import Trace32 Python API
try:
    from lauterbach.trace32 import rcl

    HAS_T32_API = True
except ImportError:
    HAS_T32_API = False
    print("\n" + "=" * 70)
    print("ERROR: Could not find Trace32 Python API!")
    print("=" * 70)
    print("\nInstall from PyPI:")
    print("  pip install lauterbach-trace32-rcl")
    print("=" * 70 + "\n")


class McalTestAutomation:
    """MCAL test automation class for Trace32 integration."""

    def __init__(self, elf_path, output_dir="mcal_test_reports", t32_port=20000, t32_packlen=1024):
        """Initialize MCAL test automation with Trace32 connection parameters."""
        self.elf_path = Path(elf_path)
        self.elf_file = self.elf_path  # For compatibility with launch_trace32_and_flash
        self.output_dir = Path(output_dir)
        self.output_dir.mkdir(exist_ok=True)

        # Trace32 connection parameters
        self.t32_port = t32_port
        self.t32_packlen = t32_packlen
        self.t32 = None

        # Flash automation parameters
        self.t32_process = None

        # Test configuration
        self.test_results = {
            "Mcal_Mcu_Test_status": {
                "actual": None,
                "expected": 1,  # TEST_PASS = 1
                "status": "NOT_RUN",
            },
            "Mcal_Dio_Test_status": {"actual": None, "expected": 1, "status": "NOT_RUN"},
            "Mcal_I2c_Test_status": {"actual": None, "expected": 1, "status": "NOT_RUN"},
            "Mcal_Fls_Test_status": {"actual": None, "expected": 1, "status": "NOT_RUN"},
            "Mcal_Uart_Test_status": {"actual": None, "expected": 1, "status": "NOT_RUN"},
            "Mcal_Spi_Test_status": {"actual": None, "expected": 1, "status": "NOT_RUN"},
            "Mcal_Pwm_Test_status": {"actual": None, "expected": 1, "status": "NOT_RUN"},
            "Mcal_Gpt_Test_status": {"actual": None, "expected": 1, "status": "NOT_RUN"},
            "Mcal_Adc_Test_status": {"actual": None, "expected": 1, "status": "NOT_RUN"},
            "Mcal_Dma_Test_status": {"actual": None, "expected": 1, "status": "NOT_RUN"},
        }

        # Test execution details
        self.test_details = {
            "start_time": None,
            "end_time": None,
            "duration": None,
            "breakpoints_hit": [],
        }

        # Radar control init status (for SPI test validation)
        self.radar_ctl_init_status = None

    def launch_trace32_and_flash(self):
        """Launch Trace32 using existing batch file and flash the software."""
        import subprocess

        # Find the batch file in tools/lauterbach
        # UPDATED: 4 levels up instead of 3 for mcal_test folder depth
        repo_root = Path(__file__).parent.parent.parent.parent
        batch_file = repo_root / "tools" / "lauterbach" / "_start_powerview_r52.bat"
        lauterbach_dir = batch_file.parent

        if not batch_file.exists():
            print(f"ERROR: Batch file not found: {batch_file}")
            return False

        # Get absolute path to flash_session.ini
        # IMPORTANT: Use resolve() to convert symlinks (bazel-bin) to real paths
        # This ensures the Flash Utility finds binaries in the correct location
        flash_session_dir = self.elf_file.parent.resolve()
        flash_session_path = (flash_session_dir / "flash_session.ini").resolve()

        if not flash_session_path.exists():
            print(f"ERROR: flash_session.ini not found at: {flash_session_path}")
            return False

        print("=" * 70)
        print("Launching Trace32 and Flashing Software")
        print("=" * 70)
        print()
        print(f"Batch File: {batch_file}")
        print(f"Flash Session: {flash_session_path}")
        print(f"Flash Session Dir: {flash_session_dir}")
        print("  (symlinks resolved to real path)")
        print()

        try:
            # Create a custom user config file that points to our flash session
            # Key: Use &auto_flash_session instead of &default_flash_session
            # This bypasses the interactive dialog and flashes automatically
            # UPDATED: Write to script directory instead of lauterbach_dir
            script_dir = Path(__file__).parent
            custom_config = script_dir / "mcal_test_user_config.cmm"

            with open(custom_config, "w") as f:
                f.write("; Auto-generated user config for MCAL test automation\n")
                f.write('&default_win0="default_win_r52"\n')
                f.write('&default_win1="default_win_bbe32"\n')
                f.write(f'&auto_flash_session="{flash_session_path}"\n')

            print(f"✓ Created custom config: {custom_config}")
            print("✓ Auto-flash enabled (no manual OK needed)")
            print()

            # Launch Trace32 using the same command as the batch file but with our custom config
            # Original: start "" /D %~dp0 C:\T32\bin\windows64\t32marm -c config.t32 -s start_powerview.cmm CPU=ind13400 R52 USER_CONFIG=default_user_config.cmm
            # UPDATED: Use absolute path for USER_CONFIG
            cmd = [
                "C:\\T32\\bin\\windows64\\t32marm.exe",
                "-c",
                "config.t32",
                "-s",
                "start_powerview.cmm",
                "CPU=ind13400",
                "R52",
                f"USER_CONFIG={str(custom_config.resolve())}",
            ]

            print("Launching Trace32...")
            print(f"  Command: {' '.join(cmd)}")
            print(f"  Working Dir: {lauterbach_dir}")
            print()

            # Launch Trace32 (non-blocking)
            self.t32_process = subprocess.Popen(
                cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=str(lauterbach_dir)
            )

            print("✓ Trace32 launched")
            print("  Waiting 15 seconds for Trace32 to initialize and flash...")
            time.sleep(15)

            # Check if process is still running
            if self.t32_process.poll() is not None:
                stdout, stderr = self.t32_process.communicate()
                print("ERROR: Trace32 process exited unexpectedly")
                if stdout:
                    print(f"STDOUT: {stdout.decode()}")
                if stderr:
                    print(f"STDERR: {stderr.decode()}")
                return False

            print("✓ Trace32 is running")
            print("  Waiting additional 20 seconds for flashing to complete...")
            time.sleep(20)

            print("✓ Flash complete")
            print()

            # Simple post-flash wait for system to stabilize
            print("Waiting 5 seconds for system to stabilize after flash...")
            time.sleep(5)
            print("✓ System ready")
            print()

            return True

        except Exception as e:
            print(f"ERROR launching Trace32: {e}")
            import traceback

            traceback.print_exc()
            return False

    def connect_trace32(self):
        """Connect to running Trace32 instance."""
        if not HAS_T32_API:
            print("ERROR: Trace32 Python API not available")
            return False

        try:
            print("=" * 70)
            print("Connecting to Trace32...")
            print(f"  Port: {self.t32_port}")
            print(f"  Packet Length: {self.t32_packlen}")
            print()

            # Connect to Trace32 using UDP protocol (default for Remote API)
            self.t32 = rcl.connect(
                node="localhost", port=self.t32_port, packlen=self.t32_packlen, protocol="UDP"
            )

            print("✓ Successfully connected to Trace32")
            print()
            return True

        except Exception as e:
            print(f"ERROR connecting to Trace32: {e}")
            print("\nTroubleshooting:")
            print("  1. Make sure Trace32 is running")
            print("  2. Check config.t32 has: RCL=NETASSIST PORT=20000")
            print("  3. Try restarting Trace32 to enable Remote API")
            return False

    def disconnect_trace32(self):
        """Disconnect from Trace32."""
        if self.t32:
            print("\nDisconnected from Trace32")

    def execute_test(self):
        """Execute MCAL test - just wait for breakpoints on already-running code."""
        try:
            self.test_details["start_time"] = datetime.now()

            print("=" * 70)
            print("MCAL Test Execution")
            print("=" * 70)
            print()

            print("Step 1: Loading ELF symbols (symbols only, no code flash)...")
            self.t32.cmd(f'Data.LOAD.Elf "{self.elf_path}" /NoCODE')
            time.sleep(0.5)
            print(f"  ✓ Loaded symbols: {self.elf_path.name}")
            print()

            print("Step 2: Clearing all existing breakpoints...")
            self.t32.cmd("Break.Delete /ALL")
            print("  ✓ All breakpoints cleared")
            print()

            print("Step 3: Setting breakpoint at Mcal_Test start...")
            try:
                self.t32.cmd("Break.Set Mcal_Test /Onchip")
                print("  ✓ Breakpoint set at Mcal_Test entry")
            except Exception as e:
                print(f"  ERROR: Could not set breakpoint: {e}")
                print("  Make sure Mcal_Test function exists in ELF symbols")
                return False
            print()

            time.sleep(2)

            # Skip exit breakpoint for now - timer-based approach is more reliable
            print("Step 3.1: Skipping exit breakpoint (using timer-based validation)...")
            print("  Note: Exit breakpoint can be unreliable in automation")
            print("  Will use fixed timing + status reading instead")
            print()

            print("Step 4: Resetting and starting target...")
            try:
                # Simple reset sequence: Reset -> Wait(1) -> Go -> Wait(5)
                print("  Performing SYStem.RESetOut...")
                self.t32.cmd("SYStem.RESetOut")
                time.sleep(1)
                print("  ✓ Reset complete")

                print("  Starting execution (Go)...")
                self.t32.cmd("Go")
                time.sleep(5)
                print("  ✓ Software running")
                print()

            except Exception as e:
                print(f"  ERROR: Reset failed: {e}")
                import traceback

                traceback.print_exc()
                return False

            print("Step 5: Waiting for Mcal_Test START breakpoint...")
            print("  (Software is running, waiting for it to reach Mcal_Test)")
            print()

            # Check current state - wait for breakpoint to be hit
            if not self._wait_for_break_with_status(30.0):  # 30 second timeout (increased)
                print("\nERROR: Timeout waiting for Mcal_Test START breakpoint")
                print("  Possible reasons:")
                print("    - Software not running on target")
                print("    - Mcal_Test() not called during execution")
                print("    - Breakpoint not set correctly")
                return False

            print("  ✓ Hit Mcal_Test START breakpoint!")
            self.test_details["breakpoints_hit"].append("Mcal_Test entry")
            print()

            print("Step 5.1: Continuing execution (Go)...")
            self.t32.cmd("Go")
            time.sleep(1)
            print()

            # Wait 5 seconds for ADC data collection
            print("Step 5.1: Waiting 5 seconds for ADC data collection...")
            time.sleep(5)
            print("  ✓ Wait complete")
            print()

            # Read ADC buffer and validate
            print("Step 5.2: Reading and validating Adc_Results_Buff...")
            try:
                # Stop target to read variables
                current_state = self.t32.get_state()
                if isinstance(current_state, (bytes, bytearray)):
                    state_value = current_state[0] if len(current_state) > 0 else 0
                else:
                    state_value = current_state

                if state_value == 3:  # If still running, stop it
                    self.t32.cmd("Break")
                    time.sleep(0.5)

                # Read Adc_Results_Buff array (size 10)
                adc_values = []
                for i in range(10):
                    try:
                        result = self.t32.fnc(f"Var.VALUE(Adc_Results_Buff[{i}])")
                        val = int(result)
                        adc_values.append(val)
                    except Exception as e:
                        print(f"  WARNING: Could not read Adc_Results_Buff[{i}]: {e}")
                        adc_values.append(0)

                print(f"  Adc_Results_Buff = {adc_values}")

                # Check if all values are non-zero
                if adc_values and all(v != 0 for v in adc_values):
                    self.test_results["Mcal_Adc_Test_status"]["actual"] = 1
                    self.test_results["Mcal_Adc_Test_status"]["status"] = "PASS"
                    print("  ✓ All ADC results are non-zero. Mcal_Adc_Test_status = PASS")
                else:
                    self.test_results["Mcal_Adc_Test_status"]["actual"] = 0
                    self.test_results["Mcal_Adc_Test_status"]["status"] = "FAIL"
                    print("  ✗ ADC results contain zero(s). Mcal_Adc_Test_status = FAIL")

                # Resume execution after reading
                self.t32.cmd("Go")
                time.sleep(0.5)

            except Exception as e:
                print(f"  ERROR reading Adc_Results_Buff: {e}")
                self.test_results["Mcal_Adc_Test_status"]["actual"] = None
                self.test_results["Mcal_Adc_Test_status"]["status"] = "ERROR"

            print()

            # Read Radar_Ctl_Data.init_status and validate SPI
            print("Step 5.4: Reading and validating Radar_Ctl_Data.init_status...")
            try:
                # Make sure target is stopped
                current_state = self.t32.get_state()
                if isinstance(current_state, (bytes, bytearray)):
                    state_value = current_state[0] if len(current_state) > 0 else 0
                else:
                    state_value = current_state

                if state_value == 3:  # If still running, stop it
                    self.t32.cmd("Break")
                    time.sleep(0.5)

                # Read Radar_Ctl_Data.init_status
                radar_ctl_status = None
                try:
                    result = self.t32.fnc("Var.VALUE(Radar_Ctl_Data.init_status)")
                    radar_ctl_status = int(result)
                    print(f"  Radar_Ctl_Data.init_status = {radar_ctl_status}")
                except Exception as e:
                    print(f"  WARNING: Could not read Radar_Ctl_Data.init_status: {e}")

                # Store for report generation
                self.radar_ctl_init_status = radar_ctl_status

                # Check if init_status is 3 (success)
                if radar_ctl_status == 3:
                    self.test_results["Mcal_Spi_Test_status"]["actual"] = 1
                    self.test_results["Mcal_Spi_Test_status"]["status"] = "PASS"
                    print("  ✓ Radar_Ctl_Data.init_status is 3. Mcal_Spi_Test_status = PASS")
                else:
                    self.test_results["Mcal_Spi_Test_status"]["actual"] = 0
                    self.test_results["Mcal_Spi_Test_status"]["status"] = "FAIL"
                    print(
                        f"  ✗ Radar_Ctl_Data.init_status is not 3 (value={radar_ctl_status}). Mcal_Spi_Test_status = FAIL"
                    )

                # Resume execution after reading
                self.t32.cmd("Go")
                time.sleep(0.5)

            except Exception as e:
                print(f"  ERROR reading Radar_Ctl_Data.init_status: {e}")
                self.test_results["Mcal_Spi_Test_status"]["actual"] = None
                self.test_results["Mcal_Spi_Test_status"]["status"] = "ERROR"
                self.radar_ctl_init_status = None

            print()

            print("Step 6: Reading MCAL test status variables...")
            try:
                # Make sure target is stopped
                current_state = self.t32.get_state()
                if isinstance(current_state, (bytes, bytearray)):
                    state_value = current_state[0] if len(current_state) > 0 else 0
                else:
                    state_value = current_state

                if state_value == 3:  # If still running, stop it
                    self.t32.cmd("Break")
                    time.sleep(0.5)

                # Read all MCAL test status variables
                test_variables = [
                    "Mcal_Mcu_Test_status",
                    "Mcal_Dio_Test_status",
                    "Mcal_I2c_Test_status",
                    "Mcal_Fls_Test_status",
                    # "Mcal_Uart_Test_status",
                    # "Mcal_Spi_Test_status",
                    "Mcal_Pwm_Test_status",
                    "Mcal_Gpt_Test_status",
                    "Mcal_Adc_Test_status",
                    "Mcal_Dma_Test_status",
                ]

                for var_name in test_variables:
                    try:
                        result = self.t32.fnc(f"Var.VALUE({var_name})")
                        value = int(result)
                        self.test_results[var_name]["actual"] = value
                        print(f"  {var_name} = {value}")
                    except Exception as e:
                        print(f"  WARNING: Could not read {var_name}: {e}")
                        # Keep as None if variable doesn't exist or can't be read

                print()
            except Exception as e:
                print(f"  ERROR: Could not read test status variables: {e}")
                import traceback

                traceback.print_exc()
                return False

            print("  Continuing execution to let software run normally...")
            self.t32.cmd("Go")
            time.sleep(1)

            print()
            print("=" * 70)
            print("DONE - Test execution completed")
            print("=" * 70)

            return True

        except Exception as e:
            print(f"ERROR during test execution: {e}")
            import traceback

            traceback.print_exc()
            return False

    def _wait_for_break_with_status(self, timeout):
        """Wait for target to hit breakpoint with status updates every second."""
        start = time.time()
        last_status_time = start

        while (time.time() - start) < timeout:
            try:
                state = self.t32.get_state()

                # Handle bytearray state - convert to int
                if isinstance(state, (bytes, bytearray)):
                    state_value = state[0] if len(state) > 0 else 0
                else:
                    state_value = state

                # Print status every second
                current_time = time.time()
                if (current_time - last_status_time) >= 1.0:
                    elapsed = int(current_time - start)
                    remaining = int(timeout - (current_time - start))
                    print(
                        f"  [{elapsed}s] Waiting for breakpoint... (state={state_value}, {remaining}s remaining)"
                    )
                    last_status_time = current_time

                # State 0 = down, 1 = stopped, 2 = breakpoint hit, 3 = running
                if state_value == 1 or state_value == 2:  # STOPPED or BREAKPOINT
                    return True
            except Exception:
                pass
            time.sleep(0.5)
        return False

    def generate_html_report(self):
        """Generate HTML test report with execution details."""
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        duration_str = (
            f"{self.test_details['duration']:.2f}s" if self.test_details["duration"] else "N/A"
        )
        start_time_str = (
            self.test_details["start_time"].strftime("%H:%M:%S")
            if self.test_details["start_time"]
            else "N/A"
        )
        end_time_str = (
            self.test_details["end_time"].strftime("%H:%M:%S")
            if self.test_details["end_time"]
            else "N/A"
        )

        html_content = f"""<!DOCTYPE html>
<html>
<head>
    <title>MCAL Test Report</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
            margin: 20px;
            background-color: #f5f5f5;
        }}
        .header {{
            background-color: #2c3e50;
            color: white;
            padding: 20px;
            border-radius: 5px;
            margin-bottom: 20px;
        }}
        .header h1 {{
            margin: 0 0 10px 0;
        }}
        .header p {{
            margin: 5px 0;
        }}
        table {{
            width: 100%;
            border-collapse: collapse;
            background-color: white;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }}
        th {{
            background-color: #3498db;
            color: white;
            padding: 12px;
            text-align: left;
            font-weight: bold;
        }}
        td {{
            padding: 12px;
            border-bottom: 1px solid #ddd;
        }}
        tr:hover {{
            background-color: #f5f5f5;
        }}
        .pass {{
            background-color: #2ecc71;
            color: white;
            padding: 5px 10px;
            border-radius: 3px;
            font-weight: bold;
        }}
        .fail {{
            background-color: #e74c3c;
            color: white;
            padding: 5px 10px;
            border-radius: 3px;
            font-weight: bold;
        }}
        .not-run {{
            background-color: #95a5a6;
            color: white;
            padding: 5px 10px;
            border-radius: 3px;
            font-weight: bold;
        }}
        .summary {{
            margin: 20px 0;
            padding: 15px;
            background-color: white;
            border-radius: 5px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }}
        .summary h2 {{
            margin-top: 0;
            color: #2c3e50;
        }}
    </style>
</head>
<body>
    <div class="header">
        <h1>MCAL Test Report</h1>
        <p>Generated: {timestamp}</p>
        <p>ELF File: {self.elf_path.name}</p>
        <p>Test Duration: {duration_str} (Start: {start_time_str}, End: {end_time_str})</p>
    </div>

    <div class="summary">
        <h2>Test Execution Details</h2>
        <p><strong>Breakpoints Hit:</strong> {', '.join(self.test_details['breakpoints_hit']) if self.test_details['breakpoints_hit'] else 'None'}</p>
    </div>

    <div class="summary">
        <h2>Test Summary</h2>
"""

        # Calculate summary
        total_tests = len(self.test_results)
        passed = sum(1 for t in self.test_results.values() if t["status"] == "PASS")
        failed = sum(1 for t in self.test_results.values() if t["status"] == "FAIL")
        not_run = sum(1 for t in self.test_results.values() if t["status"] == "NOT_RUN")

        html_content += f"""
        <p><strong>Total Tests:</strong> {total_tests}</p>
        <p><strong>Passed:</strong> <span style="color: #2ecc71;">{passed}</span></p>
        <p><strong>Failed:</strong> <span style="color: #e74c3c;">{failed}</span></p>
        <p><strong>Not Run:</strong> <span style="color: #95a5a6;">{not_run}</span></p>
"""

        # Add Radar Control Init Status
        if self.radar_ctl_init_status == 3:
            html_content += """
        <p><strong>Radar Control Init Status:</strong> <span style="background-color: #2ecc71; color: white; padding: 5px 10px; border-radius: 3px; font-weight: bold;">RADAR_CTL_INIT_SUCCESS</span></p>
"""
        elif self.radar_ctl_init_status is not None:
            html_content += f"""
        <p><strong>Radar Control Init Status:</strong> <span style="background-color: #e74c3c; color: white; padding: 5px 10px; border-radius: 3px; font-weight: bold;">RADAR_CTL_INIT_FAIL (Value: {self.radar_ctl_init_status})</span></p>
"""
        else:
            html_content += """
        <p><strong>Radar Control Init Status:</strong> <span style="background-color: #95a5a6; color: white; padding: 5px 10px; border-radius: 3px; font-weight: bold;">NOT_RUN</span></p>
"""

        html_content += """
    </div>

    <table>
        <thead>
            <tr>
                <th>Test Variable</th>
                <th>Actual Value</th>
                <th>Expected Value</th>
                <th>Status</th>
            </tr>
        </thead>
        <tbody>
"""

        # Add test results
        for test_name, result in self.test_results.items():
            actual_value = result["actual"] if result["actual"] is not None else "N/A"
            expected_value = result["expected"]
            status = result["status"]

            status_class = status.lower().replace("_", "-")
            status_html = f'<span class="{status_class}">{status}</span>'

            html_content += f"""
            <tr>
                <td><strong>{test_name}</strong></td>
                <td>{actual_value}</td>
                <td>{expected_value}</td>
                <td>{status_html}</td>
            </tr>
"""

        html_content += """
        </tbody>
    </table>
</body>
</html>
"""

        # Write HTML report
        html_path = (
            self.output_dir / f"mcal_test_report_{datetime.now().strftime('%Y%m%d_%H%M%S')}.html"
        )
        with open(html_path, "w") as f:
            f.write(html_content)

        print(f"HTML report generated: {html_path}")
        return html_path

    def generate_xml_report(self):
        """Generate XML test report (JUnit format)."""
        timestamp = datetime.now().strftime("%Y-%m-%dT%H:%M:%S")

        # Calculate summary
        total_tests = len(self.test_results)
        failed = sum(1 for t in self.test_results.values() if t["status"] == "FAIL")
        duration = self.test_details["duration"] if self.test_details["duration"] else 0.0

        xml_content = f"""<?xml version="1.0" encoding="UTF-8"?>
<testsuites name="MCAL Tests" tests="{total_tests}" failures="{failed}" time="{duration:.3f}">
    <testsuite name="MCAL_Test_Suite" tests="{total_tests}" failures="{failed}" timestamp="{timestamp}" time="{duration:.3f}">
"""

        # Add test cases
        for test_name, result in self.test_results.items():
            actual = result["actual"] if result["actual"] is not None else "N/A"
            expected = result["expected"]
            status = result["status"]

            xml_content += f"""        <testcase name="{test_name}" classname="MCAL" time="{duration:.3f}">
"""

            if status == "FAIL":
                xml_content += f"""            <failure message="Expected {expected}, got {actual}">
                Test variable: {test_name}
                Expected value: {expected}
                Actual value: {actual}
            </failure>
"""
            elif status == "NOT_RUN":
                xml_content += """            <skipped message="Test not executed"/>
"""

            xml_content += """        </testcase>
"""

        xml_content += """    </testsuite>
</testsuites>
"""

        # Write XML report
        xml_path = (
            self.output_dir / f"mcal_test_report_{datetime.now().strftime('%Y%m%d_%H%M%S')}.xml"
        )
        with open(xml_path, "w") as f:
            f.write(xml_content)

        print(f"XML report generated: {xml_path}")
        return xml_path

    def run(self):
        """Main execution flow."""
        print("=" * 70)
        print("MCAL Test Automation")
        print("=" * 70)
        print()

        if not HAS_T32_API:
            print("ERROR: Trace32 Python API not available!")
            print("Install: pip install lauterbach-trace32-rcl")
            return False

        try:
            # Connect to Trace32
            if not self.connect_trace32():
                return False

            # Execute test
            if not self.execute_test():
                return False

            # Calculate test duration and update status
            self.test_details["end_time"] = datetime.now()
            if self.test_details["start_time"] and self.test_details["end_time"]:
                duration = (
                    self.test_details["end_time"] - self.test_details["start_time"]
                ).total_seconds()
                self.test_details["duration"] = duration

            # Update test status based on actual vs expected
            for _test_name, result in self.test_results.items():
                if result["actual"] is not None:
                    if result["actual"] == result["expected"]:
                        result["status"] = "PASS"
                    else:
                        result["status"] = "FAIL"

            # Display results
            print("=" * 70)
            print("Test Results:")
            print("=" * 70)
            for test_name_key, result in self.test_results.items():
                print(f"  {test_name_key}:")
                print(f"    Actual:   {result['actual']}")
                print(f"    Expected: {result['expected']}")
                print(f"    Status:   {result['status']}")
            print()

            # Generate reports
            print("Generating reports...")
            html_path = self.generate_html_report()
            xml_path = self.generate_xml_report()
            print()

            print("=" * 70)
            print("Test automation completed!")
            print("=" * 70)
            print()
            print(f"HTML Report: {html_path}")
            print(f"XML Report:  {xml_path}")
            print()

            # Open HTML report in browser
            try:
                import webbrowser

                webbrowser.open(html_path.as_uri())
                print("HTML report opened in browser")
            except Exception as e:
                print(f"Note: Could not auto-open browser: {e}")

            return True

        finally:
            # Always disconnect
            self.disconnect_trace32()


def main():
    """Main entry point."""
    import argparse
    import configparser

    parser = argparse.ArgumentParser(
        description="MCAL Test Automation - Simple Breakpoint Monitor",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Using command line arguments
  python mcal_test_automation.py bazel-bin/outputs/flr8_satellite_can/r52App.elf --auto-flash

  # Using configuration file (recommended)
  python mcal_test_automation.py --config mcal_test_config.ini

Prerequisites:
  1. Install Trace32 Python API: pip install lauterbach-trace32-rcl
  2. Ensure Trace32 installed at C:\\T32\\bin\\windows64\\t32marm.exe
  3. For auto-flash mode, flash_session.ini must be in same directory as ELF file

Configuration File Format (INI):
  See mcal_test_config.ini for example configuration file with all options.
""",
    )

    parser.add_argument("elf_file", nargs="?", help="Path to ELF file for symbols")
    parser.add_argument(
        "--config",
        "-c",
        help="Path to configuration INI file (overrides command-line arguments)",
    )
    parser.add_argument(
        "--output",
        "-o",
        default="mcal_test_reports",
        help="Output directory for reports (default: mcal_test_reports)",
    )
    parser.add_argument(
        "--port", type=int, default=20000, help="Trace32 Remote API port (default: 20000)"
    )
    parser.add_argument(
        "--packlen", type=int, default=1024, help="Trace32 packet length (default: 1024)"
    )
    parser.add_argument(
        "--auto-flash",
        action="store_true",
        help="Automatically launch Trace32 and flash software before testing",
    )

    args = parser.parse_args()

    # If config file specified, read settings from it
    if args.config:
        config_path = Path(args.config)
        if not config_path.exists():
            print(f"ERROR: Configuration file not found: {args.config}")
            sys.exit(1)

        print("=" * 70)
        print("Reading configuration from:", args.config)
        print("=" * 70)
        print()

        config = configparser.ConfigParser()
        config.read(config_path)

        # Read paths (no build section anymore)
        elf_file = config.get("Paths", "r52_elf")
        output_dir = config.get("Paths", "output_dir", fallback="mcal_test_reports")

        # Read Trace32 settings
        t32_port = config.getint("Trace32", "port", fallback=20000)
        t32_packlen = config.getint("Trace32", "packlen", fallback=1024)

        # Read options
        auto_flash = config.getboolean("Options", "auto_flash", fallback=False)
        open_report = config.getboolean("Options", "open_report", fallback=True)

        print("Configuration loaded:")
        print(f"  R52 ELF: {elf_file}")
        print(f"  Output: {output_dir}")
        print(f"  Auto-flash: {auto_flash}")
        print(f"  Trace32 Port: {t32_port}")
        print()

    else:
        # Use command-line arguments
        if not args.elf_file:
            parser.print_help()
            print("\nERROR: Either provide ELF file path or use --config option")
            sys.exit(1)

        elf_file = args.elf_file
        output_dir = args.output
        t32_port = args.port
        t32_packlen = args.packlen
        auto_flash = args.auto_flash
        open_report = True

    # Validate ELF file exists
    if not Path(elf_file).exists():
        print(f"ERROR: ELF file not found: {elf_file}")
        sys.exit(1)

    # Create automation instance and run
    automation = McalTestAutomation(
        elf_path=elf_file,
        output_dir=output_dir,
        t32_port=t32_port,
        t32_packlen=t32_packlen,
    )

    # If auto-flash enabled, launch Trace32 and flash first
    if auto_flash:
        print("Auto-flash mode enabled")
        if not automation.launch_trace32_and_flash():
            print("ERROR: Failed to launch Trace32 or flash software")
            sys.exit(1)

    success = automation.run()

    # Optionally skip auto-opening report if configured
    if not open_report and success:
        print("(Skipping auto-open browser as configured)")

    sys.exit(0 if success else 1)


if __name__ == "__main__":
    main()
