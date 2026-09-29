#include "f360_constants.h"
#include "f360_velocity.h"
#include "f360_look_ID.h"
#include "f360_range_type.h"
#include "f360_look_type.h"
#include "f360_radar_sensor.h"
#include "init_sensors_T.h"
namespace f360_variant_A
{
   void init_sensors_T(F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS])
   {
      /*MAX_NUMBER_OF_SENSORS = 12 in this build */

      /* sensor: 0*/
      sensors[0].variable.timestamp_us=980297192;
      sensors[0].variable.number_of_valid_detections=85;
      sensors[0].variable.look_index=19930;
      sensors[0].variable.look_id=F360_DET_LOOK_ID_3;
      sensors[0].variable.vcs_velocity.longitudinal=34.3179207;
      sensors[0].variable.vcs_velocity.lateral=0.013962226;
      sensors[0].variable.yaw_rate_calc_dps=0.0;
      sensors[0].variable.vehicle_speed_calc_mps=0.0;

      /* sensor: 1*/
      sensors[1].variable.timestamp_us=0;
      sensors[1].variable.number_of_valid_detections=0;
      sensors[1].variable.look_index=0;
      sensors[1].variable.look_id=F360_DET_LOOK_ID_0;
      sensors[1].variable.vcs_velocity.longitudinal=0.0;
      sensors[1].variable.vcs_velocity.lateral=0.0;
      sensors[1].variable.yaw_rate_calc_dps=0.0;
      sensors[1].variable.vehicle_speed_calc_mps=0.0;

      /* sensor: 2*/
      sensors[2].variable.timestamp_us=0;
      sensors[2].variable.number_of_valid_detections=0;
      sensors[2].variable.look_index=0;
      sensors[2].variable.look_id=F360_DET_LOOK_ID_0;
      sensors[2].variable.vcs_velocity.longitudinal=0.0;
      sensors[2].variable.vcs_velocity.lateral=0.0;
      sensors[2].variable.yaw_rate_calc_dps=0.0;
      sensors[2].variable.vehicle_speed_calc_mps=0.0;

      /* sensor: 3*/
      sensors[3].variable.timestamp_us=0;
      sensors[3].variable.number_of_valid_detections=0;
      sensors[3].variable.look_index=0;
      sensors[3].variable.look_id=F360_DET_LOOK_ID_0;
      sensors[3].variable.vcs_velocity.longitudinal=0.0;
      sensors[3].variable.vcs_velocity.lateral=0.0;
      sensors[3].variable.yaw_rate_calc_dps=0.0;
      sensors[3].variable.vehicle_speed_calc_mps=0.0;

      /* sensor: 4*/
      sensors[4].variable.timestamp_us=0;
      sensors[4].variable.number_of_valid_detections=0;
      sensors[4].variable.look_index=0;
      sensors[4].variable.look_id=F360_DET_LOOK_ID_0;
      sensors[4].variable.vcs_velocity.longitudinal=0.0;
      sensors[4].variable.vcs_velocity.lateral=0.0;
      sensors[4].variable.yaw_rate_calc_dps=0.0;
      sensors[4].variable.vehicle_speed_calc_mps=0.0;

      /* sensor: 5*/
      sensors[5].variable.timestamp_us=0;
      sensors[5].variable.number_of_valid_detections=0;
      sensors[5].variable.look_index=0;
      sensors[5].variable.look_id=F360_DET_LOOK_ID_0;
      sensors[5].variable.vcs_velocity.longitudinal=0.0;
      sensors[5].variable.vcs_velocity.lateral=0.0;
      sensors[5].variable.yaw_rate_calc_dps=0.0;
      sensors[5].variable.vehicle_speed_calc_mps=0.0;

      /* sensor: 6*/
      sensors[6].variable.timestamp_us=0;
      sensors[6].variable.number_of_valid_detections=0;
      sensors[6].variable.look_index=0;
      sensors[6].variable.look_id=F360_DET_LOOK_ID_0;
      sensors[6].variable.vcs_velocity.longitudinal=0.0;
      sensors[6].variable.vcs_velocity.lateral=0.0;
      sensors[6].variable.yaw_rate_calc_dps=0.0;
      sensors[6].variable.vehicle_speed_calc_mps=0.0;

      /* sensor: 7*/
      sensors[7].variable.timestamp_us=0;
      sensors[7].variable.number_of_valid_detections=0;
      sensors[7].variable.look_index=0;
      sensors[7].variable.look_id=F360_DET_LOOK_ID_0;
      sensors[7].variable.vcs_velocity.longitudinal=0.0;
      sensors[7].variable.vcs_velocity.lateral=0.0;
      sensors[7].variable.yaw_rate_calc_dps=0.0;
      sensors[7].variable.vehicle_speed_calc_mps=0.0;
   }
}
