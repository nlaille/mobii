//
//  MobiiConnectionManager.m
//  Mobii
//
//  Created by Nicolas on 12/17/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//

#import "MobiiConnectionManager.h"

@implementation MobiiConnectionManager

@synthesize serverHost;
@synthesize serverTCPPort;
@synthesize serverUDPPort;

@synthesize clientHost;
@synthesize clientTCPPort;
@synthesize clientUDPPort;

@synthesize serverSocket;
@synthesize clientSocket;
@synthesize currentSocket;
@synthesize udpSocket;

@synthesize delegate;

@synthesize isConnected;

- (id)init {
    if (self = [super init]) {
        serverSocket = [[GCDAsyncSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
        udpSocket = [[GCDAsyncUdpSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
        clientSocket = [[GCDAsyncSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
    }
    return self;
}

- (id)initWithHost:(NSString *)host onTCPPort:(NSString *)tcp andUDPPort:(NSString *) udp andDelegate:(id )object {
    self = [self init];

    delegate = object;
    serverHost = host;
    serverTCPPort = tcp;
    serverUDPPort = udp;
    
    clientHost = nil;
    clientTCPPort = nil;
    clientUDPPort = nil;
    currentSocket = serverSocket;
    
    return self;
}



- (BOOL) connectToServer {
    NSError *error = nil;
    
    [serverSocket connectToHost:serverHost onPort:[serverTCPPort intValue] error:&error];
    if (error) {
        NSLog(@"[connectToServer]: Error while connecting serverSocket to host : %@", error);
        return FALSE;
    }
    [serverSocket startTLS:[NSDictionary dictionaryWithObjectsAndKeys:
                            [NSNumber numberWithBool:YES], kCFStreamSSLAllowsExpiredCertificates,
                            [NSNumber numberWithBool:YES], kCFStreamSSLAllowsAnyRoot,
                            [NSNumber numberWithBool:NO], kCFStreamSSLValidatesCertificateChain,
                            kCFNull,kCFStreamSSLPeerName,
                            nil]];
    currentSocket = serverSocket;
    [currentSocket readDataWithTimeout:-1 tag:0];
    
    return TRUE;
}

- (BOOL) connectToClient:(NSString *)host onPort:(NSString *)port {
    NSError *error = nil;
    
    clientHost = host;
    clientTCPPort = port;
    [clientSocket connectToHost:host onPort:[port intValue] error:&error];
    if (error) {
        NSLog(@"[connectToClient]: Error while connecting clientSocket to host : %@", error);
        return FALSE;
    }
    currentSocket = clientSocket;
    [currentSocket readDataWithTimeout:-1 tag:0];
    
    if (currentSocket != serverSocket) {
        [serverSocket readDataWithTimeout:-1 tag:0];
    }
    
    return TRUE;
}

- (void) writeDataToCurrentSocket:(NSData *)data {
    [currentSocket writeData:data withTimeout:-1 tag:0];
    [currentSocket readDataWithTimeout:-1 tag:0];
    
    if (currentSocket != serverSocket) {
        [serverSocket readDataWithTimeout:-1 tag:0];
    }
    
}

- (void) readDataFromServerSocket {
    [serverSocket readDataWithTimeout:-1 tag:0];
}

- (void) readDataFromClientSocket {
    [clientSocket readDataWithTimeout:-1 tag:0];
}

- (void) readDataFromCurrentSocket {
    [currentSocket readDataWithTimeout:-1 tag:0];
    if (currentSocket != serverSocket) {
        [self readDataFromServerSocket];
    }
}


- (void) disconnectFromServer {
    if ([serverSocket isConnected]) {
        [serverSocket disconnect];
    }
}

- (void) disconnectFormClient {
    if ([clientSocket isConnected]) {
        [clientSocket disconnect];
    }
}



- (void)socket:(GCDAsyncSocket *)sock didConnectToHost:(NSString *)_host port:(UInt16)_port {
    NSLog(@"[-] Socket did connect to: %@:%d .", _host, _port);
    if (isConnected == FALSE) {
        isConnected = TRUE;
    }
    currentSocket = sock;
    if (sock == serverSocket) {
    [delegate serverConnectionSucceded];
        // [delegate serverAuthentication];
    }
}

- (void)socketDidCloseReadStream:(GCDAsyncSocket *)sock {
    NSLog(@"[-] Socket did close read stream.");
}

- (void)socketDidDisconnect:(GCDAsyncSocket *)sock withError:(NSError *)error {
    NSString *source = [[NSString alloc] init];
    
    if (sock == serverSocket) {
        source = @"Server";
    }
    else {
        source = @"Client";
    }
    NSLog(@"[-] %@ socket did disconnect", source);
    if (sock == clientSocket) {
        currentSocket = serverSocket;
    [delegate clientConnectionClosed];
    }
    else {
    [delegate serverConnectionClosed];
        if (isConnected == TRUE) {
            isConnected = FALSE;
        }
    }
}

- (void)socket:(GCDAsyncSocket *)sock didWriteDataWithTag:(long)tag {
    NSString *source = [[NSString alloc] init];
    
    if (sock == serverSocket) {
        source = @"Server";
    }
    else {
        source = @"Client";
    }
    NSLog(@"[-] %@ socket did write data", source);
}

- (void)socket:(GCDAsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag {
    NSString *source = [[NSString alloc] init];
    if (sock == serverSocket) {
        source = @"Server";
    }
    else {
        source = @"Client";
    }
    NSLog(@"[-] %@ socket did read data", source);
    [delegate packetReceived:data];
}

#pragma mark ASynqUDPSocket Callbacks

- (void)udpSocket:(GCDAsyncUdpSocket *)sock didConnectToAddress:(NSData *)address {
    NSLog(@"[-] UDPSocket did connect to address:%@ .", address);
}

- (void)udpSocket:(GCDAsyncUdpSocket *)sock didNotConnect:(NSError *)error {
    NSLog(@"[-] UDPSocket did not connect.");
}

- (void)udpSocket:(GCDAsyncUdpSocket *)sock didSendDataWithTag:(long)tag {
    NSLog(@"[-] UDPSocket did send data.");
}

- (void)udpSocket:(GCDAsyncUdpSocket *)sock didReceiveData:(NSData *)data
      fromAddress:(NSData *)address
withFilterContext:(id)filterContext {
    NSLog(@"[-] UDPSocket did receive data: %@", data);
}

- (void)udpSocketDidClose:(GCDAsyncUdpSocket *)sock withError:(NSError *)error {
    
}


@end
