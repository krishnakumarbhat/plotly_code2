/*=============================================================================================*\
* FILE: container_view.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of ContainerView class
* .............................................................................
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef CONTAINER_VIEW_H
#define CONTAINER_VIEW_H

#include <algorithm>

template <typename Container, typename Predicate>
class ContainerView
{
  public:
   /**
    * @brief    Constructor of ContainerView class.
    *
    * @param    contr - reference to the container on which view is created.
    * @param    pred - predicate object used to filter container elements to make them visible in the view.
    *
    * @return   none
    **/
   ContainerView(Container &contr, Predicate pred) : m_container(contr), m_predicate(pred)
   {
   }

   class iterator
   {
     private:
      typename Container::iterator m_iter;
      Container &m_container;
      Predicate m_predicate;

     public:
      using traits            = std::iterator_traits<typename Container::iterator>;
      using iterator_category = typename traits::iterator_category;
      using value_type        = typename traits::value_type;
      using pointer           = typename traits::pointer;
      using reference         = typename traits::reference;
      using difference_type   = typename traits::difference_type;

      iterator(typename Container::iterator iter, Container &contr, Predicate pred)
          : m_iter(iter), m_container(contr), m_predicate(pred)
      {
         go_to_next_valid();
      }

      reference operator*() const
      {
         return *m_iter;
      }

      iterator &operator++()
      {
         ++m_iter;
         go_to_next_valid();
         return *this;
      }

      iterator &operator--()
      {
         --m_iter;
         go_to_prev_valid();
         return *this;
      }

      bool operator!=(const iterator &other) const
      {
         return m_iter != other.m_iter;
      }

      bool operator==(const iterator &other) const
      {
         return m_iter == other.m_iter;
      }

      typename Container::iterator base() const
      {
         return m_iter;
      }

     private:
      void go_to_next_valid()
      {
         while (m_iter != m_container.end() && !m_predicate(*m_iter))
         {
            ++m_iter;
         }
      }

      void go_to_prev_valid()
      {
         while (m_iter != m_container.begin() && !m_predicate(*m_iter))
         {
            --m_iter;
         }
      }
   };

   using reverse_iterator = std::reverse_iterator<iterator>;

   /**
    * @brief    Gives iterator to first element visible in the view (fulfilling the predicate).
    *
    * @return   iterator to the original container element.
    **/
   auto begin()
   {
      return iterator(m_container.begin(), m_container, m_predicate);
   }

   /**
    * @brief    Gives iterator to the element following the last element of the container.
    *
    * @return   iterator to the original container element.
    **/
   auto end()
   {
      return iterator(m_container.end(), m_container, m_predicate);
   }

   /**
    * @brief    Gives iterator to last element visible in the view (fulfilling the predicate).
    *
    * @return   reverse iterator to the original container element.
    **/
   auto rbegin()
   {
      return reverse_iterator(end());
   }

   /**
    * @brief    Gives iterator to the element preceding the first element of the container.
    *
    * @return   iterator to the original container element.
    **/
   auto rend()
   {
      return reverse_iterator(begin());
   }

   /**
    * @brief    Gives capacity (maximum number of elements) of the view.
    *
    * @return   std::size_t.
    **/
   inline constexpr std::size_t capacity() const
   {
      return m_container.capacity();
   }

   /**
    * @brief    Gives the number of elements in the view (fulfilling the predicate of the view).
    *
    * @return   std::size_t.
    **/
   std::size_t size() const
   {
      return std::count_if(m_container.begin(), m_container.end(), m_predicate);
   }

  private:
   Container &m_container;
   Predicate m_predicate;
};

#endif
