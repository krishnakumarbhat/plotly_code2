
#include "f360_reuse.h"
#include "f360_position.h"
#include "f360_host_raw.h"
#include "f360_accel.h"
#include "f360_host_props.h"
#include "init_host_props_T.h"
namespace f360_variant_A
{

void init_host_props_T(F360_Host_Props_T & host_props)
{
    host_props.position.x=6.86346102;
    host_props.position.y=-0.000131496054;
    host_props.vel_cov_scm[0][0]=0.0119236112;
    host_props.vel_cov_scm[0][1]=1.86583984e-05;
    host_props.vel_cov_scm[1][0]=1.86583984e-05;
    host_props.vel_cov_scm[1][1]=0.00148258137;
    host_props.vel_cov[0][0]=0.20776768;
    host_props.vel_cov[0][1]=-4.89566555e-05;
    host_props.vel_cov[1][0]=-4.89566555e-05;
    host_props.vel_cov[1][1]=0.091177702;
    host_props.position_inc_cov_scm[0][0]=2.98090308e-05;
    host_props.position_inc_cov_scm[0][1]=4.66460008e-08;
    host_props.position_inc_cov_scm[1][0]=4.66460008e-08;
    host_props.position_inc_cov_scm[1][1]=3.70645375e-06;
    host_props.position_inc_cov[0][0]=0.000519419264;
    host_props.position_inc_cov[0][1]=-1.22391654e-07;
    host_props.position_inc_cov[1][0]=-1.22391654e-07;
    host_props.position_inc_cov[1][1]=0.000227944271;
    host_props.std_speed_scm=0.109195359;
    host_props.std_yaw_rate_scm=0.00100000005;
    host_props.heading_angle=-0.000752687454;
    host_props.cos_heading=0.999999702;
    host_props.sin_heading=-0.000752687396;
    host_props.delta_pointing=-0.000326087233;
    host_props.cos_delta_pointing=0.99999994;
    host_props.sin_delta_pointing=-0.000326087233;
    host_props.delta_position.x=1.71586502;
    host_props.delta_position.y=0.000257372303;
}
}
