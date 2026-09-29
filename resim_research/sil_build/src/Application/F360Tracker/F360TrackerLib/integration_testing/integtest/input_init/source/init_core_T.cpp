#include "f360_reuse.h"
#include <stdint.h>
#include "f360_core_info.h"
#include "init_core_T.h"
namespace f360_variant_A
{

void init_core_T(F360_Core_Info_T & core_info)
{
    core_info.cnt_loops=6;
    core_info.time_us =980404000;
    core_info.prev_time_us=980454000;
    core_info.elapsed_time_s=0.0500000007;
}
}
