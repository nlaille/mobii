#include "Connection.h"

TEST_F(ConnectionTest, getters)
{
	tcpConnection = Connection::create(mockSocket, ConnectionFixture::REMOTE_IP, ConnectionFixture::REMOTE_PORT, ConnectionFixture::LOCAL_IP, ConnectionFixture::LOCAL_PORT);
	EXPECT_EQ(ConnectionFixture::REMOTE_IP, tcpConnection->getIP());
	EXPECT_EQ(ConnectionFixture::REMOTE_PORT, tcpConnection->getPort());
	EXPECT_EQ(ConnectionFixture::LOCAL_IP, tcpConnection->getLocalIP());
	EXPECT_EQ(ConnectionFixture::LOCAL_PORT, tcpConnection->getLocalPort());
}

TEST_F(ConnectionTest, exceptRead)
{
	EXPECT_CALL(*mockSocket, read(::testing::_, ::testing::_)).Times(1);
	tcpConnection = Connection::create(mockSocket, ConnectionFixture::REMOTE_IP, ConnectionFixture::REMOTE_PORT, ConnectionFixture::LOCAL_IP, ConnectionFixture::LOCAL_PORT);
	
}