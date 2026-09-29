/*=============================================================================================*\
* FILE: sg_squeeze_detections.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for squeeze_detections and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_SQUEEZE_DETECTIONS_H
#define SG_SQUEEZE_DETECTIONS_H

#include "sg_calibrations.h"
#include "sg_detection_storage.h"

namespace sg
{
   using Intervals_T = std::array<sg::Squeeze_Params_T, 3U>;

   /**
    * @brief          Squeezes x & y coordinates for all valid detections in DetectionStorage.
    *
    * @param[in, out] detections
    * @param[in]      coefficients - linear piecewise transform coefficients
    **/
   void squeeze_detections(DetectionStorage &detections, const Linear_Piecewise_Transform_Coefficients_T &coefficients);

   /**
   * @brief          Function takes inputs and scales them with a piecewise linear
                     function defined on 3 intervals.Each piece is defined by y = kx + b and
                     its endpoint is the beginning of the subsequent function.
   *
   * @param[in]      position
   * @param[in]      intervals
   *
   * @return         scaled_position
   **/
   float piecewise_linear_scaling(const float position, const Intervals_T &intervals);

   /**
    * @brief          Function transforms x coordinate with a piecewise linear function.
    *
    * @param[in]      detection
    * @param[in]      intervals
    *
    * @return         x_transformed
    **/
   float transform_x(const Detection_T &detection, const Intervals_T &intervals);

   /**
    * @brief          Function transforms y coordinate with a piecewise linear function.
    *
    * @param[in]      detection
    * @param[in]      intervals
    *
    * @return         y_transformed
    **/
   float transform_y(const Detection_T &detection, const Intervals_T &intervals);

   /**
    * @brief          Gets index of interval related to provided value.
    *
    * @param[in]      value
    * @param[in]      intervals
    *
    * @return         idx
    **/
   std::size_t get_pos_corresp_interval_idx(const float value, const Intervals_T &intervals);

   /**
    * @brief          Function checks if value is in range between low and high.
    *
    * @param[in]      value
    * @param[in]      low
    * @param[in]      high
    *
    * @return         is_in_range
    **/
   inline bool in_range(const float value, const float low, const float high)
   {
      return (low < value) && (value <= high);
   }
}
#endif
