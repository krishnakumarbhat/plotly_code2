load("@bazel_platform//toolchain/gcovr:toolchain.bzl", "gcovr_toolchain_config")

gcovr_toolchain_config(
    name = "gcovr_config",
    gcov = "@mingw64_10_0_0_rev0//:bin/gcov.exe",
    gcovr_flags = [
        "-b",
        "-e",
        ".*fake.*",
    ],
)

toolchain(
    name = "gcovr_windows_toolchain",
    toolchain = ":gcovr_config",
    toolchain_type = "@bazel_platform//toolchain/gcovr:toolchain_type",
)
