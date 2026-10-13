#include "cmn_fill_internals_helper.h"
#include "ocg_underdrivability_states.h"

namespace ocg
{
UD_Height_States_T Fill_Height_States(const float in_state[UD_HEIGHT_STATE_SIZE])
{
   UD_Height_States_T states;
   states.num_dets          = in_state[UD_HEIGHT_STATE_DET_COUNT];
   states.mean_elevation    = in_state[UD_HEIGHT_STATE_ELEVATION_ANGLE];
   states.mean_sq_elevation = in_state[UD_HEIGHT_STATE_SQUARED_ELEVATION_ANGLE];
   return states;
}

UD_RCS_States_T Fill_RCS_States(const float in_state[UD_RCS_STATE_SIZE])
{
   UD_RCS_States_T states;
   states.num_dets      = in_state[UD_RCS_STATE_DET_COUNT];
   states.mean_range    = in_state[UD_RCS_STATE_RANGE];
   states.mean_sq_range = in_state[UD_RCS_STATE_SQUARED_RANGE];
   states.mean_rcs      = in_state[UD_RCS_STATE_RCS];
   states.mean_sq_rcs   = in_state[UD_RCS_STATE_SQUARED_RCS];
   states.rcs_range     = in_state[UD_RCS_STATE_RANGE_RCS_PROD];
   return states;
}
} // namespace ocg
