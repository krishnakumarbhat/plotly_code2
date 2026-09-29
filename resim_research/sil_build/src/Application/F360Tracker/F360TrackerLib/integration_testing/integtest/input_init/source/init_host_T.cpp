#include "f360_reuse.h"
#include "f360_accel.h"
#include "f360_host.h"
#include "init_host_T.h"
namespace f360_variant_A
{

void init_host_T( F360_Host_T & host)
{
    host.vehicle_index =6;
    host.speed =34.3173103;
    host.vcs_speed=34.3172951;
    host.acceleration=-6.14509583;
    // host.vcs_acceleration.lateral=0.0328746177;
    host.vcs_lat_acceleration=0.0328746177;
    host.vcs_long_acceleration=-6.14509583;
    host.yaw_rate_rad=-0.00629700907;
    host.vcs_sideslip=0.000460191397;
    host.curvature_rear=-0.000164843179;
    host.dist_rear_axle_to_vcs_m=3.45000005;
    host.rear_cornering_compliance=0.00529999984;
    host.speed_correction_factor=0.998000026;
}
}
