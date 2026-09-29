/*===========================================================================*\
* FILE: f360_detection_multipath_filter.cpp
*============================================================================
* Copyright (C) 2023 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#include "f360_detection_multipath_filter.h"

namespace f360_variant_A
{
   static inline float32_t Get_Bad_Azimuth_Fraction_Threshold(
      const F360_Sensor_Type_T sensor_type,
      const F360_Calibrations_T & f360_calibs
   );

   static inline void List_Low_Azimuth_Confidence_Detections(
      uint32_t (&bad_az_det_idx) [MAX_DETS_FOR_SINGLE_SENSOR],
      uint32_t (&nr_of_bad_az_conf),
      const rspp_variant_A::RSPP_Detection_T(&dets)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t first_list_idx,
      const uint32_t number_of_idx
   );

   /*===========================================================================*\
   * FUNCTION: Filter_Out_Multipath_Detections()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *   const F360_Host_T& host
   *   const F360_Radar_Sensor_T& sensor
   *   const F360_Calibrations_T& f360_calibrations
   *   const uint32_t first_det_list_idx
   *   const rspp_variant_A::RSPP_Detection_T(&detections)[MAX_NUMBER_OF_DETECTIONS]
   *   F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Attempts to detect multipath detections by checking fraction of detections
   * with bad azimuth confidence. If many such detections are found we set
   * f_ok_to_use = false for all worst azimuth confidence detections.
   *
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Filter_Out_Multipath_Detections(
      const F360_Host_T& host,
      const F360_Radar_Sensor_T& sensor,
      const F360_Calibrations_T& f360_calibrations,
      const uint32_t first_det_list_idx,
      const rspp_variant_A::RSPP_Detection_T(&detections)[MAX_NUMBER_OF_DETECTIONS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      const bool first_det_list_idx_ok = ((first_det_list_idx + sensor.variable.number_of_valid_detections) < MAX_NUMBER_OF_DETECTIONS);
      if (first_det_list_idx_ok
         && (sensor.variable.number_of_valid_detections > f360_calibrations.k_min_num_valid_dets_for_bad_az_filter)
         && (host.speed > f360_calibrations.k_min_host_speed_for_bad_az_filter))
      {
         uint32_t nr_of_bad_az_conf = 0U;
         uint32_t bad_az_det_idx[MAX_DETS_FOR_SINGLE_SENSOR] = {};
         List_Low_Azimuth_Confidence_Detections(bad_az_det_idx, nr_of_bad_az_conf, detections, first_det_list_idx, sensor.variable.number_of_valid_detections);

         const float32_t fraction_bad_az = static_cast<float32_t>(nr_of_bad_az_conf) / static_cast<float32_t>(sensor.variable.number_of_valid_detections);
         const float32_t frac_threshold = Get_Bad_Azimuth_Fraction_Threshold(sensor.constant.sensor_type, f360_calibrations);

         if (fraction_bad_az > frac_threshold)
         {
            for (uint32_t i = 0U; i < nr_of_bad_az_conf; i++)
            {
               const uint32_t det_idx = bad_az_det_idx[i];
               detection_props[det_idx].f_ok_to_use = false;
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: List_Low_Azimuth_Confidence_Detections()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *   uint32_t (&bad_az_det_idx) [MAX_NUMBER_OF_DETECTIONS] - Array of
   *   detections with low azimuth confidence to be filled by the function
   *
   *   uint32_t (&nr_of_bad_az_conf) - Number of detections with bad
   *   azimuth filled by the function
   *
   *   const rspp_variant_A::F360_Detection_T (&dets)[MAX_NUMBER_OF_DETECTIONS] - Array of all
   *   detections
   *
   *   const uint32_t first_list_idx - Index for where to start looking in
   *   the array of all detections
   *
   *   const uint32_t number_of_idx - Total number of index to search
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Fills a list of all detections that have low azimuth confidence for a given
   * starting index and a total number of index to investigate.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static inline void List_Low_Azimuth_Confidence_Detections(
         uint32_t (&bad_az_det_idx) [MAX_DETS_FOR_SINGLE_SENSOR],
         uint32_t (&nr_of_bad_az_conf),
         const rspp_variant_A::RSPP_Detection_T(&dets)[MAX_NUMBER_OF_DETECTIONS],
         const uint32_t first_list_idx,
         const uint32_t number_of_idx)
   {

      nr_of_bad_az_conf = 0U;

      for (uint32_t det_idx = first_list_idx; det_idx < (first_list_idx + number_of_idx); det_idx++)
      {
         if (static_cast<int8_t>(rspp_variant_A::RSPP_CONF_AZIMUTH_LOW) == dets[det_idx].raw.confid_azimuth)
         {
            bad_az_det_idx[nr_of_bad_az_conf] = det_idx;
            nr_of_bad_az_conf++;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Get_Bad_Azimuth_Fraction_Threshold()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *   const F360_Sensor_Type_T sensor_type
   *   const F360_Calibrations_T &f360_calibs
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Returns a sensor dependent threshold for the multipath filter.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static inline float32_t Get_Bad_Azimuth_Fraction_Threshold(
      const F360_Sensor_Type_T sensor_type,
      const F360_Calibrations_T & f360_calibs)
   {
      float32_t frac_thres;

      switch (sensor_type)
      {
      case F360_SENSOR_TYPE_SRR5_RADAR:
         frac_thres = f360_calibs.k_max_fraction_of_bad_azimuth_dets_srr5;
         break;
      default:
         frac_thres = f360_calibs.k_max_fraction_of_bad_azimuth_dets_default;
         break;
      }

      return frac_thres;
   }
}

