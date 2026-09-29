#ifndef OCG_OCCUPANCY_GRID_TYPES_H
#define OCG_OCCUPANCY_GRID_TYPES_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4820)
#endif

#include <cstddef>
#include "rspp_detection_list.h"
#include "rspp_radar_sensor.h"
#include "ocg_underdrivability_type.h"
#include "ocg_internals_type.h"
#include "ocg_occupancy_grid_definition.h"
#include "rspp_host.h"

namespace ocg
{
   struct OCG_Inputs_T
   {
      rspp_variant_A::RSPP_Detection_List_T detection_list;
      rspp_variant_A::F360_Radar_Sensor_T sensors[rspp_variant_A::MAX_NUMBER_OF_SENSORS];
      RSPP_Host_T host;
   };

   struct OCG_Internals_T
   {
      OCG_Cell_Internal_T cells[NUM_CELLS_X][NUM_CELLS_Y];
      OCG_Internal_Props_T props;
      double timestamp;
      uint32_t iteration_index; // index of main iteration of the OCG algorithm
   };

   struct OCG_Outputs_T
   {
      double timestamp;
      OCG_Definition_T grid_definition;
      OCG_Underdrivability_T underdrivability;
      uint32_t iteration_index; // index of main iteration of the OCG algorithm
      bool f_valid;
   };

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
