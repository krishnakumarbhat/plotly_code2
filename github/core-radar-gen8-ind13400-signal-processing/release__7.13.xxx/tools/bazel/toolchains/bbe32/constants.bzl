"""Define constants for use across BBE simulator and compilation"""

XTENSA_CORE = select({
    "@build_config//:b0_luna_asic_windows": "bbe32_luna",
    "@build_config//:b0_luna_asic_linux": "bbe32_luna",
})
