//
//  Packet.h
//  Mobii
//
//  Created by 4ppolo on 6/12/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 The Packet object encapsulate the data sent to the server and/or client with a header and a body.
 */
@interface Packet : NSObject

/**
 A 6 bytes long string that contains the sender UID.
 */
@property unsigned char *source;
/**
 A 6 bytes long string that contains the receiver UID.
 */
@property unsigned char *destination;
/**
A char that contains the dispatch code of the packet.
 */
@property unsigned char dispatch;
/**
 A char that contains the opcode of the packet.
 */
@property unsigned char opcode;
/**
An integer that contains the (full)? size of the packet.
 */
@property unsigned int size;
/**
 The actual body of the packet.
 */
@property void *data;

/**
 Init function using the parameters to create a packet that contains a header only.
 @param src UID of the sender.
 @param dest UID of the receiver.
 @param disp Dispatch code of the packet.
 @param op Opcode of the packet.
 @param lenght Size of the packet.
 @returns Intialized Packet Object.
 */

- (id) initWithSource:(unsigned char *)src toDestination:(unsigned char *)dest withAction:(unsigned char)disp andOpCode:(unsigned char)op ofSize:(int)lenght;
/**
 Init function using the parameters to create a packet that contains a header and its body.
 @param src UID of the sender.
 @param dest UID of the receiver.
 @param disp Dispatch code of the packet.
 @param op Opcode of the packet.
 @param lenght Size of the packet.
 @param content The actual data to send.
 @returns Intialized Packet Object.
 */

- (id) initWithSource:(unsigned char *)src toDestination:(unsigned char *)dest withAction:(unsigned char)disp andOpCode:(unsigned char)op ofSize:(int)lenght andContent:(NSData *) content;
/**
 Create a packet from a NSData by parsing it's header and recreating the Packet object.
 @param packet Binary data containing header and body of the wanted Packet object.
 @returns Initialized Packet Object.
 */

- (id) initWithRawData:(NSData *) packet;

/**
 Get the packet object (header + body) as binary data.
 @returns A binary version of the current Packet.
 */

- (NSData *) getRawData;

/**
 Display the header and the body of a packet.
 @returns void
 */
- (void) display;

@end
