/*===================================================================================*\
* FILE: dc_time_update_subsegments.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains time update subsegments handcode.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef DC_TIME_UPDATE_SUBSEGMENTS_H
#define DC_TIME_UPDATE_SUBSEGMENTS_H

#include "rspp_reuse.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      class DCContourStorage;

      class TimeUpdateSubsegments
      {
        private:
         TimeUpdateSubsegments() = default;

        public:
         static void timeUpdateSubsegments(DCContourStorage &contour_list,
                                           const float elapsed_time,
                                           const float32_t speed,
                                           const float cos_heading,
                                           const float sin_heading);
      };
   }
}
#endif
