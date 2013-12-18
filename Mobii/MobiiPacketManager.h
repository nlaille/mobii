//
//  MobiiPacketManager.h
//  Mobii
//
//  Created by Nicolas on 12/17/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "MobiiMainViewController.h"
#import "MobiiConnectionManager.h"
#import "MobiiConnectionManagerDelegate.h"
#import "MobiiContactManager.h"
#import "MobiiCalendarManager.h"
#import "MobiiDeviceProfiler.h"

#import "Packet.h"

/**
 MobiiPacketManager is an object handling the packets received from the server and generate the response packets.
 Implements MobiiConnectionManagerDelegate protocol to get notified when the connection estate change.
 */
@interface MobiiPacketManager : NSObject <UIAlertViewDelegate, MobiiConnectionManagerDelegate>
/**
 ConnectionManager handles all connection operations with the server and/or a client on the same local network.
 */
@property (nonatomic, retain) MobiiConnectionManager *connectionManager;
/**
 ContactManager handles all the contact related operations.
 */
@property (nonatomic, retain) MobiiContactManager *contactManager;
/**
 CalendarManager handles all the contact related operations.
 */
@property (nonatomic, retain) MobiiCalendarManager *calendarManager;
/**
 DeviceProfiler provides information about the devices specs and the supported features.
 */
@property (nonatomic, retain) MobiiDeviceProfiler *deviceProfiler;
/**
 This delegate gives access to the MainViewController for interaction with the view.
 */
@property (nonatomic, retain) MobiiMainViewController *delegate;

/**
 firstLaunch act as a switch to create or load the UID, password and passcode.
 */
@property (nonatomic) BOOL firstLaunch;

@property (nonatomic, retain) NSString *UID;
@property (nonatomic, retain) NSData* UIDData;
@property (nonatomic, retain) NSString *password;
@property (nonatomic, retain) NSString *passCode;
/**
 ReceivedPacket stores the last received packet.
 */
@property (nonatomic, retain) Packet *receivedPacket;

/**
 Implementation of ConnectionManagerDelegate protocol.
 */
- (void) serverConnectionSucceded;
/**
 Implementation of ConnectionManagerDelegate protocol.
 */
- (void) serverConnectionClosed;
/**
 Implementation of ConnectionManagerDelegate protocol.
 */
- (void) clientConnectionClosed;
/**
 Implementation of ConnectionManagerDelegate protocol.
 */
- (void) packetReceived:(NSData *)packet;

/**
 Implementation of the FlipsideViewController protocol. Allowing the user to save a newly generated passcode.
 @param pass NSString value of the user's input.
 @returns void
 */

- (void) savePasscode:(NSString *)pass;

/**
 InitFunction with server connection details and a MainViewController delegate to notify the view controller when the data changes.
 @param host The ip address of the server
 @param tcp The tcp port on which to connection should be openened.
 @param udp The upd port on which to connection should be openened.
 @param delegate A MainViewController delegate to notify the view when the data changes.
 @returns void
 */

- (id)initWithConnexionToHost:(NSString *) host onTCPPort:(NSString *) tcp andUDPPort:(NSString *) udp withDelegate:(MobiiMainViewController *) controller;
@end
