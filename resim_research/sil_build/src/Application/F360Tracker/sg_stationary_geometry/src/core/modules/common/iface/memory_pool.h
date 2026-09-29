/*=============================================================================================*\
* FILE: memory_pool.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of MemoryPool class
* It provides fixed-size stack allocated memory block and methods for managing available space.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <array>

#include "fifo.h"

template <typename T, size_t pool_capacity>
class MemoryPool
{
  public:
   MemoryPool();
   static MemoryPool &static_instance();

   T *reserve_free_slot(void);
   void release_slot(T *slot_to_release);
   bool full() const;
   size_t capacity() const;
   size_t num_empty_slots() const;

  private:
   std::array<T, pool_capacity> m_memory;
   Fifo<T *, pool_capacity> m_available_slots;
};

/*=============================================================================================*\
 * Method        MemoryPool<T, pool_capacity>::MemoryPool()
 *
 * Description   Constructor of MemoryPool class. Initializes m_available_slots with pointers to
 *               m_memory array slots
 *
 * Parameters    None
 *
 * Returns       None
 *=============================================================================================*/
template <typename T, size_t pool_capacity>
MemoryPool<T, pool_capacity>::MemoryPool()
{
   static_assert(pool_capacity != 0, "Memory Pool cannot be empty");

   for (auto &free_slot : m_memory)
   {
      m_available_slots.push(&free_slot);
   }
}

/*=============================================================================================*\
 * Method        MemoryPool<T, pool_capacity>::static_instance()
 *
 * Description   Returns static instance of MemoryPool object. MemoryPool constructor is executed
 *               the first time this function is entered, but memory is already allocated at program
 *               start. MemoryPools with different element type or pool capacity will have separate
 *               static instances.
 *
 * Parameters    None
 *
 * Returns       MemoryPool<T, pool_capacity>&
 *=============================================================================================*/
template <typename T, size_t pool_capacity>
MemoryPool<T, pool_capacity> &MemoryPool<T, pool_capacity>::static_instance()
{
   static MemoryPool static_memory_pool;
   return static_memory_pool;
}

/*=============================================================================================*\
 * Method        MemoryPool<T, pool_capacity>::reserve_free_slot()
 *
 * Description   Reserve slot in memory pool by removing it from available slots list.
 *
 * Parameters    None
 *
 * Returns       T*
 *=============================================================================================*/
template <typename T, size_t pool_capacity>
T *MemoryPool<T, pool_capacity>::reserve_free_slot()
{
   T *free_slot = nullptr;

   if (!full())
   {
      free_slot = m_available_slots.front();
      m_available_slots.pop();
   }

   return free_slot;
}

/*=============================================================================================*\
 * Method        MemoryPool<T, pool_capacity>::release_slot(T* slot_to_release)
 *
 * Description   Release slot passed as input argument by adding it to available memory slots.
 *
 * Parameters    T* slot_to_release
 *
 * Returns       None
 *=============================================================================================*/
template <typename T, size_t pool_capacity>
inline void MemoryPool<T, pool_capacity>::release_slot(T *slot_to_release)
{
   const bool f_inside_memory = (&(m_memory[0]) <= slot_to_release) && (slot_to_release <= &(m_memory[pool_capacity - 1U]));
   if (f_inside_memory)
   {
      m_available_slots.push(slot_to_release);
   }
}

/*=============================================================================================*\
 * Method        MemoryPool<T, pool_capacity>::capacity()
 *
 * Description   Get number of elements that memory pool can accomodate.
 *
 * Parameters    None
 *
 * Returns       size_t
 *=============================================================================================*/
template <typename T, size_t pool_capacity>
inline size_t MemoryPool<T, pool_capacity>::capacity() const
{
   return pool_capacity;
}

/*=============================================================================================*\
 * Method        MemoryPool<T, pool_capacity>::full()
 *
 * Description   Check if memory pool is full (can no longer reserve slot for additional elements)
 *
 * Parameters    None
 *
 * Returns       bool
 *=============================================================================================*/
template <typename T, size_t pool_capacity>
inline bool MemoryPool<T, pool_capacity>::full() const
{
   return m_available_slots.empty();
}

/*=============================================================================================*\
 * Method        MemoryPool<T, pool_capacity>::num_empty_slots()
 *
 * Description   Check number of available slots in memory pool
 *
 * Parameters    None
 *
 * Returns       size_t
 *=============================================================================================*/
template <typename T, size_t pool_capacity>
inline size_t MemoryPool<T, pool_capacity>::num_empty_slots() const
{
   return m_available_slots.size();
}

#endif
