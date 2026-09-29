#ifndef RECU_STREAMS_H
#define RECU_STREAMS_H

#include "radar_ecu_CORE0.h"
#include "radar_ecu_CORE1.h"
#include "radar_ecu_CORE3.h"
#include "radar_ecu_calibration.h"
#include "radar_ecu_internal_CORE1.h"
#include "radar_ecu_vru_classifier.h"
#include "OSI_Input_Structure.h"
#include "DSPACE_Common_Input_Structure.h"
#include "udp_record.h"
#include "f360_log_types.h"

/* RECU Frames */
#define RECU_UDP_FRAME_LENGTH            1468
#define RECU_UDP_MAX_DATA_SIZE           (RECU_UDP_FRAME_LENGTH - sizeof(UDPRecord_Header_T) - sizeof(UDP_CUST_FRAME_HEADER_T))
#define RECU_UDP_LOGGING_DATA_BUFF_INDEX 8 // 6th index Contains OSI Stream, 7th index contains DSPACE stream
#define RECU_RADAR_ECU_POS               20
#define RECU_UDP_PLATFORM_SRR5           41
#define RECU_CUSTOMER_ID_BMW_SAT_SRR5    0x27
#define RECU_UDP_PDU_LENGTH              (1472U)
#define MILLI_TO_MICRO_CONVERSION        1000

/* Enum identifying the core to be logged. To be updated everytime a new core is added */
typedef enum Radar_Logging_Data_Source_Tag {
   CORE0_LOGGING_DATA          = 90,
   CORE1_LOGGING_DATA          = 91,
   CORE3_LOGGING_DATA          = 92,
   OG_INTERNAL_LOGGING_DATA    = 93,
   CAL_LOGGING_DATA            = 94,
   ECU_INTERNAL_LOGGING_DATA   = 95,
   VRU_CLASSIFIER_LOGGING_DATA = 96,
   MAX_LOGGING_SOURCE          = 97,
   OSI_LOGGING_DATA            = 67,
   DSPACE_INPUT_LOGGING_DATA   = 10
} Radar_Logging_Data_Source_T;

/*Structure holding information about the cores to facilitate logging */
typedef struct data_buffer_Tag {
   unsigned8_T *data_Buff_Ptr;
   unsigned32_T Total_size;
   unsigned8_T Struct_version;
   Radar_Logging_Data_Source_T Log_Data_Source;
} logging_data_buffer_T;

/* Structure defining the mux elements used to pack data into logging table */
typedef struct MUX_ID_Table_Struct_Logging_Tag {
   unsigned8_T mux_id;                 /* Mux table id*/
   unsigned16_T logging_array_size;    /* Size of the data that will go into the packet */
   unsigned8_T frame_diagostic;        /* Defines if the packet is full/not full/empty */
   unsigned8_T logging_source;         /* Identifies the core from which data is logged */
   unsigned8_T total_no_chunks;        /* Total chunks needed for logging data from a  core */
   unsigned8_T chunk_number;           /* Current chunk no.*/
   unsigned8_T Logging_Version;        /* Version of the data logged from a core */
   unsigned8_T *logging_array_address; /* Address from which data is fetched */
} MUX_ID_Table_Struct_Logging_T;

/* Strutcure of the frame that will be part of the payload */
typedef struct UDP_FRAME_STRUCTURE_Tag {
   UDPRecord_Header_T frame_header;           /* Header defining the data being transmitted */
   UDP_CUST_FRAME_HEADER_T cust_frame_header; /* Customer defined frame header */
   unsigned8_T data[RECU_UDP_MAX_DATA_SIZE];  /* Actual data */
   unsigned8_T crc32[4];
} UDP_FRAME_STRUCTURE_T;

/* Structure holding the frame and address */
typedef struct UDP_PACKAGE_POINTER_TABLE_Tag {
   unsigned32_T *frame_address; /* Address of the frame to be copied to be txed over UDP */
   UDP_FRAME_STRUCTURE_T frame; /* Actual Data Frame */
} UDP_PACKAGE_POINTER_TABLE_T;

/* Calculation of Max packets needed for UDP Logging */
#define RECU_CORE0_LEN          sizeof(Radar_ECU_CORE0_T)
#define RECU_CORE1_LEN          sizeof(Radar_ECU_CORE1_T)
#define RECU_CORE3_LEN          sizeof(Radar_ECU_CORE3_T)
#define RECU_CAL_LEN            sizeof(Radar_ECU_calibration_T)
#define RECU_INTERNAL_CORE1_LEN sizeof(Radar_ECU_Internal_CORE1_T)
#define RECU_VRU_LEN            sizeof(Radar_ECU_VRU_T)
#define RECU_OSI_LEN            sizeof(Ref_Logging_Data_T)
#define RECU_EXCESS_OSI_LEN     ((RECU_OSI_LEN % RECU_UDP_MAX_DATA_SIZE))
#define RECU_DSPACE_INPUT_LEN   sizeof(DSPACE_Common_Logging_Data_T)

#define PACKETS_FOR_CORE0          (uint16_t)((((uint32_t)RECU_CORE0_LEN) / ((uint16_t)RECU_UDP_MAX_DATA_SIZE)) + 1)
#define PACKETS_FOR_CORE1          (uint16_t)((((uint32_t)RECU_CORE1_LEN) / ((uint16_t)RECU_UDP_MAX_DATA_SIZE)) + 1)
#define PACKETS_FOR_CORE3          (uint16_t)((((uint32_t)RECU_CORE3_LEN) / ((uint16_t)RECU_UDP_MAX_DATA_SIZE)) + 1)
#define PACKETS_FOR_CAL            (uint16_t)((((uint32_t)RECU_CAL_LEN) / ((uint16_t)RECU_UDP_MAX_DATA_SIZE)) + 1)
#define PACKETS_FOR_ECU_INTERNAL   (uint16_t)((((uint32_t)RECU_INTERNAL_CORE1_LEN) / ((uint16_t)RECU_UDP_MAX_DATA_SIZE)) + 1)
#define PACKETS_FOR_VRU_CLASSIFIER (uint16_t)((((uint32_t)RECU_VRU_LEN) / ((uint16_t)RECU_UDP_MAX_DATA_SIZE)))
#define PACKETS_FOR_OSI            ((RECU_OSI_LEN / RECU_UDP_MAX_DATA_SIZE) + (0 < RECU_EXCESS_OSI_LEN && RECU_EXCESS_OSI_LEN < RECU_UDP_MAX_DATA_SIZE))
#define PACKETS_FOR_DSPACE_INPUT   (uint16_t)((((uint32_t)RECU_DSPACE_INPUT_LEN) / ((uint16_t)RECU_UDP_MAX_DATA_SIZE)) + 1)

#define MAX_PACKETS_TXED (PACKETS_FOR_CORE0 + PACKETS_FOR_CORE1 + PACKETS_FOR_CORE3 + PACKETS_FOR_CAL + PACKETS_FOR_ECU_INTERNAL + PACKETS_FOR_VRU_CLASSIFIER + PACKETS_FOR_OSI + PACKETS_FOR_DSPACE_INPUT)

/* Total packets available to fill logging data info */
#define UDP_MUX_TAB_SIZE MAX_PACKETS_TXED

/* Frame Diagnostics Macros */
#define FRAME_EMPTY        0
#define FRAME_PARTIAL_FULL 1
#define FRAME_FULL         2
#define FRAME_OVERFLOW     3

/* Get pointers for all core data buffers */
Radar_ECU_CORE0_T *GetRadarCore0DataPtr();
Radar_ECU_CORE1_T *GetRadarCore1DataPtr();
Radar_ECU_CORE3_T *GetRadarCore3DataPtr();
Radar_ECU_calibration_T *GetRadarCalDataPtr();
Radar_ECU_Internal_CORE1_T *GetRadarInternalDataPtr();
Radar_ECU_VRU_T *GetRadarVruDataPtr();
Ref_Logging_Data_T *GetRadarOsiDataPtr();
DSPACE_Common_Logging_Data_T *GetRadarDspaceInputDataPtr();
void RECUUDPLogInit(void);
void RECUUDPLogSendFrames(void);
void SetRECUMUDPScanIndex(uint16_T in_val);
uint16_t GetScanIndex(void);
void CopyFFOutputToUDPBuffer(f360_variant_A::F360_Object_Log_Output_T *obj);
#endif // RECU_STREAMS_H
