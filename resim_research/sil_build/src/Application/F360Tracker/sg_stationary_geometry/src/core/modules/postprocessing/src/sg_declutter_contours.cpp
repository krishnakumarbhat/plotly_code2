#include "sg_declutter_contours.h"

#include "sg_declutter_contours_helpers.h"

namespace sg
{
   void declutter_contours(ContourStorage &contours,
                           const Contour_Postprocessing_Calibrations_T::Declutter_Contours_T &calibrations,
                           const float azimuth_epsilon)
   {
      std::array<sg::ContourStorage::ContourList::iterator, SG_MAX_NUM_CONTOURS> contours_for_decluttering{};
      std::array<uint16_t, MAX_NUM_CLUSTERS_FOR_DECLUTTER> num_contours_in_clusters{0U};
      std::array<uint16_t, SG_MAX_NUM_CONTOURS> num_of_occluders{0U};

      select_contours_for_decluttering(contours_for_decluttering, num_contours_in_clusters, contours);

      mark_occluded_contours(num_of_occluders, contours_for_decluttering, num_contours_in_clusters, calibrations, azimuth_epsilon);

      remove_occluded_contours(contours, contours_for_decluttering, num_of_occluders, calibrations.occluders_num_threshold);
   }
}
