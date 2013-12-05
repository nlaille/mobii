#include "UserProtocol.h"

#include "session/ScopeLockSession.h"

#include "helper/Json.h"
#include "database/Model.h"

#include "PacketFactory.h"

#include <boost/foreach.hpp>

UserProtocol::UserProtocol(SessionManager& sessionManager, CommunicationManager& communicationManager, DatabaseConnector& databaseConnector, UserManager& userManager)
   : m_sessionManager(sessionManager), m_communicationManager(communicationManager), m_databaseConnector(databaseConnector), m_userManager(userManager)
{
   addFunc(Protocol::Opcode::User::OPEN_COMMUNICATION, &UserProtocol::askConnectionToPhone);
   addFunc(Protocol::Opcode::User::LOGIN, &UserProtocol::login);
   addFunc(Protocol::Opcode::User::CONTACT, &UserProtocol::contact);
}


UserProtocol::~UserProtocol(void)
{
}

void  UserProtocol::login(ISession::session source, ISession::session, Packet& packet)
{
	if (m_userManager.isLog(source))
		throw std::runtime_error("[UserProtocol] Session already login.");
	Packet	result;

	try
	{
		boost::property_tree::ptree	userPtree = Helper::Json::parse(packet.getMessage());
		User	userLogin;

		userLogin.email = userPtree.get<std::string>("login_name");
		userLogin.password_md5_digest = userPtree.get<std::string>("password");
		User user = m_databaseConnector.select<User, User>("SELECT id, email, password_md5_digest FROM users WHERE email = :email and password_md5_digest = :password_md5_digest", userLogin);
		m_userManager.login(source, user);
		result = PacketFactory::create(Protocol::Opcode::CLIENT_HANDLER, Protocol::Opcode::User::LOGIN_SUCCESSED);
	}
	catch (const NoDataFound& exception)
	{
		std::cerr << exception.what() << std::endl;
		result = PacketFactory::create(Protocol::Opcode::CLIENT_HANDLER, Protocol::Opcode::User::LOGIN_FAILURE);
	}
	source->sendTo(result);
}

void  UserProtocol::contact(ISession::session source, ISession::session, Packet& packet)
{
	if (!m_userManager.isLog(source))
		throw std::runtime_error("[UserProtocol] Session should be login.");
	boost::property_tree::ptree	userPtree = Helper::Json::parse(packet.getMessage());
	boost::property_tree::ptree	resultPtree;

	if (userPtree.get<std::string>("action") == "list")
		resultPtree.push_back(std::make_pair("favoris", listContacts(m_userManager.getUser(source))));
	else if (userPtree.get<std::string>("action") == "add")
		resultPtree.push_back(std::make_pair("favoris", addContacts(m_userManager.getUser(source), userPtree.get_child("favoris"))));
	else if (userPtree.get<std::string>("action") == "rename")
		resultPtree.push_back(std::make_pair("favoris", renameContacts(m_userManager.getUser(source), userPtree.get_child("favoris"))));
	else if (userPtree.get<std::string>("action") == "remove")
		resultPtree.push_back(std::make_pair("favoris", removeContacts(m_userManager.getUser(source), userPtree.get_child("favoris"))));
	resultPtree.put<std::string>("action", userPtree.get<std::string>("action"));

	Packet	result = PacketFactory::create(Protocol::Opcode::CLIENT_HANDLER, Protocol::Opcode::User::CONTACT);

	result.setMessage(Helper::Json::produce(resultPtree));
	source->sendTo(result);
}

boost::property_tree::ptree  UserProtocol::addContacts(User user, boost::property_tree::ptree favoris)
{
	boost::property_tree::ptree	json;
	BOOST_FOREACH(boost::property_tree::ptree::value_type& ptree, favoris)
	{
		boost::property_tree::ptree& favori = ptree.second;

		std::stringstream	addFavoriSQL;

		addFavoriSQL << "insert into users_smartphones(user_id, name, smartphone_id) ";
		addFavoriSQL << "select :user_id, :name, id from smartphones where uid = :uid";
		UserSmartPhone	userSmartPhone;

		userSmartPhone.name = favori.get<std::string>("name");
		userSmartPhone.uidSmartPhone = favori.get<std::string>("uid");
		userSmartPhone.userId = user.id;
		try
		{
			m_databaseConnector.insert<UserSmartPhone>(addFavoriSQL.str(), userSmartPhone);
			favori.push_back(std::make_pair("status", "succeeded"));
		}
		catch (const std::exception& exception)
		{
			std::cerr << exception.what() << std::endl;
			favori.push_back(std::make_pair("status", "failed"));
		}
		json.push_back(std::make_pair("", favori));
	}
	return json;
}

boost::property_tree::ptree  UserProtocol::renameContacts(User user, boost::property_tree::ptree favoris)
{
	boost::property_tree::ptree	json;
	BOOST_FOREACH(boost::property_tree::ptree::value_type& ptree, favoris)
	{
		boost::property_tree::ptree& favori = ptree.second;

		std::stringstream	renameFavoriSQL;

		renameFavoriSQL << "update users_smartphones set name = :name ";
		renameFavoriSQL << "where user_id = :user_id and smartphone_id = (select id from smartphones where uid = :uid)";
		UserSmartPhone	userSmartPhone;

		userSmartPhone.name = favori.get<std::string>("name");
		userSmartPhone.uidSmartPhone = favori.get<std::string>("uid");
		userSmartPhone.userId = user.id;
		try
		{
			m_databaseConnector.update<UserSmartPhone>(renameFavoriSQL.str(), userSmartPhone);
			favori.push_back(std::make_pair("status", "succeeded"));
		}
		catch (const std::exception& exception)
		{
			std::cerr << exception.what() << std::endl;
			favori.push_back(std::make_pair("status", "failed"));
		}
		json.push_back(std::make_pair("", favori));
	}
	return json;
}

boost::property_tree::ptree  UserProtocol::removeContacts(User user, boost::property_tree::ptree favoris)
{
	boost::property_tree::ptree	json;
	BOOST_FOREACH(boost::property_tree::ptree::value_type& ptree, favoris)
	{
		boost::property_tree::ptree& favori = ptree.second;

		std::stringstream	removeFavoriSQL;

		removeFavoriSQL << "delete from users_smartphones ";
		removeFavoriSQL << "where user_id = :user_id and smartphone_id = (select id from smartphones where uid = :uid)";
		UserSmartPhone	userSmartPhone;

		userSmartPhone.uidSmartPhone = favori.get<std::string>("uid");
		userSmartPhone.userId = user.id;
		try
		{
			m_databaseConnector.update<UserSmartPhone>(removeFavoriSQL.str(), userSmartPhone);
			favori.push_back(std::make_pair("status", "succeeded"));
		}
		catch (const std::exception& exception)
		{
			std::cout << exception.what() << std::endl;
			favori.push_back(std::make_pair("status", "failed"));
		}
		json.push_back(std::make_pair("", favori));
	}
	return json;
}

boost::property_tree::ptree  UserProtocol::listContacts(User user)
{
	std::stringstream	smartphonesSQL;

	smartphonesSQL << "select s.uid, us.name, s.imei, s.model, s.manufacturer ";
	smartphonesSQL << "from users u ";
	smartphonesSQL << "join users_smartphones us on u.id = us.user_id ";
	smartphonesSQL << "join smartphones s on s.id = us.smartphone_id ";
	smartphonesSQL << "where u.email = :email and u.password_md5_digest = :password_md5_digest";

	std::vector<SmartPhoneName>	smartphones = m_databaseConnector.selects<SmartPhoneName, User>(smartphonesSQL.str(), user);

	boost::property_tree::ptree json;

	for (std::vector<SmartPhoneName>::iterator it = smartphones.begin(); it != smartphones.end(); ++it)
	{
		boost::property_tree::ptree smartphoneJson;

		smartphoneJson.put<std::string>("uid", (*it).uid);
		smartphoneJson.put<std::string>("name", (*it).name);
		smartphoneJson.put<std::string>("imei", (*it).imei);
		smartphoneJson.put<std::string>("model", (*it).model);
		smartphoneJson.put<std::string>("manufacturer", (*it).manufacturer);
		json.push_back(std::make_pair("", smartphoneJson));
	}
	return json;
}

void  UserProtocol::askConnectionToPhone(ISession::session source, ISession::session destination, Packet& packet)
{
   if (!destination->is(ISession::SMARTPHONE))
      throw std::logic_error("Bad Type : destination should be [SMARTPHONE]");
   if (!m_communicationManager.exists(source, destination))
      source->sendFromTo(destination, packet);
   else
	   throw std::runtime_error("[UserProtocol] Communication already exists");
}

#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp> 

void  UserProtocol::operator()(IConnection::connection connection, Packet& packet)
{
   std::map<char, UserProtocolFunc>::const_iterator            it;
   ISession::session                                           source;
   ISession::session                                           destination;

   if (!m_sessionManager.valid(connection, packet.getSource()))
	   throw std::logic_error("[UserProtocol] Bad ID");
   it = m_funcs.find(packet.getOpcode());
   if (it == m_funcs.end())
      throw std::logic_error("[UserProtocol] Opcode doesn't exists");
   std::cout << "Open communication between [" << boost::algorithm::hex(packet.getSource()) << "]" << " and [" << packet.getDestination() << "]" << std::endl;
   source = m_sessionManager.get(packet.getSource());
   destination = m_sessionManager.get(packet.getDestination());

   if (source == destination)
	   throw std::logic_error("[UserProtocol] Same ID");
   if (!source->is(ISession::USER))
      throw std::logic_error("Bad Type : source should be [USER]");
   ScopeLockSession  lockSource(source);
   ScopeLockSession  lockDestination(destination);

   (this->*((*it).second))(source, destination, packet);
}

void  UserProtocol::addFunc(char opcode, UserProtocolFunc func)
{
   m_funcs.insert(std::make_pair(opcode, func));
}