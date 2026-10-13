#ifndef F360_VARIANT_VALUE_LOOKUP_H
#define F360_VARIANT_VALUE_LOOKUP_H

#include "variants/f360_variant_definition_al.h"
#include "variants/f360_variant_definition_b.h"
#include "variants/f360_variant_definition_c.h"
#include "variants/f360_variant_definition_d.h"
#include "variants/f360_variant_definition_default.h"
#include "variants/f360_variant_definition_e.h"
#include "variants/f360_variant_definition_f.h"
#include "variants/f360_variant_definition_g.h"
#include "variants/f360_variant_definition_h.h"
#include "variants/f360_variant_definition_i.h"
#include "variants/f360_variant_definition_j.h"
#include "variants/f360_variant_definition_k.h"
#include "variants/f360_variant_definition_l.h"
#include "variants/f360_variant_definition_m.h"
#include "variants/f360_variant_definition_n.h"
#include "variants/f360_variant_definition_o.h"
#include "variants/f360_variant_definition_p.h"
#include "variants/f360_variant_definition_q.h"
#include "variants/f360_variant_definition_r.h"
#include "variants/f360_variant_definition_s.h"

namespace f360_variant_value_lookup
{
typedef struct F360_Variant_Values_Tag
{
   F360_Tracker_Variant_T variant_type;
   uint32_t num_tracks;
   uint32_t num_reduced_tracks;
   uint32_t num_posn_clusters;
   uint8_t num_dets_in_track;
   uint8_t num_hist_dets_in_cluster;
   uint16_t num_clusters;
   uint16_t num_hist_dets;
} F360_Variant_Values_T;

static constexpr F360_Variant_Values_T TRACKER_VARIANT_VALUES[] = {
   /* Default variant */
   {f360_variant_default::VARIANT_TYPE, f360_variant_default::NUMBER_OF_OBJECT_TRACKS,
    f360_variant_default::NUMBER_OF_REDUCED_OBJECT_TRACKS, f360_variant_default::MAX_TRACKER_POSN_CLUSTERS,
    f360_variant_default::MAX_DETS_IN_OBJ_TRK, f360_variant_default::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_default::NUMBER_OF_CLUSTERS, f360_variant_default::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant B */
   {f360_variant_B::VARIANT_TYPE, f360_variant_B::NUMBER_OF_OBJECT_TRACKS, f360_variant_B::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_B::MAX_TRACKER_POSN_CLUSTERS, f360_variant_B::MAX_DETS_IN_OBJ_TRK, f360_variant_B::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_B::NUMBER_OF_CLUSTERS, f360_variant_B::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant C */
   {f360_variant_C::VARIANT_TYPE, f360_variant_C::NUMBER_OF_OBJECT_TRACKS, f360_variant_C::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_C::MAX_TRACKER_POSN_CLUSTERS, f360_variant_C::MAX_DETS_IN_OBJ_TRK, f360_variant_C::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_C::NUMBER_OF_CLUSTERS, f360_variant_C::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant D */
   {f360_variant_D::VARIANT_TYPE, f360_variant_D::NUMBER_OF_OBJECT_TRACKS, f360_variant_D::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_D::MAX_TRACKER_POSN_CLUSTERS, f360_variant_D::MAX_DETS_IN_OBJ_TRK, f360_variant_D::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_D::NUMBER_OF_CLUSTERS, f360_variant_D::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant E */
   {f360_variant_E::VARIANT_TYPE, f360_variant_E::NUMBER_OF_OBJECT_TRACKS, f360_variant_E::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_E::MAX_TRACKER_POSN_CLUSTERS, f360_variant_E::MAX_DETS_IN_OBJ_TRK, f360_variant_E::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_E::NUMBER_OF_CLUSTERS, f360_variant_E::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant F */
   {f360_variant_F::VARIANT_TYPE, f360_variant_F::NUMBER_OF_OBJECT_TRACKS, f360_variant_F::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_F::MAX_TRACKER_POSN_CLUSTERS, f360_variant_F::MAX_DETS_IN_OBJ_TRK, f360_variant_F::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_F::NUMBER_OF_CLUSTERS, f360_variant_F::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant G */
   {f360_variant_G::VARIANT_TYPE, f360_variant_G::NUMBER_OF_OBJECT_TRACKS, f360_variant_G::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_G::MAX_TRACKER_POSN_CLUSTERS, f360_variant_G::MAX_DETS_IN_OBJ_TRK, f360_variant_G::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_G::NUMBER_OF_CLUSTERS, f360_variant_G::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant H */
   {f360_variant_H::VARIANT_TYPE, f360_variant_H::NUMBER_OF_OBJECT_TRACKS, f360_variant_H::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_H::MAX_TRACKER_POSN_CLUSTERS, f360_variant_H::MAX_DETS_IN_OBJ_TRK, f360_variant_H::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_H::NUMBER_OF_CLUSTERS, f360_variant_H::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant I */
   {f360_variant_I::VARIANT_TYPE, f360_variant_I::NUMBER_OF_OBJECT_TRACKS, f360_variant_I::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_I::MAX_TRACKER_POSN_CLUSTERS, f360_variant_I::MAX_DETS_IN_OBJ_TRK, f360_variant_I::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_I::NUMBER_OF_CLUSTERS, f360_variant_I::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant J */
   {f360_variant_J::VARIANT_TYPE, f360_variant_J::NUMBER_OF_OBJECT_TRACKS, f360_variant_J::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_J::MAX_TRACKER_POSN_CLUSTERS, f360_variant_J::MAX_DETS_IN_OBJ_TRK, f360_variant_J::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_J::NUMBER_OF_CLUSTERS, f360_variant_J::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant K */
   {f360_variant_K::VARIANT_TYPE, f360_variant_K::NUMBER_OF_OBJECT_TRACKS, f360_variant_K::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_K::MAX_TRACKER_POSN_CLUSTERS, f360_variant_K::MAX_DETS_IN_OBJ_TRK, f360_variant_K::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_K::NUMBER_OF_CLUSTERS, f360_variant_K::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant L */
   {f360_variant_L::VARIANT_TYPE, f360_variant_L::NUMBER_OF_OBJECT_TRACKS, f360_variant_L::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_L::MAX_TRACKER_POSN_CLUSTERS, f360_variant_L::MAX_DETS_IN_OBJ_TRK, f360_variant_L::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_L::NUMBER_OF_CLUSTERS, f360_variant_L::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant M */
   {f360_variant_M::VARIANT_TYPE, f360_variant_M::NUMBER_OF_OBJECT_TRACKS, f360_variant_M::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_M::MAX_TRACKER_POSN_CLUSTERS, f360_variant_M::MAX_DETS_IN_OBJ_TRK, f360_variant_M::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_M::NUMBER_OF_CLUSTERS, f360_variant_M::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant N */
   {f360_variant_N::VARIANT_TYPE, f360_variant_N::NUMBER_OF_OBJECT_TRACKS, f360_variant_N::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_N::MAX_TRACKER_POSN_CLUSTERS, f360_variant_N::MAX_DETS_IN_OBJ_TRK, f360_variant_N::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_N::NUMBER_OF_CLUSTERS, f360_variant_N::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant O */
   {f360_variant_O::VARIANT_TYPE, f360_variant_O::NUMBER_OF_OBJECT_TRACKS, f360_variant_O::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_O::MAX_TRACKER_POSN_CLUSTERS, f360_variant_O::MAX_DETS_IN_OBJ_TRK, f360_variant_O::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_O::NUMBER_OF_CLUSTERS, f360_variant_O::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant P */
   {f360_variant_P::VARIANT_TYPE, f360_variant_P::NUMBER_OF_OBJECT_TRACKS, f360_variant_P::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_P::MAX_TRACKER_POSN_CLUSTERS, f360_variant_P::MAX_DETS_IN_OBJ_TRK, f360_variant_P::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_P::NUMBER_OF_CLUSTERS, f360_variant_P::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant Q */
   {f360_variant_Q::VARIANT_TYPE, f360_variant_Q::NUMBER_OF_OBJECT_TRACKS, f360_variant_Q::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_Q::MAX_TRACKER_POSN_CLUSTERS, f360_variant_Q::MAX_DETS_IN_OBJ_TRK, f360_variant_Q::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_Q::NUMBER_OF_CLUSTERS, f360_variant_Q::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant R */
   {f360_variant_R::VARIANT_TYPE, f360_variant_R::NUMBER_OF_OBJECT_TRACKS, f360_variant_R::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_R::MAX_TRACKER_POSN_CLUSTERS, f360_variant_R::MAX_DETS_IN_OBJ_TRK, f360_variant_R::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_R::NUMBER_OF_CLUSTERS, f360_variant_R::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant S */
   {f360_variant_S::VARIANT_TYPE, f360_variant_S::NUMBER_OF_OBJECT_TRACKS, f360_variant_S::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_S::MAX_TRACKER_POSN_CLUSTERS, f360_variant_S::MAX_DETS_IN_OBJ_TRK, f360_variant_S::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_S::NUMBER_OF_CLUSTERS, f360_variant_S::MAX_NUMBER_OF_HISTORIC_DETECTIONS},
   /* Variant AL */
   {f360_variant_AL::VARIANT_TYPE, f360_variant_AL::NUMBER_OF_OBJECT_TRACKS, f360_variant_AL::NUMBER_OF_REDUCED_OBJECT_TRACKS,
    f360_variant_AL::MAX_TRACKER_POSN_CLUSTERS, f360_variant_AL::MAX_DETS_IN_OBJ_TRK, f360_variant_AL::MAX_HIST_DETS_IN_CLUSTER,
    f360_variant_AL::NUMBER_OF_CLUSTERS, f360_variant_AL::MAX_NUMBER_OF_HISTORIC_DETECTIONS}};

static constexpr uint32_t NUM_TRACKER_VARIANT_VALUES = static_cast<uint32_t>(F360_VARIANT_TYPE_AL) + 1U;
static_assert(NUM_TRACKER_VARIANT_VALUES == (sizeof(TRACKER_VARIANT_VALUES) / sizeof(TRACKER_VARIANT_VALUES[0])),
              "TRACKER_VARIANT_VALUES must contain one entry for every F360 tracker variant");

inline bool is_valid_variant(const F360_Tracker_Variant_T variant_opt)
{
   return static_cast<uint32_t>(variant_opt) < NUM_TRACKER_VARIANT_VALUES;
}

inline bool is_default_build_variant(const F360_Tracker_Variant_T build_variant)
{
   return build_variant == F360_VARIANT_TYPE_A;
}

inline bool is_requested_variant_supported(const F360_Tracker_Variant_T build_variant,
                                           const F360_Tracker_Variant_T requested_variant)
{
   return is_default_build_variant(build_variant) || (build_variant == requested_variant);
}

inline char get_variant_letter(const F360_Tracker_Variant_T variant_opt)
{
   return is_valid_variant(variant_opt) ? static_cast<char>('A' + static_cast<int>(variant_opt)) : '?';
}

inline const F360_Variant_Values_T &get_variant_values(const F360_Tracker_Variant_T variant_opt)
{
   const uint32_t variant_index = static_cast<uint32_t>(variant_opt);
   return is_valid_variant(variant_opt) ? TRACKER_VARIANT_VALUES[variant_index] :
                                          TRACKER_VARIANT_VALUES[static_cast<uint32_t>(F360_VARIANT_TYPE_A)];
}
} // namespace f360_variant_value_lookup

#endif
