//
//  MobiiConnectionManager.h
//  Mobii
//
//  Created by Nicolas on 12/17/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "GCDAsyncSocket.h"
#import "GCDAsyncUdpSocket.h"
#import "MobiiConnectionManagerDelegate.h"
/**
 ConnectionManager is an object encapsuling the connection related operations,
 */
@interface MobiiConnectionManager : NSObject

@property (nonatomic, retain) NSString *serverHost;
@property (nonatomic, retain) NSString *serverTCPPort;
@property (nonatomic, retain) NSString *serverUDPPort;

@property (nonatomic, retain) NSString *clientHost;
@property (nonatomic, retain) NSString *clientTCPPort;
@property (nonatomic, retain) NSString *clientUDPPort;


@property (nonatomic, retain) GCDAsyncSocket *serverSocket;
@property (nonatomic, retain) GCDAsyncSocket *clientSocket;
@property (nonatomic, retain) GCDAsyncSocket *currentSocket;
@property (nonatomic, retain) GCDAsyncUdpSocket *udpSocket;

@property (nonatomic, retain) id <MobiiConnectionManagerDelegate> delegate;
/**
 Provide informations on the current state of the connection
 */
@property (nonatomic) BOOL isConnected;

/**
 Init function with connection details and a ConnectionManagerDelegate to notify when the connection estate change.
 @param host Ip address of the server.
 @param tcp TCP port on which to connect to the server.
 @param upd UDP port on which to connect to the server.
 @param delegate Delegate to notify when the connection state changes.
 @returns Initialized ConnectionManager object.
 */

- (id)initWithHost:(NSString *)host onTCPPort:(NSString *)tcp andUDPPort:(NSString *) udp andDelegate:(id) object;

/**
 Starts the connection with the server. Please note the connection start with SSL/TLS by default.
 @returns Returns YES if the connection succeded and FALSE if not.
 */

- (BOOL) connectToServer;
/**
 Starts the connection to a client using the details passed as parameters.
 @param host The IP adress of the client to connect to.
 @param port The TCP port of the client to connect to.
 @returns Returns YES if the connection succeded and FALSE if not.
 */

- (BOOL) connectToClient:(NSString *)host onPort:(NSString *)port;

/**
 Writes a NSData object to the currently opened socket(s).
 @param data The data to write to the socket.
 @returns void
 */

- (void) writeDataToCurrentSocket:(NSData *) data;
/**
 Reads data from the ServerSocket if connected to server.
 @returns void
 */

- (void) readDataFromServerSocket;
/**
 Reads data from the ClientSocket if connected to client.
 @returns void
 */
- (void) readDataFromClientSocket;
/**
 Reads data from the currently openned sockets.
 @returns void
 */
- (void) readDataFromCurrentSocket;

/**
 Disconnect the server socket.
 @return void
*/
- (void) disconnectFromServer;
/**
 Disconnect the client socket.
 @return void
 */
- (void) disconnectFormClient;

@end
