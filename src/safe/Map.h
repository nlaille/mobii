#pragma once

/**
 * \file Map.h
 * \brief Declaration of Map<T, U>
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <map>
#include <boost/thread.hpp>

/**
 * \class List<T, U>
 * \brief Map thread safe
 */
template<typename T, typename U>
class Map
{
public:
	/**
	 * \fn Map()
	 * \brief Constructor
	 */
   Map();

    /**
	 * \fn ~Map()
	 * \brief Destructor
	 */
   ~Map();

   /**
	 * \fn void  insert(const T&, const U&)
	 * \brief Insert a pair of Key/Value
	 * \param key
	 * \param value
	 */
   void  insert(const T& key, const U& value);

   /**
	 * \fn void  erase(const T&)
	 * \brief Erase pair from key
	 * \param key
	 */
   void  erase(const T& key);

   /**
	 * \fn U&    operator[](const T&)
	 * \brief Get value of pair 
	 * \param key
	 * \return value
	 */
   U&    operator[](const T& key);

   /**
	 * \fn bool  exists(const T&) const
	 * \brief Check if key exists
	 * \param key
	 */
   bool  exists(const T& key) const;
private:
   std::map<T, U>             m_map; /*!< map */
   mutable boost::mutex       m_mutex; /*!< mutex */
   boost::condition_variable  m_condition_variable; /*!< condition_variable */
};

#include "Map.inl"
