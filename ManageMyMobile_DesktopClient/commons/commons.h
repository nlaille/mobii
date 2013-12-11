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

#ifndef COMMONS_H
#define COMMONS_H

// Identify platform
#ifdef __WIN32__
#include <Windows.h>
#define C_PLATFORM_TYPE_WINDOWS
#elif defined _WIN32
#include <Windows.h>
#define C_PLATFORM_TYPE_WINDOWS
#elif defined __APPLE__
#define C_PLATFORM_TYPE_MACOS
#else
#define C_PLATFORM_TYPE_LINUX
#endif

// OS-specific includes
#ifdef C_PLATFORM_TYPE_WINDOWS
#include <Windows.h>
#endif

#include <iostream>
#include <sstream>
#include <exception>

///
/// Defines 
///

/// General: operation completed successfully
#define INFO_OPERATION_OKAY "OKAY"

/// Socket: socket connection successful
#define SOCKET_CONNECTION_OKAY "SOCKET_CONNECTION_OKAY"

/// Socket: disconnection successful
#define SOCKET_DISCONNECT_OKAY "SOCKET_DISCONNECT_OKAY"

/// Socket: attempted to abort connected but socket was not connected
#define SOCKET_ALREADY_DISCONNECTED_EXCEPTION "SOCKET_ALREADY_DISCONNECTED"

/// Socket: attempted to connect but another connection is already online
#define SOCKET_ALREADY_CONNECTED_EXCEPTION "SOCKET_ALREADY_CONNECTED"

#define CFG_GUARD_CONNECTION_TIMEOUT

/// Socket: attempted read/write operation on a socket which is not connected yet.
#define SOCKET_NOT_READY "Vous n'êtes pas encore connecté au serveur.\nVeuillez vous connecter puis réessayez."

#define UI_DEFAULT_STYLE "QWidget{font-family:\"Segoe UI\";font-size: 11pt;}QComboBox {border: 1px solid silver;height: 32px;}QComboBox:hover {border: 1px solid grey;}QDialog {background-color: white;}QPushButton, QToolButton {background-color: rgb(233, 240, 255);color: black;border: none;width: 80px;}QPushButton:hover, QToolButton:hover {background-color: rgb(69, 109, 255);}QLineEdit {border: 1px solid silver;height: 32px;}QPlainTextEdit {border: 1px solid silver;}QLineEdit:hover {border: 1px solid grey;}"

namespace Commons
{
	/**
	 * \enum E_MessageCode
	 * \brief Messages codes for use with S_ResponseMessage
	 *
	 * Specifies which category of modules is the response message about.
	 */
	enum E_MessageCode {
		E_MESSAGE_CODE_UNDEFINED = 0,		/*!< The domain of the message has been left unspecified. */
		E_MESSAGE_CODE_SOCKET = 1			/*!< The message is related to networking connectivity. */
	};

	/**
	 * \enum E_ResponseMessageLevel
	 * \brief Error level codes for use with S_ResponseMessage
	 *
	 * Specifies whether the message is a critical alert, or a simple warning.
	 */
	enum E_ResponseMessageLevel {
		E_RESPONSE_LEVEL_INFO = 0,			/*!< The message is just informative. */
		E_RESPONSE_LEVEL_WARNING = 1,		/*!< The message states that a possibly undesired event has occured. */
		E_RESPONSE_LEVEL_CRITICAL = 2		/*!< The message indicates that something went wrong and appropriate measures should be taken. */
	};

	/**
	 * \struct S_ResponseMessage
	 * \brief Unified response message format
	 *
	 * An easily sendable object, which can hold informations about some event.
	 * This structure is shared by all modules of the client, and should be used
	 * as often as possible.
	 */
	struct S_ResponseMessage {
		short code;							/*!< Module code number */
		short codeBis;						/*!< Optional: additional error code */
		std::string message;				/*!< String message which represents the textual explaination of the information. */
		E_ResponseMessageLevel msgLevel;	/*!< Criticity level of the message. */
	};

	/**
	 * \brief Converts ANSI strings to wide strings (std::wstring).
	 *
	 * Snippet taken right from MSDN
	 */
	std::wstring strToWidestr(const std::string &s);

	/**
	 * \brief Returns the current date as string.
	 */
	const std::string getCurrentDate(void);

	/**
	 * \brief Outputs debugging messages to the appropriate outputs.
	 *
	 * This function was created as debug outputs are not handled the same way between Linux/Unix compatible
	 * platforms and Windows, where debugging strings must be printed out using a VC++ specific method.
	 */
	void printDebugString(const std::string &str, bool toStderr = false);

	/**
	 * \brief Builds a universal response message.
	 *
	 * See S_ResponseMessage specifications for further details.
	 */
	S_ResponseMessage makeResponseMessage(short code, const char *message, E_ResponseMessageLevel messageLevel = E_RESPONSE_LEVEL_INFO, short codeBis = -1);

	/**
	 * \brief Convert string MAC addresses into hexadecimal array.
	 */
	const unsigned char *convertMacAddressToBytesArray(const std::string &input);

	/// Converts a hexadecimal char to its litteral ASCII representation.
	/// e.g.: '\x41' becomes '41'
	std::string hexToLitteralChar(const char hexChar);

	/// Converts a hexadecimal string to its litteral ASCII string representation.
	/// Basically acts as hexToLitteralChar(), but with strings.
	std::string hexToLitteralString(const char *hexStr, unsigned int len, const std::string &separator = "-");

	char		charToLitteralHex(const std::string &);
	std::string	stringToLitteralHex(const std::string &);
}

#endif // COMMONS_H
