//
//  MobiiDeviceProfiler.h
//  Mobii
//
//  Created by Nicolas on 12/17/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//

#import <Foundation/Foundation.h>
/**
 MobileDeviceProfiler contains many informations about the used device.
 */
@interface MobiiDeviceProfiler : NSObject

@property (nonatomic, retain) NSString *name;
@property (nonatomic, retain) NSString *model;

@property (nonatomic, retain) NSString *manufacturer;
@property (nonatomic, retain) NSString *systemName;
@property (nonatomic, retain) NSString *systemVersion;

/**
 Get the device hardware informations as a NSDictionary.
 @returns Returns a dictionary containing the following keys: @"manufacturer"; @"Device Name"; @"model"; @"Operating System"; @"Version" and the matching values.
 */
- (NSDictionary *) getDeviceInfos;

/**
 Get the supported Mobii Features.
 @returns Returns an NSArray with a list of the functionalities to offer on the client app.
 */
- (NSArray *) getSupportedFeatures;

@end
