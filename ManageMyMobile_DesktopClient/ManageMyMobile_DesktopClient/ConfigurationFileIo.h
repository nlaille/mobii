/*
** Mobii Project -- version 0.0.1000 DEVELOPMENT VERSION
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/

/*! \file */

#ifndef CONFIGURATIONFILEIO_H_
#define CONFIGURATIONFILEIO_H_

/// Registry or Plist or config file organization name.
#define CFG_ORGANIZATION_NAME "Mobii"

/// Application name, used to insert or read application specific datas.
#define CFG_PROGRAM_NAME "ClientApp"

#include <qsettings.h>
#include <quuid.h>

/*! \class ConfigurationFileIo
 *  \brief Handles configuration settings read and persistency.
 *
 *  Depending on the running platform, this class will handle persistency
 *  of application settings and return to requesting modules the desired
 *	parameters values.
 *
 *	Windows will use the Registry to store these informations, usually in
 *	HKEY_CURRENT_USER.
 *	Linux will store them in a .conf file.
 *	MacOS uses PList files.
 */
class ConfigurationFileIo
{
public:
	ConfigurationFileIo(void);
	~ConfigurationFileIo(void);

	//
	// Server connection related settings
	//

	/// \brief Read-only: returns status of arbitrary key
	/// \param	name	Name of the raw value to read from the configuration entity.
	QVariant	readValue(const QString &name) const;

	/// GUID which unically identifies a client amont others
	QByteArray	clientUid(void) const;

	/// Password token which authorizes this client to be the rightful owner of its UID.
	int			authenticationToken(void) const;

	/// Tells if the UID has been validated by the server
	bool		serverHasValidatedUid(void) const;

	/// Directory where user datas and settings are stored.
	QString		userStorageDir(void) const;

	/// Read server property: used default or user-defined settings?
	bool		serverModeCustom(void) const;

	/// Read server property: custom server hostname or IP
	QString		serverAddress(void) const;

	/// Read server property: custom TCP server connection port
	quint16		serverTcpPort(void) const;

	/// Read server property: use a SSL connection
	bool		useSsl(void) const;

	/// Write server property: GUID which unically identifies a client amont others
	void		setClientUid(QByteArray uid) const;

	/// Write: password token which authorizes this client to be the rightful owner of its UID.
	void		setAuthenticationToken(int token) const;

	/// Write: Tells if the UID has been validated by the server
	void		setServerHasValidatedUid(bool validated) const;

	/// Write server property: directory where user datas and settings are stored
	void		setUserStorageDir(const QString &) const;

	/// Write server property: used default or user-defined settings?
	void		setServerModeCustom(bool mode);
	
	/// Write server property: custom server hostname or IP
	void		setServerAddress(const QString &addr);

	/// Write server property: custom server connection port
	void		setServerTcpPort(quint16 port);

	/// Write server property: use a SSL connection
	void		setUseSsl(bool useSsl);

	/// Tells if the client should save passwords when connecting to a remote phone.
	bool		phonePasswordsAutosave(void);

	/// Tells if the client should save passwords when connecting to a remote phone.
	void		setPhonePasswordsAutosave(bool);

	/// Saved Mobii network username
	QString		savedUsername(void) const;

	/// Saved Mobii network password for the associated username
	QString		savedPassword(void) const;

	/// Tells if the client should remember the Mobii network user's credentials.
	bool		rememberCredentials(void) const;

	/// Saved Mobii network username
	void		setSavedUsername(const QString &);

	/// Saved Mobii network password for the associated username
	void		setSavedPassword(const QString &);

	/// Tells if the client should remember the Mobii network user's credentials.
	void		setRememberCredentials(bool);

	/// SMS panel: messages displayed at one in a bunch
	quint16		smsPanelNbOfMessagesToDisplay(void) const;

	/// SMS panel: messages displayed at one in a bunch
	void		setSmsPanelNbOfMessagesToDisplay(quint16 nb);

	/// TCP port on which client should listen for direct connections
	quint16		directConnectionPort(void) const;

	/// TCP port on which client should listen for direct connections
	void		setDirectConnectionPort(quint16 port);

private:
	/// Tells if the specified key exists in the settings storage facility.
	bool keyExists(const QString &);

	/// QT objects which handles platform-dependant settings persistency.
	QSettings *m_settings;
};

#endif // CONFIGURATIONFILEIO_H_