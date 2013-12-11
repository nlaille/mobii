#ifndef PACKETBUILDER_H_
#define PACKETBUILDER_H_

#include <qbytearray.h>

#include "MobiiStandardPacket.h"

/*! \file */

/*! \class PacketBuilder
 *  \brief Provides utility methods to build packets from raw datas.
 */
class PacketBuilder
{
public:
	/// Transforms a raw bunch of bytes into a Mobii protocol packet.
	static MobiiStandardPacket	buildPacketFromData(const QByteArray &datas);

	/// Adds additionnal datas to the packet.
	static MobiiStandardPacket&	addAdditionnalDatasToPacket(
		MobiiStandardPacket &packet,
		const QByteArray &datas);

	/// Transforms a Mobii packet into raw bytes.
	static const QByteArray		getRawDatasFromPacket(const MobiiStandardPacket &packet);

	/// Creates a Mobii packet from the specified code name.
	/// Is used to send packets from the client to the server/mobile.
	static MobiiStandardPacket	prepareNewPacket(
		const char *codename,
		const char *sourceMac,
		const char *destMac,
		const QByteArray &datas = QByteArray());
};

#endif // PACKETBUILDER_H_