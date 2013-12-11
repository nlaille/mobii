/*
** Mobii Project -- version 0.0.1000 DEVELOPMENT VERSION
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/

#include <qdebug.h>

#include "commons.h"
#include "ConfigurationFileIo.h"

ConfigurationFileIo::ConfigurationFileIo(void)
{
	this->m_settings = new QSettings(CFG_ORGANIZATION_NAME, CFG_PROGRAM_NAME);
}

ConfigurationFileIo::~ConfigurationFileIo(void)
{
	delete this->m_settings;
}

QVariant	ConfigurationFileIo::readValue(const QString &name) const
{
	return this->m_settings->value(name);
}

QByteArray	ConfigurationFileIo::clientUid(void) const
{
	QByteArray tmp = QByteArray::fromHex(this->m_settings->value("global/uid", "").toByteArray());
	//qDebug() << "Retrieved client UID is" << Commons::hexToLitteralString(tmp.data(), tmp.length()).c_str();
	return tmp;
}

int			ConfigurationFileIo::authenticationToken(void) const
{
	return this->m_settings->value("global/token", 0).toInt();
}

bool		ConfigurationFileIo::serverHasValidatedUid(void) const
{
	return this->m_settings->value("global/uidvalidated", false).toBool();
}

QString		ConfigurationFileIo::userStorageDir(void) const
{
	QString s = this->m_settings->value("global/stordir", "").toString();
	return s;
}

bool		ConfigurationFileIo::serverModeCustom(void) const
{
	return this->m_settings->value("server/mode", false).toBool();
}

QString		ConfigurationFileIo::serverAddress(void) const
{
	return this->m_settings->value("server/address", "").toString();
}

quint16		ConfigurationFileIo::serverTcpPort(void) const
{
	return this->m_settings->value("server/port", 0).toInt();
}

bool		ConfigurationFileIo::useSsl(void) const
{
	return this->m_settings->value("server/ssl", 0).toBool();
}

QString		ConfigurationFileIo::savedUsername(void) const
{
	return this->m_settings->value("account/username", "").toString();
}

QString		ConfigurationFileIo::savedPassword(void) const
{
	return this->m_settings->value("account/password", "").toString();
}

bool		ConfigurationFileIo::rememberCredentials(void) const
{
	return this->m_settings->value("account/remember", true).toBool();
}

void		ConfigurationFileIo::setClientUid(QByteArray uid) const
{
	QString output;
	if (uid.length() != 6)
	{
		qDebug() << "Unconsistent UID size:" << uid.length() << "bytes";
		output = QString("\x00\x00\x00\x00\x00\x00");
	}
	else
		output = QString(uid.toHex());
	this->m_settings->setValue("global/uid", output);
}

void		ConfigurationFileIo::setAuthenticationToken(int token) const
{
	this->m_settings->setValue("global/token", token);
}

void		ConfigurationFileIo::setServerHasValidatedUid(bool validated) const
{
	this->m_settings->setValue("global/uidvalidated", validated);
}

void		ConfigurationFileIo::setUserStorageDir(const QString &dir) const
{
	this->m_settings->setValue("global/stordir", dir);
}

void		ConfigurationFileIo::setServerModeCustom(bool mode)
{
	this->m_settings->setValue("server/mode", mode);
}

void		ConfigurationFileIo::setServerAddress(const QString &addr)
{
	this->m_settings->setValue("server/address", addr);
}

void		ConfigurationFileIo::setServerTcpPort(const quint16 port)
{
	this->m_settings->setValue("server/port", port);
}

void		ConfigurationFileIo::setUseSsl(bool useSsl)
{
	this->m_settings->setValue("server/ssl", useSsl);
}

void		ConfigurationFileIo::setSavedUsername(const QString &username)
{
	this->m_settings->setValue("account/username", username);
}

void		ConfigurationFileIo::setSavedPassword(const QString &password)
{
	this->m_settings->setValue("account/password", password);
}

void		ConfigurationFileIo::setRememberCredentials(bool remember)
{
	this->m_settings->setValue("account/remember", remember);
}

quint16		ConfigurationFileIo::smsPanelNbOfMessagesToDisplay(void) const
{
	return this->m_settings->value("panels/sms/displaySize", 5).toUInt();
}

void		ConfigurationFileIo::setSmsPanelNbOfMessagesToDisplay(quint16 nb)
{
	this->m_settings->setValue("panels/sms/displaySize", nb);
}

bool		ConfigurationFileIo::phonePasswordsAutosave(void)
{
	return this->m_settings->value("general/allowPhonePasswordsAutosave", true).toBool();
}

void		ConfigurationFileIo::setPhonePasswordsAutosave(bool allow)
{
	this->m_settings->setValue("general/allowPhonePasswordsAutosave", allow);
}

quint16		ConfigurationFileIo::directConnectionPort(void) const
{
	return this->m_settings->value("general/directConnectionPort", 53789).toUInt();
}

void		ConfigurationFileIo::setDirectConnectionPort(quint16 port)
{
	this->m_settings->setValue("general/directConnectionPort", port);
}