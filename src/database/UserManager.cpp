#include "UserManager.h"

UserManager::UserManager()
{
}

UserManager::~UserManager()
{
}

void	UserManager::login(ISession::session session, User user)
{
	boost::mutex::scoped_lock lock(m_mutex);

	m_users.insert(std::make_pair(session, user));
}

void	UserManager::logout(ISession::session session)
{
	boost::mutex::scoped_lock lock(m_mutex);

	m_users.erase(session);
}

User	UserManager::getUser(ISession::session session) const
{
	boost::mutex::scoped_lock lock(m_mutex);

	if (m_users.find(session) == m_users.end())
		throw std::runtime_error("Session not login.");
	return (*(m_users.find(session))).second;
}

bool	UserManager::isLog(ISession::session session) const
{
	boost::mutex::scoped_lock lock(m_mutex);

	return m_users.find(session) != m_users.end();
}