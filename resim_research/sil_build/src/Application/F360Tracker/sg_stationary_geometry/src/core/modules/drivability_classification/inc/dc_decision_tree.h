/*===================================================================================*\
* FILE: dc_decision_treey.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of the DC_Decision_Tree class.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_DECISION_TREE
#define DC_DECISION_TREE

#include "dc_features.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      class DC_Decision_Tree
      {
        public:
         /**
          * @brief            Calculates probability of the obstacle
          *
          * @param[in]        subsegment features
          *
          * @return[in]       probability_of_obstacle
          **/
         static float predict_obstacle_prob(const Features_T &features);
      };
   }
}
#endif
