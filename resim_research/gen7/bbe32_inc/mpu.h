#ifndef MPU_H
#define MPU_H
/*===========================================================================*/
/**
 * @file mpu.h
 *
 * This file contains interfaces mpu configuration
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2023 Aptiv. All rights reserved.
 * Aptiv Sensitive Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 * This file contains interfaces mpu configuration
 *
 * @section ABBR ABBREVIATIONS:
 *   - @todo List any abbreviations, precede each with a dash ('-').
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - @todo Update list of design document(s).
 *
 * Add Polarion Work Item Link to the intended line (if using Resource Link
 * for traceability)
 * Syntax:
 * @wi.<LinkRoleAsSingleWord> <PolarionProjectID>/<workitemID>
 * example:
 * @wi.implemented PDP2.0_playground/WI-5001
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
 *
 * @defgroup template Provide API description and define/delete next line
 * @ingroup <parent_API> (OPTIONAL USE if part of another API, else delete)
 * @{
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "status.h"

#ifdef __cplusplus
extern "C"
{
#endif

   /*==================================================================================================
   *                                          CONSTANTS
   ==================================================================================================*/

   /*==================================================================================================
   *                                      DEFINES AND MACROS
   ==================================================================================================*/

   /*==================================================================================================
   *                                             ENUMS
   ==================================================================================================*/

   /*==================================================================================================
   *                                STRUCTURES AND OTHER TYPEDEFS
   ==================================================================================================*/
   /**
    * @brief Defines the memory access rights attributes of an MPU entry.
    * @details See Xtensa ISA Reference Manual section 4.6.5.7 "Memory Protection Unit Option Access Rights Field"
    */
   typedef enum
   {
      DSP_MEM_AR_NONE   = 0u,  /* no access */
      DSP_MEM_AR_R      = 4u,  /* Kernel read, User no access*/
      DSP_MEM_AR_RX     = 5u,  /* Kernel read/execute, User no access */
      DSP_MEM_AR_RW     = 6u,  /* Kernel read/write, User no access */
      DSP_MEM_AR_RWX    = 7u,  /* Kernel read/write/execute, User no access */
      DSP_MEM_AR_Ww     = 8u,  /* Kernel write, User write */
      DSP_MEM_AR_RWrwx  = 9u,  /* Kernel read/write , User read/write/execute */
      DSP_MEM_AR_RWr    = 10u, /* Kernel read/write, User read */
      DSP_MEM_AR_RWXrx  = 11u, /* Kernel read/write/execute, User read/execute */
      DSP_MEM_AR_Rr     = 12u, /* Kernel read, User read */
      DSP_MEM_AR_RXrx   = 13u, /* Kernel read/execute, User read/execute */
      DSP_MEM_AR_RWrw   = 14u, /* Kernel read/write, User read/write */
      DSP_MEM_AR_RWXrwx = 15u  /* Kernel read/write/execute,*/
   } dspMemAccRights_t;

   /**
    * @brief Specifies the cache attributes of an MPU entry.
    * @details See Xtensa ISA Reference Manual section 4.6.5.8 "Memory Protection Unit Option Memory Type Field"
    */
   typedef enum
   {
      DSP_MEM_DEVICE               = 0x00008000u,
      DSP_MEM_NON_CACHEABLE        = 0x00090000u,
      DSP_MEM_WRITETHRU_NOALLOC    = 0x00080000u,
      DSP_MEM_WRITETHRU            = 0x00040000u,
      DSP_MEM_WRITETHRU_WRITEALLOC = 0x00060000u,
      DSP_MEM_WRITEBACK_NOALLOC    = 0x00050000u,
      DSP_MEM_WRITEBACK            = 0x00070000u
   } dspMemCacheAttr_t;

   /**
    * @brief Defines the scope of the sharing of the memory region.
    * @details See Xtensa ISA Reference Manual section 4.6.5.8 "Memory Protection Unit Option Memory Type Field"
    */
   typedef enum
   {
      DSP_MEM_NON_SHAREABLE    = 0x00000000u, /**< Only applicable to devices and non-cacheable regions. */
      DSP_MEM_INNER_SHAREABLE  = 0x02000000u, /**< Only applicable to cacheable regions */
      DSP_MEM_OUTER_SHAREABLE  = 0x04000000u, /**< Only applicable to cacheable regions */
      DSP_MEM_SYSTEM_SHAREABLE = 0x06000000u  /**< Only applicable to devices and non-cacheable regions.  */
   } dspMemShareAttr_t;

   /**
    * @brief Indicates a if a device read is interruptible. Only applicable to #DSP_MEM_DEVICE type memory
    * @details See Xtensa ISA Reference Manual section 4.6.5.8 "Memory Protection Unit Option Memory Type Field"
    */
   typedef enum
   {
      DSP_MEM_NON_INTERRUPTIBLE = 0x00000000u,
      DSP_MEM_INTERRUPTIBLE     = 0x08000000u /**< Interruptible access: value is discarded if load is interrupted, then load is
                                                 repeated.     Can be speculated. Ensures minimum interrupt latency*/
   } dspMemIntAttr_t;

   /**
    * @brief Indicates if writes to this memory are bufferable. Only applicable to devices, and non-cacheable memory.
    */
   typedef enum
   {
      DSP_MEM_NON_BUFFERABLE = 0x00000000u,
      DSP_MEM_BUFFERABLE     = 0x01000000u /**< only for device + non-cacheable */
   } dspMemBuffAttr_t;

   /**
    * @brief Defines the memory type attributes of an MPU entry.
    * @details See Xtensa ISA Reference Manual section 4.6.5.8 "Memory Protection Unit Option Memory Type Field"
    */
   typedef struct
   {
      dspMemCacheAttr_t cacheAttr; /**<  */
      dspMemShareAttr_t shareAttr; /**<  */
      dspMemIntAttr_t intAttr;     /**<  */
      dspMemBuffAttr_t buffAttr;   /**<  */
   } dspMemType_t;

   /**
    * @brief This structure is used to define the attributes of the foreground MPU memory regions
    * @details See Xtensa ISA Reference Manual section 4.6.5.3 "The Structure of the Memory Protection Unit Option TLB"
    */
   typedef struct
   {
      uint32_t startAddr;             /**< Start address of the memory region */
      dspMemAccRights_t accessRights; /**< Read/write/execute access rights for user and privileged (kernel) code */
      dspMemType_t memType; /**< Memory type attributes: whether it is cached, shared, buffered, or mapped to an external device */
   } dspMpuSeg_t;

   /*==================================================================================================
   *                                GLOBAL VARIABLE DECLARATIONS
   ==================================================================================================*/

   /*==================================================================================================
   *                                    FUNCTION PROTOTYPES
   ==================================================================================================*/
   status_t Mpu_Config(void);

#ifdef __cplusplus
}
#endif

#endif /*MPU_H*/
