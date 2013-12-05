#include "CommunicationManager.h"

CommunicationManager::CommunicationManager()
   : m_communications()
{
}

CommunicationManager::~CommunicationManager()
{
}

void  CommunicationManager::add(ISession::session session1, ISession::session session2)
{
   boost::mutex::scoped_lock lock(m_mutex);

   if (exists(session1, session2))
      throw std::logic_error("Communication already active.");
   get(session1).push_back(session2);
   get(session2).push_back(session1);
}

void  CommunicationManager::close(ISession::session session1, ISession::session session2)
{
   boost::mutex::scoped_lock lock(m_mutex);

   if (!exists(session1, session2))
      throw std::logic_error("Communication already off.");
   erase(session1, session2);
   erase(session2, session1);
}

void  CommunicationManager::closeAll(ISession::session session)
{
   boost::mutex::scoped_lock lock(m_mutex);

   std::list<ISession::session> sessions = get(session);

   for (std::list<ISession::session>::iterator it = sessions.begin();
      it != sessions.end(); ++it)
   {
      erase((*it), session);
   }
   sessions.clear();
   m_communications.erase(session);
}

void  CommunicationManager::sendAll(ISession::session session, Packet& packet)
{
   boost::mutex::scoped_lock lock(m_mutex);
   std::list<ISession::session>& sessions = get(session);

   for (std::list<ISession::session>::iterator it = sessions.begin();
      it != sessions.end(); ++it)
   {
      session->sendFromTo((*it), packet);
   }
}

bool  CommunicationManager::exists(ISession::session session1, ISession::session session2) const
{
   boost::mutex::scoped_lock lock(m_mutexExists);
   std::map<ISession::session, std::list<ISession::session> >::const_iterator it = m_communications.find(session1);

   if (it == m_communications.end())
      return false;
   for (std::list<ISession::session>::const_iterator itt = (*it).second.begin();
      itt != (*it).second.end(); ++itt)
   {
	   if ((*itt) == session2)
         return true;
   }
   return false;
}

std::list<ISession::session>& CommunicationManager::get(ISession::session session)
{
   if (m_communications.find(session) == m_communications.end())
      m_communications.insert(std::make_pair(session, std::list<ISession::session>()));
   return (*(m_communications.find(session))).second;
}

void  CommunicationManager::erase(ISession::session session1, ISession::session session2)
{
   std::list<ISession::session>& sessions = get(session1);
   
   sessions.remove(session2);

   if (sessions.size() == 0)
      m_communications.erase(session1);
}