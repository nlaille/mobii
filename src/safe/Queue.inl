template <typename T>
Queue<T>::Queue(void)
   : m_queue()
{
}

template <typename T>
Queue<T>::~Queue(void)
{
}

template <typename T>
void     Queue<T>::push(const T& content)
{
   boost::mutex::scoped_lock lock(this->m_mutex);

   this->m_queue.push(content);
   this->m_condition_variable.notify_one();
   lock.unlock();
}

template <typename T>
T   Queue<T>::pop(void)
{
   boost::mutex::scoped_lock  lock(this->m_mutex);
   T                          content;

   while (this->tryPop_(content) == false)
   {
      this->m_condition_variable.wait(lock);
   }
   return content;
}


template <typename T>
bool     Queue<T>::empty(void) const
{
   boost::mutex::scoped_lock lock(this->m_mutex);

   return this->m_queue.empty();
}

template <typename T>
bool     Queue<T>::tryPop_(T& content)
{
   if (this->m_queue.empty() == true)
      return false;
   content = this->m_queue.front();
   this->m_queue.pop();
   return true;
}
