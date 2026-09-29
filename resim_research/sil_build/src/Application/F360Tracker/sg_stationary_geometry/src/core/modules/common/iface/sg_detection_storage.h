/*===================================================================================*\
* FILE: sg_detection_storage.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains definition of DetectionStorage class.
*
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_DETECTION_STORAGE_H
#define SG_DETECTION_STORAGE_H

#include <cmath>

#include "container_view.h"
#include "embedded_list.h"
#include "id_handler_incremental.h"
#include "sg_clusters.h"
#include "sg_detection_cache.h"
#include "sg_detection_list.h"
#include "sg_detection_storage_dump.h"
#include "sg_reuse.h"

namespace sg
{
   using DetectionIdHandler = IdHandlerIncremental<uint32_t>;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4820)
#endif
   class DetectionStorage
   {
     private:
      using detection_bins_type = std::array<DetectionCache::collection_data_type::const_iterator, SG_NUM_BINS>;

     public:
      template <typename Predicate>
      using View = ContainerView<DetectionList, Predicate>;

      /**
       * @brief    Constructor
       *
       * @param    None
       * @return   None
       **/
      DetectionStorage() = default;

      /**
       * @brief    Destructor
       *
       * @param    None
       * @return   None
       **/
      ~DetectionStorage() = default;

      /**
       * @brief    Copy construction of DetectionStorage is not allowed
       *
       * @param    source
       * @return   None
       **/
      DetectionStorage(const DetectionStorage &source) = delete;

      /**
       * @brief    Move construction of DetectionStorage is not allowed
       *
       * @param    other
       * @return   None
       **/
      DetectionStorage(DetectionStorage &&other) = delete;

      /**
       * @brief    Copy assignment of DetectionStorage is not allowed
       *
       * @param    other
       * @return   None
       **/
      DetectionStorage &operator=(DetectionStorage &other) = delete;

      /**
       * @brief    Move assignment of DetectionStorage is not allowed
       *
       * @param    other
       * @return   None
       **/
      DetectionStorage &operator=(DetectionStorage &&other) = delete;

      /**
       * @brief    Add a new element at the end of unclustered detection list and assign free unique id
       *
       * @param    detection - detection to be added to container
       * @return   iterator - iterator pointing to added element or nullptr if operation failed
       **/
      DetectionList::iterator push_back(const Detection_T &detection);

      /**
       * @brief    Erase specific element from detection list and relase unique id
       *
       * @param    it - iterator to the element to be removed
       * @return   iterator - iterator to the next element in DetectionStorage
       **/
      DetectionList::iterator erase(const DetectionList::iterator &it);

      /**
       * @brief    Reset state of DetectionStorage - remove all detections and reset id handler
       *
       * @param    None
       * @return   None
       **/
      void clear();

      /**
       * @brief    Get total number of detections in DetectionStorage
       *
       * @param    None
       * @return   number of detections in DetectionStorage
       **/
      size_t size() const
      {
         return m_detections.size();
      }

      /**
       * @brief    Get maximum number of detections that can be stored in DetectionStorage
       *
       * @param    None
       * @return   number of detections in DetectionStorage
       **/
      inline size_t capacity() const
      {
         return m_detections.capacity();
      }

      /**
       * @brief    Check if detection container is empty (doesn't contain any detections)
       *
       * @param    None
       * @return   container emptiness status
       **/
      inline bool empty() const
      {
         return m_detections.empty();
      }

      /**
       * @brief    Get reference to clusters inside DetectionStorage
       *
       * @param    None
       * @return   reference to detections devided into clusters
       **/
      inline Clusters &get_clusters()
      {
         return m_clusters;
      }

      /**
       * @brief    Get iterator pointing to the first element of detection storage
       *
       * @param    None
       * @return   Iterator pointing to the first element of detection storage
       **/
      inline DetectionList::iterator begin() const
      {
         return m_detections.begin();
      }

      /**
       * @brief   Returns iterator to list of detection sorted by x distance, that is not
       *          further than distance specified
       *
       * @param   distance
       * @return  detection iterator
       */
      DetectionCache::collection_data_type::const_iterator begin(const float position_x) const;

      /**
       * @brief    Get iterator pointing to the one after last element of detection storage
       *
       * @param    None
       * @return   Iterator pointing to the one after last element of detection storage
       **/
      inline DetectionList::iterator end() const
      {
         return m_detections.end();
      }

      /**
       * @brief   Returns iterator to list of detection sorted by x distance, that is not
       *          closer than distance specified
       *
       * @param   distance
       * @return  detection iterator
       */
      DetectionCache::collection_data_type::const_iterator end(const float position_x) const;

      /**
       * @brief    Get iterator pointing to the last element of detection storage
       *
       * @param    None
       * @return   Iterator pointing to the last element of detection storage
       **/
      inline DetectionList::reverse_iterator rbegin() const
      {
         return m_detections.rbegin();
      }

      /**
       * @brief    Get iterator pointing to the one before first element of detection storage
       *
       * @param    None
       * @return   Iterator pointing to the one before first element of detection storage
       **/
      inline DetectionList::reverse_iterator rend() const
      {
         return m_detections.rend();
      }

      /**
       * @brief   Get reference to cache of detection storage
       *
       * @param   None
       * @return  Reference to cache of detection storage
       */
      inline const DetectionCache &get_cache() const
      {
         return m_cache_detections;
      }

      /**
       * @brief   Sorts detections cache ascending by detection x-position
       *
       * @param   None
       * @return  None
       */
      void sort_by_x_pos();

      /**
       * @brief   Checks if detections are sorted by x-position
       *
       * @param   None
       * @return  true if sorted, false otherwise
       */
      bool is_sorted_by_x() const;

      /**
       * @brief   Updates detection bins
       *
       * @param   None
       * @return  None
       */
      void update_bin_info();

      /**
       * @brief   Returns array of detection bins
       *
       * @param   None
       * @return  array of detection bins
       */
      inline const detection_bins_type &get_bin_start_its() const
      {
         return m_bin_start_its;
      }

      /**
       * @brief    Moves detection from source cluster to current cluster. This function does not remove source cluster if it
       *become empty after move.
       *
       * @param    destination_cluster
       * @param    det_it - DetectionList iterator pointing to detection that will be moved
       **/
      void move_detection(const ClusterList::iterator &destination_cluster, const DetectionList::iterator &det_it);

      /**
       * @brief          Helper function for updating the age of each cluster
       **/
      void update_cluster_ages();

      /**
       * @brief    Remove clusters without any detections inside
       *
       * @param    None
       *
       * @return   None
       **/
      void remove_empty_clusters();

      /**
       * @brief   check if there exist cluster with given id. If not it creates one.
       *
       * @param   cluster_id
       * @return  ClusterList::iterator
       */
      ClusterList::iterator find_cluster_or_create_new(const uint16_t cluster_id);

      /**
       * @brief       Initialize object's state from dumped data.
       *
       * @param[in]   dumped_internal_detections
       **/
      void initialize(const SG_Detection_Storage_Dump_T &dumped_internal_detections) const;

      DetectionList m_detections;

     private:
      /**
       * @brief    Clears detection bins array
       *
       * @ param    None
       * @ return   None
       **/
      void clear_detection_bins()
      {
         (void) std::fill_n(m_bin_start_its.begin(), SG_NUM_BINS, DetectionCache::collection_data_type::const_iterator{});
      };

      Clusters m_clusters;
      DetectionIdHandler m_id_handler;
      DetectionCache m_cache_detections;
      detection_bins_type m_bin_start_its;
   };

#ifdef _MSC_VER
#pragma warning(pop)
#endif
}

#endif
