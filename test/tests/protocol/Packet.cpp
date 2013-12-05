#include <gtest/gtest.h>

#include "Packet.h"

TEST_F(PacketTest, setEmptyHeader)
{
	emptyPacket.setHeader(PacketFixture::Empty::HEADER_VECTOR);
	EXPECT_EQ(PacketFixture::Empty::SOURCE_UID, emptyPacket.getSource());
	EXPECT_EQ(PacketFixture::Empty::DESTINATION_UID, emptyPacket.getDestination());
	EXPECT_EQ(PacketFixture::Empty::OPCODE, emptyPacket.getOpcode());
	EXPECT_EQ(PacketFixture::Empty::DISPATCH, emptyPacket.getDispatch());
	EXPECT_EQ(PacketFixture::Empty::MESSAGE_SIZE, emptyPacket.getMessageSize());
}

TEST_F(PacketTest, setComplexeHeader)
{
	complexePacket.setHeader(PacketFixture::Complexe::HEADER_VECTOR);
	EXPECT_EQ(PacketFixture::Complexe::SOURCE_UID, complexePacket.getSource());
	EXPECT_EQ(PacketFixture::Complexe::DESTINATION_UID, complexePacket.getDestination());
	EXPECT_EQ(PacketFixture::Complexe::OPCODE, complexePacket.getOpcode());
	EXPECT_EQ(PacketFixture::Complexe::DISPATCH, complexePacket.getDispatch());
	EXPECT_EQ(PacketFixture::Complexe::MESSAGE_SIZE, complexePacket.getMessageSize());
}

TEST_F(PacketTest, toString)
{
	complexePacket.setHeader(PacketFixture::Complexe::HEADER_VECTOR);
	EXPECT_EQ(PacketFixture::Complexe::HEADER, complexePacket.toString());
}

TEST_F(PacketTest, clean)
{
	complexePacket.setHeader(PacketFixture::Complexe::HEADER_VECTOR);
	complexePacket.clean();
	EXPECT_EQ(PacketFixture::Empty::HEADER, complexePacket.toString());
}