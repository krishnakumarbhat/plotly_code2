/*===================================================================================*\
* FILE: sg_clusters.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of Clusters class.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/


#ifndef SG_CLUSTERS_H
#define SG_CLUSTERS_H

#include "sg_cluster_list.h"
#include "unique_id_handler.h"

namespace sg
{
   using ClusterIdHandler = UniqueIdHandler<decltype(Detection_T::cluster_id), 1U, 2U * SG_MAX_NUM_INTERNAL_CLUSTERS>;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4820)
#endif

   class Clusters
   {
     public:
      /**
       * @brief    Default constructor creates the first cluster for unclustered detections that
       *           should always exist during Clusters object lifetime
       *
       * @param    None
       *
       * @return   None
       **/
      Clusters();

      /**
       * @brief    Copy construction is not allowed since Clusters contains EmbeddedList
       *
       * @param    const Clusters&
       *
       * @return   None
       **/
      Clusters(const Clusters &) = delete;

      /**
       * @brief    Copy assignment is not allowed since Clusters contains EmbeddedList
       *
       * @param    const Cluster&
       *
       * @return   None
       **/
      Clusters &operator=(Cluster &) = delete;

      /**
       * @brief    Create a new empty cluster on clusters list with cluster_id assigned
       *
       * @param    None
       *
       * @return   Cluster&
       **/
      ClusterList::iterator create_new();

      /**
       * @brief    Clear the container by removing all detections and clusters apart from the first
       *           one. First cluster is not removed since it is used for storing unclustered
       *           detections and should always exist.
       *
       * @param    None
       *
       * @return   None
       **/
      void clear();

      /**
       * @brief    Get reference to cluster containing unclustered detections
       *
       * @param    None
       *
       * @return   Cluster&
       **/
      inline Cluster &unclustered() const
      {
         return *(m_clusters.begin());
      }

      /**
       * @brief    Returns total number of clusters including first cluster for unclustered detections
       *
       * @param    None
       *
       * @return   size_t
       **/
      inline std::size_t size() const
      {
         return m_clusters.size();
      }

      /**
       * @brief    Returns capacity of Clusters - max number of clusters that can be stored
       *
       * @param    None
       *
       * @return   size_t
       **/
      inline std::size_t capacity() const
      {
         return m_clusters.capacity();
      }

      /**
       * @brief    Returns iterator pointing to the first cluster in Clusters
       *
       * @param    None
       *
       * @return   ClusterList::iterator
       **/
      inline ClusterList::iterator begin() const
      {
         return m_clusters.begin();
      }

      /**
       * @brief    Returns iterator pointing to the one after last element in Clusters
       *
       * @param    None
       *
       * @return   ClusterList::iterator
       **/
      inline ClusterList::iterator end() const
      {
         return m_clusters.end();
      }

      /**
       * @brief    Returns reverse iterator pointing to the last element in Clusters
       *
       * @param    None
       *
       * @return   ClusterList::reverse_iterator
       **/
      inline ClusterList::reverse_iterator rbegin() const
      {
         return m_clusters.rbegin();
      }

      /**
       * @brief    Returns reverse iterator pointing to the one before first element in Clusters
       *
       * @param    None
       *
       * @return   ClusterList::reverse_iterator
       **/
      inline ClusterList::reverse_iterator rend() const
      {
         return m_clusters.rend();
      }

      /**
       * @brief    Remove cluster from clusters list
       *
       * @param    cluster_it
       *
       * @return   ClusterList::iterator
       **/
      ClusterList::iterator erase(const ClusterList::iterator &cluster_it);

     private:
      ClusterList m_clusters; ///< list of clusters managed by Clusters class

      ClusterIdHandler m_cluster_id_handler; ///< object for handling unique ids for clusters

      friend class DetectionStorage; // TODO: FZD-844: Replace with friend method
   };
#ifdef _MSC_VER
#pragma warning(pop)
#endif

}
#endif
