"""
App Transistions are used to set the appVariant flag to mss, dss, or boot.

These flags can then be used by other modules in select statments.

Transistions should be applied above the cc_binary call. It tells Bazel to change toolchain,
variant, etc. for the below modules.
"""

def _transition_mss_app_variant_impl(ctx):
    return [DefaultInfo(
        files = depset(ctx.attr.next_step[0][DefaultInfo].files.to_list()),
    )]

def _mss_app_variant_transition_impl(_attr, _settings):
    return {
        "//:appVariantFlag": "mss",
    }

_mss_app_variant_transition = transition(
    implementation = _mss_app_variant_transition_impl,
    inputs = [],
    outputs = [
        "//:appVariantFlag",
    ],
)

transition_mss_app_variant = rule(
    implementation = _transition_mss_app_variant_impl,
    attrs = {
        "next_step": attr.label(cfg = _mss_app_variant_transition),
        "_allowlist_function_transition": attr.label(
            default = "@bazel_tools//tools/allowlists/function_transition_allowlist",
        ),
    },
    doc = "Use transition_mss_app_variant rule to transisiton to the mss appVariantFlag",
)

def _transition_dss_app_variant_impl(ctx):
    return [DefaultInfo(
        files = depset(ctx.attr.next_step[0][DefaultInfo].files.to_list()),
    )]

def _dss_app_variant_transition_impl(_attr, _settings):
    return {
        "//:appVariantFlag": "dss",
    }

_dss_app_variant_transition = transition(
    implementation = _dss_app_variant_transition_impl,
    inputs = [],
    outputs = [
        "//:appVariantFlag",
    ],
)

transition_dss_app_variant = rule(
    implementation = _transition_dss_app_variant_impl,
    attrs = {
        "next_step": attr.label(cfg = _dss_app_variant_transition),
        "_allowlist_function_transition": attr.label(
            default = "@bazel_tools//tools/allowlists/function_transition_allowlist",
        ),
    },
    doc = "Use transition_dss_app_variant rule to transisiton to the dss appVariantFlag",
)

def _transition_boot_app_variant_impl(ctx):
    return [DefaultInfo(
        files = depset(ctx.attr.next_step[0][DefaultInfo].files.to_list()),
    )]

def _boot_app_variant_transition_impl(_ctx, _settings):
    return {
        "//:appVariantFlag": "boot",
    }

_boot_app_variant_transition = transition(
    implementation = _boot_app_variant_transition_impl,
    inputs = [],
    outputs = [
        "//:appVariantFlag",
    ],
)

transition_boot_app_variant = rule(
    implementation = _transition_boot_app_variant_impl,
    attrs = {
        "next_step": attr.label(cfg = _boot_app_variant_transition),
        "_allowlist_function_transition": attr.label(
            default = "@bazel_tools//tools/allowlists/function_transition_allowlist",
        ),
    },
    doc = "Use transition_boot_app_variant rule to transisiton to the boot appVariantFlag",
)
