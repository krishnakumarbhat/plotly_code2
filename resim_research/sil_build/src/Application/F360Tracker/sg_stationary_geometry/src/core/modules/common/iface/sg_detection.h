/*===================================================================================*\
* FILE: sg_detection.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains SG component detection type definition.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/


#ifndef SG_DETECTION_H
#define SG_DETECTION_H

#include <array>

#include "geometry/geo_point.h"
#include "sg_basic.h"
#include "sg_drivability_class.h"

namespace sg
{
   class Cluster;

   struct Detection_T
   {
      Detection_T();

      geometry::Point3D_T position;          // [m] 3 dimensional position
      Covariance_2D position_cov;            // [m^2] 2 dimensional position covariance
      geometry::Point3D_T position_squeezed; // [m] transformed position used in clustering

      Cluster *cluster; // [-] pointer to a cluster which contains this detection

      float existence_probability;    // [-] 0..1
      float probability_of_detection; // [-] probability that sensor can detect entity for given position
      float range_rate_compensated;   // [m/s] compensated range rate (radial velocity OTG towards sensor, positive means det
                                      // approaches sensor)
      float importance;               // [-] the lower the value, the lower chance of being used in current iteration
      float current_num_neighbors;    // [-] number of neighbors in current scan
      float cumulated_num_neighbors;  // [-] filtered number of neighbors using data from previous scans
      float distance_to_contour;      // [-] INVALID_DISTANCE means that detection is not yet assigned to a segment

      uint32_t unique_id;  // [-] SG_INVALID_UNSIGNED_ID means that this detection is not valid, can it happen when we store
                           // detection in embedded list???
      uint32_t contour_id; // [-] INVALID_CONTOUR_ID means that detection is not yet assigned to a contour
      std::array<uint32_t, 2U> segment_id; // [-] INVALID_SEGMENT_ID means that detection is not yet assigned to a segment
      uint16_t vertex_age;                 // [-] INVALID_VERTEX_AGE means that detection is not yet assigend to a segment
      uint16_t cluster_id;                 // [-] INVALID_CLUSTER_ID means that detection is not yet assigned to a cluster
      uint16_t age;                        // [-] number of SG cycles this detection exists in detection storage

      SG_Drivability_Class_T drivability; // [-] class of drivability of detection
      int8_t look_id;

      // these to be used for meta data
      bool f_dbscan_core;       // [-] flag indicating core detections
      bool f_dbscan_visited;    // [-] flag indicating if detection was used in clustering
      bool f_subset;            // [-] flag indicating if detection belongs to relevant subset
      bool f_new;               // [-] flag indicating if detection was added in the current cycle
      uint16_t temp_cluster_id; // [-] temporary cluster id, reset before every clustering

      bool f_used_in_measurement_update; // [-] flag indicating if detection was used in measurement update
      uint8_t padding[2]{};
   };
}

#endif
