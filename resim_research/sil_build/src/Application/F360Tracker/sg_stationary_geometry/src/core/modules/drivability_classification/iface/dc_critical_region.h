/*===================================================================================*\
* FILE: dc_critical_region.h
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

#ifndef DC_CRITICAL_REGION_H
#define DC_CRITICAL_REGION_H

#include "sg_calibrations.h"

namespace sg
{
   namespace dc
   {
      class CriticalRegion
      {
        public:
         CriticalRegion() = default;
         /**
          * @brief         Constructor that initializes critical region with coordinates.
          *
          * @param[in]     coordinates - coordinates of critical region
          **/
         CriticalRegion(const std::array<geometry::Point2D_T, DC_MAX_COORDINATES_REGION_SIZE> &coordinates);
         /**
          * @brief         Creates critical region in front of the vehicle.
          *
          *
          * @param[in]     cfg - algorithm calibration
          **/
         void create(const float curvature, const sg::Drivability_Classification_Calibrations_T &cfg);

         /**
          * @brief         Returns reference to critical region's coordinates array.
          *
          **/
         const std::array<geometry::Point2D_T, DC_MAX_COORDINATES_REGION_SIZE> &get_coordinates() const;

         /**
          * @brief         Returns reference to critical region's coordinate.
          *
          * @param[in]     index - coordinate's index
          **/
         geometry::Point2D_T &operator[](const int index);

         /**
          * @brief         Returns const reference to critical region's coordinate.
          *
          * @param[in]     index - coordinate's index
          **/
         const geometry::Point2D_T &operator[](const int index) const;

         /**
          * @brief         Returns size of critical region's coordinates array.
          *
          **/
         std::size_t get_size() const;

         /**
          * @brief         Applies curvilinear transformation to the polygon.
          *
          * @param[in]     curvature - curvature of the vehicle's path
          * @param[in]     min_abs_curvature_threshold - minimum possible absolute value of the curvature
          * @param[in]     max_abs_curvature_threshold - maximum possible absolute value of the curvature
          * @param[in]     last_valid_idx - position of the last valid vertex in polygon (last vertex to transform)
          **/
         void curvilinear_transformation(float curvature,
                                         const float min_abs_curvature_threshold,
                                         const float max_abs_curvature_threshold,
                                         const uint8_t last_valid_idx);

         /**
          * @brief         Interpolates all edges of a polygon.
          *
          * @param[out]    last_valid_idx - index of a last valid point in interpolated_polygon
          * @param[in]     subdivisions - number of divisions of each each (segment) of a polygon
          * @param[in]     (&polygon)[] - polygon whose edges are interpolated
          **/
         void interpolate_polygon(uint8_t &last_valid_idx,
                                  const uint8_t subdivisions,
                                  const geometry::Point2D_T (&polygon)[DC_NUM_POLYGON_VERTICES_BEFORE_INTERPOLATION]);

         /*=============================================================================================*\
          * @brief         Interpolates edge of a polygon. Creates num_points_to_add points including polygon[from] but excluding
          *polygon[to]. Example: interpolate 5 points from 1.0 to 10.0 -> 1.0, 2.8, 4.6, 6.4, 8.2.
          *
          * @param[in]     idx - index of the next interpolated point in the interplolated_polygon
          * @param[in]     (&polygon)[] - polygon whose edge is interpolated
          * @param[in]     idx_start - index of polygon's vertex to interpolate from
          * @param[in]     idx_end - index of polygon's vertex to interpolate to (it's excluded from the output and
          *num_points_to_add)
          * @param[in]     subdivisions - number of subdivisions to interpolate and to be added
          **/
         void interpolate_edge(uint8_t idx,
                               const geometry::Point2D_T (&polygon)[DC_NUM_POLYGON_VERTICES_BEFORE_INTERPOLATION],
                               const uint8_t idx_start,
                               const uint8_t idx_end,
                               const uint8_t subdivisions);

         /**
          * @brief         Augments critical region if curvature is not close to zero.
          *
          * @param[in]     curvature - curvature of the vehicle's path
          * @param[in]     subdivisions - number of subdivisions to interpolate and to be added
          **/
         void extend_critical_region(const float curvature, const uint8_t subdivisions);

        private:
         std::array<geometry::Point2D_T, DC_MAX_COORDINATES_REGION_SIZE> m_coordinates;
      };
   }
}

#endif
