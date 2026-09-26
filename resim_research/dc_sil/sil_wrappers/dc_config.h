#ifndef DC_CONFIG_H
#define DC_CONFIG_H

#include <cinttypes>

// Pack structures with 2 byte padding
#pragma pack(push, save_pack)
#pragma pack(push, 2)
/* Enumeration to choose the RUN Mode for Algorithm Execution */
typedef enum {
   NONE_ECU = 0,
   DETECTIONS_UDP,
   OBJECT_MODE_ECU,
   DETECTIONS_PCAN,
   ECU_DSPACE,
   ECU_DETECTIONS_SOMEIP,
   RUN_BIN
} Run_Mode_T;

/* Enumeration to choose the Status of Algorithm Execution */
typedef enum SIL_Target_Status_Tag {
   SIL_INIT_SUCCESS,
   SIL_INIT_IN_PROGRESS,
   SIL_INIT_FAIL,
   SIL_EXECUTE_SUCCESS,
   SIL_EXECUTE_IN_PROGRESS,
   SIL_EXECUTE_FAIL,
   SIL_RESET_SUCCESS,
   SIL_RESET_IN_PROGRESS,
   SIL_RESET_FAIL,
   SIL_INIT_CAL_ERR,
   SIL_GET_STATISTICS_SUCCESS,
   SIL_GET_STATISTICS_ERR
} SIL_Target_Status_T;

typedef struct SCalibInfo_Tag {
   static const int MAX_CALIB          = 150;
   static const int MAX_CALIB_BUF_SIZE = 50000;
   unsigned char *psCalib[MAX_CALIB];
   unsigned char f_CalibValid[MAX_CALIB];
   unsigned int which_cal[MAX_CALIB];
} SCalibInfo_T;

// Enum data defining the status of emb init call
typedef enum Srr5SilInitStatusEnum_tag {
   e_INIT_UNKNOWN_ERR = -1, // unspecified error
   e_INIT_NO_ERR      = 0,  // emb init successful
   e_INIT_RADAR_POS_ERR,    // emb init received wrong radar/ecu position
   e_INIT_CAL_BUFFER_ERR,   // emb init received an empty/null calibration buffers
   e_INIT_GDSR_BUFFER_ERR,  // emb init received an empty GDSR buffer
   e_INIT_F360_BUFFER_ERR,  // emb init received an empty F360 buffer
   e_INIT_CHKSM_ERR         // emb init checksum error.
} Srr5SilInitStatusEnum_t;

typedef struct SIL_DC_Config_Tag {
   Run_Mode_T Run_Mode;
   uint8_t radar_pos;
   SCalibInfo_T *pCalibInfo;
   char *pEmb_Cfg_Filepath;
   char *pCustomerName;
   Srr5SilInitStatusEnum_t InitStatus;
} SIL_DC_Config_T;

#pragma pack(pop, save_pack)
#endif // DC_CONFIG_H
