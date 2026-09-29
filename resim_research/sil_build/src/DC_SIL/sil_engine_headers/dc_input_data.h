#ifndef INPUT_DATA_H
#define INPUT_DATA_H

#include <cinttypes>
#include "RR_ADAS_SYMBOL_Data.h"
#include "udp_record.h"

#define INPUT_DATA_STRUCTURE_VERSION (1)

// Pack structures with 2 byte padding
#pragma pack(push, save_pack)
#pragma pack(push, 2)

typedef enum ECU_Logging_Data_Source_Tag {
   ECU_UNKNOWN_STREAM        = -1,
   ECU_CORE_0_TRK_IAL        = 0,  // ECU UDP stream number - 90
   ECU_CORE_1_TRK_OAL        = 1,  // ECU UDP stream number - 91
   ECU_CORE_3_FF_OAL         = 2,  // ECU UDP stream number - 92
   ECU_OG_INTERNAL           = 3,  // ECU UDP stream number - 93
   ECU_CALIB_DATA            = 4,  // ECU UDP stream number - 94
   ECU_CORE1_INTERNAL        = 5,  // ECU UDP stream number - 95
   ECU_VRU_CLASSIFIER_Stream = 6,  // ECU VRU stream number - 96
   ECU_PATCH_VERSION         = 19, // Reserved for Passing RECU RESIM Patch Version
   MAX_ECU_LOGGING_SOURCE    = 40
} ECU_Logging_Data_Source_T;

typedef struct Input_Data_Hdr {
   uint32_t Sec_Size;
   uint16_t Sec_Compatibility;
   uint16_t Sec_Checksum;
   uint16_t Sensor_id;
} Input_Data_Hdr_T;

/* Input Data Srructure with UDP Log data */
typedef struct SIL_DC_Input_Data_Tag {
   Input_Data_Hdr_T Input_Data_Header;
   UDP_CUST_FRAME_HEADER_T cust_frame_header; /* Customer defined frame header */
   void *Radar_Stream[MAX_ECU_LOGGING_SOURCE];
   RR_ADAS_SYMBOL_Data_T Symbol_Record;
} SIL_DC_Input_Data_T;

#pragma pack(pop, save_pack)
#endif // INPUT_DATA_H
