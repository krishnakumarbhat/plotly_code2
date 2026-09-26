#ifndef MEM_POOL_H
#define MEM_POOL_H
/**
 * @file mem_pool.h
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2023 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 * Inline memory pool manager. Allows use of a pseudo-dynamic memory pool, based
 * upon a static memory allocation.
 *
 * Implementation simulates use of the static memory as a "stack", i.e. memory
 * allocations are made sequentially. Currently no provision for a "free" method
 * is made, the intent is for the buffer allocations within a given memory pool
 * to be used, then all completely freed (using the Mem_Pool_Reset method).
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

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>

/* ========================================================================== */
/*                                 MACROS                                     */
/* ========================================================================== */
#ifndef MEM_POOL_ALIGNMENT_IN_BYTES
   #define MEM_POOL_ALIGNMENT_IN_BYTES (32U)
#elif (MEM_POOL_ALIGNMENT_IN_BYTES == 0U) || ((MEM_POOL_ALIGNMENT_IN_BYTES & (MEM_POOL_ALIGNMENT_IN_BYTES - 1U)) != 0U)
   #error "MEM_POOL_ALIGNMENT_IN_BYTES must be a power of 2."
#endif

/**
 * @brief Determine amount of memory used for requested allocation size.
 *
 *        For example, for default @ref MEM_POOL_ALIGNMENT_IN_BYTES, a requested allocation
 *          of 1023 bytes will consume 1024 bytes of the memory pool.
 *
 */
#define MEM_POOL_USAGE(x) ((x + (MEM_POOL_ALIGNMENT_IN_BYTES - 1U)) & ~(MEM_POOL_ALIGNMENT_IN_BYTES - 1U))

/**
 * @brief Enum type for memory pool return values.
 */
typedef enum
{
   MEM_POOL_SUCCESS = 0u, /* Successful */
   MEM_POOL_FAILURE = 1u, /* Memory pool function failure */
} Memory_Pool_Return_T;

/**
 * @brief Typedef for the memory pool manager.
 */

typedef struct Mem_Pool_Tag
{
   uint8_t *base_addr;
   uint32_t size;
   uint32_t max_size;
} Mem_Pool_T;

/**
 *  @b Description
 *  @n
 *      Initialization of the memory pool structure.
 *      Sets the base address and max size and resets the size to 0.
 *
 *  @retval
 *      Returns MEM_POOL_SUCCESS for success, and MEM_POOL_FAILURE if the memory
 *      alignment of the input buffer does not match @ref MEM_POOL_ALIGNMENT_IN_BYTES.
 */
static inline Memory_Pool_Return_T Mem_Pool_Init(Mem_Pool_T *mem_pool, void *base_addr, uint32_t max_size)
{
   Memory_Pool_Return_T retVal = MEM_POOL_SUCCESS;

   if (0 == ((uintptr_t)base_addr & (MEM_POOL_ALIGNMENT_IN_BYTES - 1)))
   {
      mem_pool->base_addr = (uint8_t *)base_addr;
      mem_pool->max_size  = max_size;
      mem_pool->size      = 0;
   }
   else
   {
      mem_pool->base_addr = 0;
      mem_pool->max_size  = 0;
      mem_pool->size      = 0;

      retVal = MEM_POOL_FAILURE;
   }

   return retVal;
}

/**
 *  @b Description
 *  @n
 *      Reset the memory pool to have a size of 0.
 *
 */
static inline void Mem_Pool_Reset(Mem_Pool_T *mem_pool)
{
   mem_pool->size = 0;
}

/**
 *  @b Description
 *  @n
 *      Checks the memory pool to ensure the number of bytes to allocate is available.
 *      If so, updates the current size of the memory pool and returns a pointer to the
 *      address of the buffer.
 *  @param mem_pool Pointer to the memory pool structure.
 *  @param size     Size of buffer to allocate in the memory pool.
 *  @param addr     Pointer to a pointer for the address of the allocation. If the allocation
 *                  fails, the address for the allocation is updated to NULL, on success it is
 *                  updated to the base address of the allocation.
 *
 *  @retval
 *      Returns MEM_POOL_FAILURE if the memory pool does not have enough space for the buffer,
 *      otherwise returns MEM_POOL_SUCCESS.
 */
static inline Memory_Pool_Return_T Mem_Pool_Alloc(Mem_Pool_T *mem_pool, uint32_t size, void **addr)
{
   uint32_t newSize            = size + mem_pool->size;
   Memory_Pool_Return_T retVal = MEM_POOL_SUCCESS;

   if (newSize <= mem_pool->max_size)
   {
      *addr = (void *)(mem_pool->base_addr + mem_pool->size);

      /* Set the new memory pool size to be aligned to MEM_POOL_ALIGNMENT_IN_BYTES bytes. */
      mem_pool->size = (uint32_t)MEM_POOL_USAGE(newSize);
   }
   else
   {
      retVal = MEM_POOL_FAILURE;
      *addr  = (void *)NULL;
   }

   return retVal;
}

#endif
