/*===================================================================================*\

FILE: sg_detection.cpp
*====================================================================================

Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*

DESCRIPTION:
This file contains definitions of Detection_T methods.
*
*

Applicable Standards (in order of precedence: highest first):
ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "sg_detection.h"

#include "sg_constants.h"

namespace sg
{
   /*=============================================================================================*\

   * Method        Detection_T()
   *
   * Description   Constructor of Detection_T. Initializes default values.
   *
   * Parameters    N/A
   *
   * Returns       N/A
   *=============================================================================================*/
   Detection_T::Detection_T()
       : position{0.0F, 0.0F, 0.0F},
         position_cov{0.0F, 0.0F, 0.0F},
         position_squeezed{0.0F, 0.0F, 0.0F},

         cluster{nullptr},

         existence_probability{0.0F},
         probability_of_detection{0.0F},
         range_rate_compensated{0.0F},
         importance{0.0F},
         current_num_neighbors{0.0F},
         cumulated_num_neighbors{0.0F},
         distance_to_contour{INVALID_DISTANCE},

         unique_id{SG_INVALID_UNSIGNED_ID},
         contour_id{INVALID_CONTOUR_ID},
         segment_id{INVALID_SEGMENT_ID, INVALID_SEGMENT_ID},
         vertex_age{INAVLID_VERTEX_AGE},
         cluster_id{INVALID_CLUSTER_ID},
         age{0U},

         drivability{SG_Drivability_Class_T::UNCLASSIFIED},
         look_id{0},

         f_dbscan_core{false},                // meta data, overwritten in every clustering cycle
         f_dbscan_visited{false},             // meta data, overwritten in every clustering cycle
         f_subset{false},                     // meta data, overwritten in every clustering cycle
         f_new{false},                        // meta data, overwritten in every clustering cycle
         temp_cluster_id{INVALID_CLUSTER_ID}, // meta data, overwritten in every clustering cycle

         f_used_in_measurement_update{false},
         padding{}
   {
   }
}
