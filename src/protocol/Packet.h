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
   /**
    * \fn Packet(const Packet& packet)
	* \brief Constructor by copy
	* \param packet
	*/
   Packet(const Packet& packet);

   /**
    * \fn Packet()
	* \brief Constructor
	*/
   Packet();

   /**
    * \fn ~Packet(void)
	* \brief Destructor
	* \param packet
	*/
   ~Packet(void);

   /**
    * \fn Packet&  operator=(const Packet& packet)
	* \brief Copy operator
	* \param packet
	* \return copy of packet
	*/
   Packet&  operator=(const Packet& packet);

   /**
    * \fn void  setHeader(const std::vector<char> header)
	* \brief Set header property from vector
	* \param header
	*/
   void  setHeader(const std::vector<char> header);

   /**
    * \fn void  setHeader(const char header[])
	* \brief Set header property from char[]
	* \param packet
	*/
   void  setHeader(const char header[]);

   /**
    * \fn void  setSource(const std::string& source)
	* \brief Set source
	* \param source
	*/
   void  setSource(const std::string& source);

   /**
    * \fn void  setDestination(const std::string& destination)
	* \brief Set destination
	* \param destination
	*/
   void  setDestination(const std::string& destination);

   /**
    * \fn void  setDispatch(char dispatch)
	* \brief Set dispatch
	* \param dispatch
	*/
   void  setDispatch(char dispatch);

   /**
    * \fn void  setOpcode(char opcode)
	* \brief Set opcode
	* \param opcode
	*/
   void  setOpcode(char opcode);

   /**
    * \fn void  setMessage(const std::string& message)
	* \brief Set message to send
	* \param message
	*/
   void  setMessage(const std::string& message);
      
   /**
    * \fn const std::string&              getSource() const
	* \brief Get source
	* \return source
	*/
   const std::string&              getSource() const;

   /**
    * \fn const std::string&              getDestination() const
	* \brief Get destination
	* \return destination
	*/
   const std::string&              getDestination() const;

   /**
    * \fn char              getDispatch() const
	* \brief Get dispatch
	* \return dispatch
	*/
   char              getDispatch() const;

   /**
    * \fn char              getOpcode() const
	* \brief Get opcode
	* \return opcode
	*/
   char              getOpcode() const;
      
   /**
    * \fn unsigned int         getMessageSize() const
	* \brief Get message size
	* \return message size
	*/
   unsigned int         getMessageSize() const;

   /**
    * \fn const std::string&   getMessage() const
	* \brief Get message
	* \return message
	*/
   const std::string&   getMessage() const;

   /**
    * \fn std::string       toString()
	* \brief Convert packet to string
	* \return string
	*/
   std::string       toString();

   /**
    * \fn void              clean()
	* \brief Clean the packet
	*/
   void              clean();

   /**
    * \fn friend std::ostream&	operator<<(std::ostream& ostream, const Packet& packet)
	* \brief Constructor by copy
	* \param ostream
	* \param packet
	* \return ostream
	*/
   friend std::ostream&	operator<<(std::ostream& ostream, const Packet& packet);
private:

   std::string                            m_source; /*!< source */
   std::string                            m_destination; /*!< destination */
   char                                   m_dispatch; /*!< dispatch */
   char                                   m_opcode; /*!< opcode */
   unsigned int                           m_messageSize; /*!< message size */
   std::string                            m_message; /*!< message */
};


