#ifndef F360_INTERNAL_UNDERDRIVABILITY_H
#define F360_INTERNAL_UNDERDRIVABILITY_H

// Add pragmas to throw error if struct is padded
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(error : 4820)
#elif 0
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wpadded"
#endif

#include <f360_reuse.h>

static const int32_t F360_Internal_Underdrivability_Stream_Num = 153;
static const int32_t F360_Internal_Underdrivability_Stream_Ver = 1; 
static const int32_t F360_Internal_Underdrivability_Max_Payload_Size = 2000;

typedef struct F360_Internal_Underdrivability_Tag
{
   float state_height_can_pass[3];
   float state_height_is_likely_to_pass[3];
   float state_height_can_not_pass_upper[3];
   float state_height_can_not_pass_lower[3];

   float state_RCS_slope_can_pass[6];
   float state_RCS_slope_is_likely_to_pass[6];
   float state_RCS_slope_can_not_pass_upper[6];
   float state_RCS_slope_can_not_pass_lower[6];
} F360_Internal_Underdrivability_T;

// Restore MSVC and GCC warning settings
#if defined _MSC_VER
#pragma warning(pop)
#elif 0
#pragma GCC diagnostic pop
#endif
#endif
