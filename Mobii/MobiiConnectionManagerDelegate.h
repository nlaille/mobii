//
//  MobiiConnectionManagerDelegate.h
//  Mobii
//
//  Created by Nicolas on 12/18/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol MobiiConnectionManagerDelegate
/**
    Callback function to call on connection established with server.
 */
- (void) serverConnectionSucceded;
/**
 Callback function to call on connection closed with client.
 */
- (void) clientConnectionClosed;
/**
 Callback function to call on connection closed with server.
 */
- (void) serverConnectionClosed;
/**
 Callback function to call when new data is received.
 */
- (void) packetReceived:(NSData *) data;

@end
