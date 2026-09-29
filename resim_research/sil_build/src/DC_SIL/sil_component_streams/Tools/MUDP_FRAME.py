from dissect import cstruct

# create 3 Signal objects
MUDP_FRAME = cstruct.cstruct()

MUDP_FRAME.load(""" 

    #define MUDP_FRAME_PAYLOAD_SIZE 1444
    #define MUDP_FRAME_CRC_SIZE 4

    typedef struct FRAME_HEADER_Tag
    {
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
       uint16_t streamDataLen;       /* Number of bytes in the current message’s payload, not including the record header */
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
    }FRAME_HEADER_T; // 28 bytes
    
    typedef struct UDP_FRAME_Tag
    {
        FRAME_HEADER_T mudp_frame_header;
        uint8_t payload[MUDP_FRAME_PAYLOAD_SIZE];
    }UDP_FRAME_T;
""")
