#ifndef TRAILER_DETECTOR_INTERNAL_LOG_H
#define TRAILER_DETECTOR_INTERNAL_LOG_H
/*===========================================================================*\
* FILE: TrailerDetectorInternalLog.h
*===========================================================================
* Copyright 2023 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains F360 Trailer Detector log stream
*
* ABBREVIATIONS:
*   TODO: List of abbreviations used, or reference(s) to external document(s)
*
* TRACEABILITY INFO:
*   Design Document(s):
*   Requirements Document(s): PDD-10024333-012_(CADS4_VFP_Ethernet_Communication).doc
*   (Design & Requirements)
*
*   Applicable Standards (in order of precedence: highest first):
*     SW REF 264.15D "Delphi C Coding Standards" [12-Mar-2006]
*
*
* DEVIATIONS FROM STANDARDS:
*   None
*
\*===========================================================================*/


#include "../Types/f360_reuse.h"
/*===========================================================================*\
* Other Header Files
\*===========================================================================*/
#include "../Types/T360_Types.h"

/*===========================================================================*\
* Exported local (file scope) Constants
\*===========================================================================*/
static const int TRAILER_DETECTOR_INTERNAL_LOG_STREAM_NUM      = 87;
static const int TRAILER_DETECTOR_INTERNAL_LOG_STREAM_VERSION  = 3;
static const int TRAILER_DETECTOR_INTERNAL_LOG_NUM_CHUNKS      = 1;

#if defined(__TASKING__)
// TBD
#elif defined(__DCC__) && defined(__DCC_LLVM__)
#pragma pack(4)
#elif defined(__DCC__)
#pragma pack(4,4)
#elif defined(__TMS320C6X__)
#pragma pack(push,4)
#elif defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(push, save_pack, 4)
#else
#endif

/**
This structure is used to hold information about the F360 Trailer Detector Internal part
*/
typedef struct F360_Internal_Trailer_Detector_Tag
{
   // TP
   float TP_Mean_RR_value_array[100];
   uint32_t tp_window_timer;
   int16_t TP_Mean_RR_num_array[100];
   int16_t TP_Relative_Ratio_dets_n_02;
   int16_t TP_Relative_Ratio_dets_n_03;
   int16_t TP_Relative_Ratio_dets_n_04;
   int16_t TP_Relative_Ratio_dets_n_05;
   bool tp_f_estimation_done;
   uint8_t padding[3];

   //TL
   int32_t detection_row[80];
   int32_t detection_row_40s[60];
   int32_t detection_row_50s[60];
   int32_t detection_row_60s[60];
   uint32_t tl_reset_timer;
   uint32_t tl_window_timer;
   float trailer_length_struct_40s;
   float trailer_length_struct_50s;
   float trailer_length_struct_60s;
   float axel_trailer_length;
   float axel_trailer_length_40s;
   float axel_trailer_length_50s;
   float axel_trailer_length_60s;
   bool tl_f_estimation_done;
   uint8_t padding1[3];

   //TW
   uint32_t tw_reset_timer;
   uint32_t tw_window_timer;
   uint32_t detection_col[12][45]; //X_INTERVALS_NUMBER and COL_NUMBER
   bool tw_f_estimation_done;
   uint8_t padding2[3];

   //TA
   float HV_angle;
   float prev_trailer_angle;
   float trailer_axel_length;
   int16_t HV_cnt;
   uint8_t padding3[2];
}F360_Internal_Trailer_Detector_T;

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************

static_assert(sizeof(F360_Internal_Trailer_Detector_T) == 3884U, "F360_Internal_Trailer_Detector_T: Wrong size");

#if defined(__TASKING__)
// TBD
#elif defined(__DCC__) && defined(__DCC_LLVM__)
#pragma pack()
#elif defined(__DCC__)
#pragma pack(0)
#elif defined(__TMS320C6X__)
#pragma pack(pop)
#elif defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(pop, save_pack)
#else
#endif

#endif 
