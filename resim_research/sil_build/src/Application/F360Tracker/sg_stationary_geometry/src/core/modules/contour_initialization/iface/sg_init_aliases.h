/*===========================================================================*\
* FILE: sg_init_aliases.h
*============================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains aliases of typenames used in contour initialization.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_INIT_ALIASES_H
#define SG_INIT_ALIASES_H

#include "embedded_list.h"
#include "sg_constants.h"
#include "sg_detection_storage.h"

namespace sg
{
   using NeighborIterators = EmbeddedList<DetectionList::iterator, SG_MAX_NUM_DET_CLUSTERING_NEIGHBOURS>;
   using NewNeighborIterators =
      EmbeddedList<DetectionList::iterator, SG_MAX_NUM_DET_CLUSTERING_NEIGHBOURS + 1>; // TODO: This is a short-term solution
                                                                                       // https://jiraprod.aptiv.com/browse/FZD-969
}

#endif
