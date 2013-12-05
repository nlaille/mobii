#include "PacketFactory.h"

PacketFactory::PacketFactory()
{
}

Packet PacketFactory::create(char dispatch, char opcode)
{
   Packet   packet;

   packet.setDispatch(dispatch);
   packet.setOpcode(opcode);
   return packet;
}