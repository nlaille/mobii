//
//  MobiiDeviceProfiler.m
//  Mobii
//
//  Created by Nicolas on 12/17/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//

#import "MobiiDeviceProfiler.h"

@implementation MobiiDeviceProfiler

@synthesize name;
@synthesize model;
@synthesize manufacturer;
@synthesize systemName;
@synthesize systemVersion;


- (id)init {
    if (self = [super init]) {
        UIDevice *device = [UIDevice currentDevice];
        name = [device name];
        model = [device model];
        manufacturer = @"Apple Inc.";
        systemName = [device systemName];
        systemVersion = [device systemVersion];
    }
    return self;
}

- (NSDictionary *) getDeviceInfos {
    NSMutableDictionary *infos = [[NSMutableDictionary alloc] init];
    
    [infos setObject:manufacturer forKey:@"manufacturer"];
    [infos setObject:name forKey:@"Device Name"];
    [infos setObject:model forKey:@"model"];
    [infos setObject:systemName forKey:@"Operating System"];
    [infos setObject:systemVersion forKey:@"Version"];
    
    return infos;
}

- (NSArray *) getSupportedFeatures {
    return [NSArray arrayWithObjects:@"contacts", @"calendar", @"files", nil];
}

@end
