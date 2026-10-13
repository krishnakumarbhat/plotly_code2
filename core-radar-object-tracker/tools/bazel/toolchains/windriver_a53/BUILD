load("@bazel_platform//toolchain/windriver_v7:cc_toolchain_config.bzl", "cc_toolchain_config")

TARGET = ["-tARMARCH64CORTEXA53MH:simple"]

COPTS = TARGET + [
]

LINKEROPTS = TARGET + [
    "-m6",
    "-lc",
    "-limpl",
]

ASMOPTS = TARGET + [
]

DEFINES = [
]

toolchain(
    name = "windriver_a53_toolchain",
    exec_compatible_with = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
    target_compatible_with = [
        "@bazel_platform//platform/cpu:arm-a53",
    ],
    toolchain = ":windriver_cc_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
    visibility = ["//visibility:public"],
)

cc_toolchain(
    name = "windriver_cc_toolchain",
    all_files = "@windriver_diab_windows//:all_files",
    compiler_files = "@windriver_diab_windows//:all_files",
    dwp_files = "@windriver_diab_windows//:all_files",
    linker_files = "@windriver_diab_windows//:all_files",
    objcopy_files = "@windriver_diab_windows//:all_files",
    strip_files = "@windriver_diab_windows//:all_files",
    supports_header_parsing = True,
    supports_param_files = 0,
    toolchain_config = ":cc_toolchain_config_config",
    toolchain_identifier = "windriver-toolchain",
)

cc_toolchain_config(
    name = "cc_toolchain_config_config",
    as_executable = "@windriver_diab_windows//:diab-7.0.5.0/WIN64/bin/dcc.exe",
    asmopts = ASMOPTS,
    copts = COPTS,
    cppopts = [],

    # paths to tools
    dar_executable = "@windriver_diab_windows//:diab-7.0.5.0/WIN64/bin/dar.exe",
    dcc_executable = "@windriver_diab_windows//:diab-7.0.5.0/WIN64/bin/dcc.exe",
    defines = DEFINES,
    dld_executable = "@windriver_diab_windows//:diab-7.0.5.0/WIN64/bin/dld.exe",
    dplus_executable = "@windriver_diab_windows//:diab-7.0.5.0/WIN64/bin/dplus.exe",
    linkopts = LINKEROPTS,
    strip_executable = "@windriver_diab_windows//:diab-7.0.5.0/WIN64/bin/striparm.exe",
)
