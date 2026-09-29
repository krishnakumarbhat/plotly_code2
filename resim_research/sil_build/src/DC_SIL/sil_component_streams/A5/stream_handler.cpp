#include <stdint.h> //compiler warning fixes , std definition overlapping with fixmac.h file
#include "stream_handler.h"
#include "dc_output_data.h"
#include "Tracker_VariantA_Wrapper.h"
#include "recu_stream_log.h"
#include "rot_iso_object_stream.h"
#include "rot_sae_object_stream.h"
#include "rot_processed_detection_stream.h"
#include "sfl_stream.h"
#include "sg_stream.h"
#include "vse_stream.h"
#include "gt_input_stream.h"
#include "DGPS_stream.h"

#define PACKETS_FOR_ROT_SAE_OBJECT_STREAM          (uint16_t)((((uint32_t)SIZE_OF_RESIM_ROT_SAE_OBJECT_STREAM) / ((uint16_t)REC_PAYLOAD)) + 1)
#define PACKETS_FOR_ROT_ISO_OBJECT_STREAM          (uint16_t)((((uint32_t)SIZE_OF_RESIM_ROT_ISO_OBJECT_STREAM) / ((uint16_t)REC_PAYLOAD)) + 1)
#define PACKETS_FOR_ROT_PROCESSED_DETECTION_STREAM (uint16_t)((((uint32_t)SIZE_OF_RESIM_ROT_PROCESSED_DETECTION_STREAM) / ((uint16_t)REC_PAYLOAD)) + 1)
#define PACKETS_FOR_SFL_STREAM                     (uint16_t)((((uint32_t)SIZE_OF_RESIM_SFL_STREAM) / ((uint16_t)REC_PAYLOAD)) + 1)
#define PACKETS_FOR_VSE_STREAM                     (uint16_t)((((uint32_t)SIZE_OF_RESIM_VSE_STREAM) / ((uint16_t)REC_PAYLOAD)) + 1)
#define PACKETS_FOR_GT_INPUT_STREAM                (uint16_t)((((uint32_t)SIZE_OF_RESIM_GT_INPUT_STREAM) / ((uint16_t)REC_PAYLOAD)) + 1)
#define PACKETS_FOR_SG_STREAM                      (uint16_t)((((uint32_t)SIZE_OF_RESIM_SG_STREAM) / ((uint16_t)REC_PAYLOAD)) + 1)
#define PACKETS_FOR_DGPS_STREAM                    (uint16_t)((((uint32_t)SIZE_OF_RESIM_DGPS_STREAM) / ((uint16_t)REC_PAYLOAD)) + 1)
#define GEN7_LOGGING_STREAM_COUNT                  8
#define GEN7_MAX_PACKETS_TxED                      (PACKETS_FOR_ROT_SAE_OBJECT_STREAM + PACKETS_FOR_ROT_ISO_OBJECT_STREAM + PACKETS_FOR_ROT_PROCESSED_DETECTION_STREAM + PACKETS_FOR_SFL_STREAM + PACKETS_FOR_VSE_STREAM + PACKETS_FOR_GT_INPUT_STREAM + PACKETS_FOR_SG_STREAM + PACKETS_FOR_DGPS_STREAM)
gen7_udp_frame_T gen7_frame;

static gen7_logging_data_buffer_T gen7_logging_data_buffer[GEN7_LOGGING_STREAM_COUNT] = {{0}};
static gen7_mux_id_table_logging_T gen7_mux_id_table[GEN7_MAX_PACKETS_TxED]           = {{0}};

/* function declarations */
void StreamHandlerInit();
void InitGen7MUXTables();
void PeriodicallySendStreams();
DC_ROT_SAE_Object_Stream_T *GetROTSAEObjectDataPtr();
DC_ROT_ISO_Object_Stream_T *GetROTISOObjectDataPtr();
DC_ROT_Processed_Detection_Stream_T *GetROTProcessedDetDataPtr();
DC_SFL_Stream_T *GetSFLDataPtr();
DC_Vse_Stream_T *GetVSEDataPtr();
DC_GT_Logging_Data_T *GetDspaceInputDataPtr();
gen7_udp_frame_T *GetGen7FramePtr();
SIL_DC_Output_Data_T *GetOutputDataPtr();
DC_SG_Stream_T *Get_SG_Data_Ptr();
DC_DGPS_Data_T *GetDCDGPSDataptr();

void StreamHandlerInit() {
   // ROT_Object_Stream
   gen7_logging_data_buffer[0].data_buff_ptr   = (uint8_t *)GetROTSAEObjectDataPtr();
   gen7_logging_data_buffer[0].total_size      = SIZE_OF_RESIM_ROT_SAE_OBJECT_STREAM;
   gen7_logging_data_buffer[0].log_data_source = DC_ROT_SAE_OBJECT_STREAM_NUMBER;
   gen7_logging_data_buffer[0].struct_version  = ROT_SAE_OBJECT_STREAM_VERSION;

   gen7_logging_data_buffer[1].data_buff_ptr   = (uint8_t *)GetROTISOObjectDataPtr();
   gen7_logging_data_buffer[1].total_size      = SIZE_OF_RESIM_ROT_ISO_OBJECT_STREAM;
   gen7_logging_data_buffer[1].log_data_source = DC_ROT_ISO_OBJECT_STREAM_NUMBER;
   gen7_logging_data_buffer[1].struct_version  = ROT_ISO_OBJECT_STREAM_VERSION;

   gen7_logging_data_buffer[2].data_buff_ptr   = (uint8_t *)GetROTProcessedDetDataPtr();
   gen7_logging_data_buffer[2].total_size      = SIZE_OF_RESIM_ROT_PROCESSED_DETECTION_STREAM;
   gen7_logging_data_buffer[2].log_data_source = DC_ROT_PROCESSED_DETECTION_STREAM_NUMBER;
   gen7_logging_data_buffer[2].struct_version  = ROT_PROCESSED_DETECTION_STREAM_VERSION;

   gen7_logging_data_buffer[3].data_buff_ptr   = (uint8_t *)GetSFLDataPtr();
   gen7_logging_data_buffer[3].total_size      = SIZE_OF_RESIM_SFL_STREAM;
   gen7_logging_data_buffer[3].log_data_source = DC_SFL_STREAM_NUMBER;
   gen7_logging_data_buffer[3].struct_version  = SFL_STREAM_VERSION;

   gen7_logging_data_buffer[4].data_buff_ptr   = (uint8_t *)GetVSEDataPtr();
   gen7_logging_data_buffer[4].total_size      = SIZE_OF_RESIM_VSE_STREAM;
   gen7_logging_data_buffer[4].log_data_source = DC_VSE_STREAM_NUMBER;
   gen7_logging_data_buffer[4].struct_version  = VSE_STREAM_VERSION;

   gen7_logging_data_buffer[5].data_buff_ptr   = (uint8_t *)GetDspaceInputDataPtr();
   gen7_logging_data_buffer[5].total_size      = SIZE_OF_RESIM_GT_INPUT_STREAM;
   gen7_logging_data_buffer[5].log_data_source = DC_GT_INPUT_STREAM_NUMBER;
   gen7_logging_data_buffer[5].struct_version  = GT_INPUT_STREAM_VERSION;

   gen7_logging_data_buffer[6].data_buff_ptr   = (uint8_t *)Get_SG_Data_Ptr();
   gen7_logging_data_buffer[6].total_size      = SIZE_OF_RESIM_SG_STREAM;
   gen7_logging_data_buffer[6].log_data_source = DC_SG_STREAM_NUMBER;
   gen7_logging_data_buffer[6].struct_version  = SG_STREAM_VERSION;

   gen7_logging_data_buffer[7].data_buff_ptr   = (uint8_t *)GetDCDGPSDataptr();
   gen7_logging_data_buffer[7].total_size      = SIZE_OF_RESIM_DGPS_STREAM;
   gen7_logging_data_buffer[7].log_data_source = DGPS_STREAM_NUMBER;
   gen7_logging_data_buffer[7].struct_version  = DGPS_STREAM_VERSION;
}

void InitGen7MUXTables() {
   uint16_t no_of_chunks = 0;
   uint16_t j            = 0;
   for (uint8_t i = 0; i < GEN7_LOGGING_STREAM_COUNT; i++) {
      uint16_t temp_chunk_count = 0;
      if (gen7_logging_data_buffer[i].total_size > REC_PAYLOAD) {
         no_of_chunks = (gen7_logging_data_buffer[i].total_size % REC_PAYLOAD == 0) ? (gen7_logging_data_buffer[i].total_size / REC_PAYLOAD) : ((gen7_logging_data_buffer[i].total_size / REC_PAYLOAD) + 1);
      } else {
         no_of_chunks = 1;
      }

      for (; (j < GEN7_MAX_PACKETS_TxED) && (temp_chunk_count < no_of_chunks); j++) {
         gen7_mux_id_table[j].mux_id                = j;
         gen7_mux_id_table[j].logging_array_address = gen7_logging_data_buffer[i].data_buff_ptr + (REC_PAYLOAD * temp_chunk_count);
         gen7_mux_id_table[j].logging_source        = (uint8_t)gen7_logging_data_buffer[i].log_data_source;
         gen7_mux_id_table[j].logging_version       = gen7_logging_data_buffer[i].struct_version;
         gen7_mux_id_table[j].total_no_chunks       = no_of_chunks;
         gen7_mux_id_table[j].chunk_number          = temp_chunk_count;
         if (temp_chunk_count != (no_of_chunks - 1)) {
            gen7_mux_id_table[j].logging_array_size = REC_PAYLOAD;
         } else {
            gen7_mux_id_table[j].logging_array_size = (gen7_logging_data_buffer[i].total_size % REC_PAYLOAD == 0) ? REC_PAYLOAD : gen7_logging_data_buffer[i].total_size % REC_PAYLOAD;
         }
         temp_chunk_count++;
      }
   }
}

void PeriodicallySendStreams() {
   static int scan_index                 = 0;
   uint16_t chunk_id                     = 0;
   SIL_DC_Output_Data_T *output_data_ptr = GetOutputDataPtr();
   gen7_udp_frame_T *frame_ptr           = GetGen7FramePtr();
   // memset frame_ptr every cycle
   memset(frame_ptr, 0, sizeof(gen7_udp_frame_T));
   for (chunk_id = 0; chunk_id < GEN7_MAX_PACKETS_TxED; chunk_id++) {
      memcpy(frame_ptr->payload, gen7_mux_id_table[chunk_id].logging_array_address, gen7_mux_id_table[chunk_id].logging_array_size);
      if (gen7_mux_id_table[chunk_id].chunk_number == gen7_mux_id_table[chunk_id].total_no_chunks - 1) {
         if (gen7_mux_id_table[chunk_id].logging_array_size < REC_PAYLOAD) {
            memset(&frame_ptr->payload[gen7_mux_id_table[chunk_id].logging_array_size], 0, (REC_PAYLOAD - gen7_mux_id_table[chunk_id].logging_array_size));
         }
      }
      // fill frame header
#ifdef SRR_DC
      frame_ptr->frame_header.sensorId   = SENSOR_ID_SRR_DC; // gen7 orcas is not available
      frame_ptr->frame_header.sourceInfo = K_PLATFORM_SRR_DC;
#elif MRR_DC
      frame_ptr->frame_header.sensorId   = SENSOR_ID_MRR_DC; // gen7 orcas is not available
      frame_ptr->frame_header.sourceInfo = K_PLATFORM_MRR_DC;
#endif
      frame_ptr->frame_header.versionInfo    = RECORD_VERSIONINFO;
      frame_ptr->frame_header.sourceTxCnt    = scan_index++;
      frame_ptr->frame_header.streamNumber   = gen7_mux_id_table[chunk_id].logging_source;
      frame_ptr->frame_header.streamVersion  = gen7_mux_id_table[chunk_id].logging_version;
      frame_ptr->frame_header.streamChunkIdx = gen7_mux_id_table[chunk_id].chunk_number;
      frame_ptr->frame_header.streamChunks   = gen7_mux_id_table[chunk_id].total_no_chunks;
      frame_ptr->frame_header.streamDataLen  = gen7_mux_id_table[chunk_id].logging_array_size;
      frame_ptr->frame_header.streamRefIndex = GetScanIndex();
      frame_ptr->frame_header.streamTxCnt    = static_cast<uint8_t>(gen7_mux_id_table[chunk_id].chunk_number);
      // send udp frame to framework output tx buffer
      memcpy(&output_data_ptr->UDP_Tx_Buff[chunk_id], frame_ptr, sizeof(gen7_udp_frame_T));
   }
   output_data_ptr->UDP_Buff_Len      = chunk_id;
   output_data_ptr->f_dvsu_buff_valid = 1;
}

void TriggerStreamHandlerInit() {
   StreamHandlerInit();
   InitGen7MUXTables();
   return;
}

void TriggerStreamHandlerRun() {
   PeriodicallySendStreams();
   return;
}

gen7_udp_frame_T *GetGen7FramePtr() {
   return &gen7_frame;
}