#ifndef F360_DETECTION_VARIANT_A_H
#define F360_DETECTION_VARIANT_A_H
/*===================================================================================*\
 * FILE: f360_detection.h
 *====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   Detection data structure definitions for raw and processed detection data.
 *
 * ABBREVIATIONS:
 *  None
 *
 * TRACEABILITY INFO:
 *   Design Document(s):
 *
 *   Requirements Document(s):
 *
 *   Applicable Standards (in order of precedence: highest first):
 *  - https://confluence.asux.aptiv.com/spaces/F360Core/pages/129995883/Coding+Guidelines
 *  - ESGW_4-2_PE-SWX_00-01-A01_EN - C++ Coding Standards [20190526]
 *
 * DEVIATIONS FROM STANDARDS:
 *   None.
 *
\*==========================================================================================*/

#include "f360_reuse.h"

namespace f360_variant_A
{
/******************************************************************************
 * Name:  F360_Azimuth_Confidence_T
 *   Enumeration for azimuth confidence levels.
 ******************************************************************************/
typedef enum F360_Azimuth_Confidence_Tag
{
   F360_CONF_AZIMUTH_HIGH    = 0,
   F360_CONF_AZIMUTH_MIDHIGH = 1,
   F360_CONF_AZIMUTH_MIDLOW  = 2,
   F360_CONF_AZIMUTH_LOW     = 3,
} F360_Azimuth_Confidence_T;

/******************************************************************************
 * Name:  F360_Detection_T
 *   Structure for a detection
 *
 * Structure Members:
 *   range              - [m] Range of detection
 *   std_range          - [m] Standard deviation of range of detection
 *   range_rate         - [m/s] Range rate of detection
 *   std_range_rate     - [m/s] Standard deviation of range rate of detections
 *   azimuth            - [rad] Raw Azimuth angle. Not compensated for alignment or polarity
 *   std_azimuth        - [rad] Standard deviation of Azimuth angle.
 *   elevation          - [rad] Raw Elevation angle. Not compensated for alignment or polarity
 *   std_elevation      - [rad] Standard deviation of Elevation angle.
 *   snr                - [-] Signal to noise ratio
 *   rcs                - [dB/m^2] RCS normalized rcs of detection
 *   prob_1stazhypo     - [0-1] Probability for the reported azimuth angle, assumed 1 if not available
 *   sensor_id          - Id of the sensor that the detection came from
 *   det_id             - Id from the sensor that the detection came from (Unique only to that sensor)
 *   confid_azimuth     - Confidence on azimuth, 0 = best, 3 = worst
 *   confid_elevation   - Confidence on elevation, 0 = best, 3 = worst
 *   f_super_res        - Flag indicating that super resolution branch have been used by the sensors angle finding algo
 *   f_host_veh_clutter - Flag indicating that this detection stems from host vehicle itself
 *   f_nd_target        - Flag indicating that this detection seems to stem from a small target close to a larger target
 *   f_bistatic         - Flag indicating that this detection is a bistatic detection
 *   f_ci_det           - Flag indicating that this detection is a CI (Coherent Integration) detection
 *   f_idm_det          - Flag indicating that this detection is a IDM (Interference Detection Mitigation) detection
 *   f_below_rain_thold - Flag indicating rain per detection
 ******************************************************************************/
typedef struct F360_Detection_Tag
{
   float32_t range;
   float32_t range_rate;
   float32_t azimuth;
   float32_t elevation;
   float32_t snr;
   float32_t rcs;
   float32_t prob_1stazhypo;
   int32_t sensor_id;
   int32_t det_id;
   int8_t confid_azimuth;
   int8_t confid_elevation;
   bool f_super_res;
   bool f_host_veh_clutter;
   bool f_nd_target;
   bool f_bistatic;
   bool f_ci_det;
   bool f_idm_det;
   bool f_below_rain_thold;
   uint8_t unused[3];
} F360_Detection_T;

static_assert(48 == sizeof(F360_Detection_T), "sizeof(F360_Detection_T) not as expected. Remember to align padding if needed");

} // namespace f360_variant_A

#endif
