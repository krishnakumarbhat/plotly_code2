/*===================================================================================*\
* FILE: f360_time_update_object_timestamp.h
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/

#ifndef F360_OBJECT_LIST_TIMESTAMP_UPDATE_H
#define F360_OBJECT_LIST_TIMESTAMP_UPDATE_H

#include "f360_reuse.h"
#include "f360_radar_sensor.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   void Object_List_Timestamp_Update(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Tracker_Info_T& tracker_info);
}
#endif
