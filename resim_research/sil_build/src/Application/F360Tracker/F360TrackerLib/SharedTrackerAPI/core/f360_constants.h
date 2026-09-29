/*===========================================================================*\
* FILE: f360_constants.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains macro's used globally which are expected to be constant all time
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#ifndef F360_CONSTANTS_VARIANT_A_H
#define F360_CONSTANTS_VARIANT_A_H

#include "../Types/f360_reuse.h"
#include "f360_variant_definition.h"

namespace f360_variant_A
{
   static constexpr uint8_t STATE_DIMENSION = 6U;

   static constexpr uint8_t F360_PSEUDO_MEAS_DIM = 2U;

   static constexpr uint16_t MAX_NUMBER_OF_DETECTIONS = ((MAX_NUMBER_OF_SRR_SENSORS * NUMBER_OF_SRR_DETECTIONS) + (MAX_NUMBER_OF_MRR_SENSORS * NUMBER_OF_MRR_DETECTIONS));

   static constexpr uint8_t MAX_NUMBER_OF_SENSORS = (MAX_NUMBER_OF_SRR_SENSORS + MAX_NUMBER_OF_MRR_SENSORS);

   static constexpr uint16_t MAX_DETS_FOR_SINGLE_SENSOR = (NUMBER_OF_SRR_DETECTIONS > NUMBER_OF_MRR_DETECTIONS) ? NUMBER_OF_SRR_DETECTIONS : NUMBER_OF_MRR_DETECTIONS;

   static constexpr uint32_t MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER = (MAX_DETS_IN_OBJ_TRK + MAX_HIST_DETS_IN_CLUSTER);
   static constexpr uint8_t MSMT_UPDATE_MAX_NUM_OF_NON_RR_MSMT = 2U; // Maximum number of non-range rate measurements in measurement update function

   static constexpr uint32_t MSMT_UPDATE_MAX_NUM_OF_MSMT = MAX_DETS_IN_OBJ_TRK + static_cast<uint32_t>(MSMT_UPDATE_MAX_NUM_OF_NON_RR_MSMT); // Maximum number of measurements in measurement update function

   static constexpr uint8_t MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE = 3U; // number of measurement in measurement update for non-moveable CCA objects

   // LSC
   static constexpr uint8_t MAX_NR_OF_LONGITUDINAL_STAT_CURVES = 6U;
   static constexpr uint8_t LSC_NR_POLY_COEFF_SLOTS = 3U;
   static constexpr uint16_t NR_LONGI_STAT_CLUSTERS = static_cast<uint16_t>((NUMBER_OF_OBJECT_TRACKS / static_cast<uint32_t>(LSC_NR_POLY_COEFF_SLOTS)) + 1U);

   // CWD - Concrete Wall Detector
   static constexpr uint8_t MAX_NR_OF_CWD = 2U;

   // General static environment polynomial structure
   static constexpr uint8_t F360_NUM_OF_STATIC_ENV_POLYS = MAX_NR_OF_LONGITUDINAL_STAT_CURVES + MAX_NR_OF_CWD;

   static constexpr uint8_t MAX_NUM_UNIQUE_RDOT_INTERVAL = (MAX_NUMBER_OF_SENSORS * 4U); // Number of different look types = 4

   static constexpr uint8_t MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS = 31U; // Number of reference points for detections in vcs long sorted order

   static constexpr uint8_t MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS = (MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS + 2U); // Array of reference detection indexes is appended with smallest VCS-long position at first element and largst VCS-long position at last invalid element

   static constexpr uint8_t MAX_NUM_POLYGON_CORNERS = 8U; // Max number of points needed to create a polygon surrounding the overlapping area of two boxes

   static constexpr uint8_t NUM_CORNERS_IN_RECTANGLE = 4U;

   // Constant value for mathemetical conversion
   static constexpr float32_t F360_PI = 3.14159265358979323846F; // pi

   static constexpr float32_t F360_2PI = 6.28318530717958647693F; // 2*pi

   static constexpr float32_t F360_PI_2 = 1.57079632679489661923F; // pi/2

   static constexpr float32_t F360_1_PI = 0.318309886183790671538F; // 1/pi

   static constexpr float32_t F360_DEG2RAD(const float32_t x) { return (x * 0.0174532925F); } // converts from degree to radian

   static constexpr float32_t F360_RAD2DEG(const float32_t x) { return (x * 57.29577951F); } // converts from radian to degree

   static constexpr float32_t F360_KPH2MPS(const float32_t x) { return (x / 3.6F); } // converts from km/hr to m/s

   static constexpr float32_t INFTY = 1E+36F;

   // Aliases.
   static constexpr int32_t F360_INVALID_ID = -1;
   static constexpr uint8_t F360_INVALID_UNSIGNED_ID = 0U;
   static constexpr int32_t F360_INVALID_REDUCED_ID = 0;


   static constexpr float32_t F360_MIN_PRIORITY = 0.0F;
   static constexpr float32_t F360_MAX_PRIORITY = 1.0F;

   static constexpr float32_t F360_EPSILON = 1.19e-07F;

   static constexpr uint8_t INTERNAL_REFLECTIONS_BUFFER_SIZE = 64U; // This is an arbitrary tradeoff between needed buffer size and memory usage, same size as GDSR

   static constexpr int8_t MSMT_UPDATE_NUM_BINS_PER_SENSOR = 5;

   // Other
   static constexpr uint8_t F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE = 20U;

   static constexpr uint8_t MAX_NUM_POINTING_GRID_POINTS = 7U;

   static constexpr int8_t MAX_NUM_ANGLE_JUMPS = 8; // number of angle jump candidates

}
#endif
