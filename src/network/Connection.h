#pragma once


/**
 * \file Connection.h
 * \brief Declaration of Connection
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#include "IConnection.h"

#include "Socket.h"
#include "ConnectionError.h"

#include "safe/Queue.h"

#include "protocol/Packet.h"


/**
 * \class Connection
 * \brief Representation of Socket
 */
class Connection : public boost::enable_shared_from_this<Connection>, public IConnection
{
public:
   virtual ~Connection();

   /**
	* \fn static IConnection::connection create(Socket* socket, const std::string& remoteIp)
	* \brief Create new connection, start connection to wait for data
	* \param socket 
	* \param remoteIp
	*/
   static IConnection::connection create(Socket* socket, const std::string& remoteIp);

   /**
	* \fn void  write(const std::string& message)
	* \brief Write message to socket
	* \param message 
	*/
   virtual void  write(const std::string& message);

   /**
	* \fn void  close()
	* \brief Close connection (socket)
	*/
   virtual void  close();

   /**
	* \fn const std::string getRemoteIP() const
	* \brief return remoteIp
	* \return remoteIp 
	*/
   virtual const std::string getRemoteIP() const;

   /**
	* \fn const std::string getIP() const
	* \brief return privateIp
	* \return privateIp 
	*/
   virtual const std::string getIP() const;

   /**
	* \fn const std::string getPort() const
	* \brief return port
	* \return port 
	*/
   virtual const unsigned int getPort() const;

   /**
	* \fn void	setPort(unsigned int port)
	* \brief set port
	* \param port 
	*/
   virtual void	setPort(unsigned int port);

   /**
	* \fn void	setIP(const std::string& ip)
	* \brief set ip
	* \param port 
	*/
   virtual void	setIP(const std::string& ip);

   /**
	* \fn bool isSameNetwork(IConnection::connection connection) const
	* \brief Check if connection is in same network
	* \param connection
	* \return bool
	*/
   virtual bool isSameNetwork(IConnection::connection connection) const;
protected:	
   explicit Connection(Socket* socket, const std::string& remoteIp);
   void  waitForData();
   
private:	
   void  readHeader();
   void  handleReadHeader(const ConnectionError& error, size_t bytesRead);
   void  readData();
   void  handleReadData(const ConnectionError& error, size_t bytesRead);

   void  handleWrite(const ConnectionError& error);
   void  writeOnSocket();

   void  disconnect();

   Socket*              m_socket; /*!< Socket*/

   Queue<std::string>   m_out_datas; /*!< Queue of datas to be send*/
   std::string          m_out_data; /*!< Data to be send*/

   std::vector<char>    m_in_data; /*!< Receiver of data*/
   Packet               m_packet; /*!< packet in construction*/

   std::string			m_remoteIp; /*!< Public IP of connection*/
   std::string			m_privateIp; /*!< Private IP of connection*/
   unsigned int			m_port; /*!< Private Port of connection*/
};