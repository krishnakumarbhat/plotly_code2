/*=============================================================================================*\
* FILE: unique_id_handler.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
*  This file contains definition of UniqueIdHandler template class
*  It provides functionality for handling ID's from specific range.
*  Only unsigned integer types are allowed by this container, e.g. uint8, uint16, uint32, unsigned.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef UNIQUE_ID_HANDLER_H
#define UNIQUE_ID_HANDLER_H

#include <limits>
#include <type_traits>

#include "fifo.h"

template <typename IdType, std::size_t ID_MIN, std::size_t ID_MAX, typename = typename std::enable_if<std::is_unsigned<IdType>::value>::type>
class UniqueIdHandler
{
  public:
   /**
    * @brief    Constructor of UniqueIdHandler do some checks of input data
    *           and initializes m_ids with IDs from provided range ID_MIN to ID_MAX.
    *           Sets m_availability[] with true values.
    *
    * @param    N/A
    *
    * @return   N/A
    **/
   UniqueIdHandler() : m_id_min(static_cast<IdType>(ID_MIN)), m_id_max(static_cast<IdType>(ID_MAX))
   {
      static_assert(0U < ID_MIN, "ID_MIN must be greater than zero.");
      static_assert(ID_MAX <= std::numeric_limits<IdType>::max(), "ID_MAX must be lower than maximum value of input type");
      static_assert(ID_MIN <= ID_MAX, "ID_MIN must be equal or lower than ID_MAX.");

      for (IdType id = m_id_min - 1U; id < m_id_max; ++id)
      {
         m_ids.push(id + 1U);
      }
   }

   /**
    * @brief    Copy assignment is not allowed
    *
    * @param    UniqueIdHandler&
    *
    * @return   N/A
    **/
   UniqueIdHandler &operator=(UniqueIdHandler &) = delete;

   /**
    * @brief      Copy constructor is not allowed
    *
    * @param[in]  const UniqueIdHandler&
    *
    * @return     N/A
    **/
   UniqueIdHandler(const UniqueIdHandler &) = delete;

   /**
    * @brief    Method returns ID from pool of available numbers
    *           and sets flag about its availability to false.
    *           Returns 0 when no available ID
    *
    * @return   id
    **/
   IdType get_id()
   {
      IdType id = 0U;

      if (!m_ids.empty())
      {
         id = m_ids.front();
         m_ids.pop();
      }

      return id;
   }

   /**
    * @brief    Method gets ID number, places it in pool of avaialbe IDs
    *           and sets its availability flag to true and return true.
    *           When operation failed returns false.
    *
    * @param[in]    id
    *
    * @return       result
    **/
   bool return_id(const IdType id)
   {
      bool f_returned = false;

      if ((m_id_min <= id) && (id <= m_id_max))
      {
         m_ids.push(id);
         f_returned = true;
      }

      return f_returned;
   }

   /**
    * @brief    Method resets container to its initial state
    *           when all IDs from given range are available.
    *
    * @return   N/A
    **/
   void reset()
   {
      m_ids = Fifo<IdType, ((ID_MAX - ID_MIN) + 1U)>();

      for (IdType id = m_id_min - 1U; id < m_id_max; ++id)
      {
         m_ids.push(id + 1U);
      }
   }

  private:
   const IdType m_id_min;                        // lower input limit of template input type
   const IdType m_id_max;                        // uppper input limit of template input type
   Fifo<IdType, ((ID_MAX - ID_MIN) + 1U)> m_ids; // Pool of available ID's
};

#endif
