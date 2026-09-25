"""bbe32 action configuration"""

load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load(
    "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
    "action_config",
    "tool",
)

def declare_actions(ctx):
    """Generate action configurations for the bbe32 toolchain

    Args:
      ctx: The context of the calling rule
    Returns:
      A list of action configurations
    """
    action_configs = [
        action_config(
            action_name = ACTION_NAMES.cpp_link_executable,
            enabled = True,
            tools = [tool(tool = ctx.executable.link_executable)],
        ),
        action_config(
            action_name = ACTION_NAMES.assemble,
            enabled = True,
            tools = [tool(tool = ctx.file.assembler_executable)],
        ),
        action_config(
            action_name = ACTION_NAMES.preprocess_assemble,
            enabled = True,
            tools = [tool(tool = ctx.file.c_executable)],
        ),
        action_config(
            action_name = ACTION_NAMES.cpp_link_static_library,
            enabled = True,
            tools = [tool(tool = ctx.file.archiver_executable)],
        ),
        action_config(
            action_name = ACTION_NAMES.c_compile,
            enabled = True,
            tools = [tool(tool = ctx.file.c_executable)],
        ),
        action_config(
            action_name = ACTION_NAMES.cpp_compile,
            enabled = True,
            tools = [tool(tool = ctx.file.cpp_executable)],
        ),
        action_config(
            action_name = ACTION_NAMES.strip,
            enabled = True,
            tools = [tool(tool = ctx.file.strip_executable)],
        ),
    ]
    return action_configs
