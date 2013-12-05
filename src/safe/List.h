#pragma once

/**
 * \file List.h
 * \brief Declaration of List<T>
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#include <list>
#include <boost/thread.hpp>

/**
 * \class List<T>
 * \brief List thread safe
 */
template <typename T>
class List
{
public:
   List(void);
   ~List(void);

   void     push(const T& element);
   void     remove(const T& element);
   bool     exists(const T& element) const;
private:
   std::list<T>               m_list;
   mutable boost::mutex       m_mutex;
};

#include "List.inl"