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
    if attr.is_windows:
        platform = "@kq8_build_config//:winkq8"
    else:
        platform = "@kq8_build_config//:linuxkq8"

    return {
        "//command_line_option:platforms": platform,
        "//internal/kq8/linker:linker_package": attr.linker_package,
    }

_platform_transition = transition(
    implementation = _transition_impl,
    inputs = [],
    outputs = [
        "//command_line_option:platforms",
        "//internal/kq8/linker:linker_package",
    ],
)

# Use transition rule to transition to the correct compiler
_transition_kq8 = rule(
    implementation = _impl,
    attrs = {
        "next_step": attr.label(cfg = _platform_transition),
        "is_windows": attr.bool(mandatory = True),
        "_allowlist_function_transition": attr.label(
            default = "@bazel_tools//tools/allowlists/function_transition_allowlist",
        ),
        "linker_package": attr.string(mandatory = True),
    },
)

def transition_kq8(name, next_step, **kwargs):
    """
    Transisiton to the KQ8 compiler and app variant for building the kq8 image

    Args:
        name: Name of the rule.
        next_step: The next target to call. This is the base target for the kq8 build and is typically a cc_binary target.
        **kwargs: further keyword arguments, e.g. `visibility`
    """
    transition_kq8_impl = _transition_kq8

    transition_kq8_impl(
        name = name,
        next_step = next_step,
        is_windows = select({
            "@bazel_tools//src/conditions:host_windows": True,
            "//conditions:default": False,
        }),
        **kwargs
    )
