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
      const F360_Host_T & host_props,
      const float32_t long_pos,
      const float32_t lat_pos
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
   *
   *
   * PRECONDITIONS:
   * All the pointers should point to valid structure
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Calculate_Curvi_Lat_Pos(
      const F360_Host_T & host_props,
      const float32_t long_pos, 
      const float32_t lat_pos)
   {
      const float32_t x_vcs_squared = long_pos * long_pos;
      const float32_t y_vcs = lat_pos;

      // TODO: Use snail trail here
      // Modified host curvature slow to curvature rear available in F360 Tracker - TODO: Check if suitable
      const float32_t y_curvi_lat_pos = y_vcs - (0.5F * host_props.curvature_rear * x_vcs_squared);

      return y_curvi_lat_pos;
   }
}
