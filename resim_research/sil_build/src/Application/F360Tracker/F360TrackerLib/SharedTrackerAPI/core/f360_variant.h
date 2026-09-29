/*===================================================================================*\
* FILE: f360_variant.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of variant struct.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_VARIANT_VARIANT_A_H
#define F360_VARIANT_VARIANT_A_H

#include "../Types/f360_reuse.h"
#include "../Types/f360_variant_type.h"

namespace f360_variant_A
{
   typedef struct F360_Variant_Tag
   {
      uint32_t num_tracks;
      uint32_t num_reduced_tracks;
      uint32_t num_posn_clusters;
      uint16_t num_clusters;
      uint16_t num_hist_dets;
      uint8_t num_dets_in_track;
      uint8_t num_hist_dets_in_cluster;
      F360_Tracker_Variant_T  type;
   }F360_Variant_T;
}
#endif
