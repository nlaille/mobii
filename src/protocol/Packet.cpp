#include "Packet.h"

#include <boost/algorithm/hex.hpp>

Packet::Packet(const Packet& packet)
{
   *this = packet;
}

Packet::Packet()
   : m_source(Protocol::Header::EMPTY_ID), m_destination(Protocol::Header::EMPTY_ID), m_dispatch(0), m_opcode(0), m_messageSize(0), m_message()
{
}

Packet::~Packet(void)
{
}

Packet&  Packet::operator=(const Packet& packet)
{
   if (this != &packet)
   {
      this->m_source = packet.m_source;
      this->m_destination = packet.m_destination;
      this->m_dispatch = packet.m_dispatch;
      this->m_opcode = packet.m_opcode;
      this->m_messageSize = packet.m_messageSize;
      this->m_message = packet.m_message;
   }
   return *this;
}

void  Packet::setHeader(const std::vector<char> header)
{
   m_source.clear();
   std::copy(header.begin() + Protocol::Header::SOURCE, header.begin() + Protocol::Header::SOURCE + Protocol::Header::SOURCE_SIZE,
      std::back_inserter(m_source));
   m_destination.clear();
   std::copy(header.begin() + Protocol::Header::DESTINATION, header.begin() + Protocol::Header::DESTINATION + Protocol::Header::DESTINATION_SIZE,
      std::back_inserter(m_destination));
   m_dispatch = header[Protocol::Header::DISPATCH];
   m_opcode = header[Protocol::Header::OPCODE];
   m_messageSize = 0;
   for (int i = Protocol::Header::MESSAGE_SIZE + Protocol::Header::MESSAGE_SIZE_SIZE - 1; i >= Protocol::Header::MESSAGE_SIZE; --i)
   {
      m_messageSize = (m_messageSize << 8) | static_cast<unsigned char>(header[i]);
   }
   //this->m_messageSize = header[Protocol::Header::MESSAGE_SIZE];
}

void  Packet::setHeader(const char header[])
{
   std::string    stringHeader(header);

   this->m_source.clear();
   std::copy(stringHeader.begin() + Protocol::Header::SOURCE, stringHeader.begin() + Protocol::Header::SOURCE + Protocol::Header::SOURCE_SIZE,
      std::back_inserter(this->m_source));
   this->m_destination.clear();
   std::copy(stringHeader.begin() + Protocol::Header::DESTINATION, stringHeader.begin() + + Protocol::Header::DESTINATION + Protocol::Header::DESTINATION_SIZE,
      std::back_inserter(this->m_destination));
   this->m_dispatch = header[Protocol::Header::DISPATCH];
   this->m_opcode = header[Protocol::Header::OPCODE];
   for (int i = Protocol::Header::MESSAGE_SIZE + Protocol::Header::MESSAGE_SIZE_SIZE - 1; i >= Protocol::Header::MESSAGE_SIZE; --i)
   {
      m_messageSize = (m_messageSize << 8) | static_cast<unsigned char>(header[i]);
   }
   //this->m_messageSize = header[Protocol::Header::MESSAGE_SIZE];
}

void        Packet::setSource(const std::string& source)
{
   this->m_source = source;
}

void        Packet::setDestination(const std::string& destination)
{
   this->m_destination = destination;
}

void        Packet::setOpcode(char opcode)
{
   this->m_opcode = opcode;
}

void        Packet::setDispatch(char dispatch)
{
   this->m_dispatch = dispatch;
}

void  Packet::setMessage(const std::string& message)
{
   this->m_message = message;
   this->m_messageSize = this->m_message.size();
}

const std::string&                  Packet::getSource() const
{
   return this->m_source;
}

const std::string&                  Packet::getDestination() const
{
   return this->m_destination;
}


char              Packet::getOpcode(void) const
{
   return (this->m_opcode);
}

char              Packet::getDispatch() const
{
   return (this->m_dispatch);
}

unsigned int      Packet::getMessageSize(void) const
{
   return this->m_messageSize;
}

const std::string&   Packet::getMessage(void) const
{
   return this->m_message;
}

std::string       Packet::toString()
{
   std::string    string;

   string.append(this->m_source);
   string.append(this->m_destination);
   string.push_back(this->m_dispatch);
   string.push_back(this->m_opcode);
   std::copy(reinterpret_cast<char*>(&m_messageSize), reinterpret_cast<char*>(&m_messageSize) + sizeof m_messageSize,
	std::back_inserter(string));
   string.append(this->m_message);
   return string;
}

void              Packet::clean()
{
   this->m_source = Protocol::Header::EMPTY_ID;
   this->m_destination = Protocol::Header::EMPTY_ID;
   this->m_dispatch = 0;
   this->m_opcode = 0;
   this->m_messageSize = 0;
   this->m_message.clear();
}

std::ostream&	operator<<(std::ostream& ostream, const Packet& packet)
{
	ostream << "-----------------------------------------------" << std::endl;
    ostream << "packet[Source] : " << boost::algorithm::hex(packet.getSource()) << std::endl;
    ostream << "packet[Destination] : " << boost::algorithm::hex(packet.getDestination()) << std::endl;
	ostream << "packet[Dispatcher] : " << (int)packet.getDispatch() << std::endl;
    ostream << "packet[Opcode] : " << (int)packet.getOpcode() << std::endl;
    ostream << "packet[MessageSize] : " << (int)packet.getMessageSize() << std::endl;
    ostream << "packet[Message] : \"" << packet.getMessage() << "\"" << std::endl;
    ostream << "-----------------------------------------------" << std::endl;
	return ostream;
}