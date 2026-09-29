#ifndef F360_REFLECTION_BUFFER_H
#define F360_REFLECTION_BUFFER_H

// Add pragmas to throw error if struct is padded
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(error : 4820)
#elif 0
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wpadded"
#endif

#include <f360_reuse.h>

static const int32_t F360_Internal_Reflection_Buffer_Stream_Num = 154;
static const int32_t F360_Internal_Reflection_Buffer_Stream_Ver = 1;
static const int32_t F360_Internal_Reflection_Buffer_Max_Payload_Size = 2000;

typedef struct F360_Internal_Reflection_Buffer_Tag
{
   float range[64];
   float azimuth[64];
   float amplitude[64];
   uint32_t occurrence_count[64];
   uint32_t age[64];
}F360_Internal_Reflection_Buffer_T;

// Restore MSVC and GCC warning settings
#if defined _MSC_VER
#pragma warning(pop)
#elif 0
#pragma GCC diagnostic pop
#endif
#endif
