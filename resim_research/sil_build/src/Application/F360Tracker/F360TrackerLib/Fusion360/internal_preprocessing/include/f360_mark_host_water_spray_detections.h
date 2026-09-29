/*===========================================================================*\
* FILE: f360_mark_host_water_spray_detections.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations of Mark_Host_Water_Spray_Detections() and related
* support functions.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/


#ifndef F360_MARK_HOST_WATER_SPRAY_DETECTIONS_H
#define F360_MARK_HOST_WATER_SPRAY_DETECTIONS_H

#include "f360_reuse.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_host.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Mark_Host_Water_Spray_Detections(
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      const F360_Host_T &host,
      const F360_Calibrations_T &calibs,
      F360_Detection_Props_T (&dets_props)[MAX_NUMBER_OF_DETECTIONS]
   );

   bool Is_Det_Valid_For_Host_Water_Spray(
      const rspp_variant_A::RSPP_Detection_T &det,
      const F360_Detection_Props_T &det_p,
      const F360_Calibrations_T &calibs,
      const F360_Host_T &host
   );
}
#endif
