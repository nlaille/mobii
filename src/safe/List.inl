#include <algorithm>

template <typename T>
List<T>::List(void)
   : m_list()
{
}

template <typename T>
List<T>::~List(void)
{
}

template <typename T>
void     List<T>::push(const T& element)
{
   boost::mutex::scoped_lock lock(m_mutex);

   m_list.push_back(element);
}

template <typename T>
void     List<T>::remove(const T& element)
{
   boost::mutex::scoped_lock  lock(m_mutex);

   m_list.remove(element);
}

template <typename T>
bool     List<T>::exists(const T& element) const
{
   boost::mutex::scoped_lock  lock(m_mutex);

   return (std::find(m_list.begin(), m_list.end(), element) != m_list.end());
}
