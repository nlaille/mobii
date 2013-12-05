#pragma once

/**
 * \file CommunicationManager.h
 * \brief Declaration of CommunicationManager
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#include <boost/thread.hpp>

#include <map>
#include <list>

#include "ISession.h"
#include "protocol/Packet.h"

/**
 * \class CommunicationManager
 * \brief Stack communication between Sessions
 */
class CommunicationManager
{
public:
   CommunicationManager();
   virtual ~CommunicationManager();

   /**
	* \fn void  add(ISession::session session1, ISession::session session2)
	* \brief Stack new communication between session1 and session2, throw exception if already exists
	* \param session1 
	* \param session2
	*/
   virtual void  add(ISession::session session1, ISession::session session2);

   /**
	* \fn void  close(ISession::session session1, ISession::session session2)
	* \brief close communication between session1 and session2, throw exception if doesn't exists
	* \param session1 
	* \param session2
	*/
   virtual void  close(ISession::session session1, ISession::session session2);

   /**
	* \fn void  closeAll(ISession::session session)
	* \brief close all communication which in session is related
	* \param session 
	*/
   virtual void  closeAll(ISession::session session);

   /**
	* \fn void  sendAll(ISession::session session)
	* \brief send packet to all communication which in session is related
	* \param session 
	* \param packet
	*/
   virtual void  sendAll(ISession::session session, Packet& packet);

   /**
	* \fn bool  exists(ISession::session session1, ISession::session session2) const
	* \brief Check if communication between session1 and session2 exists
	* \param session1 
	* \param session2
	* \return Existing of communication between session1 and session2
	*/
   virtual bool  exists(ISession::session session1, ISession::session session2) const;

private:
   std::list<ISession::session>& get(ISession::session session);
   void  erase(ISession::session session1, ISession::session session2);

   std::map<ISession::session, std::list<ISession::session> >  m_communications; /*!< List of open communication*/

   mutable boost::mutex    m_mutex; /*!< Mutex for control access*/
   mutable boost::mutex    m_mutexExists; /*!< Mutex for control access*/
};