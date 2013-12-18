//
//  ConnectionDelegate.h
//  Mobii
//
//  Created by 4ppolo on 16/11/12.
//  Copyright (c) 2012 SmartBase. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Packet.h"

@class Connection;

@protocol ConnectionDelegate

- (void) connectionAttemptFailed:(Connection*)connection;
- (void) connectionAttemptSucceed:(Connection*)connection;
- (void) connectionTerminated:(Connection*)connection;
- (void) receivedNetworkPacket:(NSData*)data viaConnection:(Connection*)connection;

@end
