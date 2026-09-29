/*===================================================================================*\
* FILE: dc_critical_region.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains critical region creation handcode
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#include "dc_critical_region.h"

#include <array>

#include "sg_math.h"

namespace sg
{
   namespace dc
   {
      CriticalRegion::CriticalRegion(const std::array<geometry::Point2D_T, DC_MAX_COORDINATES_REGION_SIZE> &coordinates)
          : m_coordinates(coordinates)
      {
      }

      void CriticalRegion::create(const float curvature, const sg::Drivability_Classification_Calibrations_T &cfg)
      {
         geometry::Point2D_T critical_region_polygon[DC_NUM_POLYGON_VERTICES_BEFORE_INTERPOLATION];
         (void) std::copy(&cfg.critical_region_polygon[0],
                          &cfg.critical_region_polygon[0] + DC_NUM_POLYGON_VERTICES_BEFORE_INTERPOLATION,
                          &critical_region_polygon[0]);
         const uint8_t critical_region_subdivisions = cfg.critical_region_subdivisions;
         const float min_abs_curvature_threshold    = cfg.critical_region_min_abs_curvature_threshold;
         const float max_abs_curvature_threshold    = cfg.critical_region_max_abs_curvature_threshold;
         uint8_t last_valid_idx                     = 0U;
         interpolate_polygon(last_valid_idx, critical_region_subdivisions, critical_region_polygon);
         const float customized_curvature = curvature;
         curvilinear_transformation(customized_curvature, min_abs_curvature_threshold, max_abs_curvature_threshold, last_valid_idx);
         extend_critical_region(customized_curvature, critical_region_subdivisions);

         if (last_valid_idx < DC_MAX_COORDINATES_REGION_SIZE - 1U)
         {
            for (uint8_t i = last_valid_idx + 1U; i < DC_MAX_COORDINATES_REGION_SIZE; ++i)
            {
               m_coordinates[i] = m_coordinates[last_valid_idx];
            }
         }
      }

      const std::array<geometry::Point2D_T, DC_MAX_COORDINATES_REGION_SIZE> &CriticalRegion::get_coordinates() const
      {
         return m_coordinates;
      }

      geometry::Point2D_T &CriticalRegion::operator[](const int index)
      {
         return m_coordinates[index];
      }

      const geometry::Point2D_T &CriticalRegion::operator[](const int index) const
      {
         return m_coordinates[index];
      }

      std::size_t CriticalRegion::get_size() const
      {
         return m_coordinates.size();
      }

      void CriticalRegion::curvilinear_transformation(float curvature,
                                                      const float min_abs_curvature_threshold,
                                                      const float max_abs_curvature_threshold,
                                                      const uint8_t last_valid_idx)
      {
         if (std::fabs(curvature) >= min_abs_curvature_threshold)
         {
            curvature         = std::min(std::max(curvature, -max_abs_curvature_threshold), max_abs_curvature_threshold);
            const float R     = 1.0F / curvature;
            const float abs_R = std::fabs(R);
            for (uint8_t i{0U}; i <= last_valid_idx; i++)
            {
               const float lon = m_coordinates[i].x;
               const float lat = m_coordinates[i].y;
               float radius    = R - lat;
               float theta     = lon / R;
               if (std::fabs(theta) > sg::TWO_PI)
               {
                  theta = sg::sign(theta) * sg::TWO_PI;
               }
               if ((std::fabs(lat) > abs_R) && ((lat * R) > 0.0F))
               {
                  radius = 0.0F;
               }
               const float curved_lat = R - radius * std::cos(theta);
               float curved_lon       = radius * std::sin(theta);
               if (lon * curved_lon < 0.0F)
               {
                  curved_lon = 0.0F;
               }
               m_coordinates[i].x = curved_lon;
               m_coordinates[i].y = curved_lat;
            }
         }
         else
         {
            // MISRA
         }
      }

      void CriticalRegion::interpolate_polygon(uint8_t &last_valid_idx,
                                               const uint8_t subdivisions,
                                               const geometry::Point2D_T (&polygon)[DC_NUM_POLYGON_VERTICES_BEFORE_INTERPOLATION])
      {
         uint8_t interp_idx = 0U;
         for (uint8_t curr_idx{0U}; curr_idx < DC_NUM_POLYGON_VERTICES_BEFORE_INTERPOLATION; curr_idx += 2U)
         {
            const uint8_t next_idx = (curr_idx + 1U) % DC_NUM_POLYGON_VERTICES_BEFORE_INTERPOLATION;
            interpolate_edge(interp_idx, polygon, curr_idx, next_idx, subdivisions);
            if ((interp_idx + subdivisions) < DC_MAX_COORDINATES_REGION_SIZE)
            {
               /*Add a corner point, no need to interpolate it*/
               m_coordinates[interp_idx + subdivisions].x = polygon[curr_idx + 1U].x;
               m_coordinates[interp_idx + subdivisions].y = polygon[curr_idx + 1U].y;
            }
            interp_idx += (subdivisions + 1U);
         }
         last_valid_idx = std::min(interp_idx - 1U, DC_MAX_COORDINATES_REGION_SIZE - 1U);
      }

      void CriticalRegion::interpolate_edge(uint8_t idx,
                                            const geometry::Point2D_T (&polygon)[DC_NUM_POLYGON_VERTICES_BEFORE_INTERPOLATION],
                                            const uint8_t idx_start,
                                            const uint8_t idx_end,
                                            const uint8_t subdivisions)
      {
         const bool valid_args = (subdivisions > 0U) && (idx_start < DC_NUM_POLYGON_VERTICES_BEFORE_INTERPOLATION)
                                 && (idx_end < DC_NUM_POLYGON_VERTICES_BEFORE_INTERPOLATION);

         if (valid_args)
         {
            const float step_x = (polygon[idx_end].x - polygon[idx_start].x) / static_cast<float>(subdivisions);
            const float step_y = (polygon[idx_end].y - polygon[idx_start].y) / static_cast<float>(subdivisions);
            float point_x_val  = polygon[idx_start].x;
            float point_y_val  = polygon[idx_start].y;
            for (uint8_t i{0U}; i < subdivisions; i++)
            {
               if (idx >= DC_MAX_COORDINATES_REGION_SIZE)
               {
                  break;
               }
               m_coordinates[idx].x = point_x_val;
               m_coordinates[idx].y = point_y_val;
               idx++;
               point_x_val += step_x;
               point_y_val += step_y;
            }
            (void) step_x;      // MISRA
            (void) step_y;      // MISRA
            (void) idx;         // MISRA
            (void) point_x_val; // MISRA
            (void) point_y_val; // MISRA
         }
      }

      void CriticalRegion::extend_critical_region(const float curvature, const uint8_t subdivisions)
      {
         const uint8_t second_corner = 2U * subdivisions - (subdivisions - 1U); // Index of the upper right corner of critical
                                                                                // region
         const uint8_t third_corner = 3U * subdivisions - (subdivisions - 1U); // Index of the lower right corner of critical region

         const bool valid_args = (subdivisions >= 3U) && (subdivisions < DC_MAX_COORDINATES_REGION_SIZE)
                                 && (second_corner < DC_MAX_COORDINATES_REGION_SIZE)
                                 && (third_corner < DC_MAX_COORDINATES_REGION_SIZE);

         if (valid_args)
         {
            if ((m_coordinates[subdivisions].y > m_coordinates[0U].y)
                || (m_coordinates[second_corner].y < m_coordinates[third_corner].y))
            {
               float max_lon = m_coordinates[0U].x;
               float min_lat = m_coordinates[0U].y;
               float max_lat = m_coordinates[0U].y;
               for (uint8_t idx{1U}; idx < DC_MAX_COORDINATES_REGION_SIZE; ++idx)
               {
                  max_lon = (m_coordinates[idx].x > max_lon) ? m_coordinates[idx].x : max_lon;
                  max_lat = (m_coordinates[idx].y > max_lat) ? m_coordinates[idx].y : max_lat;
                  min_lat = (m_coordinates[idx].y < min_lat) ? m_coordinates[idx].y : min_lat;
               }

               const bool f_turn_right = (curvature < 0.0F);
               const bool f_turn_left  = (curvature > 0.0F);
               const bool f_sharp_turn = (m_coordinates[second_corner].y < m_coordinates[subdivisions].y);
               (void) f_sharp_turn;

               if (f_turn_right)
               {
                  const uint8_t p3_idx = second_corner;
                  const uint8_t p4_idx = third_corner;
                  if (f_sharp_turn)
                  {
                     m_coordinates[p3_idx + 1U].y = min_lat;
                     for (uint8_t idx = p3_idx + 1U; idx < p4_idx; ++idx)
                     {
                        m_coordinates[idx].x = max_lon;
                     }
                     m_coordinates[p4_idx - 1U].y = max_lat;
                  }
                  else
                  {
                     for (uint8_t idx = p3_idx + 1U; idx < p4_idx; ++idx)
                     {
                        m_coordinates[idx].y = max_lat;
                     }
                     m_coordinates[p3_idx + 1U].x = max_lon;
                  }
               }
               else if (f_turn_left)
               {
                  const uint8_t p1_idx = 0U;
                  const uint8_t p2_idx = subdivisions;
                  if (f_sharp_turn)
                  {
                     m_coordinates[p1_idx + 1U].y = min_lat;
                     for (uint8_t idx = p1_idx + 1U; idx < p2_idx; ++idx)
                     {
                        m_coordinates[idx].x = max_lon;
                     }
                     m_coordinates[p2_idx - 1U].y = max_lat;
                  }
                  else
                  {
                     for (uint8_t idx = p1_idx + 1U; idx < p2_idx; ++idx)
                     {
                        m_coordinates[idx].y = min_lat;
                     }
                     m_coordinates[p2_idx - 1U].x = max_lon;
                  }
               }
               else
               {
                  // MISRA
               }
               (void) max_lon;     // MISRA
               (void) max_lat;     // MISRA
               (void) min_lat;     // MISRA
               (void) f_turn_left; // MISRA
            }
         }

         (void) second_corner; // MISRA
         (void) third_corner;  // MISRA
      }
   }
}
