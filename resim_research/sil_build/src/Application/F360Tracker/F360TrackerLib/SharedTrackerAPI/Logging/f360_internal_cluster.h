#ifndef F360_INTERNAL_CLUSTER_H
#define F360_INTERNAL_CLUSTER_H

// Add pragmas to throw error if struct is padded
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(error : 4820)
#elif 0
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wpadded"
#endif

#include <f360_reuse.h>

static const int32_t F360_Internal_Cluster_Stream_Num = 151;
static const int32_t F360_Internal_Cluster_Stream_Ver = 5;
static const int32_t F360_Internal_Cluster_Max_Payload_Size = 2000;

typedef struct F360_Internal_Cluster_Tag
{
   float vcs_long_posn;
   float vcs_lat_posn;
   float rep_vcs_az;
   float rep_rdotcomp;
   float time_since_cluster_updated; /* currently contains time relative to tracker execution time instead of measurement time */
   float time_since_measurement;     /* measurement time relative to current time*/
   float priority;                   /* obsolete */
   int16_t num_types_of_dets[2];
   int16_t id;
   int16_t clutter_counter;
   int8_t motion_status;             /* obsolete */
   uint8_t f_dealiased;
   uint8_t f_to_be_killed;           /* Flag indicating that cluster should be killed by it's not yet done. */
   uint8_t low_rcs_dets_cnt;
   uint8_t stationary_cluster_cnt;  /*  #Scans this cluster has been downprioritized in object creation because it is stationary. The cluster will be killed if its counter > threshold */
   uint8_t padding[3];
} F360_Internal_Cluster_T;


// Restore MSVC and GCC warning settings
#if defined _MSC_VER
#pragma warning(pop)
#elif 0
#pragma GCC diagnostic pop
#endif
#endif
