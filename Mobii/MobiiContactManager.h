//
//  MobiiContactManager.h
//  Mobii
//
//  Created by Nicolas on 12/17/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MobiiISO8601Converter.h"
/**
 ContactManager is an object that handles all contact related operations.
 */
@interface MobiiContactManager : NSObject {
    MobiiISO8601Converter *dateConverter;
}

/**
 Get the user contacts list.
 @returns Returns an array of dictionary. Each dictionary corresponding to a single contact. The dictionary contains keys as: @"id", @"firstname", @"lastname", @"birthday", @"society", @"phoneList", @"mailList", @"notes".
 */

- (NSArray *) getAdressBookContacts;
/**
 Add contacts to the users contactBook.
 @param data Deserialized contacts to add to the users contactBook
 @returns Returns a dictionary describing the newly created contact.
 */

- (NSDictionary *) addContact:(NSArray *) data;
/**
 Update contacts in the user contactBook.
 @param data Deserialized contacts to update from the client app.
 */

- (void) updateContact:(NSArray *) data;
/**
 Delete contacts in the user contactBook.
 @param data Deserialized contacts to delete.
 */

- (void) deleteContact:(NSArray *) data;

@end
