/*===================================================================================*\
* FILE: cmn_utilities.h
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*------------------------------------------------------------------------------------
* %full_filespec: AIT-69%
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains some small utility functions
*
* ABBREVIATIONS:
*   NONE
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
\*===================================================================================*/

#ifndef OCG_CMN_UTILITIES_H
#define OCG_CMN_UTILITIES_H

#include "rspp_mounting_location.h"
#include "ocg_calibrations.h"


inline bool is_front_center_sensor(RSPP_Mounting_Location_T loc) { return loc == RSPP_MOUNTING_LOCATION_CENTER_FORWARD; }
inline bool is_front_side_sensor(RSPP_Mounting_Location_T loc) { return  ((loc == RSPP_MOUNTING_LOCATION_LEFT_FORWARD) || (loc == RSPP_MOUNTING_LOCATION_RIGHT_FORWARD)); }
inline bool use_this_sensor(RSPP_Mounting_Location_T loc, const ocg::OCG_Calibrations_T& calbs)
{
   return ((calbs.underdrive_use_front_center_sensors && is_front_center_sensor(loc)) || (calbs.underdrive_use_front_side_sensors && is_front_side_sensor(loc)));
}
inline bool is_sensor_valid(rspp_variant_A::F360_Radar_Sensor_T sensor, const ocg::OCG_Calibrations_T& calbs)
{
   return sensor.variable.is_valid && use_this_sensor(sensor.constant.mounting_location, calbs);
}

#endif //OCG_CMN_UTILITIES_H