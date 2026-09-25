"""
Macro to build a complete EVB Diagnostics Tool test suite.

This macro generates the build targets for BBE code, KQ8 code, SPT code, and M7 code,
including compilation, linking, image generation, and size analysis.
"""

load("//tools/bazel/toolchains/bbe32:cc_binary_bbe32.bzl", "cc_binary_bbe32")
load("//tools/bazel/toolchains/kq8:cc_binary_kq8.bzl", "cc_binary_kq8")
load("//tools/bazel/scripts:transition_bbe32.bzl", "transition_bbe32")
load("//tools/bazel/scripts:transition_kq8.bzl", "transition_kq8")
load("@spt_assembler//:spt_library.bzl", "spt_library")
load("//tools/bazel/scripts:transition_m7.bzl", "transition_m7")
load("//tools/bazel/scripts:transition_design_studio_gcc_m7.bzl", "transition_design_studio_gcc_m7")

def evb_diag_tool_test(
        name,
        m7_test_src,
        build_bbe = True,
        build_kq8 = True,
        spt_0_src = "",
        spt_1_src = "",
        bbe_0_src = "",
        bbe_1_src = "",
        bbe_0_srcs = [],
        bbe_0_deps = [],
        bbe_0_copts = [],
        bbe_0_defines = [],
        bbe_0_linkopts = [],
        bbe_0_hdrs = [],
        bbe_1_srcs = [],
        bbe_1_deps = [],
        bbe_1_copts = [],
        bbe_1_defines = [],
        bbe_1_linkopts = [],
        bbe_1_hdrs = [],
        kq8_0_src = "",
        kq8_1_src = "",
        kq8_0_srcs = [],
        kq8_0_deps = [],
        kq8_0_copts = [],
        kq8_0_defines = [],
        kq8_0_linkopts = [],
        kq8_0_hdrs = [],
        kq8_1_srcs = [],
        kq8_1_deps = [],
        kq8_1_copts = [],
        kq8_1_defines = [],
        kq8_1_linkopts = [],
        kq8_1_hdrs = [],
        spt_0_srcs = [],
        spt_0_deps = [],
        spt_0_linkopts = [],
        spt_1_srcs = [],
        spt_1_deps = [],
        spt_1_linkopts = [],
        m7_srcs = [],
        m7_deps = [],
        m7_copts = [],
        m7_linkopts = [],
        m7_hdrs = []):
    """
    Build a complete EVB Diagnostics Tool test suite.

    Args:
        name: The base name for generated targets (final target will be this name).
              Also provided to M7 and BBE source files as a TEST_NAME define.
        m7_test_src: Test source file for M7 core.
          build_bbe: (Optional) If False, skip all BBE compile/link/image generation targets and
                 do not include generated BBE images in the M7 build. Defaults to True.
        spt_0_src: (Optional) Source file for SPT core 0 library. Defaults to "src/main_spt_0_default.pspt".
        spt_1_src: (Optional) Source file for SPT core 1 library. Defaults to "src/main_spt_1_default.pspt".
        bbe_0_src: (Optional) Source file for BBE core 0 main library. Defaults to "src/main_bbe_0_default.c".
        bbe_1_src: (Optional) Source file for BBE core 1 main library. Defaults to "src/main_bbe_1_default.c".
        bbe_0_srcs: (Optional) Additional source files for BBE core 0. Defaults to [].
        bbe_0_deps: (Optional) Additional cc_library dependencies for BBE core 0. Defaults to [].
        bbe_0_copts: (Optional) Additional compiler options for BBE core 0. Defaults to [].
        bbe_0_defines: (Optional) Preprocessor defines for BBE core 0. Defaults to [].
        bbe_0_linkopts: (Optional) Linker options for BBE core 0 binary. Defaults to [].
        bbe_0_hdrs: (Optional) Header files for BBE core 0 to expose via `hdrs` on the generated cc_library. Defaults to [].
        bbe_1_srcs: (Optional) Additional source files for BBE core 1. Defaults to [].
        bbe_1_deps: (Optional) Additional cc_library dependencies for BBE core 1. Defaults to [].
        bbe_1_copts: (Optional) Additional compiler options for BBE core 1. Defaults to [].
        bbe_1_defines: (Optional) Preprocessor defines for BBE core 1. Defaults to [].
        bbe_1_linkopts: (Optional) Linker options for BBE core 1 binary. Defaults to [].
        bbe_1_hdrs: (Optional) Header files for BBE core 1 to expose via `hdrs` on the generated cc_library. Defaults to [].
        spt_0_srcs: (Optional) Additional source files for SPT core 0. Defaults to [].
        spt_0_deps: (Optional) Additional cc_library dependencies for SPT core 0. Defaults to [].
        spt_0_linkopts: (Optional) Additional linker options for SPT core 0. Defaults to [].
        spt_1_srcs: (Optional) Additional source files for SPT core 1. Defaults to [].
        spt_1_deps: (Optional) Additional cc_library dependencies for SPT core 1. Defaults to [].
        spt_1_linkopts: (Optional) Additional linker options for SPT core 1. Defaults to [].
        m7_srcs: (Optional) Additional source files for M7 core. Defaults to [].
        m7_deps: (Optional) Additional cc_library dependencies for M7 core. Defaults to [].
        m7_copts: (Optional) Additional compiler options for M7 core. Defaults to [].
        m7_linkopts: (Optional) Additional linker options for M7 core. Defaults to [].
        m7_hdrs: (Optional) Header files for M7 core to expose via `hdrs` on the generated cc_library. Defaults to [].
    """
    # ====================================================================
    # Argument Preparation
    # ====================================================================

    def _set_source_list(main_source_as_string, additional_source_as_array, default_source_as_string):
        sources1 = ([main_source_as_string] if main_source_as_string else []) + additional_source_as_array
        sources2 = sources1 if sources1 else [default_source_as_string]
        return sources2

    # Assign defaults if no sources are specified
    spt_0_source = _set_source_list(spt_0_src, spt_0_srcs, "//internal/evb_diag_tool/base:src/main_spt_0_default.pspt")
    spt_1_source = _set_source_list(spt_1_src, spt_1_srcs, "//internal/evb_diag_tool/base:src/main_spt_1_default.pspt")
    bbe_0_source = _set_source_list(bbe_0_src, bbe_0_srcs, "//internal/evb_diag_tool/base:src/main_bbe_0_default.c")
    bbe_1_source = _set_source_list(bbe_1_src, bbe_1_srcs, "//internal/evb_diag_tool/base:src/main_bbe_1_default.c")
    kq8_0_source = _set_source_list(kq8_0_src, kq8_0_srcs, "//internal/evb_diag_tool/base:src/main_kq8_0_default.c")
    kq8_1_source = _set_source_list(kq8_1_src, kq8_1_srcs, "//internal/evb_diag_tool/base:src/main_kq8_1_default.c")

    # ====================================================================
    # BBE Code
    # ====================================================================

    bbe_image_srcs = []
    if build_bbe:
        # Compile the BBE code
        native.cc_library(
            name = name + "_bbe_0_main_lib",
            srcs = bbe_0_source,
            hdrs = bbe_0_hdrs,
            copts = bbe_0_copts,
            defines = [
                "BBE_0=1",
                'TEST_NAME=\\"' + name + '\\"',
            ] + bbe_0_defines,
            visibility = ["//visibility:private"],
            deps = [
                "@appl_inclusion_dep//:spbb_include_h",
                "@diag_tool//:bbe_include",
            ] + bbe_0_deps,
        )

        native.cc_library(
            name = name + "_bbe_1_main_lib",
            srcs = bbe_1_source,
            hdrs = bbe_1_hdrs,
            copts = bbe_1_copts,
            defines = [
                "BBE_1=1",
                'TEST_NAME=\\"' + name + '\\"',
            ] + bbe_1_defines,
            visibility = ["//visibility:private"],
            deps = [
                "@appl_inclusion_dep//:spbb_include_h",
                "@diag_tool//:bbe_include",
            ] + bbe_1_deps,
        )

        # Link the BBE elf files
        cc_binary_bbe32(
            name = name + "_bbe_0_bin",
            srcs = [],
            copts = bbe_0_copts,
            defines = bbe_0_defines,
            linkopts = bbe_0_linkopts,
            visibility = ["//visibility:private"],
            deps = [
                "@appl_inclusion_dep//:spbb_include_h",
                ":" + name + "_bbe_0_main_lib",
                "//internal/evb_diag_tool/base:bbe_0_support_lib",
            ] + bbe_0_deps,
        )

        cc_binary_bbe32(
            name = name + "_bbe_1_bin",
            srcs = [],
            copts = bbe_1_copts,
            defines = bbe_1_defines,
            linkopts = bbe_1_linkopts,
            visibility = ["//visibility:private"],
            deps = [
                "@appl_inclusion_dep//:spbb_include_h",
                ":" + name + "_bbe_1_main_lib",
                "//internal/evb_diag_tool/base:bbe_1_support_lib",
            ] + bbe_1_deps,
        )

        # Convert the BBE elf files to C arrays, to be included in the M7 image
        # The conversion script has these parameters:
        #    ELF_NAME   ${ProjName}_${BBE_CORE}.elf - Input ELF file (e.g., Diag_Tool_BBE_0.elf)
        #    STB_DIR    ${S32DS_S32R47_XTENSA_TOOLCHAIN_DIR}/bin - Path to Xtensa toolchain binaries
        #    OUT_NAME   bbe_${BBE_CORE}_image.c - Output C-array file (e.g., bbe_0_image.c)
        #    PLATFORM   S32R47 - Chip variant
        #    OUT_DIR    Output directory
        #    CORE       BBE32 - Processor type
        #    INST_ID    ${BBE_CORE} - Core number (0 or 1)
        native.genrule(
            name = name + "_bbe_0_image_script",
            srcs = [
                ":" + name + "_bbe_0_bin",
            ],
            outs = [
                name + "_bbe_0_image.c",
                name + "_bbe_0_image_script-debug_out.txt",
            ],
            #    cmd = " ; ".join([
            #        "SCRIPT_FILE=$(location //internal/evb_diag_tool/base/base:scripts/dsp_hex_file_gen.sh)",
            #        "INPUT_FILE='$(RULEDIR)/" + name + "_bbe_0_bin.elf'",
            #        "REPO_ROOT=$(dirname $(rootpath @bbe_linux//:WORKSPACE))",
            #        "TOOLS_PATH=$$REPO_ROOT/tools/RI-2023.11-linux/XtensaTools/bin",
            #        "bash $$SCRIPT_FILE $$INPUT_FILE $$TOOLS_PATH " + name + "_bbe_0_image.c S32R47 $(RULEDIR) BBE32 0 >> $(location " + name + "_bbe_0_image_script-debug_out.txt)",
            #    ]),
            cmd_bat = " && ".join([
                "@echo off",
                "setlocal enabledelayedexpansion",
                "set \"SCRIPT_FILE=$(location //internal/evb_diag_tool/base:scripts/dsp_hex_file_gen.bat)\"",
                "set \"INPUT_FILE=$(RULEDIR)\\" + name + "_bbe_0_bin.elf\"",
                "set \"REPO_PATH=$(rootpath @bbe_windows//:WORKSPACE)\"",
                "set \"REPO_DIR=!REPO_PATH:\\WORKSPACE=\\!\"",
                "set \"TOOLS_PATH=!REPO_DIR!tools\\RI-2023.11-win32\\XtensaTools\\bin\"",
                "call !SCRIPT_FILE! !INPUT_FILE! !TOOLS_PATH! " + name + "_bbe_0_image.c S32R47 $(RULEDIR) BBE32 0 >> $(location " + name + "_bbe_0_image_script-debug_out.txt)",
            ]),
            tools = [
                "//internal/evb_diag_tool/base:scripts/dsp_hex_file_gen.bat",
                "//internal/evb_diag_tool/base:scripts/dsp_hex_file_gen.sh",
                "@bbe_linux//:WORKSPACE",  # Used to recover the external workspace path
                "@bbe_windows//:WORKSPACE",  # Used to recover the external workspace path
            ],
            visibility = ["//visibility:private"],
        )

        native.genrule(
            name = name + "_bbe_1_image_script",
            srcs = [
                ":" + name + "_bbe_1_bin",
            ],
            outs = [
                name + "_bbe_1_image.c",
                name + "_bbe_1_image_script-debug_out.txt",
            ],
            #    cmd = " ; ".join([
            #        "SCRIPT_FILE=$(location //internal/evb_diag_tool/base:scripts/dsp_hex_file_gen.sh)",
            #        "INPUT_FILE='$(RULEDIR)/" + name + "_bbe_1_bin.elf'",
            #        "REPO_ROOT=$(dirname $(rootpath @bbe_linux//:WORKSPACE))",
            #        "TOOLS_PATH=$$REPO_ROOT/tools/RI-2023.11-linux/XtensaTools/bin",
            #        "bash $$SCRIPT_FILE $$INPUT_FILE $$TOOLS_PATH " + name + "_bbe_1_image.c S32R47 $(RULEDIR) BBE32 1 >> $(location " + name + "_bbe_1_image_script-debug_out.txt)",
            #    ]),
            cmd_bat = " && ".join([
                "@echo off",
                "setlocal enabledelayedexpansion",
                "set \"SCRIPT_FILE=$(location //internal/evb_diag_tool/base:scripts/dsp_hex_file_gen.bat)\"",
                "set \"INPUT_FILE=$(RULEDIR)\\" + name + "_bbe_1_bin.elf\"",
                "set \"REPO_PATH=$(rootpath @bbe_windows//:WORKSPACE)\"",
                "set \"REPO_DIR=!REPO_PATH:\\WORKSPACE=\\!\"",
                "set \"TOOLS_PATH=!REPO_DIR!tools\\RI-2023.11-win32\\XtensaTools\\bin\"",
                "call !SCRIPT_FILE! !INPUT_FILE! !TOOLS_PATH! " + name + "_bbe_1_image.c S32R47 $(RULEDIR) BBE32 1 >> $(location " + name + "_bbe_1_image_script-debug_out.txt)",
            ]),
            tools = [
                "//internal/evb_diag_tool/base:scripts/dsp_hex_file_gen.bat",
                "//internal/evb_diag_tool/base:scripts/dsp_hex_file_gen.sh",
                "@bbe_linux//:WORKSPACE",  # Used to recover the external workspace path
                "@bbe_windows//:WORKSPACE",  # Used to recover the external workspace path
            ],
            visibility = ["//visibility:private"],
        )

        # Build the BBE code using the BBE32 toolchain
        transition_bbe32(
            name = name + "_bbe_0_image",
            linker_package = "min-rt-local",
            next_step = ":" + name + "_bbe_0_image_script",
            visibility = ["//visibility:private"],
        )

        transition_bbe32(
            name = name + "_bbe_1_image",
            linker_package = "min-rt-local",
            next_step = ":" + name + "_bbe_1_image_script",
            visibility = ["//visibility:private"],
        )

        bbe_image_srcs = [
            ":" + name + "_bbe_0_image",
            ":" + name + "_bbe_1_image",
        ]

    # ====================================================================
    # KQ8 Code
    # ====================================================================

    kq8_image_srcs = []
    if build_kq8:
        # Compile the KQ8 code
        native.cc_library(
            name = name + "_kq8_0_main_lib",
            srcs = kq8_0_source,
            hdrs = kq8_0_hdrs,
            copts = kq8_0_copts,
            defines = [
                "KQ8_0=1",
                'TEST_NAME=\\"' + name + '\\"',
            ] + kq8_0_defines,
            visibility = ["//visibility:private"],
            deps = [
                "@appl_inclusion_dep//:spbb_include_h",
                "@diag_tool//:kq8_include",
            ] + kq8_0_deps,
        )

        native.cc_library(
            name = name + "_kq8_1_main_lib",
            srcs = kq8_1_source,
            hdrs = kq8_1_hdrs,
            copts = kq8_1_copts,
            defines = [
                "KQ8_1=1",
                'TEST_NAME=\\"' + name + '\\"',
            ] + kq8_1_defines,
            visibility = ["//visibility:private"],
            deps = [
                "@appl_inclusion_dep//:spbb_include_h",
                "@diag_tool//:kq8_include",
            ] + kq8_1_deps,
        )

        # Link the KQ8 elf files
        cc_binary_kq8(
            name = name + "_kq8_0_bin",
            srcs = [],
            copts = kq8_0_copts,
            defines = kq8_0_defines,
            linkopts = kq8_0_linkopts,
            visibility = ["//visibility:private"],
            deps = [
                "@appl_inclusion_dep//:spbb_include_h",
                ":" + name + "_kq8_0_main_lib",
                "//internal/evb_diag_tool/base:kq8_0_support_lib",
            ] + kq8_0_deps,
        )

        cc_binary_kq8(
            name = name + "_kq8_1_bin",
            srcs = [],
            copts = kq8_1_copts,
            defines = kq8_1_defines,
            linkopts = kq8_1_linkopts,
            visibility = ["//visibility:private"],
            deps = [
                "@appl_inclusion_dep//:spbb_include_h",
                ":" + name + "_kq8_1_main_lib",
                "//internal/evb_diag_tool/base:kq8_1_support_lib",
            ] + kq8_1_deps,
        )

        # Convert the KQ8 elf files to C arrays, to be included in the M7 image
        # The conversion script has these parameters:
        #    ELF_NAME   ${ProjName}_${KQ8_CORE}.elf - Input ELF file (e.g., Diag_Tool_KQ8_0.elf)
        #    STB_DIR    ${S32DS_S32R47_XTENSA_TOOLCHAIN_DIR}/bin - Path to Xtensa toolchain binaries
        #    OUT_NAME   bbe_${KQ8_CORE}_image.c - Output C-array file (e.g., kq8_0_image.c)
        #    PLATFORM   S32R47 - Chip variant
        #    OUT_DIR    Output directory
        #    CORE       KQ8 - Processor type
        #    INST_ID    ${KQ8_CORE} - Core number (0 or 1)
        native.genrule(
            name = name + "_kq8_0_image_script",
            srcs = [
                ":" + name + "_kq8_0_bin",
            ],
            outs = [
                name + "_kq8_0_image.c",
                name + "_kq8_0_image_script-debug_out.txt",
            ],
            #    cmd = " ; ".join([
            #        "SCRIPT_FILE=$(location //internal/evb_diag_tool/base/base:scripts/dsp_hex_file_gen.sh)",
            #        "INPUT_FILE='$(RULEDIR)/" + name + "_kq8_0_bin.elf'",
            #        "REPO_ROOT=$(dirname $(rootpath @bbe_linux//:WORKSPACE))",
            #        "TOOLS_PATH=$$REPO_ROOT/tools/RI-2023.11-linux/XtensaTools/bin",
            #        "bash $$SCRIPT_FILE $$INPUT_FILE $$TOOLS_PATH " + name + "_kq8_0_image.c S32R47 $(RULEDIR) KQ8 0 >> $(location " + name + "_kq8_0_image_script-debug_out.txt)",
            #    ]),
            cmd_bat = " && ".join([
                "@echo off",
                "setlocal enabledelayedexpansion",
                "set \"SCRIPT_FILE=$(location //internal/evb_diag_tool/base:scripts/dsp_hex_file_gen_kq8.bat)\"",
                "set \"INPUT_FILE=$(RULEDIR)\\" + name + "_kq8_0_bin.elf\"",
                "set \"REPO_PATH=$(rootpath @bbe_windows//:WORKSPACE)\"",
                "set \"REPO_DIR=!REPO_PATH:\\WORKSPACE=\\!\"",
                "set \"TOOLS_PATH=!REPO_DIR!tools\\RI-2023.11-win32\\XtensaTools\\bin\"",
                "call !SCRIPT_FILE! !INPUT_FILE! !TOOLS_PATH! " + name + "_kq8_0_image.c S32R47 $(RULEDIR) KQ8 0 >> $(location " + name + "_kq8_0_image_script-debug_out.txt)",
            ]),
            tools = [
                "//internal/evb_diag_tool/base:scripts/dsp_hex_file_gen_kq8.bat",
                "//internal/evb_diag_tool/base:scripts/dsp_hex_file_gen_kq8.sh",
                "@bbe_linux//:WORKSPACE",  # Used to recover the external workspace path
                "@bbe_windows//:WORKSPACE",  # Used to recover the external workspace path
            ],
            visibility = ["//visibility:private"],
        )

        native.genrule(
            name = name + "_kq8_1_image_script",
            srcs = [
                ":" + name + "_kq8_1_bin",
            ],
            outs = [
                name + "_kq8_1_image.c",
                name + "_kq8_1_image_script-debug_out.txt",
            ],
            #    cmd = " ; ".join([
            #        "SCRIPT_FILE=$(location //internal/evb_diag_tool/base:scripts/dsp_hex_file_gen.sh)",
            #        "INPUT_FILE='$(RULEDIR)/" + name + "_kq8_1_bin.elf'",
            #        "REPO_ROOT=$(dirname $(rootpath @bbe_linux//:WORKSPACE))",
            #        "TOOLS_PATH=$$REPO_ROOT/tools/RI-2023.11-linux/XtensaTools/bin",
            #        "bash $$SCRIPT_FILE $$INPUT_FILE $$TOOLS_PATH " + name + "_kq8_1_image.c S32R47 $(RULEDIR) KQ8 1 >> $(location " + name + "_kq8_1_image_script-debug_out.txt)",
            #    ]),
            cmd_bat = " && ".join([
                "@echo off",
                "setlocal enabledelayedexpansion",
                "set \"SCRIPT_FILE=$(location //internal/evb_diag_tool/base:scripts/dsp_hex_file_gen_kq8.bat)\"",
                "set \"INPUT_FILE=$(RULEDIR)\\" + name + "_kq8_1_bin.elf\"",
                "set \"REPO_PATH=$(rootpath @bbe_windows//:WORKSPACE)\"",
                "set \"REPO_DIR=!REPO_PATH:\\WORKSPACE=\\!\"",
                "set \"TOOLS_PATH=!REPO_DIR!tools\\RI-2023.11-win32\\XtensaTools\\bin\"",
                "call !SCRIPT_FILE! !INPUT_FILE! !TOOLS_PATH! " + name + "_kq8_1_image.c S32R47 $(RULEDIR) KQ8 1 >> $(location " + name + "_kq8_1_image_script-debug_out.txt)",
            ]),
            tools = [
                "//internal/evb_diag_tool/base:scripts/dsp_hex_file_gen_kq8.bat",
                "//internal/evb_diag_tool/base:scripts/dsp_hex_file_gen_kq8.sh",
                "@bbe_linux//:WORKSPACE",  # Used to recover the external workspace path
                "@bbe_windows//:WORKSPACE",  # Used to recover the external workspace path
            ],
            visibility = ["//visibility:private"],
        )

        # Build the KQ8 code using the KQ8 toolchain
        transition_kq8(
            name = name + "_kq8_0_image",
            linker_package = "min-rt-local",
            next_step = ":" + name + "_kq8_0_image_script",
            visibility = ["//visibility:private"],
        )

        transition_kq8(
            name = name + "_kq8_1_image",
            linker_package = "min-rt-local",
            next_step = ":" + name + "_kq8_1_image_script",
            visibility = ["//visibility:private"],
        )

        kq8_image_srcs = [
            ":" + name + "_kq8_0_image",
            ":" + name + "_kq8_1_image",
        ]

    # ====================================================================
    # SPT Code
    # ====================================================================

    # Build the SPT files as libraries - preprocess, assemble, archive
    spt_library(
        name = name + "_spt_0_lib_make",
        srcs = spt_0_source,
        linkopts = spt_0_linkopts,
        visibility = ["//visibility:private"],
        deps = [
            "@spt_assembler//:sptAssemblerInc",
        ] + spt_0_deps,
    )

    spt_library(
        name = name + "_spt_1_lib_make",
        srcs = spt_1_source,
        linkopts = spt_1_linkopts,
        visibility = ["//visibility:private"],
        deps = [
            "@spt_assembler//:sptAssemblerInc",
        ] + spt_1_deps,
    )

    # Copy and rename the SPT libraries
    native.genrule(
        name = name + "_spt_0_lib",
        srcs = [":" + name + "_spt_0_lib_make"],
        outs = [name + "_libDiag_Tool_SPT_0.aspt"],
        # cmd = "cp $(location :" + name + "_spt_0_lib_make) $(location " + name + "_libDiag_Tool_SPT_0.aspt)",
        cmd_bat = "copy /Y $(location :" + name + "_spt_0_lib_make) $(location " + name + "_libDiag_Tool_SPT_0.aspt)",
        message = "Copying SPT library",
    )

    native.genrule(
        name = name + "_spt_1_lib",
        srcs = [":" + name + "_spt_1_lib_make"],
        outs = [name + "_libDiag_Tool_SPT_1.aspt"],
        # cmd = "cp $(location :" + name + "_spt_1_lib_make) $(location " + name + "_libDiag_Tool_SPT_1.aspt)",
        cmd_bat = "copy /Y $(location :" + name + "_spt_1_lib_make) $(location " + name + "_libDiag_Tool_SPT_1.aspt)",
        message = "Copying SPT library",
    )

    # Build the SPT code using the SPT toolchain
    transition_m7(
        name = name + "_spt_0_all",
        next_step = ":" + name + "_spt_0_lib",
        visibility = ["//visibility:private"],
    )

    transition_m7(
        name = name + "_spt_1_all",
        next_step = ":" + name + "_spt_1_lib",
        visibility = ["//visibility:private"],
    )

    # ====================================================================
    # M7 Code
    # ====================================================================

    # Compile the M7 code
    native.cc_library(
        name = name + "_m7_0_lib",
        srcs = [m7_test_src] + bbe_image_srcs + kq8_image_srcs + m7_srcs,
        hdrs = m7_hdrs,
        copts = [
            "-O0",
            "-g3",
            "-Wall",
        ] + m7_copts,
        defines = [
            "ARMCM7_SP",  # Single Precision Floating Point
            "BOARD_S32R47_EVB=1",  # Targeted for the S32R47 EVB
            'TEST_NAME=\\"' + name + '\\"',
        ],
        visibility = ["//visibility:private"],
        deps = [
            "@diag_tool//:m7_h",
            "@diag_tool//:m7_include",
            "//internal/evb_diag_tool/base:diag_test_enable_h",
        ] + m7_deps,
        alwayslink = True,  # Force all object files to be linked to resolve circular dependencies with the support library
    )

    # Link the M7 elf file
    native.cc_binary(
        name = name + "_m7_0_bin",
        additional_linker_inputs = [
            ":" + name + "_spt_0_all",  # This will provide libDiag_Tool_SPT_0.aspt
            ":" + name + "_spt_1_all",  # This will provide libDiag_Tool_SPT_1.aspt
            "@diag_tool//:s32r47_memory_ld",
            "@diag_tool//:s32r47_sections_m7_0_ld",
        ],
        copts = m7_copts,
        linkopts = [
            "$(location :" + name + "_spt_0_all)",
            "$(location :" + name + "_spt_1_all)",
            "-T $(location @diag_tool//:s32r47_memory_ld)",
            "-T $(location @diag_tool//:s32r47_sections_m7_0_ld)",
            "-Wl,-Map,$(GENDIR)/Diag_Tool_M7_0.map",
        ] + m7_linkopts,
        visibility = ["//visibility:private"],
        deps = [
            ":" + name + "_m7_0_lib",
            "//internal/evb_diag_tool/base:m7_support_lib",
        ] + m7_deps,
    )

    # Extract the M7 map file as a Bazel output and move it to the output location
    native.genrule(
        name = name + "_m7_0_map",
        srcs = [":" + name + "_m7_0_bin"],
        outs = [name + "_Diag_Tool_M7_0.map"],
        # cmd = "mv $(GENDIR)/Diag_Tool_M7_0.map $(location " + name + "_Diag_Tool_M7_0.map)",
        cmd_bat = "move /Y $(GENDIR)\\Diag_Tool_M7_0.map $(location " + name + "_Diag_Tool_M7_0.map)",
        message = "Extracting M7 map file",
        visibility = ["//visibility:private"],
    )

    # Create the M7 binary flash image
    native.genrule(
        name = name + "_m7_0_image",
        srcs = [":" + name + "_m7_0_bin"],
        outs = [name + "_Diag_Tool_M7_0.bin"],
        # cmd = "$(location @design_studio_gcc_m7//:S32DS.3.6.4/S32DS/build_tools/gcc_v11.4/gcc-11.4-arm32-eabi/bin/arm-none-eabi-objcopy) -O binary -R .bss -R .heap -R .stack $(location :" + name + "_m7_0_bin) $(location " + name + "_Diag_Tool_M7_0.bin)",
        cmd_bat = "$(location @design_studio_gcc_m7//:S32DS.3.6.4/S32DS/build_tools/gcc_v11.4/gcc-11.4-arm32-eabi/bin/arm-none-eabi-objcopy.exe) -O binary -R .bss -R .heap -R .stack $(location :" + name + "_m7_0_bin) $(location " + name + "_Diag_Tool_M7_0.bin)",
        message = "Creating flash image",
        tools = [
            # "@design_studio_gcc_m7//:S32DS.3.6.4/S32DS/build_tools/gcc_v11.4/gcc-11.4-arm32-eabi/bin/arm-none-eabi-objcopy",
            "@design_studio_gcc_m7//:S32DS.3.6.4/S32DS/build_tools/gcc_v11.4/gcc-11.4-arm32-eabi/bin/arm-none-eabi-objcopy.exe",
        ],
        visibility = ["//visibility:private"],
    )

    # Print the M7 binary size information
    native.genrule(
        name = name + "_m7_0_size",
        srcs = [":" + name + "_m7_0_bin"],
        outs = [name + "_Diag_Tool_M7_0.siz"],
        # cmd = "$(location @design_studio_gcc_m7//:S32DS.3.6.4/S32DS/build_tools/gcc_v11.4/gcc-11.4-arm32-eabi/bin/arm-none-eabi-size) --format=berkeley $(location :" + name + "_m7_0_bin) > $(location " + name + "_Diag_Tool_M7_0.siz)",
        cmd_bat = "$(location @design_studio_gcc_m7//:S32DS.3.6.4/S32DS/build_tools/gcc_v11.4/gcc-11.4-arm32-eabi/bin/arm-none-eabi-size.exe) --format=berkeley $(location :" + name + "_m7_0_bin) > $(location " + name + "_Diag_Tool_M7_0.siz)",
        message = "Printing size information",
        tools = [
            # "@design_studio_gcc_m7//:S32DS.3.6.4/S32DS/build_tools/gcc_v11.4/gcc-11.4-arm32-eabi/bin/arm-none-eabi-size",
            "@design_studio_gcc_m7//:S32DS.3.6.4/S32DS/build_tools/gcc_v11.4/gcc-11.4-arm32-eabi/bin/arm-none-eabi-size.exe",
        ],
        visibility = ["//visibility:private"],
    )

    # M7 target to build everything
    native.filegroup(
        name = name + "_m7_0_all_files",
        srcs = [
            ":" + name + "_m7_0_bin",
            ":" + name + "_m7_0_map",
            ":" + name + "_m7_0_image",
            ":" + name + "_m7_0_size",
        ],
        visibility = ["//visibility:private"],
    )

    # Build the m7 test code using the M7 toolchain
    transition_design_studio_gcc_m7(
        name = name,
        next_step = ":" + name + "_m7_0_all_files",
        visibility = ["//visibility:public"],
    )
