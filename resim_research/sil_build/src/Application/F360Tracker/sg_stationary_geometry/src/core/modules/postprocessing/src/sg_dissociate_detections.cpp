#include "sg_dissociate_detections.h"

#include <bitset>

#include "sg_constants.h"

namespace sg
{
   static void collect_all_segment_ids(const sg::ContourStorage &contours, uint32_t (&existing_segment_ids)[SG_MAX_NUM_VERTICES])
   {
      // Assumption that the total number of vertices in contours never exceed the SG_MAX_NUM_VERTICES
      uint32_t idx = 0U;
      for (const auto &contour : contours)
      {
         for (const auto &vertex : contour.vertices)
         {
            if (vertex.segment_id != INVALID_SEGMENT_ID)
            {
               existing_segment_ids[idx] = vertex.segment_id;
               ++idx;
            }
         }
      }
      (void) idx; // MISRA
   }

   static bool is_contour_id_valid(const sg::ContourStorage &contours, const sg::Detection_T &detection)
   {
      return std::any_of(contours.begin(), contours.end(),
                         [&detection](const auto &contour) { return detection.contour_id == contour.unique_id(); });
   }

   static void are_segment_ids_valid(const uint32_t (&existing_segment_ids)[SG_MAX_NUM_VERTICES],
                                     const sg::Detection_T &detection,
                                     std::bitset<2U> &valid_segment_ids)
   {
      uint8_t num_assigned = 0U;
      for (uint16_t idx = 0U; idx < SG_MAX_NUM_VERTICES; idx++)
      {
         if ((num_assigned == 2U) || (existing_segment_ids[idx] == INVALID_SEGMENT_ID))
         {
            break;
         }
         // assign mask
         if (detection.segment_id[0U] == existing_segment_ids[idx])
         {
            (void) valid_segment_ids.set(0U);
            ++num_assigned;
         }
         if (detection.segment_id[1U] == existing_segment_ids[idx])
         {
            (void) valid_segment_ids.set(1U);
            ++num_assigned;
         }
      }
      (void) num_assigned; // MISRA
   }

   /**
    * @brief Resets segment association for detection.
    *
    * @param [in]      valid_segment_ids
    * @param [in, out] detection: current detection
    **/
   static inline void reset_segment_association_for_detection(const std::bitset<2U> &valid_segment_ids, sg::Detection_T &detection)
   {
      if (!valid_segment_ids[0U])
      {
         detection.segment_id[0U] = INVALID_SEGMENT_ID;
      }
      if (!valid_segment_ids[1U])
      {
         detection.segment_id[1U] = INVALID_SEGMENT_ID;
      }
      if ((!valid_segment_ids[0U]) && (!valid_segment_ids[1U]))
      {
         detection.f_used_in_measurement_update = false;
         detection.contour_id                   = INVALID_CONTOUR_ID;
      }
      else if ((!valid_segment_ids[0U]) && valid_segment_ids[1U])
      {
         detection.segment_id[0U] = detection.segment_id[1U];
         detection.segment_id[1U] = INVALID_SEGMENT_ID;
      }
      else
      {
         // do nothing
      }
   }

   /**
    * @brief Resets contour association for detection.
    *
    * @param [in, out] detection: current detection
    **/
   static inline void reset_contour_association_for_detection(sg::Detection_T &detection)
   {
      detection.segment_id[0U]               = INVALID_SEGMENT_ID;
      detection.segment_id[1U]               = INVALID_SEGMENT_ID;
      detection.contour_id                   = INVALID_CONTOUR_ID;
      detection.f_used_in_measurement_update = false;
   }

   void dissociate_detections(sg::DetectionStorage &detections, const sg::ContourStorage &contours)
   {
      uint32_t existing_segment_ids[SG_MAX_NUM_VERTICES]{}; // default initialized to zero, which means INVALID_SEGMENT_ID
      collect_all_segment_ids(contours, existing_segment_ids);

      for (Detection_T &detection : detections)
      {
         if (is_contour_id_valid(contours, detection))
         {
            std::bitset<2U> valid_segment_ids{}; // default initialized to false
            are_segment_ids_valid(existing_segment_ids, detection, valid_segment_ids);
            reset_segment_association_for_detection(valid_segment_ids, detection);
         }
         else
         {
            reset_contour_association_for_detection(detection);
         }
      }
   }
}
