/*=============================================================================================*\
* FILE: sg_try_initialize_contour.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for try_initialize_contour and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_TRY_INITIALIZE_CONTOUR_H
#define SG_TRY_INITIALIZE_CONTOUR_H

#include <bitset>

#include "sg_calibrations.h"
#include "sg_contour.h"
#include "sg_contour_storage.h"
#include "sg_detection_storage.h"

namespace sg
{
   enum class InitializationResult : uint8_t
   {
      EMPTY = 0,
      CREATED,
      OUT_OF_MAX_NUM_VERTICES
   };

   /**
    * @brief   Try to initialize contour.
    *
    * @param[in, out]   contours
    * @param[in]        current_cluster_ptr
    * @param[in]        contour_initialization_calibrations
    * @param[in]        common_calibrations
    *
    * @return  pair of result flag and contour
    **/
   std::pair<InitializationResult, Contour_T>
   try_initialize_contour(ContourStorage &contours,
                          const Cluster *const current_cluster_ptr,
                          const Contour_Initialization_Calibrations_T &contour_initialization_calibrations,
                          const Common_Calibrations_T &common_calibrations);


   /**
    * @brief   Creates contour based on number of vertices and other input parameters.
    *
    * @param[in]        contour_initialization_calibrations
    * @param[in]        common_calibrations
    * @param[in]        num_free_slots_for_vertices
    * @param[in]        cluster_id
    * @param[in]        drivability_class
    * @param[in, out]   vertices
    * @param[in, out]   segment_id_handler
    *
    * @return  pair of result flag and contour
    **/
   std::pair<InitializationResult, Contour_T> create_contour(const Contour_Initialization_Calibrations_T &contour_initialization_calibrations,
                                                             const Common_Calibrations_T &common_calibrations,
                                                             const uint16_t num_free_slots_for_vertices,
                                                             const uint16_t cluster_id,
                                                             const SG_Drivability_Class_T drivability_class,
                                                             Contour_T::VertexList &vertices,
                                                             IdHandlerIncremental<uint32_t> &segment_id_handler);
}
#endif
