/*===================================================================================*\
* FILE: f360_dead_zone_status.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of enumeration used to identify whether object dependency to dead zone.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef F360_DEAD_ZONE_STATUS
#define F360_DEAD_ZONE_STATUS

#include "f360_reuse.h"

namespace f360_variant_A
{
   /* F360_Dead_Zone_Status_T
   * Abstract:
   *    Dead zone defines extreme longitudinal positions of sensors. Extended dead zone extends those positions by assumed calibration values.
   *    Dead zone contains entirely inside extended dead zone.
   */
   typedef enum : uint8_t
   {
      UNDEFINED = 0, // Tracker dead zone status was not analysed
      OUTSIDE = 1, // Entire object is not inside neither dead zone nor inside extended dead zone
      INSIDE = 2, // Object centroid is inside dead zone, regardless of bumpers positions
      ENTERING_REAR = 3, // Object centroid is not inside dead zone and front bumper is in extended dead zone but outside basic dead zone
      ENTERING_FRONT = 4, // Object centroid is not inside dead zone and rear bumper is in extended dead zone but outside basic dead zone
      LEAVING_FRONT = 5, // Object is leaving dead zone from front (obsolete)
      LEAVING_REAR = 6, // Object is leaving dead zone from rear (obsolete)
      IN_FRONT = 7, // Object centroid is not inside dead zone and rear bumper is in dead zone
      IN_REAR = 8 // Object centroid is not inside dead zone and front bumper is in dead zone
   }F360_Dead_Zone_Status_T;
}
#endif
