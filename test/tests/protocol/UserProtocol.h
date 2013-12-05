#pragma once

#include <gtest/gtest.h>

#include <vector>

#include "protocol/UserProtocol.h"

class UserProtocolTest : public ::testing::Test
{
protected:
	void	SetUp()
	{
	}

	UserProtocol	userProtocol;
};

namespace PacketFixture
{
	namespace Empty
	{
		static const std::string HEADER = std::string("\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0", 18);
		static const std::string SOURCE_UID = std::string("\x0\x0\x0\x0\x0\x0", 6);
		static const std::string DESTINATION_UID = std::string("\x0\x0\x0\x0\x0\x0", 6);
		static const int OPCODE = 0;
		static const int DISPATCH = 0;
		static const int MESSAGE_SIZE = 0;
		static const std::vector<char>	HEADER_VECTOR = std::vector<char>(HEADER.begin(), HEADER.end());
	}
	namespace Complexe
	{
		static const std::string HEADER = std::string("\x1A\x2B\x3C\x4D\x5E\x6F\xF6\xE5\xD4\xC3\xB2\xA1\x10\x20\x1\x1\x1\x1", 18);
		static const std::string SOURCE_UID = std::string("\x1A\x2B\x3C\x4D\x5E\x6F", 6);
		static const std::string DESTINATION_UID = std::string("\xF6\xE5\xD4\xC3\xB2\xA1", 6);
		static const int OPCODE = 32;
		static const int DISPATCH = 16;
		static const int MESSAGE_SIZE = 16843009;
		static const std::vector<char>	HEADER_VECTOR = std::vector<char>(HEADER.begin(), HEADER.end());
	}
}

