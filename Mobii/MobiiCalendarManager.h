//
//  MobiiCalendarManager.h
//  Mobii
//
//  Created by Nicolas on 12/17/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <EventKit/EventKit.h>
#import "MobiiISO8601Converter.h"
/**
 CalendarManager is an object handling all the calendar related operations.
 */
@interface MobiiCalendarManager : NSObject {
    MobiiISO8601Converter *dateConverter;
    
}
/**
 Check the application permissions on the Calendar. Notifies the user to allow access to its calendar.
 @returns void.
 */

-(void) verifyCalendarAccess;
/**
 Get the users calendar event list.
 @returns Returns an array of dictionary. Each dictionary corresponding to a single Event. The dictionary contains keys as: @"id"; @"title"; @"where"; @"startdate"; @"enddate"; @"notes"
 */
- (NSArray *) getCalendarEvents;

/**
 Add events to the users calendar.
 @param data The deserialized events sent from client.
 @returns Returns a NSDictionary describing the newly created event.
 */
- (NSDictionary *) addNewEvent:(NSArray *) data;
/**
 Update events in the users calendar.
 @param data The deserialized events sent from client.
 @returns void
 */

- (void) updateCalendarEvents:(NSArray *) events;
/**
 Delete an event from the user calendar.
 @param data The deserialized event to delete.
 @returns void
 */

- (void) deleteEvent:(NSArray *) data;
@end
