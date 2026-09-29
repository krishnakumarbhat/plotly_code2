/*===================================================================================*\
* FILE:  f360_fine_cluster_gate.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the declaration of the function Fine_Cluster_Gate() which can be used
* to check if two clusters are close to each other in position. The two clusters may be of
* different age and the possible position difference of the clusters casued by this is 
* accounted for by utilizing the range rate of the older cluster together with the age 
* difference
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
\*===================================================================================*/
#ifndef F360_FINE_CLUSTER_GATE_H
#define F360_FINE_CLUSTER_GATE_H

#include "f360_reuse.h"
#include "f360_calibrations.h"
#include "f360_constants.h"
#include "f360_cluster.h"
#include "f360_detection_props.h"
#include "f360_detection_hist.h"
namespace f360_variant_A
{
   bool Fine_Cluster_Gate(
      const F360_Calibrations_T& calib,
      const F360_Cluster_T& cluster1,
      const F360_Cluster_T& cluster2,
      const float32_t rdot_interval_1,
      const float32_t rdot_interval_2,
      const float32_t n_aliased_1,
      const float32_t n_aliased_2
   );

   float32_t Calc_Cross_Radial_Gate(
      const F360_Calibrations_T& calib,
      const float32_t abs_mean_rdot_comp,
      const float32_t range,
      const float32_t max_cross_radial_speed,
      const float32_t delta_time
   );

   float32_t Calc_Cross_Radial_Gate_Short_Range(
       const F360_Calibrations_T& calib,
       const float32_t range,
       const float32_t max_cross_radial_speed,
       const float32_t delta_time
   );

   float32_t Calc_Cross_Radial_Gate_Long_Range(
       const F360_Calibrations_T& calib,
       const float32_t abs_mean_rdot_comp,
       const float32_t range,
       const float32_t max_cross_radial_speed,
       const float32_t delta_time
   );


   float32_t Calc_Radial_Gate(
      const F360_Calibrations_T& calib,
      const float32_t abs_mean_rdot_comp,
      const float32_t range
   );
   
   void Close_Range_Beside_Host_Adjustment(
      const F360_Calibrations_T& calib,
      const float32_t range,
      const float32_t trk_az_rep_sin,
      float32_t& radial_gate,
      float32_t& max_cross_radial_speed
   );
}
#endif
