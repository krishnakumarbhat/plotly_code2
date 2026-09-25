cmake_minimum_required(VERSION 3.16)

add_executable(variant_specific_ut)

target_sources(
   variant_specific_ut
      PRIVATE
         $<$<STREQUAL:${SG_VARIANT},PLATFORM_UNLIMITED>:ut_variant_platform_unlimited.cpp>
         $<$<STREQUAL:${SG_VARIANT},PLATFORM_HIGH>:ut_variant_platform_high.cpp>
         $<$<STREQUAL:${SG_VARIANT},PLATFORM_MEDIUM>:ut_variant_platform_medium.cpp>
         $<$<STREQUAL:${SG_VARIANT},PLATFORM_LOW>:ut_variant_platform_low.cpp>
         $<$<STREQUAL:${SG_VARIANT},PLATFORM_REDUCED>:ut_variant_platform_reduced.cpp>
         $<$<STREQUAL:${SG_VARIANT},BMW_SP25_MRR>:ut_variant_bmw_sp25_mrr.cpp>
         $<$<STREQUAL:${SG_VARIANT},SCANIA_TRATON>:ut_variant_scania_traton.cpp>
         $<$<STREQUAL:${SG_VARIANT},NORTH_STAR_CORNER>:ut_variant_north_star_corner.cpp>
         $<$<STREQUAL:${SG_VARIANT},NORTH_STAR_FRONT>:ut_variant_north_star_front.cpp>
         $<$<STREQUAL:${SG_VARIANT},NORTH_STAR_REAR>:ut_variant_north_star_rear.cpp>
)

target_link_libraries(
  variant_specific_ut
    PRIVATE sg_ut_config
    PRIVATE stationary_geometries_iface
    PRIVATE SG-Common
    PRIVATE SG-Contour-Downselection
    PRIVATE SG-Calibrations
)

include(GoogleTest)
gtest_discover_tests(variant_specific_ut)