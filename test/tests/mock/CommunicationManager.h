#pragma once

class MockCommunicationManager : public CommunicationManager {
	MOCK_METHOD2(add,
		void(ISession::session session1, ISession::session session2));
	MOCK_METHOD2(close,
		void(ISession::session session1, ISession::session session2));
	MOCK_METHOD1(closeAll,
		void(ISession::session session));
	MOCK_METHOD2(sendAll,
		void(ISession::session session, Packet& packet));
	MOCK_METHOD2(exists,
		bool(ISession::session session1, ISession::session session2) const);
};
