"""This rule takes in flag targets and saves them to a text file with the value for the most recent build."""

load("@bazel_skylib//rules:common_settings.bzl", "BuildSettingInfo")

def _rule_impl(ctx):
    build_cfg_file = ctx.actions.declare_file("build-cfg.txt")

    build_cfg_args = ctx.actions.args()
    build_cfg_args.add_all(["{} = {}".format(x.label, x[BuildSettingInfo].value) for x in ctx.attr.config_flags if BuildSettingInfo in x])

    ctx.actions.write(
        output = build_cfg_file,
        content = build_cfg_args,
    )

    return [DefaultInfo(files = depset([
        build_cfg_file,
    ]))]

save_build_config = rule(
    implementation = _rule_impl,
    attrs = {
        "config_flags": attr.label_list(mandatory = True, doc = "List of the flag labels that should be included in the saved build config file."),
    },
)
