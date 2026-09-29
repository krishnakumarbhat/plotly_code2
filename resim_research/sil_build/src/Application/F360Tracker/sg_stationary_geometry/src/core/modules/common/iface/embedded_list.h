/*=============================================================================================*\
* FILE: embedded_list.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of EmbeddedList class
* It provides double linked list functionality without using dynamic memory allocation.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef EMBEDDED_LIST_H
#define EMBEDDED_LIST_H

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iterator>

#include "memory_pool.h"

/**
 * @brief Container providing double linked list functionality without using dynamic memory allocation.
 **/
template <typename DataType, size_t list_capacity>
class EmbeddedList
{
  private:
   struct Node
   {
      Node *prev{nullptr}; // Points to the previous item
      Node *next{nullptr}; // Points to the next item
   };

   struct DataNode : public Node
   {
      DataType data{}; // Data stored in a node

      void clear() const
      {
         data.~DataType(); // Clearing the node needs to trigger destruction of all underlying elements
      }
   };

  public:
   using MemoryType = MemoryPool<DataNode, list_capacity>;

  private:
   mutable Node m_dummy_head{nullptr, &m_dummy_tail}; // The Node pretending to be one node before list head
   mutable Node m_dummy_tail{&m_dummy_head, nullptr}; // The Node pretending to be one node after list tail

   size_t m_list_size;        // Number of elements stored currently in a list
   MemoryType &m_memory_pool; // Reference to memory pool used to store list objects

  public:
   /**
    * @brief        Default constructor of EmbeddedList class
    **/
   EmbeddedList();

   /**
    * @brief        Copy constructor of EmbeddedList class. Initializes values in current EmbeddedList with values from source list
    *
    * @param[in]    source - list to be copied
    *
    * @return       None
    **/
   EmbeddedList(const EmbeddedList &source);

   /**
    * @brief        Initilializer list constructor of EmbeddedList class. Initializes the list with elements from source.
    *
    * @param[in]    source - initializer list of elements
    *
    * @return       None
    **/
   EmbeddedList(const std::initializer_list<DataType> &source);

   /**
    * @brief        Destructor releasing all slots occupied in memory_pool
    **/
   ~EmbeddedList();

   /**
    * @brief        Assignment operator copying elements from source list to current list
    *
    * @param[in]    source - source list to be copied
    *
    * @return       copied list reference
    **/
   EmbeddedList &operator=(const EmbeddedList &source) noexcept;

   /**
    * @brief        Move assignment operator changing ownership of embedded list elements
    *               from source list to current list without copying elements stored in memory_pool
    *
    * @param[in, out]    source - source list to be moved
    *
    * @return        moved list reference
    **/
   EmbeddedList &operator=(EmbeddedList &&source) noexcept;

   class iterator;
   using reverse_iterator = std::reverse_iterator<iterator>;
   class const_iterator;
   using const_reverse_iterator = std::reverse_iterator<const_iterator>;

   /**
    * @brief        Provides bidirectional iterator to the first element of a list
    *
    * @param        None
    *
    * @return       iterator - bidirectional iterator
    **/
   iterator begin() const;

   /**
    * @brief        Provides bidirectional iterator to one after the last element of a list
    *
    * @param        None
    *
    * @return       iterator - bidirectional iterator
    **/
   iterator end() const;

   /**
    * @brief        Provides bidirectional const_iterator to the first element of a list
    *
    * @param        None
    *
    * @return       const_iterator - bidirectional const iterator
    **/
   const_iterator cbegin() const;

   /**
    * @brief        Provides bidirectional const_iterator to one after the last element of a list
    *
    * @param        None
    *
    * @return       const_iterator - bidirectional const iterator
    **/
   const_iterator cend() const;

   /**
    * @brief        Provides bidirectional reverse iterator to the last element of a list
    *
    * @param        None
    *
    * @return       reverse_iterator - bidirectional reverse iterator
    **/
   reverse_iterator rbegin() const;

   /**
    * @brief        Provides bidirectional reverse iterator to one before the first element of a list
    *
    * @param        None
    *
    * @return       reverse_iterator - bidirectional reverse iterator
    **/
   reverse_iterator rend() const;

   /**
    * @brief        Provides bidirectional const reverse iterator to the last element of a list
    *
    * @param        None
    *
    * @return       const_reverse_iterator - bidirectional reverse iterator
    **/
   const_reverse_iterator crbegin() const;

   /**
    * @brief        Provides bidirectional const reverse iterator to one before the first element of a list
    *
    * @param        None
    *
    * @return       const_reverse_iterator - bidirectional reverse iterator
    **/
   const_reverse_iterator crend() const;

   /**
    * @brief        Pushes data at the front (head) of the list.
    *
    * Uses forwarding reference template to deal with two types of input data: rvalue reference and lvalue reference
    * When data_reference has lvalue reference type the data are copied.
    * When data_reference has rvalue reference type the data are moved.
    *
    * @param[in]    data_reference - reference to data to be pushed
    *
    * @return       None
    **/
   template <typename DataRef>
   iterator push_front(DataRef &&data_reference);

   /**
    * @brief        Pushes data at end (tail) of the list.
    *
    * Uses forwarding reference template to deal with two types of input data: rvalue reference and lvalue reference
    * When data_reference has lvalue reference type the data are copied.
    * When data_reference has rvalue reference type the data are moved.
    *
    * @param[in]    data_reference - reference to data to be pushed
    *
    * @return       None
    **/
   template <typename DataRef>
   iterator push_back(DataRef &&data_reference);

   /**
    * @brief        Inserts data into node of a list.
    *
    * Uses forwarding reference template to deal with two types of input data: rvalue reference and lvalue reference
    * When data_reference has lvalue reference type the data are copied.
    * When data_reference has rvalue reference type the data are moved.
    *
    * @param[in]    input_it - iterator before node where the data will be inserted
    * @param[in]    data_reference - reference to data to be insert
    *
    * @return       iterator - bidirectional iterator to node where data has been inserted
    **/
   template <typename DataRef>
   iterator insert(const iterator &input_it, DataRef &&data_reference);

   /**
    * @brief        Moves nodes of a list.
    *
    * @param[in]    source_it - bidirectional iterator to node which will be moved
    * @param[in]    pos_it - bidirectional iterator before which the moved_it will be placed. In other words pos_it
    *               and all following nodes will be shifted after source_it is inserted in original place of pos_it.
    *
    * @return       None
    **/
   void move(const iterator &source_it, const iterator &pos_it);

   /**
    * @brief        Sorts the elements in ascending order.
    *
    * The order of equal elements is preserved.
    * When comparison function not provided input_it uses operator< to compare elements.
    * Selection algorithm is used for sorting.
    *
    * @param[in]    comp - comparison function object which returns ​true if first argument
    *               is less than the second.
    *               The signature of the comparison function should be equivalent to the following:
    *               bool comp(const Type1& a, const Type1& b);
    *
    * @return       None
    **/
   template <typename Compare = std::less<> >
   void sort(Compare comp = Compare{});

   /**
    * @brief        Erase element from the list
    *
    * @param[in]    forward iterator to element to erase
    *
    * @return       iterator - iterator to the element following the erased node
    **/
   iterator erase(const iterator &input_it);

   /**
    * @brief        Moves SOURCE list element pointed by SOURCE list iterator to the DESTINATION list
    *               (the list on behold of which the move_back() method is called).
    *
    * WARNING 1: iterator passed from a calling function is passed to this method
    * as a reference and is implicitly modified inside (by move_back)!.
    * WARNING 2: solution below requires that FORWARD iterator in external for loop is
    * incremented. For decremented FORWARD iterator move_back method does not work!
    *
    * @param[in, out] src_list - reference to a SOURCE list from which an element will be reassigned to the DESTINATION list
    * @param[in, out] src_it - reference to FORWARD iterator pointing SOURCE list element to be reassigned
    *
    * @return       iterator pointing new (reassigned) element in DESTINATION list.
    **/
   iterator move_back(EmbeddedList &src_list, iterator &src_it);

   /**
    * @brief        Swaps two nodes in list by setting its appropriate pointers. Data in node is not being copied.
    *
    * @param[in, out] position_1 - first elemnent to be swapped
    * @param[in, out] position_2 - second elemnent to be swapped
    *
    * @return       None
    **/
   void swap(iterator position_1, iterator position_2);

   /**
    * @brief        Helper method that swaps all list elements.
    *
    * @param[in]    other list - list that data will be swapped with "this" list
    *
    * @return       None
    **/
   void swap(EmbeddedList &other);

   /**
    * @brief        Removes data from the front (head) of the list
    **/
   void pop_front();

   /**
    * @brief        Removes data from the end (tail) of the list
    **/
   void pop_back();

   /**
    * @brief        Releases all slots occupied in memory_pool
    **/
   void clear();

   /**
    * @brief        Provides reference to data stored in first list node (head)
    *
    * @param        None
    *
    * @return       reference to the first list element
    **/
   DataType &front();

   /**
    * @brief        Provides reference to data stored in last list node (tail)
    *
    * @param        None
    *
    * @return       reference to the last list element
    **/
   DataType &back();

   /**
    * @brief        Provides const reference to data stored in first list node (head)
    *
    * @param        None
    *
    * @return       const reference to the first list element
    **/
   const DataType &front() const;

   /**
    * @brief        Provides const reference to data stored in last list node (tail)
    *
    * @param        None
    *
    * @return       const reference to the last list element
    **/
   const DataType &back() const;

   /**
    * @brief        Returns information about maximum number of items that can be stored in list
    *
    * @param        None
    *
    * @return       maximum number of items that can be stored in list
    **/
   size_t capacity() const;

   /**
    * @brief        Returns number of elements currently stored in list
    *
    * @param        None
    *
    * @return       number of list elements
    **/
   size_t size() const;

   /**
    * @brief        Returns number of empty nodes in list
    *
    * @param        None
    *
    * @return       number of empty slots
    **/
   size_t num_empty_slots() const;

   /**
    * @brief        Returns information if list is empty.
    *
    * @param        None
    *
    * @return       true if the list doesn't hold any data
    **/
   bool empty() const;

   /**
    * @brief        Returns information if list is full. If 'true' no more data can be stored in list
    *
    * @param        None
    *
    * @return       true if the list is full
    **/
   bool full() const;

  private:
   /**
    * @brief        Helper method which disconnects node from list.
    *
    * @param[in]    src_it - iterator reference to disconnected element
    *
    * @return       None
    **/
   void disconnect_from_list(const iterator &src_it);

   /**
    * @brief        Helper method which connects node to list end.
    *
    * @param[in]    src_it - iterator reference to disconnected element
    *
    * @return       None
    **/
   void connect_to_end(const iterator &src_it);

   /**
    * @brief        Helper method that checks if two nodes pointed by input iterators are adjacent.
    *
    * @param[in]    position_1 - first elemnent to be checked
    * @param[in]    position_2 - second elemnent to be checked
    *
    * @return       status - true if nodes are adjacent on list otherwise false.
    **/
   static bool adjacent_nodes(const iterator &position_1, const iterator &position_2);

   /**
    * @brief        Helper method that checks if iterator is valid regarding to reference EmbeddedList provided as first input
    *parameter.
    *
    * @param[in]    ref_list - list to be checked for iterator validity
    * @param[in]    input_it - iterator to be checked
    *
    * @return       true if iterator is valid
    **/
   static bool is_iterator_valid(const EmbeddedList &ref_list, const iterator &input_it);
};

template <typename DataType, size_t list_capacity>
inline void EmbeddedList<DataType, list_capacity>::swap(EmbeddedList &other)
{
   if ((0U < this->size()) && (0U < other.size()))
   {
      const auto this_head_next  = this->m_dummy_head.next;
      const auto this_tail_prev  = this->m_dummy_tail.prev;
      const auto this_first_prev = &(this->m_dummy_head);
      const auto this_last_next  = &(this->m_dummy_tail);

      const auto this_list_size = this->m_list_size;

      // update 'this'
      this->m_dummy_head.next->prev = &other.m_dummy_head;
      this->m_dummy_tail.prev->next = &other.m_dummy_tail;
      this->m_dummy_head.next       = other.m_dummy_head.next;
      this->m_dummy_tail.prev       = other.m_dummy_tail.prev;

      // update 'other'
      other.m_dummy_head.next->prev = this_first_prev;
      other.m_dummy_tail.prev->next = this_last_next;
      other.m_dummy_head.next       = this_head_next;
      other.m_dummy_tail.prev       = this_tail_prev;

      // update sizes
      this->m_list_size = other.m_list_size;
      other.m_list_size = this_list_size;
   }
}

/**
 * @brief Provides bidirectional iterator functionality for EmbeddedList.
 **/
template <typename DataType, size_t list_capacity>
class EmbeddedList<DataType, list_capacity>::iterator
{
   friend class EmbeddedList<DataType, list_capacity>;

  public:
   using iterator_category = std::bidirectional_iterator_tag;
   using value_type        = DataType;
   using pointer           = DataType *;
   using reference         = DataType &;
   using difference_type   = std::ptrdiff_t;

   iterator() = default;

   iterator(DataNode *const ptr) : m_data_ptr(ptr)
   {
   }
   iterator(const iterator &it) = default;

   reference operator*() const
   {
      assert(m_data_ptr != nullptr);
      return m_data_ptr->data;
   }

   pointer operator->() const
   {
      assert(m_data_ptr != nullptr);
      return &m_data_ptr->data;
   }

   iterator &operator++()
   {
      assert(m_data_ptr != nullptr);
      m_data_ptr = static_cast<DataNode *>(m_data_ptr->next);
      return *this;
   }

   iterator operator++(const int unused)
   {
      (void) unused;
      const iterator tmp_it(*this);
      ++(*this);
      return tmp_it;
   }

   iterator &operator--()
   {
      assert(m_data_ptr != nullptr);
      m_data_ptr = static_cast<DataNode *>(m_data_ptr->prev);
      return *this;
   }

   iterator operator--(int unused)
   {
      (void) unused;
      const iterator tmp_it(*this);
      --(*this);
      return tmp_it;
   }

   bool operator==(const iterator &other) const
   {
      return m_data_ptr == other.m_data_ptr;
   }
   bool operator!=(const iterator &other) const
   {
      return m_data_ptr != other.m_data_ptr;
   }

   iterator &operator=(const iterator &source) noexcept
   {
      this->m_data_ptr = source.m_data_ptr;
      return *this;
   }

  private:
   DataNode *m_data_ptr;
};

/**
 * @brief Provides bidirectional const_iterator functionality for EmbeddedList.
 **/
template <typename DataType, size_t list_capacity>
class EmbeddedList<DataType, list_capacity>::const_iterator
{
   friend class EmbeddedList<DataType, list_capacity>;

  public:
   using iterator_category = std::bidirectional_iterator_tag;
   using value_type        = DataType;
   using pointer           = const value_type *;
   using reference         = const value_type &;
   using difference_type   = std::ptrdiff_t;

   const_iterator() = default;
   const_iterator(DataNode *const ptr) : m_data_ptr{ptr}
   {
   }
   const_iterator(const const_iterator &it) = default;
   ~const_iterator()                        = default;

   /**
    * @brief Dereference operator
    *
    * @param none
    *
    * @return       Reference to data pointed by const_iterator
    **/
   reference operator*() const
   {
      assert(m_data_ptr != nullptr);
      return m_data_ptr->data;
   }

   /**
    * @brief Pointer operator
    *
    * @param none
    *
    * @return       Pointer to data pointed by const_iterator
    **/
   pointer operator->() const
   {
      assert(m_data_ptr != nullptr);
      return &m_data_ptr->data;
   }

   /**
    * @brief Pre Increment operator
    *
    * @param none
    *
    * @return       const_iterator pointing to the current collection data
    **/
   const_iterator &operator++()
   {
      assert(m_data_ptr != nullptr);
      m_data_ptr = static_cast<DataNode *>(m_data_ptr->next);
      return *this;
   }

   /**
    * @brief Post Increment operator
    *
    * @param none
    *
    * @return       const_iterator pointing to the next collection data
    **/
   const_iterator operator++(int unused)
   {
      (void) unused;
      const_iterator tmp_it(*this);
      ++(*this);
      return tmp_it;
   }

   /**
    * @brief Pre Decrement operator
    *
    * @param none
    *
    * @return   const_iterator pointing to the current collection data
    **/
   const_iterator &operator--()
   {
      assert(m_data_ptr != nullptr);
      m_data_ptr = static_cast<DataNode *>(m_data_ptr->prev);
      return *this;
   }

   /**
    * @brief Post Decrement operator
    *
    * @param none
    *
    * @return   const_iterator pointing to the next collection data
    **/
   const_iterator operator--(int unused)
   {
      (void) unused;
      const_iterator tmp_it(*this);
      --(*this);
      return tmp_it;
   }

   /**
    * @brief Equal operator
    *
    * @param[in] other - right hand side const_iterator
    *
    * @return   True if const_iterators are equal, otherwise False.
    **/
   bool operator==(const const_iterator &other) const
   {
      return m_data_ptr == other.m_data_ptr;
   }

   /**
    * @brief Inequality operator
    *
    * @param[in] other - right hand side const_iterator
    *
    * @return   False if const_iterators are equal, otherwise True.
    **/
   bool operator!=(const const_iterator &other) const
   {
      return !(*this == other);
   }

   /**
    * @brief Assignment operator
    *
    * @param[in] other - right hand side const_iterator
    *
    * @return   reference to this const_iterator
    **/
   const_iterator &operator=(const const_iterator &source) noexcept
   {
      this->m_data_ptr = source.m_data_ptr;
      return *this;
   }

  private:
   DataNode *m_data_ptr;
};

template <typename DataType, size_t list_capacity>
inline EmbeddedList<DataType, list_capacity>::EmbeddedList() : m_list_size{}, m_memory_pool{MemoryType::static_instance()}
{
}

template <typename DataType, size_t list_capacity>
inline EmbeddedList<DataType, list_capacity>::EmbeddedList(const EmbeddedList &source)
    : m_list_size{}, m_memory_pool{MemoryType::static_instance()}
{
   for (auto &value : source)
   {
      (void) push_back(value);
   }
}

template <typename DataType, size_t list_capacity>
inline EmbeddedList<DataType, list_capacity>::EmbeddedList(const std::initializer_list<DataType> &source)
    : m_list_size{}, m_memory_pool{MemoryType::static_instance()}
{
   for (auto &value : source)
   {
      (void) push_back(value);
   }
}

template <typename DataType, size_t list_capacity>
template <typename DataRef>
inline typename EmbeddedList<DataType, list_capacity>::iterator EmbeddedList<DataType, list_capacity>::push_front(DataRef &&data_reference)
{
   return insert(begin(), std::forward<DataRef>(data_reference));
}

template <typename DataType, size_t list_capacity>
inline void EmbeddedList<DataType, list_capacity>::pop_front()
{
   (void) erase(begin());
}

template <typename DataType, size_t list_capacity>
template <typename DataRef>
inline typename EmbeddedList<DataType, list_capacity>::iterator EmbeddedList<DataType, list_capacity>::push_back(DataRef &&data_reference)
{
   return insert(end(), std::forward<DataRef>(data_reference));
}

template <typename DataType, size_t list_capacity>
inline void EmbeddedList<DataType, list_capacity>::pop_back()
{
   (void) erase(iterator(static_cast<DataNode *>(m_dummy_tail.prev)));
}

template <typename DataType, size_t list_capacity>
inline typename EmbeddedList<DataType, list_capacity>::iterator EmbeddedList<DataType, list_capacity>::begin() const
{
   return iterator(static_cast<DataNode *>(m_dummy_head.next));
}

template <typename DataType, size_t list_capacity>
inline typename EmbeddedList<DataType, list_capacity>::iterator EmbeddedList<DataType, list_capacity>::end() const
{
   return iterator(static_cast<DataNode *>(&m_dummy_tail));
}

template <typename DataType, size_t list_capacity>
inline typename EmbeddedList<DataType, list_capacity>::const_iterator EmbeddedList<DataType, list_capacity>::cbegin() const
{
   return const_iterator(static_cast<DataNode *>(m_dummy_head.next));
}

template <typename DataType, size_t list_capacity>
inline typename EmbeddedList<DataType, list_capacity>::const_iterator EmbeddedList<DataType, list_capacity>::cend() const
{
   return const_iterator(static_cast<DataNode *>(&m_dummy_tail));
}

template <typename DataType, size_t list_capacity>
inline typename EmbeddedList<DataType, list_capacity>::reverse_iterator EmbeddedList<DataType, list_capacity>::rbegin() const
{
   return reverse_iterator(end());
}

template <typename DataType, size_t list_capacity>
inline typename EmbeddedList<DataType, list_capacity>::reverse_iterator EmbeddedList<DataType, list_capacity>::rend() const
{
   return reverse_iterator(begin());
}

template <typename DataType, size_t list_capacity>
inline typename EmbeddedList<DataType, list_capacity>::const_reverse_iterator EmbeddedList<DataType, list_capacity>::crbegin() const
{
   return const_reverse_iterator(cend());
}

template <typename DataType, size_t list_capacity>
inline typename EmbeddedList<DataType, list_capacity>::const_reverse_iterator EmbeddedList<DataType, list_capacity>::crend() const
{
   return const_reverse_iterator(cbegin());
}

template <typename DataType, size_t list_capacity>
template <typename DataRef>
inline typename EmbeddedList<DataType, list_capacity>::iterator
EmbeddedList<DataType, list_capacity>::insert(const iterator &input_it, DataRef &&data_reference)
{
   DataNode *node_to_update_m_data_ptr{static_cast<DataNode *>(&m_dummy_tail)};

   if (!m_memory_pool.full()) // Free slots are available in m_list_nodes[]
   {
      node_to_update_m_data_ptr       = m_memory_pool.reserve_free_slot();
      node_to_update_m_data_ptr->data = std::forward<DataRef>(data_reference);

      if (empty())
      {
         node_to_update_m_data_ptr->next = &m_dummy_tail;
         node_to_update_m_data_ptr->prev = &m_dummy_head;
         m_dummy_tail.prev               = node_to_update_m_data_ptr;
         m_dummy_head.next               = node_to_update_m_data_ptr;
      }
      else
      {
         if (begin() == input_it)
         {
            node_to_update_m_data_ptr->next = m_dummy_head.next;
            node_to_update_m_data_ptr->prev = &m_dummy_head;
            input_it.m_data_ptr->prev       = node_to_update_m_data_ptr;
            m_dummy_head.next               = node_to_update_m_data_ptr;
         }
         else if (end() == input_it)
         {
            node_to_update_m_data_ptr->next = &m_dummy_tail;
            node_to_update_m_data_ptr->prev = m_dummy_tail.prev;
            m_dummy_tail.prev->next         = node_to_update_m_data_ptr;
            m_dummy_tail.prev               = node_to_update_m_data_ptr;
         }
         else
         {
            node_to_update_m_data_ptr->next = input_it.m_data_ptr->prev->next;
            node_to_update_m_data_ptr->prev = input_it.m_data_ptr->prev;
            input_it.m_data_ptr->prev->next = node_to_update_m_data_ptr;
            input_it.m_data_ptr->prev       = node_to_update_m_data_ptr;
         }
      }
      ++m_list_size;
   }

   return iterator(node_to_update_m_data_ptr);
}

template <typename DataType, size_t list_capacity>
inline void EmbeddedList<DataType, list_capacity>::move(const iterator &source_it, const iterator &pos_it)
{
   assert(!empty());

   // source_it and pos_it have to be on the same list
   if (source_it != pos_it && source_it.m_data_ptr != pos_it.m_data_ptr->prev)
   {
      Node *const moved_prev = source_it.m_data_ptr->prev;
      Node *const moved_next = source_it.m_data_ptr->next;
      moved_prev->next       = moved_next;
      moved_next->prev       = moved_prev;

      Node *const input_it_prev  = pos_it.m_data_ptr->prev;
      input_it_prev->next        = source_it.m_data_ptr;
      pos_it.m_data_ptr->prev    = source_it.m_data_ptr;
      source_it.m_data_ptr->prev = input_it_prev;
      source_it.m_data_ptr->next = pos_it.m_data_ptr;
   }
}

template <typename DataType, size_t list_capacity>
template <typename Compare>
inline void EmbeddedList<DataType, list_capacity>::sort(Compare comp)
{
   if (size() > 1U)
   {
      auto range_begin_it = begin();

      while (range_begin_it != end())
      {
         auto min_val_it = std::min_element(range_begin_it, end(), comp);
         swap(range_begin_it++, min_val_it);
      }
   }
}

template <typename DataType, size_t list_capacity>
inline typename EmbeddedList<DataType, list_capacity>::iterator EmbeddedList<DataType, list_capacity>::erase(const iterator &input_it)
{
   Node *return_node_m_data_ptr{&m_dummy_tail};

   if ((!empty()) && is_iterator_valid(*this, input_it))
   {
      disconnect_from_list(input_it);
      return_node_m_data_ptr = input_it.m_data_ptr->next;
      m_memory_pool.release_slot(input_it.m_data_ptr);
      input_it.m_data_ptr->clear();
      --m_list_size;
   }
   return iterator(static_cast<DataNode *>(return_node_m_data_ptr));
}

template <typename DataType, size_t list_capacity>
inline typename EmbeddedList<DataType, list_capacity>::iterator
EmbeddedList<DataType, list_capacity>::move_back(EmbeddedList<DataType, list_capacity> &src_list, iterator &src_it)
{
   if (!(src_list.empty()) && is_iterator_valid(src_list, src_it))
   {
      const iterator updated_src_it(static_cast<DataNode *>(src_it.m_data_ptr->prev));

      disconnect_from_list(src_it);
      this->connect_to_end(src_it);

      --(src_list.m_list_size);
      ++(this->m_list_size);
      src_it = updated_src_it;
   }
   return this->end();
}

template <typename DataType, size_t list_capacity>
inline void EmbeddedList<DataType, list_capacity>::disconnect_from_list(const iterator &src_it)
{
   src_it.m_data_ptr->prev->next = src_it.m_data_ptr->next;
   src_it.m_data_ptr->next->prev = src_it.m_data_ptr->prev;
}

template <typename DataType, size_t list_capacity>
inline void EmbeddedList<DataType, list_capacity>::connect_to_end(const iterator &src_it)
{
   src_it.m_data_ptr->prev       = this->m_dummy_tail.prev;
   src_it.m_data_ptr->next       = &(this->m_dummy_tail);
   src_it.m_data_ptr->prev->next = src_it.m_data_ptr;
   src_it.m_data_ptr->next->prev = src_it.m_data_ptr;
}

template <typename DataType, size_t list_capacity>
inline void EmbeddedList<DataType, list_capacity>::clear()
{
   while (!empty())
   {
      (void) erase(begin());
   }
}

template <typename DataType, size_t list_capacity>
inline void EmbeddedList<DataType, list_capacity>::swap(iterator position_1, iterator position_2)
{
   if ((position_1 != position_2) && is_iterator_valid(*this, position_1) && is_iterator_valid(*this, position_2))
   {
      // position_2 before position_1 on list and adjacent - swap iterators
      if (position_2.m_data_ptr->next == position_1.m_data_ptr)
      {
         iterator tmp = position_1;
         position_1   = position_2;
         position_2   = tmp;
      }

      const auto position_1_prev = position_1.m_data_ptr->prev;
      const auto position_1_next = position_1.m_data_ptr->next;
      const auto position_2_prev = position_2.m_data_ptr->prev;
      const auto position_2_next = position_2.m_data_ptr->next;

      if (adjacent_nodes(position_1, position_2))
      {
         position_1.m_data_ptr->prev = position_1_next;
         position_1.m_data_ptr->next = position_2_next;
         position_2.m_data_ptr->prev = position_1_prev;
         position_2.m_data_ptr->next = position_2_prev;
      }
      else
      {
         position_1.m_data_ptr->prev = position_2_prev;
         position_1.m_data_ptr->next = position_2_next;
         position_2.m_data_ptr->prev = position_1_prev;
         position_2.m_data_ptr->next = position_1_next;
      }

      // Setting pointers in neighboring nodes of swapped nodes.
      position_1.m_data_ptr->prev->next = position_1.m_data_ptr;
      position_1.m_data_ptr->next->prev = position_1.m_data_ptr;

      position_2.m_data_ptr->prev->next = position_2.m_data_ptr;
      position_2.m_data_ptr->next->prev = position_2.m_data_ptr;
   }
}

template <typename DataType, size_t list_capacity>
inline bool EmbeddedList<DataType, list_capacity>::adjacent_nodes(const iterator &position_1, const iterator &position_2)
{
   return ((position_1.m_data_ptr->next == position_2.m_data_ptr) || (position_2.m_data_ptr->next == position_1.m_data_ptr));
}

template <typename DataType, size_t list_capacity>
bool EmbeddedList<DataType, list_capacity>::is_iterator_valid(const EmbeddedList &ref_list, const iterator &input_it)
{
   return (input_it != nullptr) && (input_it != static_cast<DataNode *>(&ref_list.m_dummy_head))
          && (input_it != static_cast<DataNode *>(&ref_list.m_dummy_tail));
}

template <typename DataType, size_t list_capacity>
inline DataType &EmbeddedList<DataType, list_capacity>::front()
{
   return static_cast<DataNode *>(m_dummy_head.next)->data;
}

template <typename DataType, size_t list_capacity>
inline DataType &EmbeddedList<DataType, list_capacity>::back()
{
   return static_cast<DataNode *>(m_dummy_tail.prev)->data;
}

template <typename DataType, size_t list_capacity>
inline const DataType &EmbeddedList<DataType, list_capacity>::front() const
{
   return static_cast<DataNode *>(m_dummy_head.next)->data;
}

template <typename DataType, size_t list_capacity>
inline const DataType &EmbeddedList<DataType, list_capacity>::back() const
{
   return static_cast<DataNode *>(m_dummy_tail.prev)->data;
}

template <typename DataType, size_t list_capacity>
inline size_t EmbeddedList<DataType, list_capacity>::size() const
{
   return m_list_size;
}

template <typename DataType, size_t list_capacity>
inline size_t EmbeddedList<DataType, list_capacity>::num_empty_slots() const
{
   return m_memory_pool.num_empty_slots();
}

template <typename DataType, size_t list_capacity>
inline bool EmbeddedList<DataType, list_capacity>::empty() const
{
   return size() == 0U;
}

template <typename DataType, size_t list_capacity>
inline bool EmbeddedList<DataType, list_capacity>::full() const
{
   return num_empty_slots() == 0U;
}

template <typename DataType, size_t list_capacity>
inline size_t EmbeddedList<DataType, list_capacity>::capacity() const
{
   return list_capacity;
}

template <typename DataType, size_t list_capacity>
EmbeddedList<DataType, list_capacity> &EmbeddedList<DataType, list_capacity>::operator=(const EmbeddedList &source) noexcept
{
   // Remove all elements from the current list
   clear();

   // Copy all elements from source to current
   for (auto &data : source)
   {
      (void) push_back(data);
   }

   return *this;
}

template <typename DataType, size_t list_capacity>
EmbeddedList<DataType, list_capacity> &EmbeddedList<DataType, list_capacity>::operator=(EmbeddedList &&source) noexcept
{
   // Remove all elements from the this list
   clear();

   // Move data when source is not empty
   if (!source.empty())
   {
      // Move ownership of data from source to this
      m_list_size = source.m_list_size;

      m_dummy_head.next = source.m_dummy_head.next;
      m_dummy_tail.prev = source.m_dummy_tail.prev;

      m_dummy_head.next->prev = &m_dummy_head;
      m_dummy_tail.prev->next = &m_dummy_tail;

      // Reset source object
      source.m_list_size       = 0U;
      source.m_dummy_head.next = &(source.m_dummy_tail);
      source.m_dummy_tail.prev = &(source.m_dummy_head);
   }
   return *this;
}

template <typename DataType, size_t list_capacity>
inline EmbeddedList<DataType, list_capacity>::~EmbeddedList()
{
   // On destruction object needs to free all slots input_it occupies in memory pool
   clear();
}

#endif
