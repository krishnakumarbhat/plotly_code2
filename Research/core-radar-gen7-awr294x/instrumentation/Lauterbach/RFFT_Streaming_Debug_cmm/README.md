Following scripts are applicable for RFFT Streaming mode verification

# Build option used:
1. ./software/app/dss/src/range_proc:disable_lauterbach_test_adc_output
    Disables ADC tracing via Lauterbach MDO lines

2. ./software/app/dss/src/range_proc:enable_dss_rfft_streaming
    Enables RFFT Streaming mode for sending RFFT compressed data onto communication bus.

# Scripts:
1. Read_rfft_streaming_compressed_data.cmm
   -- Used for collecting rfft compressed output for all chirps.
      Build option: RFFT Streaming enabled,  ADC Offine mode enabled and RFFT_STREAMING_DEBUG enabled.
2. Read_rfft_compressed_enet_dump.cmm
   -- Used for collecting rfft enet frame data for all the chirps. This will be used for Matlab verification by Radar System.
      Build option: RFFT Streaming enabled, ADC Offine mode enabled and RFFT_STREAMING_DEBUG disabled.
3. Read_rfft_streaming_enet_packet_data.cmm
   -- For collecting rfft enr frame data for all chirps.
      Build option: RFFT Streaming enabled,  ADC Offine mode enabled and RFFT_STREAMING_DEBUG disabled.
4. Read_rfft_compressed_transpose_data.cmm
   -- For collecting rfft output data in Normal mode(RFFT-DFFT-RDD-AF).
      Build option: RFFT Streaming disbaled and ADC Offine mode enabled.
5. Read_xcp_rfft_compressed_rbin_data.cmm
   -- For collecting rfft output data for single selected Rangebin(XCP_range_idx) all chirp data in RFFT streaming mode from instrumentation buffer.
      Build option: RFFT Streaming enabled
6. Read_rfft_compressed_rbin_data.cmm
   -- For collecting rfft output data for single selected Rangebin(XCP_range_idx) all chirp data in RFFT streaming mode from RFFT output buffer.
      Build option: RFFT Streaming enabled

File extracted from #2, #3 and #4 can be beyond compared. This should match bit exact.
File extracted from #5 and #6 can be beyond compared. This should match bit exact.

# Note:
RFFT_STREAMING_DEBUG can be enabled/disabled in main_application.h
ADC Offine mode can be enabled by making the global variable Offline_Mode_Flag as TRUE.
Also allocate enet maximum buffer to hold all 512 chirps in rfft_stream_enet_definitions.h
     /* To hold entire 512 chirp data*/
     #define NUM_ENET_RFFT_TX_BUFFERS (750U)
