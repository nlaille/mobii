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
	/**
	 * \fn List(void)
	 * \brief Constructor
	 */
   List(void);

    /**
	 * \fn ~List(void)
	 * \brief Destructor
	 */
   ~List(void);

    /**
	 * \fn void     push(const T& element)
	 * \brief Add an element to the list
	 * \param element
	 */
   void     push(const T& element);

    /**
	 * \fn void     remove(const T& element)
	 * \brief Remove an element of the list
	 * \param element
	 */
   void     remove(const T& element);

   /**
	 * \fn bool     exists(const T& element) const
	 * \brief Check if element is in the list
	 * \param element
	 * \return boolean
	 */
   bool     exists(const T& element) const;
private:
   std::list<T>               m_list; /*!< list*/
   mutable boost::mutex       m_mutex; /*!< mutex*/
};

#include "List.inl"