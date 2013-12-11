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

#ifndef MOBIISTANDARDPACKET_H_
#define MOBIISTANDARDPACKET_H_

#include <qbytearray.h>

/*! \struct MobiiStandardPacket
 *  \brief Data packet built according to the Mobii protocol
 *
 *  This is the standard data packet structure for the Mobii protocol.
 *  Such packets are sent to and are received from the Mobii server, and
 *  are translated as commands for the recipient client.
 */
class MobiiStandardPacket
{
public:
	MobiiStandardPacket(void);
	MobiiStandardPacket(const MobiiStandardPacket &);

	/// Returns the expected size of a Mobii packet header.
	unsigned int	getExpectedDatasSize(void);

	/*! \brief Originator's MAC address or unique identifier.
	 *
	 *	SIZE = 6 bytes
	 */
	char sourceMac[6];

	/*! \brief Destination's MAC address or unique identifier.
	 *
	 *	SIZE = 6 bytes
	 */
	char destMac[6];

	/*! \brief Dispatcher byte
	 *
	 *	SIZE = 1 byte
	 */
	char dispatch;

	/*! \brief Operation code as specified in the MobiiOpCodes.
	 *
	 *	SIZE = 1 byte
	 */
	char opcode;

	/*! \brief Useful datas size (without this header).
	 *
	 *	SIZE = 4 bytes
	 */
	unsigned int dataSize;

	/*! \brief Useful datas themselves.
	 *
	 *	SIZE = variable, depending of the value of dataSize.
	 */
	QByteArray datas;
};

#endif // MOBIISTANDARDPACKET_H_