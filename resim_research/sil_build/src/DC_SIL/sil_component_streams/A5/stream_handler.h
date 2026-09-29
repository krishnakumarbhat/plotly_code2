#ifndef STREAM_HANDLER_H
#define STREAM_HANDLER_H

#include "fixmac.h"

#define REC_VER (0xA5)

#if (0xA5 == REC_VER)
typedef struct Rec_Hdr_Tag {
   /* Application Layer Info */
   uint16_t versionInfo;  /* Version of this header specification [set to RECORD_VERSIONINFO] */
   uint16_t sourceTxCnt;  /* Incremented on every transmission from this source.  */
   uint32_t sourceTxTime; /* Timestamp (ms) when packet queued for transmit. */
   uint8_t sourceInfo;    /* Definition not available yet */
   uint8_t sourceInfo1;
   uint8_t sourceInfo2;
   uint8_t sourceInfo3;

   /* Process Layer Info */
   uint32_t streamRefIndex;      /* Stream-specific index used for data retrieval (e.g. Look Index for radar) */
   uint16_t streamDataLen;       /* Number of bytes in the current message�s payload, not including the record header */
   uint8_t streamTxCnt;          /* Increments on each transmission of associated stream number. */
   uint8_t streamNumber;         /* Stream number [0:FF]. One source may transmit multiple, asynchronous streams. */
   uint8_t streamVersion;        /* Stream data structure format/version (how to interpret data). */
   uint8_t streamChunksPerCycle; /* Stream chunks per cycle, applicable for static streams e.g. calibration */
   uint16_t streamChunks;        /* A Stream may be transmitted from the process layer as M equal-size
                                     chunks that can be reconstructed on the receive side.
                                     Set to 0 if transmission is not part of a series of chunks (normal mode).
                                     Set to M (>=2) to indicate stream is sent as a series of M chunks.
                                     NOTE: streamTxCnt must increment for each chunk while streamRefIndex
                                     would be the same for each chunk of a series. */
   uint16_t streamChunkIdx;      /* Zero when streamChunks is zero. Otherwise, represents this chunk's
                                     position in the reconstruction queue [0:(streamChunks-1)]. */
   uint8_t reserved;             /* Set to zero */
   uint8_t sensorId;             /* Sensor Position ID */
} Rec_Hdr_T;
#endif
#define ETH_PAYLOAD ((uint16_t)1500U)
#define IP_V4_LEN   ((uint16_t)20U)
#define UDP_HDR_LEN ((uint16_t)8U)
#define REC_HDR_LEN ((uint16_t)(sizeof(Rec_Hdr_T)))

/* UDP is used now, may be TCP/IP later */
#define PROTOCOL_HDR_LEN (UDP_HDR_LEN)
#define IP_PKT_HDR_LEN   ((uint16_t)(IP_V4_LEN + PROTOCOL_HDR_LEN))

/* Data payload is same as udp payload i.e. 1472U */
#define ETH_DATA_PAYLOAD ((uint16_t)(ETH_PAYLOAD - IP_PKT_HDR_LEN))
#define REC_PAYLOAD      ((uint16_t)(ETH_DATA_PAYLOAD - REC_HDR_LEN))

#define RECORD_VERSIONINFO (uint16_t)((REC_VER << 8U) | REC_HDR_LEN)
#define K_PLATFORM_SRR_DC  ((uint8_t)128U)
#define K_PLATFORM_MRR_DC  ((uint8_t)129U)
#define K_PLATFORM_FL      ((uint8_t)70U)
#define K_PLATFORM_FR      ((uint8_t)71U)
#define K_PLATFORM_RL      ((uint8_t)72U)
#define K_PLATFORM_RR      ((uint8_t)73U)

#define SENSOR_ID_SRR_DC ((uint8_t)128)
#define SENSOR_ID_MRR_DC ((uint8_t)129)
#define SENSOR_ID_RL     ((uint8_t)1)
#define SENSOR_ID_RR     ((uint8_t)2)
#define SENSOR_ID_FL     ((uint8_t)3)
#define SENSOR_ID_FR     ((uint8_t)4)
typedef struct gen7_data_buffer_Tag {
   uint8_t *data_buff_ptr;
   uint32_t total_size;
   uint8_t struct_version;
   uint8_t log_data_source;
} gen7_logging_data_buffer_T;

/* Structure defining the mux elements used to pack data into logging table */
typedef struct gen7_mux_id_table_logging_Tag {
   uint16_t mux_id;                /* Mux table id*/
   uint16_t logging_array_size;    /* Size of the data that will go into the packet */
   uint8_t frame_diagostic;        /* Defines if the packet is full/not full/empty */
   uint8_t logging_source;         /* Identifies the core from which data is logged */
   uint16_t total_no_chunks;       /* Total chunks needed for logging data from a  core */
   uint16_t chunk_number;          /* Current chunk no.*/
   uint8_t logging_version;        /* Version of the data logged from a core */
   uint8_t *logging_array_address; /* Address from which data is fetched */
} gen7_mux_id_table_logging_T;

typedef struct gen7_udp_frame_Tag {
   Rec_Hdr_T frame_header;
   uint8_t payload[REC_PAYLOAD];
} gen7_udp_frame_T;

void TriggerStreamHandlerInit();
void TriggerStreamHandlerRun();

#endif // STREAM_HANDLER_H