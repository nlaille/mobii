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
   /**
    * \fn Queue(void)
	* \brief Constructor
	*/
   Queue(void);

   /**
    * \fn ~Queue(void)
	* \brief Destructor
	*/
   ~Queue(void);

   /**
    * \fn void     push(const T& content)
	* \brief Push an element to the end of the queue
	* \param content
	*/
   void     push(const T& content);

   /**
    * \fn T        pop(void)
	* \brief Return first element of the queue, block until the queue is empty
	* \return an element
	*/
   T        pop(void);

   /**
    * \fn bool     empty(void) const
	* \brief Check if the queue is empty
	* \return emptiness
	*/
   bool     empty(void) const;
private:
   bool     tryPop_(T& content);
   
   std::queue<T>              m_queue; /*!< queue */
   mutable boost::mutex       m_mutex; /*!< mutex */
   boost::condition_variable  m_condition_variable; /*!< conditional variable */
};
#include "Queue.inl"