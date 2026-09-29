/*=============================================================================================*\
* FILE: geo_intervals.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of geometry intervals type.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef GEO_INTERVALS_H
#define GEO_INTERVALS_H

#include <algorithm>
#include <array>

#include "geometry/geo_rectangle.h"

namespace sg
{
   namespace geometry
   {
      /**
       * @brief This class defines intervals on the x and y axis
       **/
      class Intervals_T
      {
        private:
         float m_min_x{}; // [-] minimum longitudinal bound
         float m_max_x{}; // [-] maximum longitudinal bound
         float m_min_y{}; // [-] minimum lateral bound
         float m_max_y{}; // [-] maximum lateral bound

        public:
         /**
          * @brief default constructor, coordinates not initialized
          **/
         Intervals_T() = default;

         /**
          * @brief Parametric constructor
          * @param [in] min_x_in: minimum longitudinal bound
          * @param [in] max_x_in: maximum longitudinal bound
          * @param [in] min_y_in: minimum lateral bound
          * @param [in] max_y_in: maximum lateral bound
          **/
         Intervals_T(const float min_x_in, const float max_x_in, const float min_y_in, const float max_y_in)
             : m_min_x{min_x_in}, m_max_x{max_x_in}, m_min_y{min_y_in}, m_max_y{max_y_in} {};

         /**
          * @brief Parametric constructor
          * @param [in] rectangle
          **/
         Intervals_T(const Rectangle_T &rectangle)
         {
            const std::array<float, 4> x_coordinates = {
               {rectangle.vertex_a().x, rectangle.vertex_b().x, rectangle.vertex_c().x, rectangle.vertex_d().x}};
            const std::array<float, 4> y_coordinates = {
               {rectangle.vertex_a().y, rectangle.vertex_b().y, rectangle.vertex_c().y, rectangle.vertex_d().y}};
            const auto minmax_x = std::minmax_element(x_coordinates.begin(), x_coordinates.end());
            const auto minmax_y = std::minmax_element(y_coordinates.begin(), y_coordinates.end());
            m_min_x             = *minmax_x.first;
            m_max_x             = *minmax_x.second;
            m_min_y             = *minmax_y.first;
            m_max_y             = *minmax_y.second;
         }

         float min_x() const
         {
            return m_min_x;
         }

         float max_x() const
         {
            return m_max_x;
         }

         float min_y() const
         {
            return m_min_y;
         }

         float max_y() const
         {
            return m_max_y;
         }
      };
   }
}
#endif
