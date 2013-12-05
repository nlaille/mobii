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
   Map();
   ~Map();

   void  insert(const T&, const U&);
   void  erase(const T&);
   U&    operator[](const T&);

   bool  exists(const T&) const;
private:
   std::map<T, U>             m_map;
   mutable boost::mutex       m_mutex;
   boost::condition_variable  m_condition_variable;
};

#include "Map.inl"
