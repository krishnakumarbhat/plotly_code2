load("@bazel_platform//toolchain/windriver_v7:cc_toolchain_config.bzl", "cc_toolchain_config")

TARGET = ["-tARMCORTEXM7MF:simple"]
# TARGET = ["-##", "-tARMCORTEXM7MF:simple"]

COPTS = TARGET + [
    "-Xlicense-wait",
    "-ffunction-sections",
    "-fdata-sections",
    "-g",
]

LINKEROPTS = TARGET + [
    "-m6",
    "-lc",
    "-limpl",
    "-Xremove-unused-sections",
]

ASMOPTS = TARGET + [
]

DEFINES = [
    "CPU_CORTEX_M7",
    "ARMCM7_SP",
    "SAF85XX",
]

toolchain(
    name = "windriver_m7_toolchain",
    exec_compatible_with = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
    target_compatible_with = [
        "@bazel_platform//platform/cpu:arm-m7",
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
    compiler_inc_paths = ["@windriver_diab_windows//:compiler_include_directories"],
    copts = COPTS,
    cppopts = [],
    dar_executable = "@windriver_diab_windows//:diab-7.0.5.0/WIN64/bin/dar.exe",
    dcc_executable = "@windriver_diab_windows//:diab-7.0.5.0/WIN64/bin/dcc.exe",
    defines = DEFINES,
    dld_executable = "@windriver_diab_windows//:diab-7.0.5.0/WIN64/bin/dld.exe",
    dplus_executable = "@windriver_diab_windows//:diab-7.0.5.0/WIN64/bin/dplus.exe",
    enable_lto = True,
    linkopts = LINKEROPTS,
    path_env_paths = ["@windriver_diab_windows//:path_env_directories"],
    strip_executable = "@windriver_diab_windows//:diab-7.0.5.0/WIN64/bin/striparm.exe",
)

toolchain(
    name = "windriver_m7_toolchain_linux",
    exec_compatible_with = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
    target_compatible_with = [
        "@bazel_platform//platform/cpu:arm-m7",
    ],
    toolchain = ":windriver_cc_toolchain_linux",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
    visibility = ["//visibility:public"],
)

cc_toolchain(
    name = "windriver_cc_toolchain_linux",
    all_files = "@windriver_diab_linux//:all_files",
    ar_files = "@windriver_diab_linux//:all_files",
    as_files = "@windriver_diab_linux//:all_files",
    compiler_files = "@windriver_diab_linux//:all_files",
    dwp_files = "@windriver_diab_linux//:all_files",
    linker_files = "@windriver_diab_linux//:all_files",
    objcopy_files = "@windriver_diab_linux//:all_files",
    strip_files = "@windriver_diab_linux//:all_files",
    supports_header_parsing = True,
    supports_param_files = 0,
    toolchain_config = ":cc_toolchain_config_config_linux",
    toolchain_identifier = "windriver-toolchain",
)

cc_toolchain_config(
    name = "cc_toolchain_config_config_linux",
    as_executable = "@windriver_diab_linux//:diab-7.0.5.0/LINUX64/bin/dcc",
    asmopts = ASMOPTS,
    copts = COPTS,
    cppopts = [],

    # paths to tools
    dar_executable = "@windriver_diab_linux//:diab-7.0.5.0/LINUX64/bin/dar",
    dcc_executable = "@windriver_diab_linux//:diab-7.0.5.0/LINUX64/bin/dcc",
    defines = DEFINES,
    dld_executable = "@windriver_diab_linux//:diab-7.0.5.0/LINUX64/bin/dld",
    dplus_executable = "@windriver_diab_linux//:diab-7.0.5.0/LINUX64/bin/dplus",
    enable_lto = True,
    linkopts = LINKEROPTS,
    strip_executable = "@windriver_diab_linux//:diab-7.0.5.0/LINUX64/bin/striparm",
)
