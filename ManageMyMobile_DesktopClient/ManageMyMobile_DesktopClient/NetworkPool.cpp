#include		<stdexcept>
#include		<exception>
#include		<qdebug.h>

#include		"NetworkPool.h"
#include		"PacketBuilder.h"


void			NetworkPool::feedWithRawDatas(const QByteArray &src)
{
	// Feed internal stack
	this->m_rawstack.push_back(src);

	// Try to complete and/or create Mobii packets
	if (!this->m_packetQueue.empty())
		this->completePacketFromStack(this->m_packetQueue.front());
	
	// Try to build new packets only if the next one in queue is ready
	if (this->isNextPacketReady() || this->m_packetQueue.empty())
	{
		while (this->createPacketsFromStack()) ;
	}
}

bool			NetworkPool::isNextPacketReady(void)
{
	if (this->m_packetQueue.empty())
		return false;
	MobiiStandardPacket packet = this->m_packetQueue.front();
	if (packet.datas.size() == packet.dataSize)
		return true;
	return false;
}

MobiiStandardPacket NetworkPool::nextPacket(void)
{
	if (!this->isNextPacketReady())
	{
		qDebug() << Q_FUNC_INFO << "There is no packet ready to be popped from the queue yet.";
		throw std::runtime_error("There is no packet ready to be popped from the queue yet.");
	}
	MobiiStandardPacket packet = this->m_packetQueue.front();
	this->m_packetQueue.pop();
	return packet;
}

MobiiStandardPacket &NetworkPool::completePacketFromStack(MobiiStandardPacket &packet)
{
	// Nothing to do if no additionnal datas are expected
	if (packet.dataSize == 0)
		return packet;

	// Attempts to add datas
	while (packet.datas.size() < packet.dataSize
		&& this->m_rawstack.size() != 0)
	{
		packet.datas.append(popNextCharFromStack());
	}
	return packet;
}

bool			NetworkPool::createPacketsFromStack(void)
{
	MobiiStandardPacket packet;

	// Do not do anything if there's not enough static datas
	if (this->m_rawstack.size() < packet.getExpectedDatasSize())
		return false;

	// Builds the packet's header
	packet = PacketBuilder::buildPacketFromData(this->m_rawstack);
	this->m_rawstack.remove(0, packet.getExpectedDatasSize());

	// Attempts to build the packets datas
	this->completePacketFromStack(packet);

	// Pushes the new packet into the queue
	this->m_packetQueue.push(packet);
	
	return (this->isNextPacketReady());
}

char			NetworkPool::popNextCharFromStack(void)
{
	char res = this->m_rawstack[0];
	this->m_rawstack.remove(0, 1);
	return (res);
}

void			NetworkPool::flushAll(void)
{
	this->m_rawstack.clear();
	while (!this->m_packetQueue.empty())
		this->m_packetQueue.pop();
}