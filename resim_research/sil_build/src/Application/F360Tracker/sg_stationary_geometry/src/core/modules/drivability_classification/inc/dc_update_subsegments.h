/*===================================================================================*\
* FILE: dc_update_subsegments.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of update_subsegments function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_UPDATE_SUBSEGMENTS
#define DC_UPDATE_SUBSEGMENTS

#include <bitset>

#include "dc_contour_storage.h"
#include "dc_critical_region.h"
#include "sg_calibrations.h"
#include "sg_contour.h"
#include "sg_contour_storage.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      class DC_Update_Subsegments
      {
        public:
         /**
          * @brief         Updates one of DC contours.
          *
          * @param[out]   current_dc_contour - DC contour being updated
          * @param[out]   dc_contour_list - storage of DC contours
          * @param[in]    f_critical - vertexes in critical region
          * @param[in]    current_old_dc_contour_iter - iterator to the DC contour from previous step (before current update)
          * @param[in]    current_contour - current SG contour
          * @param[in]    critical_region - critical region object
          * @param[in]    cfg - calibrations data
          **/

         static void update_contour_subsegments(DC_Contour_T &current_dc_contour,
                                                DCContourStorage &dc_contour_list,
                                                const std::bitset<SG_MAX_NUM_VERTICES> &f_critical,
                                                const DCContourStorage::ContourList::iterator current_old_dc_contour_iter,
                                                const Contour_T &current_contour,
                                                const CriticalRegion &critical_region,
                                                const Drivability_Classification_Calibrations_T &cfg);

         /**
          * @brief         Updates DC contours container with data calculated for current scan index.
          *
          * @param[out]    dc_contour_list - reference to the storage of DC contours
          * @param[in]     contour_list - reference to the storage of SG contours
          * @param[in]     critical_region - critical region vertices
          * @param[in]     cfg - calibrations data
          **/

         static void update_subsegments(DCContourStorage &dc_contour_list,
                                        const ContourStorage &contour_list,
                                        const CriticalRegion &critical_region,
                                        const Drivability_Classification_Calibrations_T &cfg);
         // contour_list is modified inside the function
         // it should be changed to be const in the future
         // FZD-1469
         // to be refactored
         // FZD-1478

         static DCContourStorage::ContourList::iterator get_partner_dc_contour(
            const DCContourStorage &dc_contours, const DCContourStorage::ContourList::iterator dc_contour_list_it, const uint32_t id);
      };
   }
}
#endif
