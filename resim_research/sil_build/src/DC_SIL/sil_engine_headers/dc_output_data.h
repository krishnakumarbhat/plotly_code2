#ifndef OUTPUT_DATA_H
#define OUTPUT_DATA_H

#include <cinttypes>
#include "tracker_out_iface.h"
#include "dvl_message.h"
#include "RR_ADAS_SYMBOL_Data.h"

#define OUTPUT_DATA_STRUCTURE_VERSION    (9)
#define DC_OUTPUT_DATA_STRUCTURE_VERSION (12)
#define MAX_CAN_MESSAGES                 500

#pragma pack(push, save_pack)
#pragma pack(push, 2)

/* Header for the Output Data Structure */
typedef struct DC_Output_Data_Hdr {
   uint32_t Sec_Size;
   uint16_t Sec_Compatibility;
   uint16_t Sec_Checksum;
   uint16_t sym_version; // symbol interface version number
   uint32_t sym_Size;    // size of the SIL_Data_Output structure
} DC_Output_Data_Hdr_T;

// Structure to hold SIL error status from emb library: Sensors &ECU: Enums are defined in the file "apt_sil_error_enum.h"
typedef struct emb_sil_status_tag {
   uint32_t ver;
   uint8_t silErrStatus[150];
} emb_sil_status_t;

typedef struct ENET_BUFF {
   uint8_t Buf_Bytes[1500];
} ENET_BUFF_T;

typedef struct SOMEIP_BUFF {
   uint64_t PLPTimeStamp_ns; // plp timestamp to given in nano secs
   uint64_t srcMACaddress;   // Source MAC Addr decoded from the field ETH_Frame.Source of input mf4 file
   uint64_t destMACaddress;  // Destination MAC Addr decoded from the field ETH_Frame.Destination of input mf4 file
   uint16_t Ether_Type;      // Currently Ether type is set to Internet Protocol version 4 (IPv4/0x800)
   uint16_t vLAN_ID;         // This field can be filled only when theEthertype is set to VLAN_TAG
   uint32_t data_length;     // Total number of Buf_Bytes (IPv4 length field to be given)
   uint8_t Buf_Bytes[1500];
} SOMEIP_BUFF_T;

typedef struct DVL_PAYLOAD {
   dvlMessageUnion dvl[MAX_CAN_MESSAGES];
   uint32_t msg_count;
} DVL_PAYLOAD_T;

/* patch fix for someip && dft instead of adding headers */
typedef struct Some_IP_Signal_Symbol_Tag {
   uint8_t buff[49820];
} Some_IP_Signal_Symbol_t;

typedef struct Dft_Interface_Tag {
   uint8_t buff[39318];
} Dft_Interface_t;

/* Output Data Structure Declaration */
typedef struct SIL_DC_Output_Data_Tag {
   DC_Output_Data_Hdr_T Output_Data_Header;
   /* DVSU Payload*/
   ENET_BUFF_T UDP_Tx_Buff[1000];
   uint32_t UDP_Buff_Len;
   unsigned char f_dvsu_buff_valid;
   /*SOMEIP Payload*/
   SOMEIP_BUFF_T SOMEIP_Tx_Buff[120];
   unsigned char SOMEIP_Buff_Len;
   unsigned char SOMEIP_f_buff_valid;
   /* DVL Payload */
   DVL_PAYLOAD_T dvl_payload_out;
   Some_IP_Signal_Symbol_t SomeIp_Signals_iface;
   /* DFT Structures */
   Dft_Interface_t Dft_Signal_iface;
   /* MUDP Record */
   RR_ADAS_SYMBOL_Data_T Symbol_Record;
   /* Embedded sil process status */
   emb_sil_status_t silProcessStatus;
   /*............*/
   TRACKER_OUTPUT_Tag_Values_T tracker_output;
} SIL_DC_Output_Data_T;
#pragma pack(pop, save_pack)
#endif // OUTPUT_DATA_H
