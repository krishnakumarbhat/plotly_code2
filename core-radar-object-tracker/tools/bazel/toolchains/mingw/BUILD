load("@bazel_platform//toolchain/mingw:cc_toolchain_config.bzl", "cc_toolchain_config")

package(default_visibility = ["//visibility:public"])

# default toolchain on windows x86_64
toolchain(
    name = "mingw_windows_toolchain",
    exec_compatible_with = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
    target_compatible_with = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
    target_settings = [
        "@bazel_platform//toolchain:compiler_gcc",
    ],
    toolchain = ":mingw_cc_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
)

cc_toolchain(
    name = "mingw_cc_toolchain",
    all_files = "@mingw64_10_0_0_rev0//:all_files",
    compiler_files = "@mingw64_10_0_0_rev0//:compiler_files",
    dwp_files = "@mingw64_10_0_0_rev0//:dwp_files",
    linker_files = "@mingw64_10_0_0_rev0//:linker_files",
    objcopy_files = "@mingw64_10_0_0_rev0//:objcopy_files",
    strip_files = "@mingw64_10_0_0_rev0//:strip_files",
    toolchain_config = ":mingw_cc_toolchain_config",
)

cc_toolchain_config(
    name = "mingw_cc_toolchain_config",
    ar_executable = "@mingw64_10_0_0_rev0//:bin/ar.exe",
    copts = [
        "--coverage",
    ],
    defines = ["DEFINED_BY_TOOLCHAIN"],
    gcc_executable = "@mingw64_10_0_0_rev0//:bin/gcc.exe",
    gcov_executable = "@mingw64_10_0_0_rev0//:bin/gcov.exe",
    gplusplus_executable = "@mingw64_10_0_0_rev0//:bin/g++.exe",
    ld_executable = "@mingw64_10_0_0_rev0//:bin/ld.exe",
    linkopts = ["--coverage"],
)
