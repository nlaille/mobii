//
//  MobiiISO8601Converter.m
//  Mobii
//
//  Created by Nicolas on 12/17/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//

#import "MobiiISO8601Converter.h"

@implementation MobiiISO8601Converter

- (NSString *) strFromISO8601:(NSDate *) date {
    static NSDateFormatter* sISO8601 = nil;
    
    if (!sISO8601) {
        sISO8601 = [[NSDateFormatter alloc] init];
        NSMutableString *strFormat = [NSMutableString stringWithString:@"yyyy'-'MM'-'dd'T'HH':'mm':'ssZZZZZ"];
        
        [sISO8601 setTimeStyle:NSDateFormatterFullStyle];
        [sISO8601 setDateFormat:strFormat];
    }
    return [sISO8601 stringFromDate:date];
}

- (NSDate *) dateFromISO8601:(NSString *) string {
    
    NSDateFormatter *dateFormater = [[NSDateFormatter alloc] init];
    [dateFormater setDateFormat:@"yyyy'-'MM'-'dd'T'HH':'mm':'ssZZZZZ"];
    
    return [dateFormater dateFromString:string];
}


@end
