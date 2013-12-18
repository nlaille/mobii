//
//  MobiiFileManager.h
//  Mobii
//
//  Created by Nicolas on 12/17/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MobiiISO8601Converter.h"


@interface MobiiFileManager : NSObject {
    MobiiISO8601Converter *dateConverter;
}

//- (NSMutableArray *) getDevicePictures;
//- (UIImage *) getImageFromID:(NSString *) ID;
@end
