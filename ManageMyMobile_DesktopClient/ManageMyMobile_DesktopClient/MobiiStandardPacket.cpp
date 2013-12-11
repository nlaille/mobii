/*
** Mobii Project -- version 0.0.1000 DEVELOPMENT VERSION
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/

#include <cstring>

#include "MobiiStandardPacket.h"

MobiiStandardPacket::MobiiStandardPacket(void)
{
	// Zero fill all appropriate fields
	memset(&(this->sourceMac), 0, sizeof(MobiiStandardPacket::sourceMac));
	memset(&(this->destMac), 0, sizeof(MobiiStandardPacket::destMac));
	this->dispatch = 0;
	this->opcode = 0;
	this->dataSize = 0;
}

MobiiStandardPacket::MobiiStandardPacket(const MobiiStandardPacket &cpy)
{
	if (this != &cpy)
	{
		memcpy(this->sourceMac, cpy.sourceMac, sizeof(this->sourceMac));
		memcpy(this->destMac, cpy.destMac, sizeof(this->destMac));
		this->dispatch = cpy.dispatch;
		this->opcode = cpy.opcode;
		this->dataSize = cpy.dataSize;
		this->datas = cpy.datas;
	}
}

unsigned int		MobiiStandardPacket::getExpectedDatasSize(void)
{
	return sizeof(sourceMac)
		+ sizeof(destMac)
		+ sizeof(dispatch)
		+ sizeof(opcode)
		+ sizeof(dataSize);
}