#ifndef RSPP_DETECTION_VARIANT_A_H
#define RSPP_DETECTION_VARIANT_A_H
/*===========================================================================*/
/**
 * @file rspp_detection.h
 *
 * @brief Detection Data Structure
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Detection data structure definitions for raw and processed detection data.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
 *
 *   - Requirements Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/51-SoftwareRequirementsSpecifications/CMP_SRS_TrackerCore
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - https://confluence.asux.aptiv.com/spaces/F360Core/pages/129995883/Coding+Guidelines
 *     - ESGW_4-2_PE-SWX_00-01-A01_EN - C++ Coding Standards [20190526]
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None.
 *
 * @ updates to areas outside the scope of procedures:
 *   - Refer to module footer comment block.
 *
 * @defgroup rspp_detection Detection
 * @{
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include "rspp_reuse.h"

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "rspp_detection_motion_status.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace rspp_variant_A
{
   /*===========================================================================*
    * Exported Enum Class Declarations
    *===========================================================================*/
   /******************************************************************************
    * Name:  RSPP_Azimuth_Confidence_T
    *   Enumeration for azimuth confidence levels.
    ******************************************************************************/
   typedef enum RSPP_Azimuth_Confidence_Tag
   {
      RSPP_CONF_AZIMUTH_HIGH = 0,
      RSPP_CONF_AZIMUTH_MIDHIGH = 1,
      RSPP_CONF_AZIMUTH_MIDLOW = 2,
      RSPP_CONF_AZIMUTH_LOW = 3,
   } RSPP_Azimuth_Confidence_T;

   /*===========================================================================*
    * Exported Type Declarations
    *===========================================================================*/
   /******************************************************************************
    * Name:  Raw_Detection_T
    *   Structure for raw detection data from sensors.
    *
    * Structure Members:
    *   range          - [m] Range of detection
    *   std_range      - [m] Standard deviation of range of detection
    *   range_rate     - [m/s] Range rate of detection
    *   std_range_rate - [m/s] Standard deviation of range rate of detections
    *   azimuth        - [rad] Raw Azimuth angle. Not compensated for alignment or polarity
    *   std_azimuth    - [rad] Standard deviation of Azimuth angle.
    *   elevation      - [rad] Raw Elevation angle. Not compensated for alignment or polarity
    *   std_elevation  - [rad] Standard deviation of Elevation angle.
    *   snr            - [-] Signal to noise ratio
    *   rcs            - [dB/m^2] RCS normalized rcs of detection
    *   prob_1stazhypo - [0-1] Probability for the reported azimuth angle, assumed 1 if not available
    *   sensor_id     - Id of the sensor that the detection came from
    *   det_id         - Id from the sensor that the detection came from (Unique only to that sensor)
    *   confid_azimuth  - Confidence on azimuth, 0 = best, 3 = worst
    *   confid_elevation- Confidence on elevation, 0 = best, 3 = worst
    *   f_super_res     - Flag indicating that super resolution branch have been used by the sensors angle finding algo
    *   f_host_veh_clutter - Flag indicating that this detection stems from host vehicle itself
    *   f_nd_target     - Flag indicating that this detection seems to stem from a small target close to a larger target
    *   f_bistatic      - Flag indicating that this detection is a bistatic detection
    *   f_ci_det       - Flag indicating that this detection is a CI (Coherent Integration) detection
    *   f_idm_det      - Flag indicating that this detection is a IDM (Interference Detection Mitigation) detection
    *   f_below_rain_thold - Flag indicating rain per detection
    ******************************************************************************/
   typedef struct Raw_Detection_Tag
   {
      float32_t range;
      float32_t std_range;
      float32_t range_rate;
      float32_t std_range_rate;
      float32_t azimuth;
      float32_t std_azimuth;
      float32_t elevation;
      float32_t std_elevation;
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
   } Raw_Detection_T;

   /*===========================================================================*
    * Exported Type Declarations
    *===========================================================================*/
   /******************************************************************************
    * Name:  Processed_Detection_T
    *   Structure for processed detection data.
    *
    * Structure Members:
    *   vcs_position_x         - longitudinal position in Vehicle Coordinate System (VCS) [m]
    *   vcs_position_y         - lateral position in Vehicle Coordinate System (VCS) [m]
    *   vcs_position_z         - z coordinate in Vehicle Coordinate System (VCS) [m],
    *                            Note: The zero plane is defined at ground level and negative above ground
    *   range_rate_compensated - (Raw detection extension) range rate after host motion compensation
    *                            (like Over-The-Ground range rate, extesion - raw detection has the same signal) [m/s]
    *   vcs_az                 - [rad] azimuth aligned with VCS (corrected by boresight angle in VCS)
    *   vcs_el                 - NOTE: This signal is to be compensated by elevation misaligment in future releases.
    *                            For now it is populated by the raw elevation angle but compensated by polarity [rad]
    *   cos_vcs_az             - [-] cosine of vcs azimuth
    *   sin_vcs_az             - [-] sine of vcs azimuth
    *   next_sorted_idx        - index of the next sorted detection (with higher VCS longitudinal position) [-]
    *   prev_sorted_idx        - index of the previous sorted detection (with lower VCS longitudinal position) [-]
    *   motion_status          - [-] -1: invalid, 0: stationary, 1: moving, 2: ambiguous status of detection motion
    *   f_ok_to_use            - flag indicating that detection pass plausibility checks [-]
    ******************************************************************************/
   typedef struct Processed_Detection_Tag
   {
      float32_t vcs_position_x;
      float32_t vcs_position_y;
      float32_t vcs_position_z;
      float32_t range_rate_compensated;
      float32_t vcs_az;
      float32_t vcs_el;
      float32_t cos_vcs_az;
      float32_t sin_vcs_az;
      int16_t next_sorted_idx;
      int16_t prev_sorted_idx;
      int8_t motion_status;
      bool f_ok_to_use;
      uint8_t unused[2];
   } Processed_Detection_T;

   /******************************************************************************
    * Name:  RSPP_Detection_T
    *   Structure for a detection, including both raw and processed data.
    *
    * Structure Members:
    *   raw       - substruct populated with raw detections information from the sensors
    *   processed - substruct of the output provided by Inputs Preprocessing
    ******************************************************************************/
   typedef struct RSPP_Detection_Tag
   {
      Raw_Detection_T raw;
      Processed_Detection_T processed;
   } RSPP_Detection_T;

   /*===========================================================================*
    * Exported Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Function Declarations
    *===========================================================================*/

   static_assert(64 == sizeof(Raw_Detection_T),
                 "sizeof(Raw_Detection_T) not as expected. Remember to align padding if needed");
   static_assert(40 == sizeof(Processed_Detection_T),
                 "sizeof(Processed_Detection_T) not as expected. Remember to align padding if needed");
   static_assert(104 == sizeof(RSPP_Detection_T),
                 "sizeof(RSPP_Detection_T) not as expected. Remember to align padding if needed");

} // namespace rspp_variant_A

/** @} doxygen end group */
#endif

/*============================================================================*\
 * AUTHOR(S) IDENTITY (AID)
 *-----------------------------------------------------------------------------
 *
 *  AID         NAME
 *  ---------------------------------------------------------------------------
 *  wzfkqj      Tobias Almroth
\*============================================================================*/

/*============================================================================*\
 * FILE REVISION HISTORY
 *-----------------------------------------------------------------------------
 *
 *  File history can be traced by URL:
 *  "https://gitgerrit.asux.aptiv.com/q/project:CORECOMP%252FALSW%252FOT_ObjectTracking"
\*============================================================================*/

/* END OF FILE -------------------------------------------------------------- */
