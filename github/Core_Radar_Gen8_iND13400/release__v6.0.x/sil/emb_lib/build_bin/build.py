#!/usr/bin/env python
"""Build script for SIL embedded library."""

from sys import stdout
from typing import List
import platform
import subprocess
import os
import argparse


def get_bazelisk_exe() -> List[str]:
    """Get bazelisk executable + output_base depending on platform."""
    # https://gitgerrit.asux.aptiv.com/plugins/gitiles/bazel_platform/+/refs/heads/master/docs/quality/ut/using_coverage.md#too-long-paths-under-windows
    # We can't add output_base to .bazelrc, because there is no way to selectively set this option only on Windows.
    if platform.system() == "Windows":
        return ["../../../bazelisk.exe", "--output_base=C:/d"]
    else:
        return ["../../../bazelisk"]


class colours:
    """List of colours."""

    RED = "\033[0;31m"
    YELLOW = "\033[0;33m"
    BLUE = "\033[0;34m"
    CYAN = "\033[0;36m"
    GREEN = "\033[0;32m"
    RESET = "\033[0;0m"
    BOLD = "\033[;1m"
    REVERSE = "\033[;7m"
    UNDERLINE = "\033[0;4m"


ROT_OUT = [
    "disabled",
    "partner_sensor",
    "platform_srr8p_standalone",
    "platform_flr8_standalone",
    "platform_srr8p_2_sensor_fusion",
    "AL",
]
TARGET_CHOICES = [
    "dynamic library",
    "static library",
    "statically linked executable",
    "runtime-loading executable",
]
TARGET_BAZEL = {
    "dynamic library": "//sil/emb_lib/sil_source:emb_lib_sharedlib",
    "static library": "//sil/emb_lib/sil_source:emb_lib_static",
    "statically linked executable": "//sil/emb_lib/sil_source/test:test_static",
    "runtime-loading executable": "//sil/emb_lib/sil_source/test:test_dlopen",
}

VARIANT = [
    "srr8p",
    "flr8",
]

CUSTOMER = [
    "gpo",
    "al",
]

# DBC_VERSION = [
#     "v3.0",
#     "v2.0",
# ]

CONFIG = [
    "emblib_debug",
    "emblib_release",
]

SIL_MODE = [
    "RDD",
    "AF",
    "DETECT",
]

FEATURE_FUNCTION = [
    "true",
    "false",
]


if platform.system() == "Windows":
    PATH_TO_COPY = r"C:\Git\10028634_01_SRR_RESIM_Release\RESIM_SIL\GEN8_SIL_ENGINE"
else:
    PATH_TO_COPY = (
        "/media/aptiv/DATADRIVE2/git/10028634_01_SRR_RESIM_Release/RESIM_SIL/GEN7_SIL_ENGINE"
    )


def print_underline(text: str):
    """Print text with underline."""
    stdout.write(colours.UNDERLINE)
    print(text)
    stdout.write(colours.RESET)


def run_platform_script(variant: str, customer: str = "gpo"):
    """Script running function."""
    path = os.path.normpath(PATH_TO_COPY)

    if not os.path.exists(path):
        print(
            f"{colours.RED}Path {path} does not exist. Skipping post-build script.{colours.RESET}"
        )
        return

    if platform.system() == "Windows":
        if customer == "al":
            script = "copy_position_dll_AL.bat"
        elif variant == "srr8p":
            script = "copy_position_dll_srr.bat"
        elif variant == "flr8":
            script = "copy_position_dll_flr.bat"
        else:
            return

        try:
            print(colours.RESET + f"Running {script} {path}")
            result = subprocess.run(
                [script, path], check=False, shell=True, capture_output=True, text=True
            )
            output = (result.stdout or "") + (result.stderr or "")
            if output:
                for line in output.splitlines():
                    lower_line = line.lower()
                    if "cannot access the file" in lower_line or "0 file(s) copied" in lower_line:
                        print(f"{colours.RED}{line}{colours.RESET}")
                    elif "file(s) copied" in lower_line:
                        print(f"{colours.GREEN}{line}{colours.RESET}")
                    else:
                        print(line)

            lower_output = output.lower()
            has_copy_error = (
                "cannot access the file" in lower_output
                or "0 file(s) copied" in lower_output
                or result.returncode != 0
            )
            if has_copy_error:
                print(f"{colours.RED}Failed to copy files to {path}.{colours.RESET}")
            else:
                print(f"{colours.GREEN}Successfully copied files to {path}!{colours.RESET}")
        except OSError:
            print(f"{colours.RED}Failed to copy files to {path}.{colours.RESET}")

    else:
        if variant == "srr8p":
            script = "./copy_position_so_srr.sh"
        elif variant == "flr8":
            script = "./copy_position_so_flr.sh"
        else:
            return

        try:
            print(colours.RESET + f"Running {script} {path}")
            os.chmod(script, 0o755)
            subprocess.run([script, path], check=True)
            print(f"{colours.GREEN}✅ Successfully copied files to {path}!{colours.RESET}")
        except subprocess.CalledProcessError:
            pass

        second_path = "/media/aptiv/DATADRIVE2/Docker/platform/creation/GEN7/OUTPUT"
        if os.path.exists(second_path):
            try:
                print(colours.RESET + f"Running {script} {second_path}")
                subprocess.run([script, second_path], check=True)
                print(
                    f"{colours.GREEN}✅ Successfully copied files to {second_path}!{colours.RESET}"
                )
            except subprocess.CalledProcessError:
                pass


def build_prompt():
    """Entry point."""
    # needed to make windows conhost print colours
    os.system("")

    variant_choice = ""
    while not variant_choice:
        print_underline("Select variant:")
        for idx, variant in enumerate(VARIANT):
            print("\t" + str(idx + 1) + ") " + variant)
        variant_choice = input()
    variant = VARIANT[int(variant_choice) - 1]

    print_underline("Select SIL mode (default RDD):")
    for idx, sil_choice in enumerate(SIL_MODE):
        print("\t" + str(idx + 1) + ") " + sil_choice)
    sil_mode = SIL_MODE[int(input() or "1") - 1]

    print_underline("Enable Feature Function (default false):")
    for idx, feature_choice in enumerate(FEATURE_FUNCTION):
        print("\t" + str(idx + 1) + ") " + feature_choice)
    feature_function = FEATURE_FUNCTION[int(input() or "2") - 1]

    print_underline("Select ROT out (default disabled):")
    for idx, rot_out_choice in enumerate(ROT_OUT):
        print("\t" + str(idx + 1) + ") " + rot_out_choice)
    rot_out = ROT_OUT[int(input() or "1") - 1]

    print_underline("Select customer (default gpo):")
    for idx, customer_choice in enumerate(CUSTOMER):
        print("\t" + str(idx + 1) + ") " + customer_choice)
    customer = CUSTOMER[int(input() or "1") - 1]

    print_underline("Select target (default dynamic library):")
    for idx, choice in enumerate(TARGET_CHOICES):
        print("\t" + str(idx + 1) + ") " + choice)
    target_choice = TARGET_CHOICES[int(input() or "1") - 1]
    target = TARGET_BAZEL[target_choice]

    # print_underline("Select DBC Version (default latest version):")
    # for idx, dbc_version in enumerate(DBC_VERSION):
    #     print("\t" + str(idx + 1) + ") " + dbc_version)
    # dbc_version = DBC_VERSION[int(input() or "1") - 1]

    print_underline("Select build configuration (default emblib_debug):")
    for idx, config in enumerate(CONFIG):
        print("\t" + str(idx + 1) + ") " + config)
    config = CONFIG[int(input() or "1") - 1]

    stdout.write(colours.CYAN)
    print_underline("Configuration is:")
    stdout.write(colours.YELLOW)

    print("\ttarget: " + target_choice)
    print("\tSIL mode: " + sil_mode)
    print("\tROT out: " + rot_out)
    print("\tFeature Function: " + feature_function)
    print("\tcustomer: " + customer)
    print("\tvariant: " + variant)
    # print("\tDBC version: " + dbc_version)
    print("\tbuild mode: " + config)
    stdout.write(colours.RESET)

    build_args = [
        "build",
        target,
        ("--config=emblib"),
        ("--//sil/emb_lib/rsp_emb_lib:sil_mode=" + sil_mode),
        ("--tracker_variant=" + rot_out),
        ("--enable_features=" + feature_function),
        ("--//sil/emb_lib/sil_source:customer=" + customer),
        # ("--//sil/emb_lib/sil_source:dbc_version=" + dbc_version),
        ("--@build_config//:variant=" + variant),
        ("--config=" + config),
        # ("--noremote_accept_cached"), #Use only when debugging build issues, otherwise it can cause significant slowdown due to not using remote cache
    ]

    if customer == "al" and rot_out == "AL":
        build_args.append("--veh_com=al_can_standalone")

    print(
        colours.GREEN
        + "bazel command: "
        + colours.RESET
        + colours.BOLD
        + (" ").join(build_args)
        + colours.RESET
    )

    build_success = False

    try:
        subprocess.run(get_bazelisk_exe() + build_args, check=True)
        build_success = True
        print(colours.BOLD + colours.GREEN + "Successfully built configuration:")
    except subprocess.CalledProcessError:
        print(colours.BOLD + colours.RED + "Failed to build configuration:")
    print("\ttarget: " + target_choice)
    print("\tSIL mode: " + sil_mode)
    print("\tROT out: " + rot_out)
    print("\tFeature Function: " + feature_function)
    print("\tcustomer: " + customer)
    print("\tvariant: " + variant)
    # print("\tDBC version: " + dbc_version)
    print("\tbuild mode: " + config)
    stdout.write(colours.RESET)

    if build_success:
        print(colours.GREEN + "Running post-build script..." + colours.RESET)
        run_platform_script(variant, customer)


def build_all():
    """Build all variant and ROT combinations."""
    for variant in VARIANT:
        for rot_out in ROT_OUT:
            build_args = [
                "build",
                "//sil/emb_lib/sil_source:emb_lib_sharedlib",
                "--//sil/emb_lib/rsp_emb_lib:sil_mode=RDD",
                ("--tracker_variant=" + rot_out),
                ("--enable_features=false"),
                "--//sil/emb_lib/sil_source:customer=gpo",
                # ("--//sil/emb_lib/sil_source:dbc_version=v3.0"),
                ("--@build_config//:variant=" + variant),
                ("--config=emblib_debug"),
                ("--enable_features=false"),
            ]
            try:
                subprocess.run(get_bazelisk_exe() + build_args, check=True, capture_output=True)
                run_platform_script(variant)
                print(colours.BOLD + colours.GREEN + "Successfully built configuration:")
            except subprocess.CalledProcessError:
                print(colours.BOLD + colours.RED + "Failed to build configuration:")

            print("\tROT OUT: " + rot_out)
            print("\tvariant: " + variant)
            stdout.write(colours.RESET)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--all", action="store_true", default=False, help="build all configurations"
    )

    subparsers = parser.add_subparsers(title="subcommands", help="additional help")
    # subparser for `clean' subcommand
    clean_parser = subparsers.add_parser("clean", help="clean the output")
    clean_parser.add_argument(
        "--expunge", action="store_true", default=False, help="delete dependencies too"
    )

    args = parser.parse_args()
    if hasattr(args, "expunge"):
        if args.expunge:
            subprocess.run(get_bazelisk_exe() + ["clean", "--expunge"])
        else:
            subprocess.run(get_bazelisk_exe() + ["clean"])
    else:
        if args.all:
            build_all()
        else:
            build_prompt()
