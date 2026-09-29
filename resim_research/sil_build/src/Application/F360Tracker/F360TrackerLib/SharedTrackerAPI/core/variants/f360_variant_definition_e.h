/*===================================================================================*\
* FILE: f360_variant_definition_e.h
*====================================================================================
* Copyright (C) 2022 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*  Definition of constants that are variant specific
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_VARIANT_DEFINITION_E_H
#define F360_VARIANT_DEFINITION_E_H

#include "f360_reuse.h"
#include "f360_variant_type.h"
namespace f360_variant_E
{
   static constexpr F360_Tracker_Variant_T VARIANT_TYPE = F360_VARIANT_TYPE_E; // Variant type E
   static constexpr uint32_t NUMBER_OF_OBJECT_TRACKS = 128U;
   static constexpr uint32_t NUMBER_OF_REDUCED_OBJECT_TRACKS = 64U;

   static constexpr uint16_t NUMBER_OF_CLUSTERS = 400U; // maximum number of clusters that can be tracked
   static constexpr uint16_t MAX_NUMBER_OF_HISTORIC_DETECTIONS = 400U;
   static constexpr uint32_t MAX_TRACKER_POSN_CLUSTERS = 40U; // number of clusters that can be initialized every tracker loop

   static constexpr uint8_t MAX_NUMBER_OF_SRR_SENSORS = 4U;
   static constexpr uint8_t MAX_NUMBER_OF_MRR_SENSORS = 0U;
   static constexpr uint16_t NUMBER_OF_SRR_DETECTIONS = 128U;
   static constexpr uint16_t NUMBER_OF_MRR_DETECTIONS = 0U;

   static constexpr uint8_t MAX_DETS_IN_OBJ_TRK = 40U;
   static constexpr uint8_t MAX_HIST_DETS_IN_CLUSTER = 40U;
}
#endif
