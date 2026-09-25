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

def tracker_enabled_defines():
    """Return defines list selecting TRACKER_ENABLED or TRACKER_DISABLED based on tracker variant."""
    return select({
        "@Gen8_iND13400//software/bbe32/emb_tracker:tracker_disabled": ["TRACKER_DISABLED"],
        "@Gen8_iND13400//software/bbe32/emb_tracker:partner_sensor": ["TRACKER_DISABLED"],
        "//conditions:default": ["TRACKER_ENABLED"],
    })

def tracker_enabled_d_defines():
    """Return -D prefixed defines for TRACKER_ENABLED or TRACKER_DISABLED, for use in ld preprocessing."""
    return select({
        "@Gen8_iND13400//software/bbe32/emb_tracker:tracker_disabled": ["-DTRACKER_DISABLED"],
        "@Gen8_iND13400//software/bbe32/emb_tracker:partner_sensor": ["-DTRACKER_DISABLED"],
        "//conditions:default": ["-DTRACKER_ENABLED"],
    })

def tracker_variant_defines():
    """Return defines list selecting the active tracker variant macro."""
    return select({
        "@Gen8_iND13400//software/bbe32/emb_tracker:platform_flr8_standalone": ["PLATFORM_FLR8_STANDALONE"],
        "@Gen8_iND13400//software/bbe32/emb_tracker:platform_srr8p_standalone": ["PLATFORM_SRR8P_STANDALONE"],
        "@Gen8_iND13400//software/bbe32/emb_tracker:platform_srr8p_2_sensor_fusion": ["PLATFORM_SRR8P_2_SENSOR_FUSION"],
        "@Gen8_iND13400//software/bbe32/emb_tracker:AL": ["AL_TRACKER"],
        "//conditions:default": ["PLATFORM_SRR8P_STANDALONE"],
    })

def tracker_mode_defines():
    """Return defines list selecting the tracker mode macro."""
    return select({
        "@Gen8_iND13400//software/bbe32/emb_tracker:tracker_disabled": ["TRACKER_NONE"],
        "@Gen8_iND13400//software/bbe32/emb_tracker:partner_sensor": ["TRACKER_PARTNER_FUSION_SENSOR"],
        "@Gen8_iND13400//software/bbe32/emb_tracker:platform_flr8_standalone": ["TRACKER_STANDALONE"],
        "@Gen8_iND13400//software/bbe32/emb_tracker:platform_srr8p_standalone": ["TRACKER_STANDALONE"],
        "@Gen8_iND13400//software/bbe32/emb_tracker:platform_srr8p_2_sensor_fusion": ["TRACKER_MAIN_FUSION_SENSOR"],
        "@Gen8_iND13400//software/bbe32/emb_tracker:AL": ["TRACKER_MAIN_FUSION_SENSOR"],
        "//conditions:default": ["TRACKER_NONE"],
    })

def OCG_SG_disable_defines():
    """Return defines list to disable OCG and/or SG based on tracker variant."""
    return select({
        "@Gen8_iND13400//software/bbe32/emb_tracker:platform_flr8_standalone": ["DISABLE_SG"],
        "@Gen8_iND13400//software/bbe32/emb_tracker:platform_srr8p_standalone": ["DISABLE_SG"],
        "@Gen8_iND13400//software/bbe32/emb_tracker:platform_srr8p_2_sensor_fusion": ["DISABLE_SG"],
        "@Gen8_iND13400//software/bbe32/emb_tracker:AL": ["DISABLE_SG"],
        "//conditions:default": ["DISABLE_SG", "DISABLE_OCG"],
    })

def OCG_deps():
    """Return deps list optionally including the OCG library based on tracker variant."""
    return select({
        "@Gen8_iND13400//software/bbe32/emb_tracker:platform_flr8_standalone": ["@Gen8_iND13400//software/bbe32/emb_tracker:ocg"],
        "@Gen8_iND13400//software/bbe32/emb_tracker:platform_srr8p_standalone": ["@Gen8_iND13400//software/bbe32/emb_tracker:ocg"],
        "@Gen8_iND13400//software/bbe32/emb_tracker:platform_srr8p_2_sensor_fusion": ["@Gen8_iND13400//software/bbe32/emb_tracker:ocg"],
        "@Gen8_iND13400//software/bbe32/emb_tracker:AL": ["@Gen8_iND13400//software/bbe32/emb_tracker:ocg"],
        "//conditions:default": [],
    })

def SG_variant_defines():
    """Return defines list selecting the SG variant macro."""
    return select({
        "//conditions:default": ["SG_VARIANT_PLATFORM"],
    })

def SG_deps():
    """Return deps list optionally including the SG library based on tracker variant."""
    return select({
        "//conditions:default": [],
    })

# New radar generations that use rotbb should be added as needed
def rot_gen_build_flag():
    """Create the rot_gen string flag and associated config_setting targets for radar generation selection."""
    string_flag(
        name = "rot_gen",
        build_setting_default = "rot_gen8",
        values = [
            "rot_gen7v1",
            "rot_gen7v2",
            "rot_gen8",
        ],
        visibility = ["//visibility:public"],
    )

    native.config_setting(
        name = "ROT_GEN7V1",
        flag_values = {":rot_gen": "rot_gen7v1"},
        visibility = ["//visibility:public"],
    )

    native.config_setting(
        name = "ROT_GEN7V2",
        flag_values = {":rot_gen": "rot_gen7v2"},
        visibility = ["//visibility:public"],
    )

    native.config_setting(
        name = "ROT_GEN8",
        flag_values = {":rot_gen": "rot_gen8"},
        visibility = ["//visibility:public"],
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
