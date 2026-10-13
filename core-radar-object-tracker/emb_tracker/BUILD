# Bazel includes
load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_skylib//rules:expand_template.bzl", "expand_template")

# emb_tracker bzl file includes
load("@emb_tracker_wrapper//:emb_tracker_project_profile.bzl", "EMB_TRACKER_PROJECT_PROFILE")
load("//emb_tracker:emb_tracker_project_contract.bzl", "f360_get_wall_time_settings", "load_and_validate_emb_tracker_project_profile")

# Create project profile struct for using project options in these libraries
# Every project using building block must provide this and pass validation check
PROJECT_PROFILE = load_and_validate_emb_tracker_project_profile(EMB_TRACKER_PROJECT_PROFILE)

# Disable LTO for Gen8
FEATURES = ["-lto"] if PROJECT_PROFILE.GENERATION == "ROT_GEN8" else []

# Decide the OCG variant to use based on project generation
OCG_VARIANT_OPTIONS = {
    "ROT_GEN8": "SRR8p_front",
    "ROT_GEN7V2": "SRR7px2_v1",
}

OCG_VARIANT_CHOSEN = OCG_VARIANT_OPTIONS[PROJECT_PROFILE.GENERATION]

# Internal targets are only consumable from this package and the wrapper repo.
INTERNAL_VISIBILITY = [
    "//emb_tracker:__pkg__",
    "@emb_tracker_wrapper//:__pkg__",
]

# Library for the embedded tracker stream interface
# Relies on emb_tracker_project_parameters.h that each project repo must have
cc_library(
    name = "emb_tracker_interface_h",
    srcs = [],
    hdrs = [
        "emb_tracker_stream_interface.h",
    ],
    defines = [],
    features = FEATURES,
    strip_include_prefix = "//emb_tracker",
    visibility = ["//visibility:public"],
    deps = [
        ":ocg_h",
        ":shared_tracker_api",
        "@emb_tracker_wrapper//:emb_tracker_project_parameters_h",
    ],
)

# Exporting files necessary for streamdef .txt file generation
filegroup(
    name = "fg_tracker_streams",
    srcs = [
        "OCG/src/iface/ocg_output_log_type.h",
        "OCG/src/iface/ocg_reuse.h",
        "OCG/src/iface/ocg_variant_type.h",
        "ROT/SharedTrackerAPI/Logging/CVTrailerOutputLog.h",
        "ROT/SharedTrackerAPI/Logging/F360HostPropsLog.h",
        "ROT/SharedTrackerAPI/Logging/HostCalibsLog.h",
        "ROT/SharedTrackerAPI/Logging/StaticEnvPolysLog.h",
        "ROT/SharedTrackerAPI/Logging/SyncInfoLog.h",
        "ROT/SharedTrackerAPI/Logging/TimingInfoLog.h",
        "ROT/SharedTrackerAPI/Logging/TrackerInfoLog.h",
        "ROT/SharedTrackerAPI/Logging/TrailerDetectorInternalLog.h",
        "ROT/SharedTrackerAPI/Logging/TrailerDetectorLog.h",
        "ROT/SharedTrackerAPI/Logging/VehicleInfoLog.h",
        "ROT/SharedTrackerAPI/Logging/f360_detection_log.h",
        "ROT/SharedTrackerAPI/Logging/f360_functional_safety_faults_log.h",
        "ROT/SharedTrackerAPI/Logging/f360_internal_cluster.h",
        "ROT/SharedTrackerAPI/Logging/f360_internal_cwd.h",
        "ROT/SharedTrackerAPI/Logging/f360_internal_detection_history.h",
        "ROT/SharedTrackerAPI/Logging/f360_internal_object.h",
        "ROT/SharedTrackerAPI/Logging/f360_internal_reflection_buffer.h",
        "ROT/SharedTrackerAPI/Logging/f360_object_log.h",
        "ROT/SharedTrackerAPI/Logging/f360_sensor_calib_log.h",
        "ROT/SharedTrackerAPI/Types/T360_Types.h",
        "ROT/SharedTrackerAPI/Types/f360_reuse.h",
        "OCG/src/iface/src/iface/ocg_variants/" + OCG_VARIANT_CHOSEN + "/ocg_constants.h",
        "emb_tracker_stream_interface.h",
    ],
    visibility = ["//visibility:public"],
)

# Library for all files in the SharedTrackerAPI folder
cc_library(
    name = "shared_tracker_api",
    srcs = [],
    hdrs = glob(["ROT/SharedTrackerAPI/Logging/*.h"]) + glob([
        "ROT/SharedTrackerAPI/Types/*.h",
    ]),
    defines = [],
    features = FEATURES,
    includes = [
        "ROT/SharedTrackerAPI/Logging/",
        "ROT/SharedTrackerAPI/Types/",
    ],
    strip_include_prefix = "//emb_tracker",
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":shared_tracker_api_core",
    ],
)

# Have to keep this library separate to facilitate the generated f360_variant_definition.h file usage
cc_library(
    name = "shared_tracker_api_core",
    srcs = [],
    hdrs = glob(
        ["ROT/SharedTrackerAPI/core/*.h"],
        exclude = ["ROT/SharedTrackerAPI/core/f360_variant_definition.h"],
    ) + [
        ":f360_variant_definition",
    ],
    defines = PROJECT_PROFILE.OCG_SG_DISABLE_DEFINES,
    features = FEATURES,
    includes = [],
    strip_include_prefix = "//emb_tracker/ROT/SharedTrackerAPI/core",
    visibility = INTERNAL_VISIBILITY,
    deps = [],
)

# Generate variant definition files with the variant letter replaced by A to facilitate namespace usage in wrapper
expand_template(
    name = "f360_variant_definition",
    out = "ROT/SharedTrackerAPI/core/f360_variant_definition.h",
    substitutions = PROJECT_PROFILE.F360_VARIANT_SUBS,
    template = PROJECT_PROFILE.F360_VARIANT_TEMPLATE,
)

# Library for aggregating all Fusion360 files
cc_library(
    name = "fusion_360",
    srcs = glob(["ROT/Fusion360/*.cpp"]),
    hdrs = [],
    defines = PROJECT_PROFILE.TRAILER_MANAGER_DISABLE_DEFINES +
              PROJECT_PROFILE.OCG_SG_DISABLE_DEFINES,
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":f360_get_wall_time",
        ":fusion_360_cluster_grouping",
        ":fusion_360_clustering",
        ":fusion_360_detection_to_track_association",
        ":fusion_360_e2e_protection",
        ":fusion_360_headers",
        ":fusion_360_inputs_preprocessing",
        ":fusion_360_internal_preprocessing",
        ":fusion_360_logging",
        ":fusion_360_measurement_update_tracks",
        ":fusion_360_object_track_initialization",
        ":fusion_360_occlusion",
        ":fusion_360_passenger_trailer_estimator",
        ":fusion_360_post_update_track_adjustments",
        ":fusion_360_pre_association_track_management",
        ":fusion_360_sensor_postprocessing",
        ":fusion_360_static_environment",
        ":fusion_360_time_update_tracks",
        ":fusion_360_track_classification",
        ":fusion_360_track_grouping",
        ":fusion_360_track_validity",
        ":fusion_360_trailer_manager",
        ":fusion_360_update_relative_timestamps",
        # These must be last - they provide utility symbols used by modules above
        ":fusion_360_autocode_reuse",
        ":fusion_360_multipath_detector",
        ":fusion_360_common",
        ":fusion_360_track_downselection",
        ":fusion_360_cv_trailer_estimator",
    ],
)

# Library for all Fusion360 headers
cc_library(
    name = "fusion_360_headers",
    srcs = [],
    hdrs = glob(["ROT/Fusion360/*.h"]) + glob([
        "ROT/Fusion360/include/*.h",
    ]) + glob(
        ["ROT/Fusion360/*/include/*.h"],
        exclude = ["ROT/Fusion360/track_classification/include/f360_assign_underdrivability_status_to_tracks_*.h"],
    ) + PROJECT_PROFILE.OCG_SG_HDRS,
    defines = [PROJECT_PROFILE.GENERATION],
    features = FEATURES,
    includes = [
        "ROT/Fusion360/",
        "ROT/Fusion360/autocode_reuse/include/",
        "ROT/Fusion360/cluster_grouping/include/",
        "ROT/Fusion360/clustering/include/",
        "ROT/Fusion360/common/include/",
        "ROT/Fusion360/cv_trailer_estimator/include/",
        "ROT/Fusion360/detection_to_track_association/include/",
        "ROT/Fusion360/e2e_protection/include/",
        "ROT/Fusion360/include/",
        "ROT/Fusion360/inputs_preprocessing/include/",
        "ROT/Fusion360/internal_preprocessing/include/",
        "ROT/Fusion360/logging/include/",
        "ROT/Fusion360/measurement_update_tracks/include/",
        "ROT/Fusion360/multipath_detector/include/",
        "ROT/Fusion360/object_track_initialization/include/",
        "ROT/Fusion360/occlusion/include/",
        "ROT/Fusion360/passenger_trailer_estimator/include/",
        "ROT/Fusion360/post_update_track_adjustments/include/",
        "ROT/Fusion360/pre_association_track_management/include/",
        "ROT/Fusion360/sensor_postprocessing/include/",
        "ROT/Fusion360/static_environment/include/",
        "ROT/Fusion360/time_update_tracks/include/",
        "ROT/Fusion360/track_classification/include/",
        "ROT/Fusion360/track_downselection/include/",
        "ROT/Fusion360/track_grouping/include/",
        "ROT/Fusion360/track_validity/include/",
        "ROT/Fusion360/trailer_manager/include/",
        "ROT/Fusion360/update_relative_timestamps/include/",
    ],
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":ocg_h",
        ":sg_small_h",
        ":shared_tracker_api",
    ] + ([
        "//emb_tracker/FastMath:fast_math_h",
        "//emb_tracker/FastMath:fast_math_lib",
    ] if PROJECT_PROFILE.GENERATION == "ROT_GEN7V2" else []),
)

# Fusion360 libraries split by subfolder
cc_library(
    name = "fusion_360_autocode_reuse",
    srcs = glob(["ROT/Fusion360/autocode_reuse/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_cluster_grouping",
    srcs = glob(["ROT/Fusion360/cluster_grouping/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_clustering",
    srcs = glob(["ROT/Fusion360/clustering/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_common",
    srcs = glob(["ROT/Fusion360/common/source/*.cpp"]),
    hdrs = [],
    defines = [
        "NDEBUG",  # Disables sanity check code in algorithm that doesn't need run on embedded tracker
    ],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_cv_trailer_estimator",
    srcs = glob(["ROT/Fusion360/cv_trailer_estimator/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_detection_to_track_association",
    srcs = glob(["ROT/Fusion360/detection_to_track_association/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_e2e_protection",
    srcs = glob(["ROT/Fusion360/e2e_protection/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_inputs_preprocessing",
    srcs = glob(["ROT/Fusion360/inputs_preprocessing/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_internal_preprocessing",
    srcs = glob(["ROT/Fusion360/internal_preprocessing/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_logging",
    srcs = glob(["ROT/Fusion360/logging/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_measurement_update_tracks",
    srcs = glob(["ROT/Fusion360/measurement_update_tracks/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_multipath_detector",
    srcs = glob(["ROT/Fusion360/multipath_detector/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_object_track_initialization",
    srcs = glob(["ROT/Fusion360/object_track_initialization/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_occlusion",
    srcs = glob(["ROT/Fusion360/occlusion/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_passenger_trailer_estimator",
    srcs = glob(["ROT/Fusion360/passenger_trailer_estimator/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_post_update_track_adjustments",
    srcs = glob(["ROT/Fusion360/post_update_track_adjustments/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_pre_association_track_management",
    srcs = glob(["ROT/Fusion360/pre_association_track_management/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_sensor_postprocessing",
    srcs = glob(["ROT/Fusion360/sensor_postprocessing/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_static_environment",
    srcs = glob(["ROT/Fusion360/static_environment/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_time_update_tracks",
    srcs = glob(["ROT/Fusion360/time_update_tracks/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_track_classification",
    srcs = glob(
        ["ROT/Fusion360/track_classification/source/*.cpp"],
        exclude = ["ROT/Fusion360/track_classification/source/f360_assign_underdrivability_status_to_tracks_*.cpp"],
    ) + PROJECT_PROFILE.OCG_SG_SRCS,
    hdrs = [],
    defines = PROJECT_PROFILE.OCG_SG_DISABLE_DEFINES,
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_track_downselection",
    srcs = glob(["ROT/Fusion360/track_downselection/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_track_grouping",
    srcs = glob(["ROT/Fusion360/track_grouping/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_track_validity",
    srcs = glob(["ROT/Fusion360/track_validity/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_trailer_manager",
    srcs = glob(["ROT/Fusion360/trailer_manager/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

cc_library(
    name = "fusion_360_update_relative_timestamps",
    srcs = glob(["ROT/Fusion360/update_relative_timestamps/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360_headers",
    ],
)

# Library for the StateManager files
cc_library(
    name = "state_manager",
    srcs = glob(["ROT/StateManager/source/*.cpp"]),
    hdrs = [],
    defines = [],
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360",
        ":shared_tracker_api",
    ],
)

# Get generation specific deps and non-resim defines for the f360_get_wall_time library
F360_GET_WALL_TIME_DEFINES, F360_GET_WALL_TIME_DEPS = f360_get_wall_time_settings(PROJECT_PROFILE)

# Library for the timing function used in the algorithm
cc_library(
    name = "f360_get_wall_time",
    srcs = ["Timing/f360_get_wall_time.cpp"],
    hdrs = [],
    defines = F360_GET_WALL_TIME_DEFINES,
    features = FEATURES,
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":shared_tracker_api",
    ] + F360_GET_WALL_TIME_DEPS,
)

# Library for the VSE files
cc_library(
    name = "vse",
    srcs = glob(["VSE/source/*.cpp"]) + ["VSE/VSE_Core.cpp"],
    hdrs = glob(["VSE/include/*.h"]) + ["VSE/VSE_Core.h"],
    defines = [],
    features = FEATURES,
    includes = [
        "VSE/",
        "VSE/include/",
    ],
    strip_include_prefix = "//emb_tracker",
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":fusion_360",
        ":shared_tracker_api",
    ],
)

# Library for the OCG files
cc_library(
    name = "ocg",
    srcs = glob(["OCG/src/core/*/source/*.cpp"]) + glob([
        "OCG/src/core/*.cpp",
    ]) + [
        "OCG/src/core/ocg_calibrations/" + OCG_VARIANT_CHOSEN + "/ocg_calibrations.cpp",
    ],
    hdrs = [],
    defines = [],
    features = FEATURES,
    strip_include_prefix = "//emb_tracker",
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":ocg_h",
    ],
)

cc_library(
    name = "ocg_h",
    srcs = [],
    hdrs = glob(["OCG/src/iface/*.h"]) + glob([
        "OCG/src/core/*/include/*.h",
    ]) + glob([
        "OCG/src/core/*/iface/*.h",
    ]) + [
        "OCG/src/iface/ocg_variants/" + OCG_VARIANT_CHOSEN + "/ocg_constants.h",
    ],
    defines = [],
    features = FEATURES,
    includes = [
        "OCG/src/core/common/include/",
        "OCG/src/core/underdrivability/iface/",
        "OCG/src/core/underdrivability/include/",
        "OCG/src/iface/",
        "OCG/src/iface/ocg_variants/" + OCG_VARIANT_CHOSEN + "/",
    ],
    strip_include_prefix = "//emb_tracker",
    visibility = INTERNAL_VISIBILITY,
    deps = [
    ],
)

# SG to be properly integrated, calibrated, and tested still
# These libraries need cleaned up and optimized as part of that full integration
# SG currently being reworked by algo team (July 13, 2026)

# Library for the SG files
cc_library(
    name = "sg",
    srcs = glob(
        [
            "SG/src/core/src/*.cpp",
            "SG/src/core/modules/calibrations/src/*.cpp",
            "SG/src/core/modules/common/src/*.cpp",
            "SG/src/core/modules/contour_downselection/src/*.cpp",
            "SG/src/core/modules/contour_initialization/src/*.cpp",
            "SG/src/core/modules/detection_clustering/src/*.cpp",
            "SG/src/core/modules/detection_processing/src/*.cpp",
            "SG/src/core/modules/drivability_classification/src/*.cpp",
            "SG/src/core/modules/drivability_classification_wrapper/src/*.cpp",
            "SG/src/core/modules/host/src/*.cpp",
            "SG/src/core/modules/math/src/*.cpp",
            "SG/src/core/modules/math/src/geometry/*.cpp",
            "SG/src/core/modules/measurement_association/src/*.cpp",
            "SG/src/core/modules/measurement_update/src/*.cpp",
            "SG/src/core/modules/postprocessing/src/*.cpp",
            "SG/src/core/modules/safety/src/*.cpp",
            "SG/src/core/modules/time_update/src/*.cpp",
        ],
        exclude = [
            "SG/src/core/modules/drivability_classification/src/dc_interface_autocode.cpp",
            "SG/src/core/modules/drivability_classification/src/dc_subsegment_controller_autocode.cpp",
            "SG/src/core/modules/drivability_classification_wrapper/src/dc_additional_features_calculator.cpp",
        ],
    ),
    hdrs = glob(
        [
            "SG/src/iface/*.h",
            "SG/src/iface/types/*.h",
            "SG/src/iface/types/contour/*.h",
            "SG/src/iface/types/dump/*.h",
            "SG/src/iface/variants/*.h",
            "SG/src/core/inc/*.h",
            "SG/src/core/modules/calibrations/iface/*.h",
            "SG/src/core/modules/calibrations/iface/calibrations/*.h",
            "SG/src/core/modules/common/iface/*.h",
            "SG/src/core/modules/contour_downselection/iface/*.h",
            "SG/src/core/modules/contour_initialization/iface/*.h",
            "SG/src/core/modules/contour_initialization/inc/*.h",
            "SG/src/core/modules/detection_clustering/iface/*.h",
            "SG/src/core/modules/detection_clustering/inc/*.h",
            "SG/src/core/modules/detection_processing/iface/*.h",
            "SG/src/core/modules/detection_processing/inc/*.h",
            "SG/src/core/modules/drivability_classification/iface/*.h",
            "SG/src/core/modules/drivability_classification/inc/*.h",
            "SG/src/core/modules/drivability_classification_wrapper/iface/*.h",
            "SG/src/core/modules/host/iface/*.h",
            "SG/src/core/modules/math/iface/*.h",
            "SG/src/core/modules/math/iface/geometry/*.h",
            "SG/src/core/modules/measurement_association/iface/*.h",
            "SG/src/core/modules/measurement_association/inc/*.h",
            "SG/src/core/modules/measurement_update/iface/*.h",
            "SG/src/core/modules/measurement_update/inc/*.h",
            "SG/src/core/modules/postprocessing/iface/*.h",
            "SG/src/core/modules/postprocessing/inc/*.h",
            "SG/src/core/modules/safety/iface/*.h",
            "SG/src/core/modules/time_update/iface/*.h",
            "SG/src/core/modules/time_update/inc/*.h",
        ],
        exclude = [
            "SG/src/core/modules/drivability_classification/iface/dc_interface_autocode.h",
            "SG/src/core/modules/drivability_classification/iface/dc_subsegment_controller_autocode.h",
            "SG/src/core/modules/drivability_classification/inc/dc_interface_autocode.h",
            "SG/src/core/modules/drivability_classification/inc/dc_subsegment_controller_autocode.h",
            "SG/src/core/modules/drivability_classification_wrapper/iface/dc_additional_features_calculator.h",
        ],
    ),
    defines = PROJECT_PROFILE.SG_VARIANT_DEFINES,
    features = FEATURES,
    includes = [
        "SG/src/core/inc",
        "SG/src/core/modules/calibrations/iface",
        "SG/src/core/modules/calibrations/iface/calibrations",
        "SG/src/core/modules/common/iface",
        "SG/src/core/modules/contour_downselection/iface",
        "SG/src/core/modules/contour_initialization/iface",
        "SG/src/core/modules/contour_initialization/inc",
        "SG/src/core/modules/detection_clustering/iface",
        "SG/src/core/modules/detection_clustering/inc",
        "SG/src/core/modules/detection_processing/iface",
        "SG/src/core/modules/detection_processing/inc",
        "SG/src/core/modules/drivability_classification/iface",
        "SG/src/core/modules/drivability_classification/inc",
        "SG/src/core/modules/drivability_classification_wrapper/iface",
        "SG/src/core/modules/host/iface",
        "SG/src/core/modules/math/iface",
        "SG/src/core/modules/math/iface/geometry",
        "SG/src/core/modules/measurement_association/iface",
        "SG/src/core/modules/measurement_association/inc",
        "SG/src/core/modules/measurement_update/iface",
        "SG/src/core/modules/measurement_update/inc",
        "SG/src/core/modules/postprocessing/iface",
        "SG/src/core/modules/postprocessing/inc",
        "SG/src/core/modules/safety/iface",
        "SG/src/core/modules/time_update/iface",
        "SG/src/core/modules/time_update/inc",
        "SG/src/iface",
        "SG/src/iface/types",
        "SG/src/iface/types/contour",
        "SG/src/iface/types/dump",
        "SG/src/iface/variants",
    ],
    visibility = INTERNAL_VISIBILITY,
    deps = [
        ":shared_tracker_api",
    ],
)

cc_library(
    name = "sg_small_h",
    hdrs = [
        "SG/src/iface/sg_reuse.h",
        "SG/src/iface/types/sg_drivability_class.h",
    ],
    defines = [],
    features = FEATURES,
    includes = [
        "SG/src/iface",
        "SG/src/iface/types",
    ],
    visibility = INTERNAL_VISIBILITY,
    deps = [],
)
