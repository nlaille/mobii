#include <cstring>

#include "MobiiOperationCodes.h"

const sInputMobiiCommands	MobiiInputCommands[] =
{
	// Server errors hadling
	{'\x00', '\xFF', &CommandsProcessor::do_handleServerError},

	// Client registration acknowledgment
	{'\x05', '\x11', &CommandsProcessor::do_handleRegistrationAck},

	// Authentication result
	//{'\x10', '\x10', &CommandsProcessor::do_processAuthSuccess},
	// Code \x10\x10 is deprecated fpr \x30\x01 (connection open)
	{'\x10', '\x11', &CommandsProcessor::do_processAuthFailure},

	// User login result
	{'\x20', '\x01', &CommandsProcessor::do_userLoginSuccess},
	{'\x20', '\x02', &CommandsProcessor::do_userLoginFailure},
	{'\x20', '\x05', &CommandsProcessor::do_mobileHistoryAction},

	// Mobile disconnected
	{'\x30', '\x00', &CommandsProcessor::do_mobileHasDisconnected},
	{'\x30', '\x01', &CommandsProcessor::do_processAuthSuccess},

	// Client <> Mobile messages exchange
	{'\x30', '\x10', &CommandsProcessor::do_getMessageFromMobile},
	{'\x30', '\x30', &CommandsProcessor::do_handleUdpAcknowledgement},

	// UDP hole punching
	{'\x40', '\x00', &CommandsProcessor::do_performUdpHolePunching},
	{'\x40', '\x01', &CommandsProcessor::do_serverHasConfirmedUdpDatagram},

	// UID registration result
	{'\x50', '\x11', &CommandsProcessor::do_uidRegistrationSuccess},
	{'\x50', '\x12', &CommandsProcessor::do_uidRegistrationFailure}
};


const sOutputMobiiCommands	MobiiOutputCommands[] =
{
	// Register client with server
	{"SRV_REGISTER_CLIENT", '\x05', '\x01'},

	// Send login informations to server
	{"SRV_SEND_CREDENTIALS", '\x20', '\x00'},

	// Send a mobile history-related request to server
	{"SRV_MOBILEHISTORY", '\x20', '\x05'},

	// Attempt connection with mobile
	{"MOB_AUTHENTICATE", '\x20', '\x10'},

	// Client <> Mobile messages exchange
	{"MOB_SENDMESSAGE", '\x30', '\x10'},

	// UDP ACK packet
	{"UDP_SENDACK", '\x30', '\x30'},

	// UDP hole punching (request)
	{"SRC_REQ_UDPHOLEPUNCHING", '\x40', '\x00'},

	// Request UID registration for this client
	{"SRV_UID_REGISTER", '\x50', '\x10'}
};


bool			MobiiOpcodes::getOpcodeFromCodename(const char *codename,
													unsigned char &dispatch,
													unsigned char &opcode)
{
	for (int i = 0; i < sizeof(MobiiOutputCommands) / sizeof(sOutputMobiiCommands); ++i)
	{
		if (::strcmp(MobiiOutputCommands[i].codename, codename) == 0)
		{
			dispatch = MobiiOutputCommands[i].dispatch;
			opcode = MobiiOutputCommands[i].opcode;
			return true;
		}
	}
	dispatch = '\x00';
	opcode = '\x00';
	return false;
}


fCommand		MobiiOpcodes::getCommandForOpcode(unsigned char dispatch,
												  unsigned char opcode)
{
	for (int i = 0; i < sizeof(MobiiInputCommands) / sizeof(sInputMobiiCommands); ++i)
	{
		if (opcode == MobiiInputCommands[i].opcode
			&& dispatch == MobiiInputCommands[i].dispatch)
			return MobiiInputCommands[i].cmd;
	}
	return NULL;
}