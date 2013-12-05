#include "UdpProtocol.h"

#include "session/ScopeLockSession.h"
#include "PacketFactory.h"

#include "helper/Json.h"

#include <boost/algorithm/hex.hpp>

UdpProtocol::UdpProtocol(SessionManager& sessionManager, CommunicationManager& communicationManager, DatabaseConnector& databaseConnector)
   : m_sessionManager(sessionManager), m_communicationManager(communicationManager), m_databaseConnector(databaseConnector)
{
   //addFunc(Protocol::Opcode::Udp::UDP_HOLE_PUNCHING, &UdpProtocol::udpHolePunching);
}


UdpProtocol::~UdpProtocol(void)
{
}

void  UdpProtocol::udpHolePunching(ISession::session source, ISession::session destination, IConnection::connection connection, Packet& packet)
{
   /*if (m_communicationManager.exists(source, destination))
   {
	   /*Packet	udpHolePunchingPacket = PacketFactory::create(Protocol::Opcode::UDP_HANDLER, Protocol::Opcode::Udp::UDP_HOLE_PUNCHING);
	   Packet	udpHolePunchingPacketAck = PacketFactory::create(Protocol::Opcode::UDP_HANDLER, Protocol::Opcode::Udp::UDP_HOLE_PUNCHING_ACK);

	   boost::property_tree::ptree json;

	   json.put<std::string>("ip", connection->getIP());
	   json.put<int>("port", connection->getPort());
	   udpHolePunchingPacket.setMessage(Helper::Json::produce(json));
       source->sendFromTo(destination, udpHolePunchingPacket);
	   source->sendTo(udpHolePunchingPacketAck);
   }
   else
	   throw std::runtime_error("[UdpProtocol] No communication");*/
}

void  UdpProtocol::operator()(IConnection::connection connection, Packet& packet)
{
   std::map<char, UdpProtocolFunc>::const_iterator            it;
   ISession::session                                           source;
   ISession::session                                           destination;

   it = m_funcs.find(packet.getOpcode());
   if (it == m_funcs.end())
      throw std::logic_error("[UdpProtocol] Opcode doesn't exists");

   boost::property_tree::ptree login = Helper::Json::parse(packet.getMessage());
   std::string					uid = login.get<std::string>("uid");

   if (boost::algorithm::unhex(uid) != packet.getSource() || !isValidUidPassword(uid, login.get<int>("password")))
	   throw std::logic_error("[UdpProtocol] uid and password doesn't match");
   source = m_sessionManager.get(packet.getSource());
   destination = m_sessionManager.get(packet.getDestination());

   ScopeLockSession  lockSource(source);
   ScopeLockSession  lockDestination(destination);

   (this->*((*it).second))(source, destination, connection, packet);
}

void  UdpProtocol::addFunc(char opcode, UdpProtocolFunc func)
{
   m_funcs.insert(std::make_pair(opcode, func));
}

bool  UdpProtocol::isValidUidPassword(const std::string& uid, int password)
{
	try
	{
		Uid	uidStruct;
		uidStruct.uid = uid;
		uidStruct.password = password;
		m_databaseConnector.select<Uid>("SELECT uid, password FROM uids WHERE uid = :uid and password = :password", uidStruct);
		return true;
	}
	catch (const NoDataFound& exception)
	{
		return false;
	}
}