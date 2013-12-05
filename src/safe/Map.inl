template <typename T, typename U>
Map<T, U>::Map()
   : m_map()
{
}

template <typename T, typename U>
Map<T, U>::~Map()
{
}

template <typename T, typename U>
void  Map<T, U>::insert(const T& key, const U& value)
{
   boost::mutex::scoped_lock lock(this->m_mutex);

   this->m_map.insert(std::pair<T, U>(key, value));
}

template <typename T, typename U>
void  Map<T, U>::erase(const T& key)
{
   boost::mutex::scoped_lock lock(this->m_mutex);

   this->m_map.erase(key);
}

template <typename T, typename U>
U&    Map<T, U>::operator[](const T& key)
{
   boost::mutex::scoped_lock lock(this->m_mutex);
   typename std::map<T, U>::iterator it = this->m_map.find(key);

   if (it != this->m_map.end())
      return (*it).second;
   throw std::logic_error("Key doesn't exists");
}

template <typename T, typename U>
bool   Map<T, U>::exists(const T& key) const
{
   boost::mutex::scoped_lock lock(this->m_mutex);

   return (this->m_map.find(key) != this->m_map.end());
}