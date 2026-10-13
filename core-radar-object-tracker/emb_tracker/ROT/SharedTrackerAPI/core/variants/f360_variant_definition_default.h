#ifndef F360_VARIANT_DEFINITION_VALUES_A_H
#define F360_VARIANT_DEFINITION_VALUES_A_H

#include "f360_reuse.h"
#include "f360_variant_type.h"

namespace f360_variant_default
{
static constexpr F360_Tracker_Variant_T VARIANT_TYPE      = F360_VARIANT_TYPE_A;
static constexpr uint32_t NUMBER_OF_OBJECT_TRACKS         = 1000U;
static constexpr uint32_t NUMBER_OF_REDUCED_OBJECT_TRACKS = 500U;

static constexpr uint16_t NUMBER_OF_CLUSTERS                = 2000U;
static constexpr uint16_t MAX_NUMBER_OF_HISTORIC_DETECTIONS = 2010U;
static constexpr uint32_t MAX_TRACKER_POSN_CLUSTERS         = 150U;

static constexpr uint8_t MAX_NUMBER_OF_SRR_SENSORS = 6U;
static constexpr uint8_t MAX_NUMBER_OF_MRR_SENSORS = 4U;
static constexpr uint16_t NUMBER_OF_SRR_DETECTIONS = 768U;
static constexpr uint16_t NUMBER_OF_MRR_DETECTIONS = 1200U;

static constexpr uint8_t MAX_DETS_IN_OBJ_TRK      = 80U;
static constexpr uint8_t MAX_HIST_DETS_IN_CLUSTER = 80U;
} // namespace f360_variant_default

#endif
