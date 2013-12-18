//
//  Connection.h
//  Mobii
//
//  Created by 4ppolo on 16/11/12.
//  Copyright (c) 2012 SmartBase. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CFNetwork/CFSocketStream.h>
#import "ConnectionDelegate.h"

@interface Connection : NSObject {
    
    // Read stream
    CFReadStreamRef readStream;
    bool readStreamOpen;
    NSMutableData* incomingDataBuffer;
    int packetBodySize;
    
    // Write stream
    CFWriteStreamRef writeStream;
    bool writeStreamOpen;
    NSMutableData* outgoingDataBuffer;
}

@property(nonatomic,retain) id<ConnectionDelegate> delegate;

// Initialize and store connection information until 'connect' is called
- (id)initWithHostAddress:(NSString*)host andPort:(int)port usingSSL:(BOOL)secure;

// Initialize using a native socket handle, assuming connection is open
- (id)initWithNativeSocketHandle:(CFSocketNativeHandle)nativeSocketHandle;

// Initialize using an instance of NSNetService
- (id)initWithNetService:(NSNetService*)netService;

// Connect using whatever connection info that was passed during initialization
- (BOOL)open;

// Close connection
- (void)close;

// Send network message
- (void)sendNetworkPacket:(NSData*)packet;
@end
