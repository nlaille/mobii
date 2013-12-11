#include <stdexcept>
#include <qbytearray.h>
#include <qdebug.h>
#include <qdatastream.h>

#include "MobiiOperationCodes.h"
#include "PacketBuilder.h"
#include "commons.h"


MobiiStandardPacket		PacketBuilder::buildPacketFromData(const QByteArray &datas)
{
	MobiiStandardPacket packet;
	unsigned int pos = 0;

	// Checks if datas size is sufficient
	if (datas.size() < packet.getExpectedDatasSize())
	{
		Commons::printDebugString("CAUTION: Unsufficient datas to build header", true);
		return packet;
	}

	// Fill source MAC
	for (int i = 0; i < sizeof(packet.sourceMac); ++i, ++pos)
		packet.sourceMac[i] = datas[pos];

	// Fill destination MAC
	for (int i = 0; i < sizeof(packet.destMac); ++i, ++pos)
		packet.destMac[i] = datas[pos];

	// Set dispatch code
	packet.dispatch = datas[pos++];

	// Set opcode
	packet.opcode = datas[pos++];

	// Set packet size
	QByteArray rawsize = datas.mid(pos, 4);
	QDataStream ds(rawsize);
	ds.setByteOrder(QDataStream::LittleEndian);
	ds >> packet.dataSize;

	// Note: datas filling is done separately
	return packet;
}

MobiiStandardPacket &		PacketBuilder::addAdditionnalDatasToPacket(MobiiStandardPacket &packet,
																	   const QByteArray &datas)
{
	if (datas.size() != packet.dataSize)
		Commons::printDebugString("CAUTION: Additionnal datas size mismatch.", true);

	packet.datas = datas;
	return packet;
}

const QByteArray		PacketBuilder::getRawDatasFromPacket(const MobiiStandardPacket &packet)
{
	QByteArray			result;
	
	// Extract packet
	for (int i = 0; i < sizeof(packet.sourceMac); ++i)
		result.push_back(packet.sourceMac[i]);
	for (int i = 0; i < sizeof(packet.destMac); ++i)
		result.push_back(packet.destMac[i]);
	result.push_back(packet.dispatch);
	result.push_back(packet.opcode);
	const char *datasize = (const char *)&(packet.dataSize);
	QByteArray data(datasize, sizeof(packet.dataSize));
	result.push_back(data);
	result.push_back(packet.datas);
	return result;
}

MobiiStandardPacket		PacketBuilder::prepareNewPacket(const char *codename,
														const char *sourceMac,
														const char *destMac,
														const QByteArray &datas)
{
	MobiiStandardPacket packet;
	unsigned char dispatch, opcode;

	if (MobiiOpcodes::getOpcodeFromCodename(codename, dispatch, opcode) == false)
	{
		std::string msg("Invalid operation codename specified: ");
		msg.append(codename);
		throw new std::invalid_argument(msg);
	}
	packet.dispatch = dispatch;
	packet.opcode = opcode;
	::strncpy(packet.sourceMac, sourceMac, sizeof(packet.sourceMac));
	::strncpy(packet.destMac, destMac, sizeof(packet.destMac));
	packet.dataSize = datas.size();
	packet.datas.append(datas);
	return packet;
}
