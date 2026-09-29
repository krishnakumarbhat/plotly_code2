/*===================================================================================*\
* FILE: dc_append_remaining_sg_contours.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of dc_append_remaining_sg_contours function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_APPEND_REMAINING_SG_CONTOURS
#define DC_APPEND_REMAINING_SG_CONTOURS

#include "dc_array_wrapper.h"
#include "dc_fused_contour_storage.h"
#include "sg_calibrations.h"
#include "sg_contour_storage.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      /**
       * @brief            Appends remaining SG contours to FusedContourStorage.
       * @brief            Contours that were outside critical region and consolidated DC contours that were too large are added to
       *fused contour storage.
       * @param[out]       fused_contour_storage - reference to list of fused SG and DC contours.
       * @param[in]        contour_list - reference to list of SG contours.
       **/
      void append_remaining_sg_contours(FusedContourStorage &fused_contour_storage, const ContourStorage &contour_list);

      /**
       * @brief            Appends fused contour to fused contour storage based on data from SG contour.
       * @param[out]       fused_contour_storage - reference to list of fused SG and DC contours.
       * @param[in]        sg_contour - reference to SG contour.
       **/
      void append_fused_contour(FusedContourStorage &fused_contour_storage, const Contour_T &sg_contour);

      /**
       * @brief            Check if contour id is not present in fused contour ids list and adding a new contour is possible.
       * @param[in]        sg_contour - reference to SG contour.
       * @param[in]        fused_contour_ids_list - fused contour ids list.
       * @param[in]        number_of_fused_contours - number of currently fused contours.
       * @param[in]        number_of_fused_verticies - number of currently fused vertices.
       *
       * @return           if contour can be added to fused contour storage
       **/
      bool sg_contour_can_be_added(const Contour_T &sg_contour,
                                   const ArrayWrapper<uint32_t, SG_MAX_NUM_FUSED_CONTOURS> &fused_contour_ids_list,
                                   const uint32_t number_of_valid_fused_contours,
                                   const uint32_t number_of_fused_verticies);

      /**
       * @brief            Get ids of contours in fused contour storage.
       * @param[out]       fused_contour_ids - sorted array of all present fused contour ids, one id can be duplicated in array.
       * @param[in]        fused_contour_storage - reference to list of fused SG and DC contours.
       **/
      void get_fused_contour_ids(ArrayWrapper<uint32_t, SG_MAX_NUM_FUSED_CONTOURS> &fused_contour_ids,
                                 const FusedContourStorage &fused_contour_storage);
   }
}

#endif
