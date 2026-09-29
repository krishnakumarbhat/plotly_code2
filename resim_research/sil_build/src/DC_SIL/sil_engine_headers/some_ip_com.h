#ifndef _SOME_IP_COM_H_
#define _SOME_IP_COM_H_

#include "fixmac.h"

typedef unsigned char uint8;
typedef unsigned char UINT8;
typedef float float32;
typedef float32 FLOAT32;
typedef unsigned int UINT32;
typedef uint8 CommonEventDataQualifier;
typedef uint8 Std_ReturnType;
typedef Std_ReturnType Rte_TransformerErrorCode;
typedef uint8 Rte_TransformerClass;
#ifndef uint16
typedef unsigned short uint16;
#endif
#ifndef boolean
typedef unsigned char boolean;
#endif
/*******************************************************************************************************************************************************************
*******************************************************************************************************************************************************************
CTA DATA ON SOME IP
*******************************************************************************************************************************************************************
*******************************************************************************************************************************************************************/
typedef uint8 CounterRequestCTB;
typedef uint8 DisplayWarningGraphicCTB;
typedef uint8 Qualifier_Function_CTB;
typedef uint8 ReqExtMirrorDispWarningCTB;
typedef uint8 ReqExtMirrorDispWarningCTB;
typedef struct
{
   CounterRequestCTB counterRequestCTB;
   DisplayWarningGraphicCTB displayWarningGraphicCTB;
   Qualifier_Function_CTB qualifierFunctionCTB;
   ReqExtMirrorDispWarningCTB reqExtMirrorDispWarningLeftHandCTB;
   ReqExtMirrorDispWarningCTB reqExtMirrorDispWarningRightHandCTB;
} RequestCTBWrapper;
extern RequestCTBWrapper Appl_BCP21_GW_RequestCTBWrapper_Setter;

#define Rte_CS_ClientConfigIndex_Aptiv_Client_SWC_CSI_RequestCTB_SetterReturnrequestCTB_FieldSetterrequestCTB 0U

typedef struct
{
   Rte_TransformerErrorCode errorCode;
   Rte_TransformerClass transformerClass;
} Rte_TransformerError;

typedef struct
{
   boolean Rte_CallCompleted;
   RequestCTBWrapper Setter;
   Std_ReturnType Rte_Result;
   uint16 Rte_SequenceCounter;
   Rte_TransformerError Rte_TransformationResult;
} Rte_CS_ClientQueueType_Rte_RequestCTB_1_requestCTB_SETTER_CALL_9I0U3Y_f6e85cb7_Tx;

extern Rte_CS_ClientQueueType_Rte_RequestCTB_1_requestCTB_SETTER_CALL_9I0U3Y_f6e85cb7_Tx Rte_CS_ClientQueue_Aptiv_Client_SWC_CSI_RequestCTB_SetterReturnrequestCTB_FieldSetterrequestCTB;
typedef struct
{
   uint16 clientId;
   uint16 sequenceCounter;
} Rte_Cs_TransactionHandleType;
#endif /*_SOME_IP_COM_H_*/
       /*=======================================================================================================*\
       * File Revision History (top to bottom: first revision to last revision)
       *=======================================================================================================
       
       * 08-JAN-2020	qj4jv7		DET-110 Split the RECU RESIM SOMIP current header file into input and output header files
       * 8-Jun-2020	qj4jv7		DET- 198 RECU - No CTB output in MID/HIGH resim.
       
       *\*========================================================================================================*/