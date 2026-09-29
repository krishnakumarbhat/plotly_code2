
#include "f360_set_variant.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   void Set_Tracker_Variant(F360_Variant_T& variant)
   {
      variant.num_tracks = NUMBER_OF_OBJECT_TRACKS;
      variant.num_reduced_tracks = NUMBER_OF_REDUCED_OBJECT_TRACKS;
      variant.num_posn_clusters = MAX_TRACKER_POSN_CLUSTERS;
      variant.num_dets_in_track = MAX_DETS_IN_OBJ_TRK;
      variant.num_hist_dets_in_cluster = MAX_HIST_DETS_IN_CLUSTER;
      variant.num_clusters = NUMBER_OF_CLUSTERS;
      variant.num_hist_dets = MAX_NUMBER_OF_HISTORIC_DETECTIONS;
      variant.type = VARIANT_TYPE;
   }
}
