#pragma once

/**
 * \file Queue.h
 * \brief Declaration of Queue<T>
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <queue>
#include <boost/thread.hpp>

/**
 * \class Queue<T>
 * \brief Queue thread safe
 */
template <typename T>
class Queue
{
public:
   Queue(void);
   ~Queue(void);

   void     push(const T& content);
   T        pop(void);
   bool     empty(void) const;
private:
   bool     tryPop_(T& content);
   
   std::queue<T>              m_queue;
   mutable boost::mutex       m_mutex;
   boost::condition_variable  m_condition_variable;
};
#include "Queue.inl"