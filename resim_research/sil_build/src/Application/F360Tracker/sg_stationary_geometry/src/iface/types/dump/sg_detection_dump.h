/*===================================================================================*\
* FILE: sg_detection_dump.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains SG component detection dump type definition
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_DETECTION_DUMP_H
#define SG_DETECTION_DUMP_H

#include "sg_drivability_class.h"

namespace sg
{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4820)
#endif

   struct SG_Detection_Dump_T
   {
      struct Position_3D_T
      {
         float x;
         float y;
         float z;
      } position; // [m] 3 dimensional position

      struct Covariance_T
      {
         float xx;
         float yy;
         float xy;
      } position_cov; // [m^2] covariance of 2D position on ground plane

      struct DB_Scan_T
      {
         float current_num_neighbors;   // [-] number of neighbors in current scan
         float cumulated_num_neighbors; // [-] filtered number of neighbors using data from previous scans
         bool f_core;                   // [-] flag indicating core detection
         bool f_visited;                // [-] flag indicating if detection was used in clustering
      } db_scan;

      Position_3D_T position_squeezed; // [m] transformed position used in clustering

      float existence_probability;    // [-] 0..1
      float probability_of_detection; // [-] probability that sensor can detect entity for given position
      float range_rate_compensated;   // [m/s] compensated range rate (radial velocity OTG towards sensor, positive means det
                                      // approaches sensor)
      float importance;               // [-] the lower the value, the lower chance of being used in current iteration

      uint32_t unique_id; // [-] 0 means that this detection is not valid, can it happen when we store detection in embedded list???
      uint32_t contour_id;    // [-] 0 means that detection is not yet assigned to contour
      uint32_t segment_id[2]; // [-] 0 means that detection is not yet assigned to segment
      uint16_t cluster_id;    // [-] 0 means that detection is not yet assigned to a cluster
      uint16_t age;           // [-] number of SG cycles this detection exists in detection storage

      SG_Drivability_Class_T drivability; // [-] class of drivability of detection
      int8_t look_id;
      bool f_used_in_measurement_update; // [-] flag indicating if detection was used in measuremet update
      bool f_valid;                      // [-] validity of detection
   };
#ifdef _MSC_VER
#pragma warning(pop)
#endif
}

#endif
