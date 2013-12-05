#pragma once

/**
 * \file UserManager.h
 * \brief Declaration of UserManager
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <map>
#include <boost/thread.hpp>

#include "session/ISession.h"
#include "Model.h"

/**
 * \class UserManager
 * \brief Stack logged User
 */
class UserManager
{
public:
	UserManager();
	virtual ~UserManager();

	virtual void	login(ISession::session session, User user);
	virtual void	logout(ISession::session session);
	virtual User	getUser(ISession::session session) const;
	virtual bool	isLog(ISession::session session) const;
private:
	std::map<ISession::session, User>	m_users;
	mutable boost::mutex    m_mutex;
};