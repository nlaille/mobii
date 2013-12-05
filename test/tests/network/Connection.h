#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "network/Connection.h"
#include "../mock/Socket.h"

namespace ConnectionFixture
{
	static const std::string	LOCAL_IP = "127.0.0.1";
	static const unsigned int	LOCAL_PORT = 52142;
	static const std::string	REMOTE_IP = "10.245.2.12";
	static const unsigned int	REMOTE_PORT = 45879;
}

class ConnectionTest : public ::testing::Test
{
protected:
	void	SetUp()
	{
		mockSocket = new MockSocket();
	}
	IConnection::connection		tcpConnection;
	MockSocket*					mockSocket;
};