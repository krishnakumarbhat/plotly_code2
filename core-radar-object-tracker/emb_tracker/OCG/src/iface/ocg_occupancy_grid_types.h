#ifndef OCG_OCCUPANCY_GRID_TYPES_H
#define OCG_OCCUPANCY_GRID_TYPES_H

#ifdef _MSC_VER
   #pragma warning(push)
   #pragma warning(disable:4820)
#endif

#include "ocg_internals_type.h"
#include "ocg_occupancy_grid_definition.h"
#include "ocg_underdrivability_type.h"
#include <cstdint>

namespace ocg
{
const int16_t OCG_NUM_DETS = 768;

typedef struct OCG_Sensor_Tag
{
   uint64_t timestamp_us;
   float v_wrapping;
   float vcs_position_x;
   float vcs_position_y;
   float vcs_position_z;
   float vacs_boresight_az_estimated;
   float vacs_boresight_el_estimated;
   float polarity;
} OCG_Sensor_T;

typedef struct OCG_Detection_Tag
{
   float range;
   float range_rate;
   float azimuth;
   float elevation;
   float rcs;
   int8_t confid_azimuth;
   int8_t confid_elevation;
   bool f_super_res;
} OCG_Detection_T;

typedef struct OCG_Detection_Properties_Tag
{
   float vcs_position_x;
   float vcs_position_y;
   float vcs_position_z;
   float range_rate_compensated;
} OCG_Detection_Properties_T;

typedef struct OCG_Detection_List_Tag
{
   OCG_Detection_T detections[OCG_NUM_DETS];
   int16_t number_of_valid_detections;
} OCG_Detection_List_T;

typedef struct OCG_Host_Tag
{
   float speed;
   float curvature_rear;
   float dist_rear_axle_to_vcs_m;
   float vcs_speed;
   float yaw_rate_rad;
   float vcs_sideslip;
} OCG_Host_T;

typedef struct OCG_Inputs_Tag
{
   OCG_Detection_List_Tag detection_list;
   OCG_Sensor_T sensor;
   OCG_Host_T host;
} OCG_Inputs_T;

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

} // namespace ocg

#ifdef _MSC_VER
   #pragma warning(pop)
#endif

#endif
