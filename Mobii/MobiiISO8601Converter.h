//
//  MobiiISO8601Converter.h
//  Mobii
//
//  Created by Nicolas on 12/17/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MobiiISO8601Converter : NSObject

- (NSString *) strFromISO8601:(NSDate *) date;
- (NSDate *) dateFromISO8601:(NSString *) string;

@end
