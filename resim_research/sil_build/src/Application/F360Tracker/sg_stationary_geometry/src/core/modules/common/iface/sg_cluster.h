/*===================================================================================*\
* FILE: sg_cluster.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of Cluster.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/


#ifndef SG_CLUSTER_H
#define SG_CLUSTER_H

#include "sg_detection_list.h"

namespace sg
{
   /// Class for the storage of metadata information needed for effective iteration over clusters and detections
   class Cluster
   {
     public:
      /**
       * @brief    Constructor.
       **/
      Cluster() : m_size{0U}, m_age{SG_DEFAULT_AGE}, m_unique_id{INVALID_CLUSTER_ID}, m_begin{nullptr}
      {
      }

      /**
       * @brief    Increments the cluster's age.
       **/
      inline void increment_age()
      {
         m_age++;
      }

      /**
       * @brief    Get the cluster's age.
       *
       * @return   cluster's age
       **/
      inline uint32_t get_age() const
      {
         return m_age;
      }

      /**
       * @brief    Returns the cluster's unique id.
       *
       * @return   cluster's unique id
       **/

      inline decltype(Detection_T::cluster_id) unique_id() const
      {
         return m_unique_id;
      }

      /**
       * @brief    Returns number of detections currently stored in cluster.
       *
       * @return   number of detections currently stored in cluster
       **/
      inline std::size_t size() const
      {
         return m_size;
      }

      /**
       * @brief    Verifies if cluster is empty
       *
       * @return   true of cluster is empty
       **/
      inline bool empty() const
      {
         return size() == 0U;
      }

      /**
       * @brief    Returns iterator pointing to the first detection in cluster.
       *
       * @return   iterator pointing to the first element in cluster
       **/
      inline DetectionList::iterator begin() const
      {
         return m_begin;
      }

      /**
       * @brief    Returns iterator pointing to the one after last detection in cluster
       *
       * @return   iterator pointing to the one after last detection in cluster
       **/
      inline DetectionList::iterator end() const
      {
         return std::next(m_begin, static_cast<ptrdiff_t>(m_size));
      }

      friend class Clusters; // TODO: FZD-844: Replace with friend method
      friend class DetectionStorage;

     private:
      std::size_t m_size;
      uint32_t m_age;
      decltype(Detection_T::cluster_id) m_unique_id;
      DetectionList::iterator m_begin;
   };

}

#endif
