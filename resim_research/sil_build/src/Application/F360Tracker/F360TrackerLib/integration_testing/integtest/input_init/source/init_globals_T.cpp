#include "init_globals_T.h"

namespace f360_variant_A
{

void init_globals_T(F360_Globals_T & globals)
{
    globals.obj_mov_stat_spd_thresh=1.5;
    globals.rear_cornering_compliance=0.00529999984;
    globals.oncoming_speed_thresh=-1.0;
    globals.obj_vehicular_spd_thresh=3.0;
    globals.default_half_length=3.0;
    globals.default_half_width=1.0;
    globals.default_length=6.0;
    globals.default_width=2.0;
    globals.max_otg_speed=1.0;
}
}
