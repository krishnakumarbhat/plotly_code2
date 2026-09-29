/*=============================================================================================*\
* FILE: sg_measurement_association_utils.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of measurement association helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_MEASUREMENT_ASSOCIATION_UTILS_H
#define SG_MEASUREMENT_ASSOCIATION_UTILS_H

#include "geometry/geo_rectangle.h"
#include "geometry/geo_segment.h"
#include "sg_calibrations.h"
#include "sg_contour_storage.h"
#include "sg_detection_storage.h"

namespace sg
{
   // pair containing detection iterator and corresponding segment_ID
   using boundary_dets_properties_T = std::array<std::pair<Detection_T *, uint32_t>, SG_MAX_NUM_INTERNAL_DETS>;
   using det_x_pos_interval =
      std::pair<DetectionCache::collection_data_type::const_iterator, DetectionCache::collection_data_type::const_iterator>;
   /**
    * @brief    Types of association impact.
    *
    **/
   enum class VERTEX_ASSOCIATION_IMPACT_TYPE : std::uint8_t
   {
      INVALID      = 0,
      PROPORTIONAL = 1,
      BINARY       = 2
   };

   /**
    * @brief             Creates a bounding box for a specified segment
    *
    * @param[in, out]    assoc_width_extension
    * @param[in]         contour
    * @param[in]         segment_first_vtx
    * @param[in]         calibrations
    * @param[in]         segment_vector
    * @param[in]         normal_vector
    *
    * @return            segment bounding box
    **/
   sg::geometry::Rectangle_T create_segment_bounding_box(float &assoc_width_extension,
                                                         const Contour_T &contour,
                                                         const sg::Contour_T::VertexList::iterator &segment_first_vtx,
                                                         const Measurement_Association_Calibrations_T &calibrations,
                                                         const sg::geometry::Point2D_T segment_vector,
                                                         const sg::geometry::Point2D_T normal_vector);

   /**
    * @brief             Associates detections to particular segment of contour.
    *
    * @param[in, out]    association_impact
    * @param[in]         measurement_association_calibrations
    * @param[in]         r_position_covariance
    * @param[in]         x_pos_interval
    * @param[in]         contour
    * @param[in]         segment_bounding_box
    * @param[in]         segment
    * @param[in]         normal_vector
    * @param[in]         segment_id
    * @param[in]         vertex_age
    * @param[in]         assoc_width_extension
    *
    * @return            number of associated detections
    **/
   void associate_detections_to_segment(std::pair<float, float> &association_impact,
                                        const Measurement_Association_Calibrations_T &measurement_association_calibrations,
                                        const R_Position_Covariance_T &r_position_covariance,
                                        const det_x_pos_interval &x_pos_interval,
                                        const Contour_T &contour,
                                        const geometry::Rectangle_T &segment_bounding_box,
                                        const geometry::Segment2D_T &segment,
                                        const sg::geometry::Point2D_T normal_vector,
                                        const uint32_t &segment_id,
                                        const uint16_t &vertex_age,
                                        const float assoc_width_extension);

   /**
    * @brief             Updates boundary detection properties after being assigned to a segment
    *
    * @param[in, out]    detection
    * @param[in]         calibrations
    * @param[in]         r_position_covariance
    * @param[in]         det_to_segment_distance
    * @param[in]         assoc_width_extension
    *
    * @return            None
    **/
   void set_detection_covariances(Detection_T &detection,
                                  const Measurement_Association_Calibrations_T &calibrations,
                                  const R_Position_Covariance_T &r_position_covariance,
                                  const float det_to_segment_distance,
                                  const float assoc_width_extension);

   /**
    * @brief        Calculates association impact for segment vertices from particular detection associated to segment.
    *
    * @param[in]    det_position
    * @param[in]    segment
    * @param[in]    impact_type
    *
    * @return       pair: first -> association impact for first segment vertx,
    *                     second -> association impact for second segment vertx
    *
    **/
   std::pair<float, float> calculate_association_impact(const sg::geometry::Point2D_T &det_position,
                                                        const sg::geometry::Segment2D_T &segment,
                                                        const VERTEX_ASSOCIATION_IMPACT_TYPE impact_type);

   /**
    * @brief        Finds the most frequent cluster id of all detections associated to a particular contour.
    *
    * @param[in]    detections
    * @param[in]    contour_id
    *
    * @return       pair: first -> number of ocurrences, second -> value of cluster_id
    *
    **/
   std::pair<uint16_t, uint16_t> get_most_frequent_cluster_id(const DetectionStorage &detections, const uint32_t contour_id);

   /**
    * @brief        Calculates association width.
    *
    * @param[in]    assoc_width_min
    * @param[in]    width_min
    * @param[in]    width_max
    * @param[in]    lower bound for dynamic association gates
    * @param[in]    upper bound for dynamic association gates
    * @param[in]    f_dynamic_gates
    *
    * @return       value of association width
    *
    **/
   inline float calculate_association_width(const float distance,
                                            const float assoc_width_min,
                                            const float assoc_width_max,
                                            const float lower_distance,
                                            const float upper_distance,
                                            const bool f_dynamic_gates)
   {
      float result{};

      if (!f_dynamic_gates)
      {
         // when dynamic gates are disabled, choose minimum of the two gates
         result = std::min(assoc_width_min, assoc_width_max);
      }
      else if ((distance <= lower_distance))
      {
         result = assoc_width_min;
      }
      else if (distance >= upper_distance)
      {
         result = assoc_width_max;
      }
      else
      {
         // change calculated as linear function
         const float slope    = (assoc_width_max - assoc_width_min) / (upper_distance - lower_distance);
         const float constant = assoc_width_min - slope * lower_distance;

         result = slope * distance + constant;
      }

      return result;
   }

   /**
    * @brief        Calculates association length margin.
    *
    * @param[in]    length_margin_min
    * @param[in]    length_margin_max
    *
    * @return       value of association length margin
    *
    **/
   inline float calculate_association_length_margin(const float length_margin_min, const float length_margin_max)
   {
      return std::min(length_margin_min, length_margin_max);
   }

   /**
    * @brief          compute_normal_vectors of contour segments pointing towards host.
    *
    * @param[in,out]  normal_vectors
    * @param[in]      contour
    * @param[in]      host_position
    **/
   void compute_normal_vectors(std::array<geometry::Point2D_T, SG_MAX_NUM_VERTICES_PER_CONTOUR> &normal_vectors,
                               const Contour_T &contour,
                               const geometry::Point2D_T &host_position);

}

#endif
