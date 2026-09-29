/*===================================================================================*\
* FILE: dc_update_subsegments_drivability.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of update_subsegments_drivability function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_UPDATE_SUBSEGMENTS_DRIVABILITY
#define DC_UPDATE_SUBSEGMENTS_DRIVABILITY

#include "dc_contour_storage.h"
#include "dc_decision_tree.h"
#include "dc_decision_tree_overdrivability.h"
#include "sg_calibrations.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      class DC_Update_Subsegments_Drivability
      {
        private:
         /**
          * @brief            Set subsegment classification depending on decision tree
          *
          * @param[in/out]    subsegment - subsegment to be classified
          * @param[in]        cfg - configuration
          **/
         static void classify_subsegment(Subsegment_T &subsegment, const Drivability_Classification_Calibrations_T &cfg);

        public:
         /**
          * @brief            Calculates confidence of drivability classification
          *
          * @param[in]        probability_of_obstacle - obtained from decision tree
          * @param[in]        obstacle_probability_threshold
          * @param[in]        probability_of_obstacle_overdrivability - obtained from overdrivability decision tree
          * @param[in]        overdrivability_obstacle_probability_threshold
          * @param[in]        classification_uncertainty
          *
          * @return           confidence of the drivability_classification
          **/
         static float calculate_drivability_confidence(const float probability_of_obstacle,
                                                       const float obstacle_probability_threshold,
                                                       const float probability_of_obstacle_overdrivability,
                                                       const float overdrivability_obstacle_probability_threshold,
                                                       const float classification_uncertainty);

         /**
          * @brief            Update subsegments drivability classification
          *
          * @param[in/out]    contour_list - container of DC contours
          * @param[in]        cfg - configuration
          **/
         static void update_subsegments_drivability(DCContourStorage &contour_list,
                                                    const Drivability_Classification_Calibrations_T &cfg);
      };
   }
}
#endif
