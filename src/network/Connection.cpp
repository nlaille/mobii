#include "Connection.h"

#include "protocol/Protocol.h"
#include "protocol/ProtocolQueue.h"
#include "protocol/PacketFactory.h"


Connection::Connection(Socket* socket, const std::string& remoteIp)
   : m_socket(socket), m_out_datas(), m_out_data(), 
   m_in_data(Protocol::Header::SIZE), m_packet(),
   m_remoteIp(remoteIp), m_privateIp(), m_port(-1)
{
}

Connection::~Connection()
{
   delete m_socket;
}

IConnection::connection Connection::create(Socket* socket, const std::string& remoteIp)
{
   Connection* connection = new Connection(socket, remoteIp);
   
   connection->waitForData();
   return IConnection::connection(connection);
}

void  Connection::waitForData()
{
   readHeader();
}

void  Connection::write(const std::string& message)
{
   m_out_datas.push(message);
   writeOnSocket();
}

void  Connection::close()
{
   m_socket->close();
}

void  Connection::writeOnSocket()
{
   if (m_out_data.empty() && !m_out_datas.empty())
   {
      m_out_data = m_out_datas.pop();
      m_socket->write(m_out_data, boost::bind(&Connection::handleWrite, this, _1));
   }
}

void  Connection::handleWrite(const ConnectionError& error)
{
   if (!error)
   {
      m_out_data.clear();
      writeOnSocket();
   }
   else
      disconnect();
}

void  Connection::handleReadHeader(const ConnectionError& error, size_t bytesRead)
{
   if (!error && bytesRead == Protocol::Header::SIZE)
   {
      m_packet.setHeader(m_in_data);

      readData();      
   }
   else
      disconnect();
}

void  Connection::readHeader()
{
   m_packet.clean();
   m_in_data.clear();
   m_in_data.resize(Protocol::Header::SIZE);
   m_socket->read(m_in_data, boost::bind(&Connection::handleReadHeader, this, _1, _2));
}

void  Connection::handleReadData(const ConnectionError& error, size_t bytesRead)
{
   if (!error)
   {
      if (bytesRead == m_packet.getMessageSize())
      {
         std::string    data;

         std::copy(m_in_data.begin(), m_in_data.begin() + bytesRead, std::back_inserter(data));
         m_packet.setMessage(data);
         ProtocolQueue::instance().push(std::make_pair(shared_from_this(), m_packet));
      }
      readHeader();
   }
   else
      disconnect();
}

void  Connection::readData()
{
   try
   {
      m_in_data.clear();
      m_in_data.resize(m_packet.getMessageSize());
      m_socket->read(m_in_data, boost::bind(&Connection::handleReadData, shared_from_this(), _1, _2));
   }
   catch (const std::bad_alloc&)
   {
      std::cerr << "[ERROR] Message size too big : " << m_packet.getMessageSize() << std::endl;
      readHeader();
   }
}

void  Connection::disconnect()
{
   Packet   packet = PacketFactory::create(Protocol::Opcode::SERVER_HANDLER, Protocol::Opcode::Server::DISCONNECT);

   ProtocolQueue::instance().push(std::make_pair(shared_from_this(), packet));
}

const std::string  Connection::getIP() const
{
   return m_privateIp;
}

const unsigned int Connection::getPort() const
{
   return m_port;
}

void	Connection::setPort(unsigned int port)
{
	m_port = port;
}

void	Connection::setIP(const std::string& ip)
{
	m_privateIp = ip;
}


const std::string Connection::getRemoteIP() const
{
	return m_remoteIp;
}

bool Connection::isSameNetwork(IConnection::connection connection) const
{
	return m_remoteIp == connection->getRemoteIP();
}