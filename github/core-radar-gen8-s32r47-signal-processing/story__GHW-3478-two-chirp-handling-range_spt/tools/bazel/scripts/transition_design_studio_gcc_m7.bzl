"""
This transition is used to set the platform to one that uses the correct compiler.

Transistions should be applied above the cc_binary call. It tells Bazel to change toolchain,
variant, etc. for the below modules.
"""

def _impl(ctx):
    return [DefaultInfo(
        files = depset(ctx.attr.next_step[0][DefaultInfo].files.to_list()),
    )]

def _transition_impl(_settings, attr):
    platform = ""
    if attr.is_windows:  # placeholder if we need to differentiate
        platform = "//tools/bazel/toolchains/design_studio_gcc_m7:design_studio_gcc_m7_platform"
    else:
        platform = "//tools/bazel/toolchains/design_studio_gcc_m7:design_studio_gcc_m7_platform"

    return {
        "//command_line_option:platforms": platform,
    }

_platform_transition = transition(
    implementation = _transition_impl,
    inputs = [],
    outputs = ["//command_line_option:platforms"],
)

# Use transition rule to transition to the correct compiler
_transition_design_studio_gcc_m7 = rule(
    implementation = _impl,
    attrs = {
        "next_step": attr.label(cfg = _platform_transition),
        "is_windows": attr.bool(mandatory = True),
        "_allowlist_function_transition": attr.label(
            default = "@bazel_tools//tools/allowlists/function_transition_allowlist",
        ),
    },
)

def transition_design_studio_gcc_m7(name, next_step, **kwargs):
    """
    Transisiton to the M7 compiler for building the m7 image

    Args:
        name: Name of the rule.
        next_step: The next target to call. This is the base target for the m7 build and is typically a cc_binary target.
        **kwargs: further keyword arguments, e.g. `visibility`
    """
    transition_design_studio_gcc_m7_impl = _transition_design_studio_gcc_m7

    transition_design_studio_gcc_m7_impl(
        name = name,
        next_step = next_step,
        is_windows = select({
            "@bazel_tools//src/conditions:host_windows": True,
            "//conditions:default": False,
        }),
        **kwargs
    )
