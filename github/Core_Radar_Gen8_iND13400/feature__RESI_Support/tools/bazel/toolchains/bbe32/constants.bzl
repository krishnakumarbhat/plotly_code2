"""Define constants for use across BBE simulator and compilation"""

XTENSA_CORE = select({
    "@build_config//:luna_asic_windows": "bbe32_luna",
    "@build_config//:luna_asic_linux": "bbe32_luna",
    "@build_config//:luna_fpga_windows": "bbe32_luna_fpga",
    "@build_config//:luna_fpga_linux": "bbe32_luna_fpga",
})
