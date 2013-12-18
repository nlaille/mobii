//
//  MobiiFileManager.m
//  Mobii
//
//  Created by Nicolas on 12/17/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//

#import <AssetsLibrary/AssetsLibrary.h>
#import "MobiiFileManager.h"

@implementation MobiiFileManager

- (id)init {
    if (self = [super init]) {
        dateConverter = [[MobiiISO8601Converter alloc] init];
    }
    return self;
}



@end
