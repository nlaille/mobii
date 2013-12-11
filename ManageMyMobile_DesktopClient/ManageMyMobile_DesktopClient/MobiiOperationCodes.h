#ifndef MOBIIOPERATIONCODES_H_
#define MOBIIOPERATIONCODES_H_

#include "commons.h"
#include "MobiiStandardPacket.h"
#include "CommandsProcessor.h"

/*! \file */

typedef void (CommandsProcessor::*fCommand)(const MobiiStandardPacket &);

/// Matches an operation code as sent by the server to a command in the commands processor.
struct sInputMobiiCommands
{
	/// \brief Dispatch code for the operation code.
	/// See the client-server protocol documentation for further details.
	unsigned char	dispatch;

	/// \brief Operation code itself.
	/// See the client-server protocol documentation for further details.
	unsigned char	opcode;

	/// \brief Pointer to the class method to which this dispatch/opcode pair is associated.
	/// Must be a method provided within CommandsProcessor.
	fCommand		cmd;
};

/// Matches an procedure code name with its associated operation code, allowing to create a Mobii packet for sending.
struct sOutputMobiiCommands
{
	/// Explicit name of the event, which CommunicationServices will use to create the Mobii packet.
	char *			codename;

	/// \brief Dispatch code for the operation code.
	/// See the client-server protocol documentation for further details.
	unsigned char	dispatch;

	/// \brief Operation code itself.
	/// See the client-server protocol documentation for further details.
	unsigned char	opcode;
};

/// Helper functions for Mobii operation codes.
namespace MobiiOpcodes
{
	/// \brief Returns the operation code associated with the given procedure name.
	/// Used for output operations.
	bool			getOpcodeFromCodename(const char *codename,
										unsigned char &dispatch,
										unsigned char &opcode);

	/// \brief Returns the command pointer for the given operation code.
	/// Used for input operations.
	fCommand		getCommandForOpcode(unsigned char dispatch,
										unsigned char opcode);
}

#endif // MOBIIOPERATIONCODES_H_