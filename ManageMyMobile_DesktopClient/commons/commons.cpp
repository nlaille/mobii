/*
** Mobii Project -- version 0.0.1000 DEVELOPMENT VERSION
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/

#include <time.h>
#include <sstream>

#include "commons.h"


std::wstring			Commons::strToWidestr(const std::string &s)
{
#ifdef C_PLATFORM_TYPE_WINDOWS
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
#else
	return std::wstring(L"");
#endif
}

const std::string		Commons::getCurrentDate(void)
{
	time_t tempTime = time(NULL);
	struct tm *currTime = localtime(&tempTime);

	if (!currTime)
		return "";
	std::stringstream ss;
	ss << currTime->tm_mday << "/" << currTime->tm_mon + 1 << "/" << currTime->tm_year + 1900;
	return ss.str();
}

void					Commons::printDebugString(const std::string &str, bool toStderr)
{
#ifdef C_PLATFORM_TYPE_WINDOWS
	OutputDebugStringW(strToWidestr(str).c_str());
	if (str.length() > 0
		&& str[str.length() - 1] != '\n')
		OutputDebugStringW(strToWidestr("\r\n").c_str());
#else
	std::cout << str << std::endl;
#endif
}

Commons::S_ResponseMessage Commons::makeResponseMessage(short code, const char *message, E_ResponseMessageLevel messageLevel, short codeBis)
{
	struct Commons::S_ResponseMessage response;

	response.code = code;
	response.codeBis = codeBis;
	response.message = message;
	response.msgLevel = messageLevel;
	return response;
}

const unsigned char *	Commons::convertMacAddressToBytesArray(const std::string &input)
{
	return NULL;
}

std::string				Commons::hexToLitteralChar(const char hexChar)
{
	std::ostringstream oss;
	oss << std::hex << static_cast<int>((unsigned char)hexChar);
	std::string str = oss.str();
	if (str.size() == 1)
		str = "0" + str;
	return str;
}

std::string				Commons::hexToLitteralString(const char *hexStr,
													 unsigned int len,
													 const std::string &separator)
{
	std::string			output;
	for (int i = 0; i < len; ++i)
	{
		if (i != 0)
			output.append(separator);
		output.append(hexToLitteralChar(hexStr[i]));
	}
	return output;
}

char					Commons::charToLitteralHex(const std::string &s)
{
	int result = 0;
	std::stringstream ss;
	ss << s;
	ss >> std::hex >> result;
	return result;
}

std::string				Commons::stringToLitteralHex(const std::string &s)
{
	std::string res;
	if (s.size() % 2 != 0)
		return res;
	int i = 0;
	while (i + 2 <= s.length())
	{
		res.push_back(charToLitteralHex(s.substr(i, 2)));
		i += 2;
	}
	return res;
}