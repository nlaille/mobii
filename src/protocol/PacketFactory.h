#pragma once

/**
 * \file PacketFactory.h
 * \brief Declaration of PacketFactory
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include "Packet.h"

/**
 * \class PacketFactory
 * \brief Create Packet
 */
class PacketFactory
{
public:
   static Packet create(char dispatch, char opcode);
private:
   explicit PacketFactory();
};