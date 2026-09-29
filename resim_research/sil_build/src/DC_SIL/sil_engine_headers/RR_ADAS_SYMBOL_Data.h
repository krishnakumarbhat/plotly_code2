#ifndef _RR_ADAS_SYMBOL_H_
#define _RR_ADAS_SYMBOL_H_

#include <cinttypes>

#define MAX_STRM_COUNT 40
#define MAX_STRM_LEN   12000

#pragma pack(push, save_pack)
#pragma pack(push, 2)
typedef struct UDPRecord_Header_Symbol_TAG {
   /*!
    *  Application Layer Info
    */
   uint16_t versionInfo;   //!< Version of this header specification [set to UDP_RECORD_VERSIONINFO]
   uint16_t sourceTxCnt;   //!< Incremented on every transmission from this source.
   uint32_t sourceTxTime;  //!< Timestamp (ms) when packet queued for transmit.
   uint8_t Platform;       /*  Constant identifying sending application [0:63], see udp_sources.h.  SRR3 21-24/SRR5+ >40 */
   uint8_t Radar_Position; /*  RL :71 / RR :72 / FR :73 / FL  :74 / RC :75 /  LC :76 */
   uint8_t reservedSrc2;   //!< (Set to zero)
   uint8_t reservedSrc3;   //!< (Set to zero)

   /*!
    *  Process Layer Info
    */
   uint32_t streamRefIndex; //!< Stream-specific index used for data retrieval (e.g. Look Index for radar)
   uint32_t streamDataLen;  //!< Stream data payload size (bytes).  Size for each stream must be constant.
   uint8_t streamTxCnt;     //!< Increments on each transmission of associated stream number.
   uint8_t streamNumber;    //!< Stream number [0:31]. One source may transmit multiple, asynchronous streams.
   uint8_t streamVersion;   //!< Stream data structure format/version (how to interpret data).
   uint8_t streamChunks;    /*!< A Stream may be transmitted from the process layer as M equal-size
                            chunks that can be reconstructed on the receive side.
                            Set to 0 if transmission is not part of a series of chunks (normal mode).
                            Set to M (>=2) to indicate stream is sent as a series of M chunks.
                            NOTE: streamTxCnt must increment for each chunk while streamRefIndex
                            would be the same for each chunk of a series. */
   uint8_t streamChunkIdx;  /*!< Zero when streamChunks is zero. Otherwise, represents this chunk's
                            position in the reconstruction queue [0:(streamChunks-1)]. */
   uint8_t customerID;      //!< used as enum Customer_T in newer implementation (uint8_t  reservedStr3 & Set to zero in older implementation)
} UDPRecord_Header_Symbol;

/* Sample layout of UDP Logging transmission.
 *    This structure is not necessarily use in any source code.
 *    The payload source, size and format is described by the header.
 */
typedef struct UDPRecord_Symbol {
   UDPRecord_Header_Symbol header; //!< Record header
   void *payload;                  //!< Data payload
} UDPRecord_Symbol_T;

typedef struct RR_ADAS_SYMBOL_Data_Tag {
   UDPRecord_Symbol_T raw_stream[MAX_STRM_COUNT]; // Srr3 Internal Data
   /*.............*/
} RR_ADAS_SYMBOL_Data_T;
#pragma pack(pop, save_pack)
#endif //_RR_ADAS_SYMBOL_H_
