/*===========================================================================*\
* FILE: f360_data_generator.cpp
*============================================================================
* Copyright © 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains common support functions for data generation for unit testing
*
* ABBREVIATIONS:
*   None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===========================================================================*/

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "f360_data_generator.h"

#include "f360_math.h"

/*===========================================================================*\
* FUNCTION: Get_Azimuth_From_Cart_Pos()
*===========================================================================
* RETURN VALUE:
* float32_t - azimuth of cartesian coordinates.
*
* PARAMETERS:
* const float32_t posn_lat,
* const float32_t posn_long
*
* EXTERNAL REFERENCES:
* None.
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT: 
* Based on  lateral and longitudinal vector psotion,
* Function calculates azimuth of cartesian coordinates. 
* --------------------------------------------------------------------------
*
* PRECONDITIONS:
* None
*
* POSTCONDITIONS:
* None
*
\*===========================================================================*/
namespace f360_variant_A
{
   float32_t Get_Azimuth_From_Cart_Pos(
      const float32_t posn_lat,
      const float32_t posn_long
   )
   {
      float32_t azimuth = F360_Atan2f(posn_lat, posn_long);
      return azimuth;
   }
}
