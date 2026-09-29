#ifndef F360_POPULATE_DETECTIONS_LOG_H
#define F360_POPULATE_DETECTIONS_LOG_H
/*===================================================================================*\
* FILE:  f360_populate_detections_log.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*------------------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*/

#include "f360_log_types.h"
#include "rspp_detection_list.h"
#include "f360_tracker_info.h"
#include "f360_detection_props.h"
#include "f360_constants.h"

namespace f360_variant_A {
   void Populate_Detections_Log(F360_Detection_Log_Output_T* const log, const rspp_variant_A::RSPP_Detection_List_T &det_list,
      const F360_Detection_Props_T (&detections)[MAX_NUMBER_OF_DETECTIONS]);
}

#endif 
