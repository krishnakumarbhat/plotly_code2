def EMB_TRACKER_PROJECT_PROFILE():
    """Every project using the ROT building block (rotbb) must have a project profile with these fields to complete the contract."""
    return struct(

        # For knowing the radar generation this profile is building ROT for
        GENERATION = "ROT_GEN8",

        # For knowing if tracker is enabled or disabled based on tracker variant build flag
        TRACKER_ENABLED_DEFINES =
            select({
                "@emb_tracker_wrapper//:tracker_disabled": ["TRACKER_DISABLED"],
                "@emb_tracker_wrapper//:partner_sensor": ["TRACKER_DISABLED"],
                "//conditions:default": ["TRACKER_ENABLED"],
            }),

        # For selecting tracker variant macro based on tracker variant build flag
        TRACKER_VARIANT_DEFINES =
            select({
                "@emb_tracker_wrapper//:platform_flr8_standalone": ["PLATFORM_FLR8_STANDALONE"],
                "@emb_tracker_wrapper//:platform_srr8p_standalone": ["PLATFORM_SRR8P_STANDALONE"],
                "@emb_tracker_wrapper//:platform_srr8p_2_sensor_fusion": ["PLATFORM_SRR8P_2_SENSOR_FUSION"],
                "@emb_tracker_wrapper//:AL": ["AL_TRACKER"],
                "//conditions:default": ["PLATFORM_SRR8P_STANDALONE"],
            }),

        # For knowing tracker 'mode' based on tracker variant build flag
        TRACKER_MODE_DEFINES =
            select({
                "@emb_tracker_wrapper//:tracker_disabled": ["TRACKER_NONE"],
                "@emb_tracker_wrapper//:partner_sensor": ["TRACKER_PARTNER_FUSION_SENSOR"],
                "@emb_tracker_wrapper//:platform_flr8_standalone": ["TRACKER_STANDALONE"],
                "@emb_tracker_wrapper//:platform_srr8p_standalone": ["TRACKER_STANDALONE"],
                "@emb_tracker_wrapper//:platform_srr8p_2_sensor_fusion": ["TRACKER_MAIN_FUSION_SENSOR"],
                "@emb_tracker_wrapper//:AL": ["TRACKER_MAIN_FUSION_SENSOR"],
                "//conditions:default": ["TRACKER_NONE"],
            }),

        # For choosing and regenerating variant definition files based on tracker variant build flag
        F360_VARIANT_TEMPLATE =
            select({
                "@emb_tracker_wrapper//:platform_flr8_standalone": "ROT/SharedTrackerAPI/core/variants/f360_variant_definition_g.h",
                "@emb_tracker_wrapper//:platform_srr8p_standalone": "ROT/SharedTrackerAPI/core/variants/f360_variant_definition_j.h",
                "@emb_tracker_wrapper//:platform_srr8p_2_sensor_fusion": "ROT/SharedTrackerAPI/core/variants/f360_variant_definition_r.h",
                "@emb_tracker_wrapper//:AL": "ROT/SharedTrackerAPI/core/variants/f360_variant_definition_al.h",
                "//conditions:default": "ROT/SharedTrackerAPI/core/variants/f360_variant_definition_j.h",
            }),
        F360_VARIANT_SUBS =
            select({
                "@emb_tracker_wrapper//:platform_flr8_standalone": {"f360_variant_G": "f360_variant_A"},
                "@emb_tracker_wrapper//:platform_srr8p_standalone": {"f360_variant_J": "f360_variant_A"},
                "@emb_tracker_wrapper//:platform_srr8p_2_sensor_fusion": {"f360_variant_R": "f360_variant_A"},
                "@emb_tracker_wrapper//:AL": {"f360_variant_AL": "f360_variant_A"},
                "//conditions:default": {"f360_variant_J": "f360_variant_A"},
            }),

        # For disabling trailer manager code in tracker algo based on tracker variant build flag
        TRAILER_MANAGER_DISABLE_DEFINES =
            select({
                "@emb_tracker_wrapper//:platform_flr8_standalone": ["DISABLE_TRAILER_MANAGER"],
                "@emb_tracker_wrapper//:platform_srr8p_standalone": [],
                "@emb_tracker_wrapper//:platform_srr8p_2_sensor_fusion": [],
                "@emb_tracker_wrapper//:AL": [],
                "//conditions:default": ["DISABLE_TRAILER_MANAGER"],
            }),

        # July 13, 2026: SG must be disabled for all builds until it is done being reworked by algorithm team
        # For disabling SG or OCG based on tracker variant build flag
        OCG_SG_DISABLE_DEFINES =
            select({
                "@emb_tracker_wrapper//:platform_flr8_standalone": ["DISABLE_SG"],
                "@emb_tracker_wrapper//:platform_srr8p_standalone": ["DISABLE_SG", "DISABLE_OCG"],
                "@emb_tracker_wrapper//:platform_srr8p_2_sensor_fusion": ["DISABLE_SG", "DISABLE_OCG"],
                "@emb_tracker_wrapper//:AL": ["DISABLE_SG"],
                "//conditions:default": ["DISABLE_SG", "DISABLE_OCG"],
            }),
        OCG_SG_HDRS =
            select({
                "@emb_tracker_wrapper//:platform_flr8_standalone": ["ROT/Fusion360/track_classification/include/f360_assign_underdrivability_status_to_tracks_ocg.h"],
                "@emb_tracker_wrapper//:platform_srr8p_standalone": [],
                "@emb_tracker_wrapper//:platform_srr8p_2_sensor_fusion": [],
                "@emb_tracker_wrapper//:AL": ["ROT/Fusion360/track_classification/include/f360_assign_underdrivability_status_to_tracks_ocg.h"],
                "//conditions:default": [],
            }),
        OCG_SG_SRCS =
            select({
                "@emb_tracker_wrapper//:platform_flr8_standalone": ["ROT/Fusion360/track_classification/source/f360_assign_underdrivability_status_to_tracks_ocg.cpp"],
                "@emb_tracker_wrapper//:platform_srr8p_standalone": [],
                "@emb_tracker_wrapper//:platform_srr8p_2_sensor_fusion": [],
                "@emb_tracker_wrapper//:AL": ["ROT/Fusion360/track_classification/source/f360_assign_underdrivability_status_to_tracks_ocg.cpp"],
                "//conditions:default": [],
            }),
        OCG_DEPS =
            select({
                "@emb_tracker_wrapper//:platform_flr8_standalone": ["@rotbb//emb_tracker:ocg"],
                "@emb_tracker_wrapper//:platform_srr8p_standalone": [],
                "@emb_tracker_wrapper//:platform_srr8p_2_sensor_fusion": [],
                "@emb_tracker_wrapper//:AL": ["@rotbb//emb_tracker:ocg"],
                "//conditions:default": [],
            }),
        SG_DEPS =
            select({
                "//conditions:default": [],
            }),

        # For selecting SG variant based on tracker variant build flag
        SG_VARIANT_DEFINES =
            select({
                "//conditions:default": ["SG_VARIANT_PLATFORM"],
            }),

        # For enabling  PC tracker based on tracker variant build flag
        PC_TRACKER_ENABLE_DEFINES =
            select({
                "@emb_tracker_wrapper//:pc_resim_tracker_enabled": ["PC_RESIM_TRACKER"],
                "//conditions:default": [],
            }),
    )
