/*! \file */

#ifndef NETWORKPOOL_H_
#define NETWORKPOOL_H_

#include		<qbytearray.h>
#include		<queue>

#include		"MobiiStandardPacket.h"

/*! \class NetworkPool
 *  \brief Collects all incoming datas and ensures proper Mobii data packets delivery.
 */
class			NetworkPool
{
public:
	/// Accepts fresh datas from the socket then processes them.
	void		feedWithRawDatas(const QByteArray &);

	/// Returns true if the front packet in the queue is ready.
	bool		isNextPacketReady(void);

	/// Returns the first packet in the queue.
	/// Throws an exception if the next packet isn't ready; it is your responsibility to check that.
	MobiiStandardPacket nextPacket(void);

	/// Flushes all remaining datas in the stack and the queue upon disconnect.
	void		flushAll(void);

private:
	/// Raw byte array which stores raw datas awaiting for processing.
	QByteArray						m_rawstack;

	/// Queue which contains complete packets, ready for delivery.
	std::queue<MobiiStandardPacket> m_packetQueue;

	/// Attempts to complete a pending packet in the queue
	MobiiStandardPacket &completePacketFromStack(MobiiStandardPacket &packet);

	/// Attempts to create new packets from the stack
	bool		createPacketsFromStack(void);

	/// Returns and pops the next character from the stack.
	/// Undefined behavior if the stack is empty; it is your responsibility to check that.
	char		popNextCharFromStack(void);
};

#endif // NETWORKPOOL_H_