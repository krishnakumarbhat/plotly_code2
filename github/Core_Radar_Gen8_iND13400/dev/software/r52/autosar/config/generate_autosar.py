#!/usr/bin/env python3
"""
Automated AUTOSAR Code Generation Script.

This script automates the generation of AUTOSAR BSW/MCAL configuration files
using DaVinci Configurator Command Line (DVCfgCmd.exe) without manual GUI intervention.

Usage:
    python generate_autosar.py                          # Generate all modules
    python generate_autosar.py --validate               # Validate only (no generation)
    python generate_autosar.py --modules Dio Port Gpt   # Generate specific modules
    python generate_autosar.py --modules Gpt Mcu --hardware-variant B0  # Set HW variant and generate
    python generate_autosar.py --verbose                # Enable verbose output
"""

import argparse
import json
import logging
import subprocess
import sys
from pathlib import Path
from typing import List, Optional, Dict


class AutosarCodeGenerator:
    """Handles automated AUTOSAR code generation using DaVinci Configurator CLI."""

    def __init__(self, repo_root: Optional[Path] = None, verbose: bool = False):
        """
        Initialize the AUTOSAR code generator.

        Args:
            repo_root: Repository root path. Auto-detected if None.
            verbose: Enable verbose logging output.
        """
        self.repo_root = repo_root or self._find_repo_root()
        self.config_dir = self.repo_root / "software" / "r52" / "autosar" / "config"
        self.sip_base = self.repo_root / "software" / "r52" / "autosar" / "sip"
        self.project_file = self.config_dir / "Gen8_iND13400.dpa"

        self._setup_logging(verbose)
        self._validate_environment()

    def _find_repo_root(self) -> Path:
        """Auto-detect repository root by searching for MODULE.bazel."""
        current = Path(__file__).resolve().parent
        while current != current.parent:
            if (current / "MODULE.bazel").exists():
                return current
            current = current.parent
        raise RuntimeError("Cannot find repository root (MODULE.bazel not found)")

    def _setup_logging(self, verbose: bool):
        """Configure logging output."""
        level = logging.DEBUG if verbose else logging.INFO
        logging.basicConfig(
            level=level,
            format="%(asctime)s - %(levelname)s - %(message)s",
            datefmt="%Y-%m-%d %H:%M:%S",
        )
        self.logger = logging.getLogger(__name__)

    def _validate_environment(self):
        """Validate that all required files and tools exist."""
        self.logger.info("Validating environment...")

        # Check project file exists
        if not self.project_file.exists():
            raise FileNotFoundError(
                f"DaVinci project file not found: {self.project_file}\n"
                f"Expected location: {self.config_dir}"
            )

        # Find DaVinci Configurator executable
        self.dvcfgcmd = self._find_dvcfgcmd()
        if not self.dvcfgcmd:
            self.logger.warning("DVCfgCmd.exe not found. Attempting to download SIP generators...")
            if self._auto_download_sip():
                self.dvcfgcmd = self._find_dvcfgcmd()
                if not self.dvcfgcmd:
                    raise FileNotFoundError(
                        "DVCfgCmd.exe still not found after downloading SIP generators!\n"
                        "Please check the SIP folder structure or download manually."
                    )
            else:
                raise FileNotFoundError(
                    "Failed to download SIP generators automatically!\n\n"
                    "Please run the following command manually:\n"
                    "  cd software\\r52\\autosar\n"
                    "  init_sip_and_download_generators.bat\n\n"
                    "Or ensure the SIP folder is properly initialized."
                )

        # Run repo_copy.py to copy latest MCAL packages
        if not self._run_repo_copy():
            raise RuntimeError(
                "Failed to run repo_copy.py!\n"
                "MCAL package copy is required before code generation."
            )

        # Find automation script
        self.automation_script = self.config_dir / "set_hardware_variant.py"

        self.logger.info(f"✓ Repository root: {self.repo_root}")
        self.logger.info(f"✓ Project file: {self.project_file}")
        self.logger.info(f"✓ DVCfgCmd.exe: {self.dvcfgcmd}")

    def _find_dvcfgcmd(self) -> Optional[Path]:
        """Locate DVCfgCmd.exe in SIP generators folder."""
        if not self.sip_base.exists():
            return None

        # Priority 1: Check direct SIP path (correct location)
        dvcfgcmd_direct = self.sip_base / "DaVinciConfigurator" / "Core" / "DVCfgCmd.exe"
        if dvcfgcmd_direct.exists():
            return dvcfgcmd_direct

        # Priority 2: Search subdirectories (fallback for older structures)
        for sip_variant in self.sip_base.glob("*"):
            if sip_variant.is_dir():
                dvcfgcmd_path = sip_variant / "DaVinciConfigurator" / "Core" / "DVCfgCmd.exe"
                if dvcfgcmd_path.exists():
                    return dvcfgcmd_path
        return None

    def _auto_download_sip(self) -> bool:
        """
        Automatically download SIP generators by running init_sip_and_download_generators.bat.

        Returns:
            True if download succeeded, False otherwise.

        Notes:
            - Waits indefinitely until the batch file completes
            - Returns False immediately if script fails mid-execution
            - No timeout - will wait as long as needed for download to finish
        """
        sip_init_script = (
            self.repo_root
            / "software"
            / "r52"
            / "autosar"
            / "init_sip_and_download_generators.bat"
        )

        if not sip_init_script.exists():
            self.logger.error(f"SIP initialization script not found: {sip_init_script}")
            return False

        self.logger.info("")
        self.logger.info("=" * 70)
        self.logger.info("STEP 1: CHECKING/DOWNLOADING SIP GENERATORS")
        self.logger.info("=" * 70)
        self.logger.info("DOWNLOADING SIP GENERATORS (this may take several minutes)...")
        self.logger.info("=" * 70)

        try:
            # Use Popen to stream output in real-time
            process = subprocess.Popen(
                [str(sip_init_script)],
                cwd=str(sip_init_script.parent),
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,  # Merge stderr into stdout
                text=True,
                encoding="utf-8",
                errors="replace",
                bufsize=1,  # Line buffered
                universal_newlines=True,
            )

            # Stream output line by line in real-time
            for line in process.stdout:
                line = line.rstrip()
                if line:  # Only print non-empty lines
                    self.logger.info(f"  {line}")

            # Wait for process to complete and get return code
            process.wait()
            returncode = process.returncode

            # Check if script failed
            if returncode != 0:
                self.logger.error("")
                self.logger.error("=" * 70)
                self.logger.error(f"SIP DOWNLOAD FAILED (exit code: {returncode})")
                self.logger.error("=" * 70)
                self.logger.error("Possible causes:")
                self.logger.error("  - Network connection issues")
                self.logger.error("  - Authentication/credentials missing (.netrc file)")
                self.logger.error("  - Git submodule not initialized")
                self.logger.error("  - Artifactory server unreachable")
                self.logger.error("")
                self.logger.error("Manual steps:")
                self.logger.error(f"  1. cd {sip_init_script.parent}")
                self.logger.error(f"  2. Run: {sip_init_script.name}")
                self.logger.error("  3. Check output for specific error messages")
                self.logger.error("=" * 70)
                return False

            # Verify download by checking if DVCfgCmd.exe now exists
            dvcfgcmd_path = self._find_dvcfgcmd()
            if dvcfgcmd_path:
                self.logger.info("")
                self.logger.info("=" * 70)
                self.logger.info("✓ SIP GENERATORS DOWNLOADED SUCCESSFULLY")
                self.logger.info(f"✓ DVCfgCmd.exe found at: {dvcfgcmd_path}")
                self.logger.info("=" * 70)
                return True
            else:
                self.logger.error("")
                self.logger.error("=" * 70)
                self.logger.error("SIP DOWNLOAD VERIFICATION FAILED")
                self.logger.error("=" * 70)
                self.logger.error(
                    "The batch file completed successfully, but DVCfgCmd.exe was not found."
                )
                self.logger.error("Expected locations:")
                self.logger.error(
                    f"  - {self.sip_base / 'DaVinciConfigurator' / 'Core' / 'DVCfgCmd.exe'}"
                )
                self.logger.error(
                    f"  - {self.sip_base / '*' / 'DaVinciConfigurator' / 'Core' / 'DVCfgCmd.exe'}"
                )
                self.logger.error("")
                self.logger.error("Check if:")
                self.logger.error("  - SIP folder structure is correct")
                self.logger.error("  - Generators were extracted properly")
                self.logger.error("  - Zip files were downloaded and unzipped")
                self.logger.error("=" * 70)
                return False

        except Exception as e:
            self.logger.error("")
            self.logger.error("=" * 70)
            self.logger.error("SIP DOWNLOAD SCRIPT EXECUTION FAILED")
            self.logger.error("=" * 70)
            self.logger.error(f"Exception: {type(e).__name__}: {e}")
            self.logger.error("")
            self.logger.error("Verify the script exists and is executable:")
            self.logger.error(f"  {sip_init_script}")
            self.logger.error("=" * 70)
            return False

    def _run_repo_copy(self) -> bool:
        """Execute repo_copy.py to copy MCAL packages from repository.

        Returns:
            True if repo_copy.py executed successfully, False otherwise.
        """
        repo_copy_script = (
            self.repo_root / "tools" / "python" / "copy_mcal_package" / "repo_copy.py"
        )

        if not repo_copy_script.exists():
            self.logger.error("")
            self.logger.error("=" * 70)
            self.logger.error("REPO_COPY.PY NOT FOUND")
            self.logger.error("=" * 70)
            self.logger.error(f"Expected location: {repo_copy_script}")
            self.logger.error("=" * 70)
            return False

        try:
            self.logger.info("")
            self.logger.info("=" * 70)
            self.logger.info("STEP 2: COPYING MCAL PACKAGES")
            self.logger.info("=" * 70)
            self.logger.info(f"Running: python {repo_copy_script}")
            self.logger.info("")

            # Run repo_copy.py with real-time output
            process = subprocess.Popen(
                ["python", str(repo_copy_script)],
                cwd=str(self.config_dir),
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                bufsize=1,
                universal_newlines=True,
            )

            # Stream output line by line
            for line in process.stdout:
                line = line.rstrip()
                if line:
                    self.logger.info(f"  {line}")

            process.wait()
            returncode = process.returncode

            if returncode != 0:
                self.logger.error("")
                self.logger.error("=" * 70)
                self.logger.error(f"REPO_COPY.PY FAILED (exit code: {returncode})")
                self.logger.error("=" * 70)
                self.logger.error("Possible causes:")
                self.logger.error("  - Source MCAL packages not found in repository")
                self.logger.error("  - Permission issues copying files")
                self.logger.error("  - Invalid paths in repo_copy.py configuration")
                self.logger.error("")
                self.logger.error("Manual steps:")
                self.logger.error(f"  1. cd {repo_copy_script.parent}")
                self.logger.error(f"  2. Run: python {repo_copy_script.name}")
                self.logger.error("  3. Check output for specific error messages")
                self.logger.error("=" * 70)
                return False

            self.logger.info("")
            self.logger.info("=" * 70)
            self.logger.info("✓ MCAL PACKAGES COPIED SUCCESSFULLY")
            self.logger.info("=" * 70)
            return True

        except Exception as e:
            self.logger.error("")
            self.logger.error("=" * 70)
            self.logger.error(f"REPO_COPY.PY EXECUTION FAILED: {type(e).__name__}: {e}")
            self.logger.error("=" * 70)
            return False

    def validate_configuration(self) -> bool:
        """
        Validate the AUTOSAR configuration without generating code.

        Returns:
            True if validation passes, False otherwise.
        """
        self.logger.info("=" * 70)
        self.logger.info("STEP 3: VALIDATING AUTOSAR CONFIGURATION")
        self.logger.info("=" * 70)

        cmd = [str(self.dvcfgcmd), "--project", str(self.project_file), "--validate"]

        return self._execute_command(cmd, "Validation")

    def set_hardware_variant(self, variant: str, modules: List[str]) -> bool:
        """
        Set hardware variant for specified modules using DaVinci automation script.

        Args:
            variant: Hardware variant ('A0' or 'B0').
            modules: List of module short names to update.

        Returns:
            True if hardware variant was set successfully, False otherwise.
        """
        if not self.automation_script.exists():
            self.logger.error(f"Automation script not found: {self.automation_script}")
            return False

        variant_upper = variant.upper()
        if variant_upper not in ["A0", "B0"]:
            self.logger.error(f"Invalid hardware variant: {variant}. Must be 'A0' or 'B0'")
            return False

        self.logger.info("=" * 70)
        self.logger.info(f"SETTING HARDWARE VARIANT: {variant_upper}")
        self.logger.info(f"Modules: {', '.join(modules)}")
        self.logger.info("=" * 70)

        # Construct DVCfgCmd automation command
        modules_arg = " ".join(modules)
        task_args = f"--variant {variant_upper} --modules {modules_arg}"

        cmd = [
            str(self.dvcfgcmd),
            "--project",
            str(self.project_file),
            "--scriptLocations",
            str(self.config_dir),
            "--scriptTask",
            "SetHardwareVariant",
            "--taskArgs",
            task_args,
        ]

        return self._execute_command(cmd, "Hardware variant configuration")

    def generate_code(
        self, modules: Optional[List[str]] = None, module_paths: Optional[List[str]] = None
    ) -> bool:
        """
        Generate AUTOSAR code from configuration.

        Args:
            modules: List of specific module short names to generate (e.g., ['Dio', 'Port']).
                    If None, generates all modules configured in .dpa file.
            module_paths: List of AUTOSAR definition paths to generate (e.g., ['/MICROSAR/Dio']).
                         If specified, takes precedence over modules parameter.

        Returns:
            True if generation succeeds, False otherwise.
        """
        self.logger.info("=" * 70)
        if module_paths:
            self.logger.info(
                f"STEP 3: GENERATING AUTOSAR CODE - MODULE PATHS: {', '.join(module_paths)}"
            )
        elif modules:
            self.logger.info(f"STEP 3: GENERATING AUTOSAR CODE - MODULES: {', '.join(modules)}")
        else:
            self.logger.info("GENERATING ALL AUTOSAR CODE")
        self.logger.info("=" * 70)

        cmd = [str(self.dvcfgcmd), "--project", str(self.project_file), "--generate"]

        # Add module specifications if provided
        if module_paths:
            # Use AUTOSAR definition paths (more precise)
            modules_str = ",".join(module_paths)
            cmd.extend(["--modulesToGenerate", modules_str])
        elif modules:
            # Use short names (may match multiple modules if names clash)
            modules_str = ",".join(modules)
            cmd.extend(["--modulesToGenerate", modules_str])

        success = self._execute_command(cmd, "Code generation")

        if success:
            self._run_post_generation_scripts()

        return success

    def _execute_command(self, cmd: List[str], operation: str) -> bool:
        """
        Execute DVCfgCmd command and handle output.

        Args:
            cmd: Command and arguments to execute.
            operation: Description of operation for logging.

        Returns:
            True if command succeeds, False otherwise.
        """
        self.logger.debug(f"Executing: {' '.join(cmd)}")

        try:
            result = subprocess.run(
                cmd,
                cwd=str(self.config_dir),
                capture_output=True,
                text=True,
                encoding="utf-8",
                errors="replace",
            )

            # Always log stdout (important for DaVinci output)
            if result.stdout.strip():
                output_level = logging.INFO if result.returncode != 0 else logging.DEBUG
                for line in result.stdout.strip().split("\n"):
                    self.logger.log(output_level, f"  {line}")

            # Always log stderr if present
            if result.stderr.strip():
                for line in result.stderr.strip().split("\n"):
                    self.logger.error(f"  {line}")

            # Check for errors
            if result.returncode != 0:
                self.logger.error(f"{operation} FAILED (exit code: {result.returncode})")
                self.logger.error(f"Check logs at: {self.config_dir / 'Log'}")
                self._parse_and_display_errors()
                return False

            self.logger.info(f"✓ {operation} completed successfully")
            return True

        except FileNotFoundError:
            self.logger.error(f"Command not found: {cmd[0]}")
            return False
        except Exception as e:
            self.logger.error(f"{operation} failed with exception: {e}")
            return False

    def _parse_and_display_errors(self):
        """Parse the latest log file and display module-specific errors."""
        log_dir = self.config_dir / "Log"
        if not log_dir.exists():
            return

        # Find the most recent log file
        log_files = list(log_dir.glob("*.log"))
        if not log_files:
            return

        latest_log = max(log_files, key=lambda p: p.stat().st_mtime)

        try:
            with open(latest_log, "r", encoding="utf-8", errors="replace") as f:
                log_content = f.read()

            # Extract module-specific errors
            failed_modules = set()
            lines = log_content.split("\n")

            for i, line in enumerate(lines):
                # Look for generator failure patterns
                if "Generator could not be started" in line or "Generation failed" in line:
                    # Try to extract module name from the line
                    for word in line.split():
                        if word.startswith("/") and "/" in word[1:]:
                            module = word.split("/")[-1].rstrip(":")
                            failed_modules.add(module)
                        elif word.endswith("Generator") or word.endswith("Configurator"):
                            module = (
                                word.replace("Generator", "").replace("Configurator", "").strip()
                            )
                            if module:
                                failed_modules.add(module)

                # Look for ECUC validation errors with module names
                if "AR-ECUC" in line and i > 0:
                    prev_line = lines[i - 1]
                    for word in prev_line.split():
                        if "/" in word:
                            parts = word.split("/")
                            if len(parts) >= 3:
                                failed_modules.add(parts[2])

            if failed_modules:
                self.logger.error("")
                self.logger.error("=" * 70)
                self.logger.error("FAILED MODULES:")
                self.logger.error("=" * 70)
                for module in sorted(failed_modules):
                    self.logger.error(f"  ✗ {module}")
                self.logger.error("=" * 70)
                self.logger.error("")

        except Exception as e:
            self.logger.debug(f"Could not parse error details: {e}")

    def _run_post_generation_scripts(self):
        """Execute post-generation scripts if they exist."""
        post_gen_scripts = self.config_dir / "Post_Gen_Scripts"

        if not post_gen_scripts.exists():
            return

        # Check for Post_OS_Gen.bat
        post_os_script = post_gen_scripts / "Post_OS_Gen.bat"
        if post_os_script.exists():
            self.logger.info("Running post-generation script: Post_OS_Gen.bat")
            try:
                subprocess.run(
                    [str(post_os_script)],
                    cwd=str(self.config_dir),
                    check=True,
                    capture_output=True,
                )
                self.logger.info("✓ Post-generation script completed")
            except subprocess.CalledProcessError as e:
                self.logger.warning(f"Post-generation script failed: {e}")

    def get_generation_logs(self) -> Optional[Path]:
        """
        Get the latest generation report/log file.

        Returns:
            Path to latest log directory if found, None otherwise.
        """
        log_dir = self.config_dir / "Log"
        if not log_dir.exists():
            return None

        # Find most recent GenerationReport directory
        log_folders = sorted(
            [d for d in log_dir.glob("GenerationReport_*") if d.is_dir()],
            key=lambda x: x.name,
            reverse=True,
        )

        return log_folders[0] if log_folders else None

    def get_module_results(self, requested_modules: Optional[List[str]] = None) -> Dict[str, str]:
        """
        Parse generation output and return per-module validation/generation results.

        Args:
            requested_modules: List of modules requested for generation (short names).

        Returns:
            Dictionary mapping module name to result status (SUCCESSFUL/FAILED).
        """
        log_dir = self.config_dir / "Log"
        output_json = log_dir / "GenerationOutput.json"

        if not output_json.exists():
            self.logger.warning(
                "GenerationOutput.json not found, cannot determine module-wise results"
            )
            return {}

        try:
            with open(output_json, "r", encoding="utf-8") as f:
                data = json.load(f)

            module_results = {}
            for module_info in data.get("modules", []):
                identifier = module_info.get("identifier", "")
                result = module_info.get("result", "UNKNOWN")

                # Extract short name from identifier (e.g., "/indieSemi/Dio" -> "Dio")
                if "/" in identifier:
                    short_name = identifier.split("/")[-1]
                else:
                    short_name = identifier

                module_results[short_name] = result

            # If specific modules were requested, filter results
            if requested_modules:
                filtered_results = {}
                for module in requested_modules:
                    # Case-insensitive lookup
                    for key, value in module_results.items():
                        if key.lower() == module.lower():
                            filtered_results[module] = value
                            break
                    else:
                        # Module not found in results
                        filtered_results[module] = "NOT_GENERATED"
                return filtered_results

            return module_results

        except Exception as e:
            self.logger.warning(f"Failed to parse GenerationOutput.json: {e}")
            return {}

    def print_module_summary(self, requested_modules: Optional[List[str]] = None):
        """
        Print a formatted summary of module generation results.

        Args:
            requested_modules: List of modules that were requested for generation.
        """
        module_results = self.get_module_results(requested_modules)

        if not module_results:
            return

        self.logger.info("=" * 70)
        self.logger.info("MODULE VALIDATION & GENERATION SUMMARY")
        self.logger.info("=" * 70)

        # Sort by module name for consistent output
        for module, result in sorted(module_results.items()):
            status_symbol = "✓" if result == "SUCCESSFUL" else "✗"
            status_color = "SUCCESS" if result == "SUCCESSFUL" else "FAILED"

            self.logger.info(f"  {status_symbol} {module:20s} - {status_color}")

        self.logger.info("=" * 70)


def main():
    """Main entry point for command-line usage."""
    parser = argparse.ArgumentParser(
        description="Automated AUTOSAR Code Generation using DaVinci Configurator CLI",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Generate all modules
  python generate_autosar.py

  # Validate configuration only
  python generate_autosar.py --validate

  # Generate specific modules
  python generate_autosar.py --modules Dio Port Gpt Mcu

  # Set hardware variant and generate
  python generate_autosar.py --modules Gpt Mcu Adc --hardware-variant B0

  # Verbose output
  python generate_autosar.py --verbose

  # Validate and generate
  python generate_autosar.py --validate --generate
        """,
    )

    parser.add_argument(
        "--validate", action="store_true", help="Validate configuration without generating code"
    )

    parser.add_argument(
        "--generate", action="store_true", default=True, help="Generate code (default behavior)"
    )

    parser.add_argument(
        "--modules",
        nargs="+",
        metavar="MODULE",
        help="Specific modules to generate (e.g., Dio Port Gpt). If not specified, generates all.",
    )

    parser.add_argument(
        "--hardware-variant",
        choices=["A0", "B0", "a0", "b0"],
        help="Set hardware variant (A0 or B0) for specified modules before generation",
    )

    parser.add_argument("--verbose", "-v", action="store_true", help="Enable verbose debug output")

    parser.add_argument(
        "--repo-root", type=Path, help="Repository root path (auto-detected if not specified)"
    )

    args = parser.parse_args()

    try:
        # Initialize generator
        generator = AutosarCodeGenerator(repo_root=args.repo_root, verbose=args.verbose)

        success = True

        # Run validation if requested
        if args.validate:
            success = generator.validate_configuration()
            if not success:
                sys.exit(1)
            if not args.generate and args.validate:
                # If only validating, exit here
                sys.exit(0)

        # Set hardware variant if requested (before generation)
        if args.hardware_variant and args.modules:
            variant_success = generator.set_hardware_variant(
                variant=args.hardware_variant, modules=args.modules
            )
            if not variant_success:
                logging.error("Failed to set hardware variant")
                sys.exit(1)
        elif args.hardware_variant and not args.modules:
            logging.error("--hardware-variant requires --modules to be specified")
            sys.exit(1)

        # Run code generation
        if args.generate or (not args.validate):
            success = generator.generate_code(modules=args.modules)

        # Show module-wise results
        if success and (args.generate or not args.validate):
            generator.print_module_summary(requested_modules=args.modules)

        # Show log location
        if success:
            log_path = generator.get_generation_logs()
            if log_path:
                print("\n" + "=" * 70)
                print("Generation logs available at:")
                print(f"  {log_path}")
                print("=" * 70)

        sys.exit(0 if success else 1)

    except Exception as e:
        logging.error(f"Fatal error: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
