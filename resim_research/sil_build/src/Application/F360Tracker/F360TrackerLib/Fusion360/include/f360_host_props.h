#ifndef F360_HOST_PROPS_H
#define F360_HOST_PROPS_H
/*===================================================================================*\
* FILE: f360_host_props.h
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
*   This file contains Host  structure  declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s): defineFusion360Types.m
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/

#include "f360_reuse.h"
#include "f360_point.h"

namespace f360_variant_A
{
   typedef struct F360_Host_Props_Tag
   {
      float32_t position_x; // dead reckoning from since init [m]
      float32_t position_y; // dead reckoning from since init [m]
      float32_t delta_position_x; // Delta for host position between previous and current tracker iteration [m]
      float32_t delta_position_y; // Delta for host position between previous and current tracker iteration [m]
      float32_t heading_angle;
      float32_t cos_heading;
      float32_t sin_heading;
      float32_t delta_pointing; // Delta for host pointing angle between previous and current tracker iteration
      float32_t cos_delta_pointing; // Cosine of delta for host pointing angle between previous and current tracker iteration
      float32_t sin_delta_pointing; // Sine of delta for host pointing angle between previous and current tracker iteration
   } F360_Host_Props_T;
}
#endif


