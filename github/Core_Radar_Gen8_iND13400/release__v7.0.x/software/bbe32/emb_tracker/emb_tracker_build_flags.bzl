load("@bazel_skylib//rules:common_settings.bzl", "bool_flag", "string_flag")

# New customer variants should be added as needed
def tracker_variant_build_flag():
    """Create the tracker_variant string flag and associated config_setting targets."""
    string_flag(
        name = "tracker_variant",
        build_setting_default = "disabled",
        values = [
            "disabled",
            "partner_sensor",
            "platform_flr8_standalone",
            "platform_srr8p_standalone",
            "platform_srr8p_2_sensor_fusion",
            "AL",
        ],
        visibility = ["//visibility:public"],
    )

    native.config_setting(
        name = "tracker_disabled",
        flag_values = {":tracker_variant": "disabled"},
    )

    native.config_setting(
        name = "partner_sensor",
        flag_values = {":tracker_variant": "partner_sensor"},
    )

    native.config_setting(
        name = "platform_flr8_standalone",
        flag_values = {":tracker_variant": "platform_flr8_standalone"},
    )

    native.config_setting(
        name = "platform_srr8p_standalone",
        flag_values = {":tracker_variant": "platform_srr8p_standalone"},
    )

    native.config_setting(
        name = "platform_srr8p_2_sensor_fusion",
        flag_values = {":tracker_variant": "platform_srr8p_2_sensor_fusion"},
    )

    native.config_setting(
        name = "AL",
        flag_values = {":tracker_variant": "AL"},
    )

def stub_tracker_build_flag():
    """Create the stub_tracker bool flag and associated config_setting target."""
    bool_flag(
        name = "stub_tracker",
        build_setting_default = False,
        visibility = ["//visibility:public"],
    )

    native.config_setting(
        name = "stub_tracker_enabled",
        flag_values = {":stub_tracker": "true"},
        visibility = ["//visibility:public"],
    )

def stub_tracker_defines():
    """Return select for stub tracker macro value."""
    return select({
        "@emb_tracker_wrapper//:stub_tracker_enabled": ["EMB_TRACKER_STUBBED=1"],
        "//conditions:default": ["EMB_TRACKER_STUBBED=0"],
    })

def pc_resim_tracker_build_flag():
    """Create the enable_pc_resim_tracker bool flag and associated config_setting target."""
    bool_flag(
        name = "enable_pc_resim_tracker",
        build_setting_default = False,
        visibility = ["//visibility:public"],
    )

    native.config_setting(
        name = "pc_resim_tracker_enabled",
        flag_values = {":enable_pc_resim_tracker": "true"},
        visibility = ["//visibility:public"],
    )
