#ifndef SG_MERGE_CONTOURS_H
#define SG_MERGE_CONTOURS_H

#include "geometry/geo_distance.h"
#include "geometry/geo_length.h"
#include "sg_calibrations.h"
#include "sg_common.h"
#include "sg_contour_storage.h"

namespace sg
{
   using Contour_it = EmbeddedList<Contour_T, SG_MAX_NUM_CONTOURS>::iterator;

   static constexpr uint16_t NUM_CLUSTERS_IN_ONE_ITERATION = 10U;

   class CommonClusterContoursRow_T
   {
     public:
      std::array<Contour_it, SG_MAX_NUM_CONTOURS> contour_its = {};

      void push_back(const Contour_it &contour_to_push_it)
      {
         if (m_number_of_contours < SG_MAX_NUM_CONTOURS)
         {
            contour_its[m_number_of_contours] = contour_to_push_it;
            m_number_of_contours++;
         }
      }

      void remove(const uint16_t contour_its_idx)
      {
         if (contour_its_idx < SG_MAX_NUM_CONTOURS)
         {
            contour_its[contour_its_idx] = nullptr;
         }
      }

      uint16_t number_of_contours() const
      {
         return m_number_of_contours;
      }

      void clear_number_of_contours()
      {
         m_number_of_contours = 0U;
      }

     private:
      uint16_t m_number_of_contours = 0U;
   };

   struct contour_merge_setup
   {
      bool f_merge_beginnings           = false;
      bool f_merge_ends                 = false;
      bool f_begin_flip_partner_contour = false;
      bool f_end_flip_partner_contour   = false;
   };

   /**
    * @brief            Wrapper function performing merge of contours
    *
    * @param[in, out]   contours
    * @param[in]        calibrations
    * @param[in]        curvature_rear
    *
    * @return           N/A
    **/
   void merge_contours(ContourStorage &contours,
                       const Contour_Postprocessing_Calibrations_T::Merge_Contours_T &calibrations,
                       const float curvature_rear);

   /**
    * @brief            Function copies contour iterators - of contours which belong to a cluster -
    *                   into an array for faster access and returns number of them
    *
    * @param[in, out]   contour iterators array
    * @param[in]        contours
    *
    * @return           number of contours which belong to a cluster
    **/
   uint16_t select_contours_belonging_to_cluster(std::array<Contour_it, SG_MAX_NUM_CONTOURS> &contour_its,
                                                 const ContourStorage &contours);

   /**
    * @brief            Clears contents of common_cluster_contours
    *
    * @param[in, out]   common_cluster_contours
    *
    * @return           N/A
    **/
   void clear_common_cluster_contours(std::array<CommonClusterContoursRow_T, NUM_CLUSTERS_IN_ONE_ITERATION> &common_cluster_contours);

   /**
    * @brief            Wrapper function performing merge of contours within single cluster
    *
    * @param[in, out]   clustered contour iterators
    * @param[in, out]   contours
    * @param[in]        calibrations
    * @param[in]        curvature_rear
    *
    * @return           f_pair_found
    **/
   bool find_and_merge_pair(CommonClusterContoursRow_T &clustered_contour_its,
                            ContourStorage &contours,
                            const Contour_Postprocessing_Calibrations_T::Merge_Contours_T &calibrations,
                            const float curvature_rear);

   /**
    * @brief            Function finding partner contour that is located closest to a given current conour
    *
    * @param[in, out]   merge_flags - structure containing flags with merge settings
    * @param[in]        clustered_contours
    * @param[in]        current_contour
    * @param[in]        current_idx
    * @param[in]        curvature_rear
    * @param[in]        calibrations
    *
    * @return           partner_contour_idx - array index of the partner contour within the clustered_contours
    **/
   uint16_t find_closest_partner_contour(contour_merge_setup &merge_flags,
                                         const CommonClusterContoursRow_T &clustered_contours,
                                         const Contour_it current_contour,
                                         const uint16_t current_idx,
                                         const float curvature_rear,
                                         const Contour_Postprocessing_Calibrations_T::Merge_Contours_T &calibrations);

   /**
    * @brief            A function for performing merge of two contours
    *
    * @param[in, out]   destination_contour
    * @param[in, out]   source_contour
    *
    * @return           N/A
    **/
   void merge_two_contours(const Contour_it &destination_contour_it, const Contour_it &source_contour_it);

   /**
    * @brief            An auxiliary function to determine the destination contour (the one that will be kept after merge)
    *                   and source cluster (the one from which detections will be added to the destination cluter)
    *
    * @param[in, out]   current_contour
    * @param[in, out]   partner_contour
    * @param[in, out]   destination_contour
    * @param[in, out]   source_contour
    *
    * @return           f_contours_swapped
    **/
   bool determine_destination_contour(const Contour_it &current_contour_it,
                                      const Contour_it &partner_contour_it,
                                      Contour_it &destination_contour,
                                      Contour_it &source_contour);

   /**
    * @brief            A function reversing order of vertexes inside contour, swapping beginning and end
    *
    * @param[in, out]   contour
    *
    * @return           f_contours_swapped
    **/
   void flip_contour(const Contour_T &contour);
}
#endif
