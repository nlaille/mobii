//
//  Packet.m
//  Mobii
//
//  Created by 4ppolo on 6/12/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//

#import <string.h>
#import "Packet.h"


@implementation Packet

@synthesize source;
@synthesize destination;
@synthesize dispatch;
@synthesize opcode;
@synthesize size;
@synthesize data;

//@synthesize header;
//@synthesize body;

- (id) init {
    source = malloc(6 * sizeof(char));
    destination = malloc(6 * sizeof(char));

    bzero(source, 6);
    bzero(destination, 6);
    // Where to free those two char* !?
    return self;
}

- (id) initWithSource:(unsigned char *)src toDestination:(unsigned char *)dest withAction:(unsigned char) disp andOpCode:(unsigned char)op ofSize:(int)lenght {
   
    self = [self init];
    
    memcpy(source, src, 6);
    memcpy(destination, dest, 6);
    
    dispatch = disp;
    opcode = op;
    size = lenght;
    
    return self;
}

- (id) initWithSource:(unsigned char *)src toDestination:(unsigned char *)dest withAction:(unsigned char)disp andOpCode:(unsigned char)op ofSize:(int)lenght andContent:(NSData *)content
{
    self = [self initWithSource:src toDestination:dest withAction:disp andOpCode:op ofSize:lenght];
    
    data = malloc(size * sizeof(char));
    bzero(data, size);
    memcpy(data, [content bytes], size);
    return self;
    
}


- (id) initWithRawData:(NSData *)packet {
    self = [self init];

    int tmp;
        
    memcpy(source, [packet bytes], 6);
    memcpy(destination, ([packet bytes]) + 6, 6);
    memcpy(&dispatch, ([packet bytes]) + 12, 1);
    memcpy(&opcode, ([packet bytes]) + 13, 1);
    memcpy(&tmp, ([packet bytes]) + 14, 4);
    size = tmp;
    
    data = malloc(size * sizeof(char));
    memcpy(data, ([packet bytes]) + 18, size);
    return self;
}


- (NSData *) getRawData {
    NSMutableData *rawData = [[NSMutableData alloc] init];
    
    [rawData appendBytes:source length:6];
    [rawData appendBytes:destination length:6];
    [rawData appendBytes:&dispatch length:1];
    [rawData appendBytes:&opcode length:1];
    [rawData appendBytes:&size length:4];
    [rawData appendBytes:data length:size];
    
    NSData *fixedData = [[NSData alloc] initWithBytes:[rawData bytes] length:[rawData length]];
    return fixedData;
}

- (void) display {
    
    NSLog(@"------------------");
    NSLog(@"source: %02X%02X%02X%02X%02X%02X", source[0], source[1], source[2], source[3], source[4], source[5]);
    NSLog(@"destination: %02X%02X%02X%02X%02X%02X", destination[0], destination[1], destination[2], destination[3], destination[4], destination[5]);
    NSLog(@"dispatch: %x", dispatch);
    NSLog(@"opcode: %x", opcode);
    NSLog(@"size: %d", size);
    NSMutableString *dataString = [[NSMutableString alloc] init];
    if (size > 0) {
        for (int i = 0; i < size; i++) {
            [dataString appendString:[NSString stringWithFormat:@"%X", ((unsigned char*)data)[i]]];
        }
    }
    else
        [dataString appendString:@"No data."];
    NSLog(@"data: %@", dataString);
    NSLog(@"------------------");
}

@end
