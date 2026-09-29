#ifndef F360_INTERNAL_CWD_H
#define F360_INTERNAL_CWD_H

// Add pragmas to throw error if struct is padded
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(error : 4820)
#elif 0
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wpadded"
#endif

#include <f360_reuse.h>

static const int32_t F360_Internal_CWD_Stream_Num = 155;
static const int32_t F360_Internal_CWD_Stream_Ver = 3;
static const int32_t F360_Internal_CWD_Max_Payload_Size = 2000;

typedef struct F360_Internal_CWD_Tag
{
   float measurement_lateral_position[5];
   uint8_t measurement_is_valid[5];
   uint8_t mounting_loc;
   uint8_t padding[2];
}F360_Internal_CWD_T;

// Restore MSVC and GCC warning settings
#if defined _MSC_VER
#pragma warning(pop)
#elif 0
#pragma GCC diagnostic pop
#endif
#endif
