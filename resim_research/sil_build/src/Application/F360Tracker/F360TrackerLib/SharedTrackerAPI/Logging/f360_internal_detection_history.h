#ifndef F360_INTERNAL_DETECTION_HISTORY_H
#define F360_INTERNAL_DETECTION_HISTORY_H

// Add pragmas to throw error if struct is padded
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(error : 4820)
#elif 0
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wpadded"
#endif

#include <f360_reuse.h>

static const int32_t F360_Internal_Detection_History_Stream_Num = 152;
static const int32_t F360_Internal_Detection_History_Stream_Ver = 4;
static const int32_t F360_Internal_Detection_History_Max_Payload_Size = 2000;

typedef struct F360_Internal_Detection_Hist_Tag
{
   float vcs_long_posn;
   float vcs_lat_posn;
   float rdot;
   float rdot_comp;
   float vcs_az;
   float time_since_meas;
   float v_wrapping;
   float r_wrapping;
   float elevation;
   int16_t cluster_idx;
   int16_t occupied_idx;
   int8_t motion_status;        /* [(-1)-2] (enum RSPP_Detection_Motion_Status_T) -1=Invalid, 0=stationary, 1=moving, 2=ambiguos */
   int8_t wheel_spin_type;      /* [0-2]type of wheel spin reflection 0=invalid, 1=detection pairs, 2= object */
   int8_t az_conf;
   int8_t el_conf;
   uint8_t f_dealiased;
   uint8_t f_FOV_edge;
   uint8_t f_selected;
   uint8_t f_azimuth_error_stat_mov; // obsolete
   uint8_t f_is_range_in_all_looks; /* flag that detection is from object that is visible by all look ids -> for AIT-959 implementation */
   uint8_t f_potential_angle_jump;
   uint8_t f_super_res;
   uint8_t sensor_id;
}F360_Internal_Detection_Hist_T;

// Restore MSVC and GCC warning settings
#if defined _MSC_VER
#pragma warning(pop)
#elif 0
#pragma GCC diagnostic pop
#endif
#endif
