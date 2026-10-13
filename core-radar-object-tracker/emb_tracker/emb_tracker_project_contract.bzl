# All projects using rotbb must fulfill this contract
# Otherwise, compilation will fail early
_CONTRACT_NAME = "emb_tracker PROJECT CONTRACT"

# These fields must be present in the profile
_REQUIRED_FIELDS = [
    "GENERATION",
    "TRACKER_ENABLED_DEFINES",
    "TRACKER_VARIANT_DEFINES",
    "TRACKER_MODE_DEFINES",
    "F360_VARIANT_TEMPLATE",
    "F360_VARIANT_SUBS",
    "TRAILER_MANAGER_DISABLE_DEFINES",
    "OCG_SG_DISABLE_DEFINES",
    "OCG_DEPS",
    "SG_DEPS",
    "SG_VARIANT_DEFINES",
    "OCG_SG_HDRS",
    "OCG_SG_SRCS",
    "PC_TRACKER_ENABLE_DEFINES",
]

# Fields consumed in list-like contexts in BUILD.
# Keep this permissive for configurable values.
_LISTLIKE_FIELDS = [
    "TRACKER_ENABLED_DEFINES",
    "TRACKER_VARIANT_DEFINES",
    "TRACKER_MODE_DEFINES",
    "TRAILER_MANAGER_DISABLE_DEFINES",
    "OCG_SG_DISABLE_DEFINES",
    "OCG_DEPS",
    "SG_DEPS",
    "SG_VARIANT_DEFINES",
    "OCG_SG_HDRS",
    "OCG_SG_SRCS",
    "PC_TRACKER_ENABLE_DEFINES",
]

# Used by expand_template(template = ...)
_TEMPLATE_FIELDS = [
    "F360_VARIANT_TEMPLATE",
]

# Used by expand_template(substitutions = ...)
_SUBSTITUTION_FIELDS = [
    "F360_VARIANT_SUBS",
]

def _fail(msg):
    fail("Contract error (" + _CONTRACT_NAME + "): " + msg)

def _require_field(profile, field):
    if not hasattr(profile, field):
        _fail("missing required field '" + field + "'. Required fields: " + ", ".join(_REQUIRED_FIELDS))

def _require_type(field, value, allowed_types):
    t = type(value)
    if t not in allowed_types:
        _fail(
            "field '" + field + "' has unsupported type '" + t +
            "'. Allowed: " + ", ".join(allowed_types),
        )

def _validate_required_fields_present(profile):
    for field in _REQUIRED_FIELDS:
        _require_field(profile, field)

def _validate_generation(profile):
    # No allowed-value list by design; only enforce that it is a string.
    _require_type("GENERATION", profile.GENERATION, ["string"])

def _validate_listlike_fields(profile):
    for field in _LISTLIKE_FIELDS:
        _require_type(field, getattr(profile, field), ["list", "tuple", "select"])

def _validate_template_fields(profile):
    for field in _TEMPLATE_FIELDS:
        _require_type(field, getattr(profile, field), ["string", "select"])

def _validate_substitution_fields(profile):
    for field in _SUBSTITUTION_FIELDS:
        _require_type(field, getattr(profile, field), ["dict", "select"])

def _validate_emb_tracker_project_profile(profile):
    _validate_required_fields_present(profile)
    _validate_generation(profile)
    _validate_listlike_fields(profile)
    _validate_template_fields(profile)
    _validate_substitution_fields(profile)
    return profile

def load_and_validate_emb_tracker_project_profile(profile_function):
    """Validate the project profile against this contract."""
    profile = profile_function()
    return _validate_emb_tracker_project_profile(profile)

def f360_get_wall_time_settings(profile):
    """Get f360_get_wall_time settings based on the project profile generation."""
    defines = []
    deps = []

    if profile.PC_TRACKER_ENABLE_DEFINES != "PC_RESIM_TRACKER":
        defines = [profile.GENERATION]

        if profile.GENERATION == "ROT_GEN8":
            deps = ["@spbb//modules/helpers:profiling_helpers_h"]
        elif profile.GENERATION == "ROT_GEN7V2":
            deps = ["@spbb//modules/helpers:timing_helpers_h"]

    return defines, deps
