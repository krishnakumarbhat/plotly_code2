/*===================================================================================*\
* FILE: f360_calculate_curvi_position.cpp
*====================================================================================
* Copyright 2018 Delphi Technologies, Inc., All Rights Reserved.
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
*   This file contains implementation for Curvi position conversions
*
\*==========================================================================================*/

/******************************
 * Includes
 *******************************/
#include "f360_calculate_curvi_position.h"

namespace f360_variant_A
{
/*===========================================================================*\
* FUNCTION: Calculate_Curvi_Lat_Pos()
*===========================================================================
* RETURN VALUE:
* float32_t
*
* PARAMETERS:
* const float32_t curvature
* const float32_t long_pos
* const float32_t lat_pos
*
* EXTERNAL REFERENCES:
* None.
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Computes curvilinear lateral position by subtracting the curvature-induced
* lateral offset from the VCS lateral position.
*
* PRECONDITIONS:
* None
*
* POSTCONDITIONS:
* None
*
\*===========================================================================*/
float32_t Calculate_Curvi_Lat_Pos(const float32_t curvature, const float32_t long_pos, const float32_t lat_pos)
{
   const float32_t x_vcs_squared   = long_pos * long_pos;
   const float32_t y_curvi_lat_pos = lat_pos - (0.5F * curvature * x_vcs_squared);

   return y_curvi_lat_pos;
}
} // namespace f360_variant_A
