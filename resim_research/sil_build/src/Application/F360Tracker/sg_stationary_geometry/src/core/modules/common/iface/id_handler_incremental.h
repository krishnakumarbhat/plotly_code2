/*=============================================================================================*\
* FILE: id_handler_incremental.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
*  This file contains definition of IdHandlerIncremental template class
*  It provides functionality for handling ID's as consecutive numbers from 1 to IdType capacity.
*  When get max IdType capacity overflow and starts from beginning.
*  Only unsigned integer types are allowed by this container, e.g. uint8, uint16, uint32, unsigned.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef ID_HANDLER_INCREMENTAL
#define ID_HANDLER_INCREMENTAL

template <typename IdType, std::enable_if_t<std::is_unsigned<IdType>::value, bool> = true>
class IdHandlerIncremental
{
  public:
   /**
    * @brief    Constructor of IdHandlerIncremental
    *
    * @return   None
    **/
   IdHandlerIncremental() : m_current_id(static_cast<IdType>(1U))
   {
   }

   /**
    * @brief    Get next available id
    *
    * @return   m_current_id
    **/
   IdType get_id()
   {
      if (m_current_id == static_cast<IdType>(0U))
      {
         m_current_id = static_cast<IdType>(1U);
      }

      return m_current_id++;
   }

   /**
    * @brief    Set current available id
    *
    * @return   None
    **/
   void set_id(const IdType id)
   {
      m_current_id = id;
   }

   /**
    * @brief    Reset handler state
    *
    * @return   None
    **/
   void reset()
   {
      m_current_id = static_cast<IdType>(1U);
   }

  private:
   IdType m_current_id;
};

#endif
