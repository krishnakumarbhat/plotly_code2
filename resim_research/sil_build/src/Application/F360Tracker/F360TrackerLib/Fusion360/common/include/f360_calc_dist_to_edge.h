#ifndef F360_CALC_DIST_TO_EDGE_H
#define F360_CALC_DIST_TO_EDGE_H
/*===========================================================================*\
* FILE: f360_calc_dist_to_edge.h
*============================================================================
* Copyright © 2019 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*----------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Calculate_Distance_To_Edge().
*
* ABBREVIATIONS:
*   None
*
* TRACEABILITY INFO:
*   Requirements Document(s):
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===========================================================================*/

#include "f360_point.h"

namespace f360_variant_A
{
   float32_t Calculate_Distance_To_Edge(
      const Point & p1,
      const Point & p2,
      const Point & det);
}
#endif
