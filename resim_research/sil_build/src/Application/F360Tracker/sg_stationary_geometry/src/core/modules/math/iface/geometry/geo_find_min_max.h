/*=============================================================================================*\
* FILE: geo_find_min_max.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of geometry find_min finctions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef GEO_FIND_MIN_MAX_H
#define GEO_FIND_MIN_MAX_H

#include "geometry/geo_rectangle.h"

namespace sg
{
   namespace geometry
   {
      /**
       * @brief         Function min x value from rectangle vertices position
       *
       * @param [in]    rectangle
       *
       * @return        min x value
       **/
      float find_min_x(const Rectangle_T &rectangle);

      /**
       * @brief         Function max x value from rectangle vertices position
       *
       * @param [in]    rectangle
       *
       * @return        max x value
       **/
      float find_max_x(const Rectangle_T &rectangle);

      /**
       * @brief         Function returns min max x value from rectangle vertices position.
       *                The min value is in pair.first, the max value is in pair.second
       *
       * @param [in]    rectangle
       *
       * @return        pair<min, max> x value
       **/
      std::pair<float, float> find_min_max_x(const Rectangle_T &rectangle);
   }
}
#endif