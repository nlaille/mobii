//
//  MobiiCalendarManager.m
//  Mobii
//
//  Created by Nicolas on 12/17/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//

#import "MobiiCalendarManager.h"

@interface MobiiCalendarManager()
@property EKEventStore *eventStore;
@end

@implementation MobiiCalendarManager
@synthesize eventStore;

-(id)init {
    if (self = [super init]) {
        eventStore = [[EKEventStore alloc] init];
        dateConverter = [[MobiiISO8601Converter alloc] init];
        [self verifyCalendarAccess];
    }
    return self;
}

-(void) verifyCalendarAccess {
    
    [eventStore requestAccessToEntityType:EKEntityTypeEvent completion:^(BOOL granted, NSError *error) {
        if (granted) {
            NSLog(@"[verifyCalendarAcess]: Access granted.");
        }
        else {
            NSLog(@"[verifyCalendarAcess]: Acess Denied. Error: %@", error);
        }
    }];
}

- (NSArray *) getCalendarEvents {
    const double secondsInAYear = (60.0*60.0*24.0)*365.0;
    NSDate *start = [[NSDate alloc] initWithTimeIntervalSinceNow:-secondsInAYear];
    NSDate *end = [[NSDate alloc] initWithTimeIntervalSinceNow:secondsInAYear];
    
    NSMutableDictionary *eventsDict = [NSMutableDictionary dictionaryWithCapacity:1024];
    NSDate* currentStart = [NSDate dateWithTimeInterval:0 sinceDate:start];
    
    while ([currentStart compare:end] == NSOrderedAscending) {
        
        NSDate* currentFinish = [NSDate dateWithTimeInterval:secondsInAYear sinceDate:currentStart];
        
        if ([currentFinish compare:end] == NSOrderedDescending) {
            currentFinish = [NSDate dateWithTimeInterval:0 sinceDate:end];
        }
        NSPredicate *predicate = [eventStore predicateForEventsWithStartDate:currentStart endDate:currentFinish calendars:nil];
        [eventStore enumerateEventsMatchingPredicate:predicate
                                     usingBlock:^(EKEvent *event, BOOL *stop) {
                                         
                                         if (event) {
                                             [eventsDict setObject:event forKey:event.eventIdentifier];
                                         }
                                         
                                     }];
        currentStart = [NSDate dateWithTimeInterval:(secondsInAYear + 1) sinceDate:currentStart];
        
    }
    NSMutableArray *eventList = [[NSMutableArray alloc] init];
    
    for (EKEvent *event in eventsDict.allValues) {
        NSMutableString *location = [[NSMutableString alloc] initWithString:@""];
        NSMutableString *notes = [[NSMutableString alloc] initWithString:@""];
        if ([event location]) {
            location = [[NSMutableString alloc] initWithString:[event location]];
        }
        if ([event notes]) {
            notes = [[NSMutableString alloc] initWithString:[event notes]];
        }
        
        NSMutableDictionary *entry = [[NSMutableDictionary alloc] init];
        [entry setObject:[event eventIdentifier] forKey:@"id"];
        [entry setObject:[event title] forKey:@"title"];
        [entry setObject:location forKey:@"where"];
        [entry setObject:[dateConverter strFromISO8601:[event startDate]] forKey:@"startdate"];
        [entry setObject:[dateConverter strFromISO8601:[event endDate]] forKey:@"enddate"];
        [entry setObject:notes forKey:@"notes"];
        
        [eventList addObject:entry];
    }
    NSLog(@"[sendCalendarEvents]: Calendar events format: %@", eventList);
    return eventList;
}

- (NSDictionary *) addNewEvent:(NSArray *) data {
    
    NSMutableDictionary *event = [data objectAtIndex:0];
    EKEvent *newEvent = [EKEvent eventWithEventStore:eventStore];
    NSError *error = [[NSError alloc] init];
    NSLog(@"[addPromptedEvent]: Adding event with title: %@, start date: %@, end date: %@", [event objectForKey:@"title"], [dateConverter dateFromISO8601:[event objectForKey:@"startdate"]], [dateConverter dateFromISO8601:[event objectForKey:@"enddate"]]);
    [newEvent setCalendar:[eventStore defaultCalendarForNewEvents]];
    [newEvent setTitle:[event objectForKey:@"title"]];
    [newEvent setLocation:[event objectForKey:@"where"]];
    [newEvent setNotes:[event objectForKey:@"notes"]];
    [newEvent setStartDate:[dateConverter dateFromISO8601:[event objectForKey:@"startdate"]]];
    [newEvent setEndDate:[dateConverter dateFromISO8601:[event objectForKey:@"startdate"]]];
    if (![eventStore saveEvent:newEvent span:EKSpanThisEvent commit:YES error:&error]) {
        NSLog(@"[addPromptedEvent]: Couldn't save event to calendar: %@", error);
    }
    [event setValue:[newEvent eventIdentifier] forKey:@"id"];
    
    return event;
}

- (void) updateCalendarEvents:(NSArray *) events{
    NSError *error = [[NSError alloc] init];
    for (NSDictionary *event in events) {
        NSString *identifier = [event objectForKey:@"id"];
        NSLog(@"[updateCalendarEvents]: Updating event with identifier: %@", identifier);
        EKEvent *baseEvent = [eventStore eventWithIdentifier:identifier];
        [baseEvent setTitle:[event objectForKey:@"title"]];
        [baseEvent setLocation:[event objectForKey:@"where"]];
        [baseEvent setNotes:[event objectForKey:@"notes"]];
        [baseEvent setStartDate:[dateConverter dateFromISO8601:[event objectForKey:@"startdate"]]];
        [baseEvent setEndDate:[dateConverter dateFromISO8601:[event objectForKey:@"enddate"]]];
        if (![eventStore saveEvent:baseEvent span:EKSpanThisEvent commit:YES error:&error]) {
            NSLog(@"[updateCalendarEvents]: Couldn't save event to calendar: %@", error);
        }
    }
}

- (void) deleteEvent:(NSArray *) data {
    NSMutableDictionary *event = [[NSMutableDictionary alloc] initWithDictionary:[data objectAtIndex:0]];
    NSError *error = [[NSError alloc] init];
    NSString *identifier = [event objectForKey:@"id"];
    NSLog(@"[deleteEvent]: Deleting event identifier: %@", identifier);
    EKEvent *baseEvent = [eventStore eventWithIdentifier:identifier];
    
    if (![eventStore removeEvent:baseEvent span:EKSpanThisEvent commit:YES error:&error]) {
        NSLog(@"[deleteEvent]: Couldn't delete event: %@", error);
    }
    
}

@end
