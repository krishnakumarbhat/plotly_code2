#ifndef CDC_PACKING_H
#define CDC_PACKING_H

#ifdef CDC_ENABLE
   /*===========================================================================*/
   /**
    * @file cdc_packing.h
    *
    * This module packs the CDC Beam vectors into CDC Records
    * and copies onto ENET buffer.
    *
    *------------------------------------------------------------------------------
    *
    * Copyright (C) 2024 Aptiv. All rights reserved.
    *Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
    *
    *------------------------------------------------------------------------------
    *
    * @section DESC DESCRIPTION:
    *
    * @todo Add full description here
    *
    * @section ABBR ABBREVIATIONS:
    *   - @todo List any abbreviations, precede each with a dash ('-').
    *
    * @section TRACE TRACEABILITY INFO:
    *   - Design Document(s):
    *     - @todo Update list of design document(s).
    *
    *   - Requirements Document(s):
    *     - @todo Update list of requirements document(s)
    *
    *   - Applicable Standards (in order of precedence: highest first):
    *     - ESGW_4-2_PE-SWx_00-01-A02_EN - C Coding Standards [20120506]
    *     - @todo Update list of other applicable standards
    *
    * @section DFS DEVIATIONS FROM STANDARDS:
    *   - @todo List of deviations from standards in this file, or "None".
    *
    * @ updates to areas outside the scope of procedures:
    *   - Refer to module footer comment block.
    */
   /*==========================================================================*/
   /*===========================================================================*\
    * Standard Header Files
   \*===========================================================================*/

   /*===========================================================================*\
    * Other Header Files
   \*===========================================================================*/
   #include "bb_cfg.h"
   #include "cdc_frame_interface.h"
   #include "cdc_stream.h"
   #include "doppler_proc.h"
   #include "ipc_data.h"
   #include "ipc_dsp.h"
   #include "spbb_include.h"
   #include "stream_handler_if.h"
   /*===========================================================================*\
 * Exported Preprocessor #define Constants
\*===========================================================================*/
   /* Depth of ENET RAM Buffers */
   /* For full log of 5016 dbin without roll over:139 + 1*/
   /** @todo: Check for the right size after cdc logs collection */
   #define CDC_LAST_CHUNK_INDICATOR          (1U)
   #define CDC_NUM_ENET_TX_BUFFERS           ((144U) + CDC_LAST_CHUNK_INDICATOR)
   #define CDC_NUM_BYTES_PER_CDC_DATA_RECORD ((uint16_t)sizeof(CDC_Record_T))
   #define CDC_NUM_RECORDS_PER_FRAME         ((uint16_t)((sizeof(CDC_Stream_T) - sizeof(Dyn_Hdr_T)) / CDC_NUM_BYTES_PER_CDC_DATA_RECORD))
/*===========================================================================*\
 * Exported Preprocessor #define MACROS
\*===========================================================================*/

/*===========================================================================*\
 * Exported Type Declarations
\*===========================================================================*/
typedef BV_Comp_Type_T CDC_OutputDataCube_T[SPBB_MAX_DOPPLER_FFT_SIZE][NUM_CDM_CHANNELS];
typedef CDC_OutputDataCube_T *CDC_OutputDataCube_Ptr_T;

typedef struct Cdc_Packing_Param_Tag
{
   CDC_Record_T *p_enet_buffer;
   float32_t recip_cdc_num_records_per_frame;
   uint16_t cdc_records_in_partial_frame;
   uint16_t cdc_message_id;
   uint16_t enet_frame_buffer_depth;
   uint16_t cdc_enet_frame_count;
   uint16_t cdc_enet_packet_tx_cnt;
   uint16_t cdc_look_total_bins;          /* max cdc records per radar cycle*/
   uint16_t cdc_look_total_bins_max;      /* max cdc records across radar cycle*/
   uint16_t cdc_look_num_enet_frames;     /* max cdc enet frames per radar cycle*/
   uint16_t cdc_look_num_enet_frames_max; /* max cdc enet frames across radar cycle*/
   uint16_t cdc_bins_per_rbin_max;        /*  max cdc records per rbin in radar cycle*/
   uint16_t cdc_look_bins_per_rbin_max;   /* max cdc records per rbin across radar cycle*/
   uint8_t enet_frame_index;
   bool partial_frame_available;
} Cdc_Packing_Param_T;

extern Cdc_Packing_Param_T Cdc_Log_Param;

   /*===========================================================================*\
    * Exported Object Declarations
   \*===========================================================================*/
   #ifdef ENABLE_CDC_FRAME_INTERFACE_CRC_MULTI_BUFF
extern volatile Cdc_Frame_Interface_Sync_T Cdc_Frame_Interface_Buff;
   #else
extern volatile Cdc_Frame_Interface_T Cdc_Frame_Interface;
   #endif
/*===========================================================================*\
 * Exported Function Prototypes
\*===========================================================================*/
void Cdc_Packing(uint16_t range_idx, uint16_t cdc_dbin_count, rdop_avg_t **rdop_avg_data_ptr, uint16_t fft_n);

#endif /* ifdef CDC_ENABLE */
#endif /* CDC_PACKING_H */
