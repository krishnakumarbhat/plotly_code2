/*===================================================================================*\
* FILE: f360_calculate_curvi_position.h
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
* This file contains function signature for all Curvi convert methods
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): f360_kill_obj_trk.h
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
#ifndef F360_CALCULATE_CURVI_POSITION_H
#define F360_CALCULATE_CURVI_POSITION_H

#include "f360_reuse.h"
#include "f360_host.h"

namespace f360_variant_A
{
   float32_t Calculate_Curvi_Lat_Pos(
      const F360_Host_T & host_props,
      const float32_t long_pos, 
      const float32_t lat_pos
   );
}

#endif
