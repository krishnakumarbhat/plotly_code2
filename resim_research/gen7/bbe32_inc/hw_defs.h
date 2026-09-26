/**************************************************************************************************
 *
 * NXP Confidential Proprietary
 *
 * Copyright 2020-2023 NXP
 * All Rights Reserved
 *
 *****************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 **************************************************************************************************/

#ifndef HW_DEFS_H
#define HW_DEFS_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "typedefs.h"
#include <xtensa/tie/radar24.h>

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
#define ADDR_DSP_ERR_INFO_REG    (0x9C)
#define ADDR_DSP_ERR_INFO_INT_EN (0xA0)

#define ADDR_DSP_DEBUG1_REG (0xA8)
#define ADDR_DSP_DEBUG2_REG (0xAC)
#define ADDR_DSP_DEBUG3_REG (0xB0)
#define ADDR_DSP_DEBUG4_REG (0xB4)
#define ADDR_DSP_DEBUG5_REG (0xB8)
#define ADDR_DSP_DEBUG6_REG (0xBC)

#define ADDR_DSP_WR_R0_RE (0x198)
#define ADDR_DSP_WR_R0_IM (0x19C)

   /*==================================================================================================
    *                                      BBE MACROS
    *==================================================================================================*/
   extern uint64_t __BBE32_DTCM_START_ADDR;
   extern uint64_t __BBE32_ITCM_START_ADDR;
   extern uint64_t __BBE32_ITCM_END_ADDR;

#define BBE_DRAM_BASE_ADDR ((const uint32_t)(&(__BBE32_DTCM_START_ADDR)))
#define BBE_IRAM_BASE_ADDR ((const uint32_t)(&(__BBE32_ITCM_START_ADDR)))
#define BBE_END_MEM_SPACE  ((const uint32_t)(&(__BBE32_ITCM_END_ADDR)))
#define SPT_BASE_ADDR      (0x440A0000U)
#define SPT_END_ADDR       (0x440A1000U)
#define CRC_BASE_ADDR      (IP_CRC_0_BASE)
#define CRC_END_ADDR       (0x4012AFFFU + 1U)

   /*==================================================================================================
    *                                      SRAM MACROS
    *==================================================================================================*/
   extern uint64_t __APP_SRAM_DSP_START_ADDR;
   extern uint64_t __APP_SRAM_DSP_END_ADDR;
   extern uint64_t __REALTIME_SRAM_IPC_D2M_START_ADDR;
   extern uint64_t __REALTIME_SRAM_IPC_D2M_END_ADDR;
   extern uint64_t __RETENTION_RAM_START_ADDR;

#define APP_SRAM_DSP_START_ADDR          ((const uint32_t)(&(__APP_SRAM_DSP_START_ADDR)))
#define APP_SRAM_DSP_END_ADDR            ((const uint32_t)(&(__APP_SRAM_DSP_END_ADDR)))
#define REALTIME_SRAM_IPC_D2M_START_ADDR ((const uint32_t)(&(__REALTIME_SRAM_IPC_D2M_START_ADDR)))
#define REALTIME_SRAM_IPC_D2M_END_ADDR   ((const uint32_t)(&(__REALTIME_SRAM_IPC_D2M_END_ADDR)))

#define RETENTION_SRAM_BASE_ADDR ((const uint32_t)(&(__RETENTION_RAM_START_ADDR)))

/*==================================================================================================
 *                                      MSCM MACROS
 *==================================================================================================*/
#define MSCM_BASE_ADDR (0x40010000U)
#define MSCM_IRCP0ISR3 (MSCM_BASE_ADDR + 0x218U) // Interrupt targeting CPU0 from CPU3
#define MSCM_IRCP0IGR3 (MSCM_BASE_ADDR + 0x21CU) // Interrupt generation to the M7 core (CPU3 to CPU0)
#define MSCM_IRCP2ISR3 (MSCM_BASE_ADDR + 0x258U) // Interrupt targeting CPU2 from CPU3
#define MSCM_IRCP2IGR3 (MSCM_BASE_ADDR + 0x25CU) // Interrupt generation to the A53 core (CPU3 to CPU2)
#define MSCM_END_ADDR  (0x40011000U)

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                          DEFINES
==================================================================================================*/
/**
 *  \brief   This macro writes a 32-bit value to a hardware register.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *  \param   value   unsigned 32-bit value which has to be written to the
 *                   register.
 */
#define HW_WR_REG32(addr, value) HW_WR_REG32_RAW((uint32_t)(addr), (uint32_t)(value))

   /*==================================================================================================
   *                                    FUNCTION PROTOTYPES
   ==================================================================================================*/
   /**
    *  \brief   This function writes a 32-bit value to a hardware register.
    *
    *  \param   addr    Address of the memory mapped hardware register.
    *  \param   value   unsigned 32-bit value which has to be written to the
    *                   register.
    */
   static inline void HW_WR_REG32_RAW(uint32_t addr, uint32_t value);

   /*==================================================================================================
   *                                    FUNCTION DEFINITION
   ==================================================================================================*/
   static inline void Set_Dsp_Error(status_t err_info)
   {
      BBX_SCRLU((int32_t)err_info,
                ADDR_DSP_ERR_INFO_REG); // writing to DSP_ERR_INFO_REG triggeres an SPT "DSP" interrupt to the application core
   }

   static inline void HW_WR_REG32_RAW(uint32_t addr, uint32_t value)
   {
      *(volatile uint32_t *)addr = value;
      return;
   }

#ifdef __cplusplus
}
#endif

#endif /*HW_DEFS_H*/
