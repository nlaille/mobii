/**
 * \file Protocol.h
 * \brief Declaration of protocol variables
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#pragma once

#include <string>

namespace Protocol
{
   namespace Header
   {
      // SOURCE + DESTINATION + DISPATCH + OPCODE + MESSAGE_SIZE
      static const int  SIZE = 6 + 6 + 1 + 1 + 4;
      enum {
         SOURCE=0,
         SOURCE_SIZE=6,
         DESTINATION=6,
         DESTINATION_SIZE=6,
         DISPATCH=12,
         OPCODE=13,
         MESSAGE_SIZE=14,
         MESSAGE_SIZE_SIZE=4
      };
      static const std::string EMPTY_ID("\x0\x0\x0\x0\x0\x0", 6);
   }
   namespace Opcode
   {
      enum {
         SERVER_HANDLER='\x00',
         REGISTER_HANDLER='\x05',
         PHONE_HANDLER='\x10',
         CLIENT_HANDLER='\x20',
         COMMUNICATION_HANDLER='\x30',
		 UDP_HANDLER='\x40',
		 DATABASE_HANDLER='\x50',
      };
      namespace Server
      {
         enum {
            CONNECT='\x00',
            DISCONNECT='\x01',
			EXCEPTION='\xff'
         };
      }
      namespace Register
      {
         enum {
            SMARTPHONE='\x00',
            USER='\x01',
			REGISTERED='\x11',
            UNABLE_TO_REGISTER='\xF0'
         };
      }
      namespace SmartPhone
      {
         enum {
            ACCEPT_COMMUNICATION='\x10',
            DECLINE_COMMUNICATION='\x11'
         };
      }
      namespace User
      {
         enum {
			LOGIN='\x00',
			LOGIN_SUCCESSED='\x01',
			LOGIN_FAILURE='\x02',
			CONTACT='\x05',
            OPEN_COMMUNICATION='\x10'
         };
      }
      namespace Communication
      {
         enum {
            CLOSE_COMMUNICATION='\x00',
            OPEN_COMMUNICATION='\x01',
            REDIRECT_PACKET='\x10',
         };
      }
	  namespace Database
	  {
		  enum {
			  UID='\x10',
			  UID_CREATED='\x11',
			  UID_ALREADY_USED='\x12'
		  };
	  }
   }
}