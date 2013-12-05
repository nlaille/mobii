#pragma once

class MockSessionManager : public SessionManager {
public:
	MOCK_METHOD2(add,
		void(const std::string& id, ISession::session session));
	MOCK_METHOD1(get,
		ISession::session(const std::string& id) const);
	MOCK_METHOD1(exists,
		bool(const std::string& id) const);
	MOCK_METHOD2(add,
		void(IConnection::connection& connection, ISession::session session));
	MOCK_METHOD1(get,
		ISession::session(IConnection::connection& connection) const);
	MOCK_METHOD1(exists,
		bool(IConnection::connection& connection) const);
	MOCK_METHOD2(valid,
		bool(IConnection::connection& connection, const std::string& id) const);
	MOCK_METHOD1(remove,
		void(ISession::session session));
};
