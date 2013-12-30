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
	/**
	 * \fn UserManager()
	 * \brief Constructor
	 */
	UserManager();

	/**
	 * \fn ~UserManager()
	 * \brief Destructor
	 */
	virtual ~UserManager();

	/**
	 * \fn void	login(ISession::session session, User user)
	 * \brief Log In session for user
	 * \param session
	 * \param user
	 */
	virtual void	login(ISession::session session, User user);

	/**
	 * \fn void	login(ISession::session session, User user)
	 * \brief Log Out session
	 * \param session
	 */
	virtual void	logout(ISession::session session);

	/**
	 * \fn User	getUser(ISession::session session) const
	 * \brief Get user associated to the session
	 * \param session
	 * \return user
	 */
	virtual User	getUser(ISession::session session) const;

	/**
	 * \fn bool	isLog(ISession::session session) const
	 * \brief Check if session is log
	 * \param session
	 * \return boolean
	 */
	virtual bool	isLog(ISession::session session) const;
private:
	std::map<ISession::session, User>	m_users; /*!< users */
	mutable boost::mutex    m_mutex; /*!< mutex */
};