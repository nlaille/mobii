#pragma once

/**
 * \file Packet.h
 * \brief Declaration of Packet
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <string>

#include "Protocol.h"
#include "network/IConnection.h"

/**
 * \class Packet
 * \brief Representation of Packet (Protocol)
 */
class Packet
{
public:
   Packet(const Packet& packet);
   Packet();

   ~Packet(void);

   Packet&  operator=(const Packet& packet);

   void  setHeader(const std::vector<char> header);
   void  setHeader(const char header[]);
   void  setSource(const std::string& source);
   void  setDestination(const std::string& destination);
   void  setDispatch(char dispatch);
   void  setOpcode(char opcode);
   void  setMessage(const std::string& message);
      
   const std::string&              getSource() const;
   const std::string&              getDestination() const;

   char              getDispatch() const;
   char              getOpcode() const;
      
   unsigned int         getMessageSize() const;
   const std::string&   getMessage() const;

   std::string       toString();
   void              clean();

   friend std::ostream&	operator<<(std::ostream& ostream, const Packet& packet);
private:

   std::string                            m_source;
   std::string                            m_destination;
   char                                   m_dispatch;
   char                                   m_opcode;
   unsigned int                           m_messageSize;
   std::string                            m_message;
};


