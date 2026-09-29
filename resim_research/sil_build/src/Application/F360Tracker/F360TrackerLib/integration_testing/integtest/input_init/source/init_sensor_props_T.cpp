
#include "f360_constants.h"
#include "f360_reuse.h"
#include "f360_position.h"
#include "f360_velocity.h"
#include "f360_radar_sensor_props.h"
#include "init_sensor_props_T.h"
namespace f360_variant_A
{
void init_sensor_props_T(F360_Radar_Sensor_Props_T (&sensor_props)[MAX_NUMBER_OF_SENSORS])
{

   sensor_props[0].time_since_measurement_s=0.140803993;
   sensor_props[0].f_object_track_next_to_sensor=false;
   sensor_props[0].next_to_sensor_object_track_id=0;
   sensor_props[0].next_to_sensor_object_track_min_long_pos=0.0;
   sensor_props[0].next_to_sensor_object_track_max_long_pos=0.0;
   sensor_props[0].next_to_sensor_object_track_min_lat_pos=0.0;
   sensor_props[0].next_to_sensor_object_track_max_lat_pos=0.0;
   sensor_props[0].interior_fov[F360_DET_LOOK_ID_0]=-0.785398185;
   sensor_props[0].interior_fov[F360_DET_LOOK_ID_1]=0.785398185;
   sensor_props[0].interior_fov[F360_DET_LOOK_ID_2]=-0.785398185;
   sensor_props[0].interior_fov[F360_DET_LOOK_ID_3]=0.785398185;
   sensor_props[0].left_fov_normal[F360_DET_LOOK_ID_0]=0.707106769;
   sensor_props[0].left_fov_normal[F360_DET_LOOK_ID_1]=0.707106769;
   sensor_props[0].right_fov_normal[F360_DET_LOOK_ID_0]=0.707106769;
   sensor_props[0].right_fov_normal[F360_DET_LOOK_ID_1]=-0.707106769;
   sensor_props[0].left_fov_normal[F360_DET_LOOK_ID_2]=0.707106769;
   sensor_props[0].left_fov_normal[F360_DET_LOOK_ID_3]=0.707106769;
   sensor_props[0].right_fov_normal[F360_DET_LOOK_ID_2]=0.707106769;
   sensor_props[0].right_fov_normal[F360_DET_LOOK_ID_3]=-0.707106769;

   sensor_props[1].time_since_measurement_s=980.403992;
   sensor_props[1].f_object_track_next_to_sensor=false;
   sensor_props[1].next_to_sensor_object_track_id=-1;
   sensor_props[1].next_to_sensor_object_track_min_long_pos=0.0;
   sensor_props[1].next_to_sensor_object_track_max_long_pos=0.0;
   sensor_props[1].next_to_sensor_object_track_min_lat_pos=0.0;
   sensor_props[1].next_to_sensor_object_track_max_lat_pos=0.0;
   sensor_props[1].interior_fov[F360_DET_LOOK_ID_0]=0.0;
   sensor_props[1].interior_fov[F360_DET_LOOK_ID_1]=0.0;
   sensor_props[1].interior_fov[F360_DET_LOOK_ID_2]=0.0;
   sensor_props[1].interior_fov[F360_DET_LOOK_ID_3]=0.0;
   sensor_props[1].left_fov_normal[F360_DET_LOOK_ID_0]=-0.0;
   sensor_props[1].left_fov_normal[F360_DET_LOOK_ID_1]=1.0;
   sensor_props[1].right_fov_normal[F360_DET_LOOK_ID_0]=0.0;
   sensor_props[1].right_fov_normal[F360_DET_LOOK_ID_1]=-1.0;
   sensor_props[1].left_fov_normal[F360_DET_LOOK_ID_2]=-0.0;
   sensor_props[1].left_fov_normal[F360_DET_LOOK_ID_3]=1.0;
   sensor_props[1].right_fov_normal[F360_DET_LOOK_ID_2]=0.0;
   sensor_props[1].right_fov_normal[F360_DET_LOOK_ID_3]=-1.0;

   sensor_props[2].time_since_measurement_s=980.403992;
   sensor_props[2].f_object_track_next_to_sensor=false;
   sensor_props[2].next_to_sensor_object_track_id=-1;
   sensor_props[2].next_to_sensor_object_track_min_long_pos=0.0;
   sensor_props[2].next_to_sensor_object_track_max_long_pos=0.0;
   sensor_props[2].next_to_sensor_object_track_min_lat_pos=0.0;
   sensor_props[2].next_to_sensor_object_track_max_lat_pos=0.0;
   sensor_props[2].interior_fov[F360_DET_LOOK_ID_0]=0.0;
   sensor_props[2].interior_fov[F360_DET_LOOK_ID_1]=0.0;
   sensor_props[2].interior_fov[F360_DET_LOOK_ID_2]=0.0;
   sensor_props[2].interior_fov[F360_DET_LOOK_ID_3]=0.0;
   sensor_props[2].left_fov_normal[F360_DET_LOOK_ID_0]=-0.0;
   sensor_props[2].left_fov_normal[F360_DET_LOOK_ID_1]=1.0;
   sensor_props[2].right_fov_normal[F360_DET_LOOK_ID_0]=0.0;
   sensor_props[2].right_fov_normal[F360_DET_LOOK_ID_1]=-1.0;
   sensor_props[2].left_fov_normal[F360_DET_LOOK_ID_2]=-0.0;
   sensor_props[2].left_fov_normal[F360_DET_LOOK_ID_3]=1.0;
   sensor_props[2].right_fov_normal[F360_DET_LOOK_ID_2]=0.0;
   sensor_props[2].right_fov_normal[F360_DET_LOOK_ID_3]=-1.0;

   sensor_props[3].time_since_measurement_s=980.403992;
   sensor_props[3].f_object_track_next_to_sensor=false;
   sensor_props[3].next_to_sensor_object_track_id=-1;
   sensor_props[3].next_to_sensor_object_track_min_long_pos=0.0;
   sensor_props[3].next_to_sensor_object_track_max_long_pos=0.0;
   sensor_props[3].next_to_sensor_object_track_min_lat_pos=0.0;
   sensor_props[3].next_to_sensor_object_track_max_lat_pos=0.0;
   sensor_props[3].interior_fov[F360_DET_LOOK_ID_0]=0.0;
   sensor_props[3].interior_fov[F360_DET_LOOK_ID_1]=0.0;
   sensor_props[3].interior_fov[F360_DET_LOOK_ID_2]=0.0;
   sensor_props[3].interior_fov[F360_DET_LOOK_ID_3]=0.0;
   sensor_props[3].left_fov_normal[F360_DET_LOOK_ID_0]=-0.0;
   sensor_props[3].left_fov_normal[F360_DET_LOOK_ID_1]=1.0;
   sensor_props[3].right_fov_normal[F360_DET_LOOK_ID_0]=0.0;
   sensor_props[3].right_fov_normal[F360_DET_LOOK_ID_1]=-1.0;
   sensor_props[3].left_fov_normal[F360_DET_LOOK_ID_2]=-0.0;
   sensor_props[3].left_fov_normal[F360_DET_LOOK_ID_3]=1.0;
   sensor_props[3].right_fov_normal[F360_DET_LOOK_ID_2]=0.0;
   sensor_props[3].right_fov_normal[F360_DET_LOOK_ID_3]=-1.0;

   sensor_props[4].time_since_measurement_s=980.403992;
   sensor_props[4].f_object_track_next_to_sensor=false;
   sensor_props[4].next_to_sensor_object_track_id=-1;
   sensor_props[4].next_to_sensor_object_track_min_long_pos=0.0;
   sensor_props[4].next_to_sensor_object_track_max_long_pos=0.0;
   sensor_props[4].next_to_sensor_object_track_min_lat_pos=0.0;
   sensor_props[4].next_to_sensor_object_track_max_lat_pos=0.0;
   sensor_props[4].interior_fov[F360_DET_LOOK_ID_0]=0.0;
   sensor_props[4].interior_fov[F360_DET_LOOK_ID_1]=0.0;
   sensor_props[4].interior_fov[F360_DET_LOOK_ID_2]=0.0;
   sensor_props[4].interior_fov[F360_DET_LOOK_ID_3]=0.0;
   sensor_props[4].left_fov_normal[F360_DET_LOOK_ID_0]=-0.0;
   sensor_props[4].left_fov_normal[F360_DET_LOOK_ID_1]=1.0;
   sensor_props[4].right_fov_normal[F360_DET_LOOK_ID_0]=0.0;
   sensor_props[4].right_fov_normal[F360_DET_LOOK_ID_1]=-1.0;
   sensor_props[4].left_fov_normal[F360_DET_LOOK_ID_2]=-0.0;
   sensor_props[4].left_fov_normal[F360_DET_LOOK_ID_3]=1.0;
   sensor_props[4].right_fov_normal[F360_DET_LOOK_ID_2]=0.0;
   sensor_props[4].right_fov_normal[F360_DET_LOOK_ID_3]=-1.0;

   sensor_props[5].time_since_measurement_s=980.403992;
   sensor_props[5].f_object_track_next_to_sensor=false;
   sensor_props[5].next_to_sensor_object_track_id=-1;
   sensor_props[5].next_to_sensor_object_track_min_long_pos=0.0;
   sensor_props[5].next_to_sensor_object_track_max_long_pos=0.0;
   sensor_props[5].next_to_sensor_object_track_min_lat_pos=0.0;
   sensor_props[5].next_to_sensor_object_track_max_lat_pos=0.0;
   sensor_props[5].interior_fov[F360_DET_LOOK_ID_0]=0.0;
   sensor_props[5].interior_fov[F360_DET_LOOK_ID_1]=0.0;
   sensor_props[5].interior_fov[F360_DET_LOOK_ID_2]=0.0;
   sensor_props[5].interior_fov[F360_DET_LOOK_ID_3]=0.0;
   sensor_props[5].left_fov_normal[F360_DET_LOOK_ID_0]=-0.0;
   sensor_props[5].left_fov_normal[F360_DET_LOOK_ID_1]=1.0;
   sensor_props[5].right_fov_normal[F360_DET_LOOK_ID_0]=0.0;
   sensor_props[5].right_fov_normal[F360_DET_LOOK_ID_1]=-1.0;
   sensor_props[5].left_fov_normal[F360_DET_LOOK_ID_2]=-0.0;
   sensor_props[5].left_fov_normal[F360_DET_LOOK_ID_3]=1.0;
   sensor_props[5].right_fov_normal[F360_DET_LOOK_ID_2]=0.0;
   sensor_props[5].right_fov_normal[F360_DET_LOOK_ID_3]=-1.0;

   sensor_props[6].time_since_measurement_s=980.403992;
   sensor_props[6].f_object_track_next_to_sensor=false;
   sensor_props[6].next_to_sensor_object_track_id=0;
   sensor_props[6].next_to_sensor_object_track_min_long_pos=0.0;
   sensor_props[6].next_to_sensor_object_track_max_long_pos=0.0;
   sensor_props[6].next_to_sensor_object_track_min_lat_pos=0.0;
   sensor_props[6].next_to_sensor_object_track_max_lat_pos=0.0;
   sensor_props[6].interior_fov[F360_DET_LOOK_ID_0]=0.0;
   sensor_props[6].interior_fov[F360_DET_LOOK_ID_1]=0.0;
   sensor_props[6].interior_fov[F360_DET_LOOK_ID_2]=0.0;
   sensor_props[6].interior_fov[F360_DET_LOOK_ID_3]=0.0;
   sensor_props[6].left_fov_normal[F360_DET_LOOK_ID_0]=-0.0;
   sensor_props[6].left_fov_normal[F360_DET_LOOK_ID_1]=1.0;
   sensor_props[6].right_fov_normal[F360_DET_LOOK_ID_0]=0.0;
   sensor_props[6].right_fov_normal[F360_DET_LOOK_ID_1]=-1.0;
   sensor_props[6].left_fov_normal[F360_DET_LOOK_ID_2]=-0.0;
   sensor_props[6].left_fov_normal[F360_DET_LOOK_ID_3]=1.0;
   sensor_props[6].right_fov_normal[F360_DET_LOOK_ID_2]=0.0;
   sensor_props[6].right_fov_normal[F360_DET_LOOK_ID_3]=-1.0;

   sensor_props[7].time_since_measurement_s=980.403992;
   sensor_props[7].f_object_track_next_to_sensor=false;
   sensor_props[7].next_to_sensor_object_track_id=0;
   sensor_props[7].next_to_sensor_object_track_min_long_pos=0.0;
   sensor_props[7].next_to_sensor_object_track_max_long_pos=0.0;
   sensor_props[7].next_to_sensor_object_track_min_lat_pos=0.0;
   sensor_props[7].next_to_sensor_object_track_max_lat_pos=0.0;
   sensor_props[7].interior_fov[F360_DET_LOOK_ID_0]=0.0;
   sensor_props[7].interior_fov[F360_DET_LOOK_ID_1]=0.0;
   sensor_props[7].interior_fov[F360_DET_LOOK_ID_2]=0.0;
   sensor_props[7].interior_fov[F360_DET_LOOK_ID_3]=0.0;
   sensor_props[7].left_fov_normal[F360_DET_LOOK_ID_0]=-0.0;
   sensor_props[7].left_fov_normal[F360_DET_LOOK_ID_1]=1.0;
   sensor_props[7].right_fov_normal[F360_DET_LOOK_ID_0]=0.0;
   sensor_props[7].right_fov_normal[F360_DET_LOOK_ID_1]=-1.0;
   sensor_props[7].left_fov_normal[F360_DET_LOOK_ID_2]=-0.0;
   sensor_props[7].left_fov_normal[F360_DET_LOOK_ID_3]=1.0;
   sensor_props[7].right_fov_normal[F360_DET_LOOK_ID_2]=0.0;
   sensor_props[7].right_fov_normal[F360_DET_LOOK_ID_3]=-1.0;
}
}

