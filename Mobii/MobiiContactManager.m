//
//  MobiiContactManager.m
//  Mobii
//
//  Created by Nicolas on 12/17/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//


#import <AddressBook/AddressBook.h>
#import <AddressBook/ABPerson.h>
#import "MobiiContactManager.h"

@implementation MobiiContactManager : NSObject

- (id)init {
    if (self = [super init]) {
        dateConverter = [[MobiiISO8601Converter alloc] init];
        [self verifyAdressBookAccess];
    }
   return self;
}


- (void) verifyAdressBookAccess {
    ABAddressBookRef addressBookRef = ABAddressBookCreateWithOptions(NULL, NULL);
    
    if (ABAddressBookGetAuthorizationStatus() == kABAuthorizationStatusNotDetermined) {
        ABAddressBookRequestAccessWithCompletion(addressBookRef, ^(bool granted, CFErrorRef error) {
            // First time access has been granted, add the contact
            //[self getAdressBookContacts];
        });
    }
    else if (ABAddressBookGetAuthorizationStatus() == kABAuthorizationStatusAuthorized) {
        // The user has previously given access, add the contact
        //[self getAdressBookContacts];
    }
    else {
        // The user has previously denied access
        // Send an alert telling user to change privacy setting in settings app
    }
}

-(NSArray *) getAdressBookContacts {
    
    ABAddressBookRef addressBook = ABAddressBookCreateWithOptions(NULL, NULL);
    CFArrayRef people = ABAddressBookCopyArrayOfAllPeople(addressBook);
    NSMutableArray *addressBookContacts = [[NSMutableArray alloc] init];
    
    //    NSMutableArray* allPeoplesDicts = [NSMutableArray array];
    for (CFIndex i = 0; i < CFArrayGetCount(people); i++)
    {
        
        ABRecordRef person = (ABRecordRef)CFArrayGetValueAtIndex(people, i);
        CFStringRef firstName = ABRecordCopyValue(person, kABPersonFirstNameProperty);
        CFStringRef lastName = ABRecordCopyValue(person, kABPersonLastNameProperty);
        CFStringRef company = ABRecordCopyValue(person, kABPersonOrganizationProperty);
        CFTypeRef birthday = ABRecordCopyValue(person, kABPersonBirthdayProperty);
        CFStringRef notes = ABRecordCopyValue(person, kABPersonNoteProperty);
        
        NSMutableArray *adressList = [[NSMutableArray alloc] init];
        
        ABMultiValueRef phones = ABRecordCopyValue(person, kABPersonPhoneProperty);
        ABMultiValueRef emails = ABRecordCopyValue(person, kABPersonEmailProperty);
        NSMutableArray* phoneList = [[NSMutableArray alloc] init];
        NSMutableArray* emailList = [[NSMutableArray alloc] init];
        
        
        for (int i = 0 ; i < ABMultiValueGetCount(emails); ++i)
        {
            CFStringRef email = ABMultiValueCopyValueAtIndex(emails, i);
            //CFStringRef label = ABMultiValueCopyLabelAtIndex(emails, i);
            // TODO: Check label compatibilty issues with clients
            // NSDictionary *entry = [[NSDictionary alloc] initWithObjectsAndKeys:(__bridge id)(email), @"value", (__bridge id)(label), @"label", nil];
            NSDictionary *entry = [[NSDictionary alloc] initWithObjectsAndKeys:(__bridge id)(email), @"value", @"Autre", @"label", nil];
            [emailList addObject:entry];
        }
        
        for (CFIndex i = 0; i < ABMultiValueGetCount(phones); i++)
        {
            CFStringRef number = ABMultiValueCopyValueAtIndex(phones,i);
            //CFStringRef label = ABMultiValueCopyLabelAtIndex(phones, i);
            // TODO: Check label compatibilty issues with clients
           // NSDictionary *entry = [[NSDictionary alloc] initWithObjectsAndKeys:(__bridge id)(number), @"value", (__bridge id)(label), @"label", nil];
            NSDictionary *entry = [[NSDictionary alloc] initWithObjectsAndKeys:(__bridge id)(number), @"value", @"Autre", @"label", nil];
            [phoneList addObject:entry];
        }

        ABMultiValueRef addresses = ABRecordCopyValue(person, kABPersonAddressProperty);
        
        for (CFIndex j = 0; j < ABMultiValueGetCount(addresses); ++j)
        {
            CFDictionaryRef addressDict = ABMultiValueCopyValueAtIndex(addresses, j);
            CFStringRef streetValue = CFDictionaryGetValue(addressDict, kABPersonAddressStreetKey);
            CFStringRef cityValue = CFDictionaryGetValue(addressDict, kABPersonAddressCityKey);
            CFStringRef stateValue = CFDictionaryGetValue(addressDict, kABPersonAddressStateKey);
            CFStringRef zipValue = CFDictionaryGetValue(addressDict, kABPersonAddressZIPKey);
            CFStringRef countryValue = CFDictionaryGetValue(addressDict, kABPersonAddressCountryKey);
            
            NSString * addStr = [[NSString alloc] initWithFormat:@"%@, %@, %@, %@. %@", (__bridge NSString*)streetValue, (__bridge NSString*)cityValue, (__bridge NSString*)stateValue, (__bridge NSString*)zipValue, (__bridge NSString*)countryValue];
            CFStringRef label = ABMultiValueCopyLabelAtIndex(addresses, j);
            // TODO: Check label compatibility issue with clients
            NSDictionary *entry = [[NSDictionary alloc] initWithObjectsAndKeys:addStr, @"value", (__bridge id)(label), @"label", nil];
            [adressList addObject:entry];
        }
        
        
        UIImage * personImage;
        if (ABPersonHasImageData(person))
        {
            personImage = [UIImage imageWithData:(__bridge NSData*)ABPersonCopyImageData(person)];
        }
        
        NSMutableDictionary* contactDict = [[NSMutableDictionary alloc] initWithObjectsAndKeys:
                                           [NSString stringWithFormat:@"%d", ABRecordGetRecordID(person)], @"id",
                                           (__bridge id)(firstName), @"firstname",
                                           (__bridge id)(lastName), @"lastname",
                                            (__bridge id)(company), @"society",
                                            (__bridge id)(notes), @"notes",
                                           [dateConverter strFromISO8601:(__bridge NSDate*)birthday], @"birthday",
                                           nil];
        [contactDict setObject:phoneList forKey:@"phoneList"];
        [contactDict setObject:emailList forKey:@"mailList"];
        [addressBookContacts addObject:contactDict];
    }
    return addressBookContacts;
}


- (NSDictionary *) addContact:(NSArray *)data {
    NSMutableDictionary *contact = [data objectAtIndex:0];
    
    ABAddressBookRef addressBook = ABAddressBookCreateWithOptions(NULL, NULL);
    ABRecordRef newContact = ABPersonCreate();
    
    CFStringRef firstname = (__bridge CFStringRef)[contact objectForKey:@"firstname"];
    CFStringRef lastname = (__bridge CFStringRef)[contact objectForKey:@"lastname"];
    CFDateRef birthday = (__bridge CFDateRef)[dateConverter dateFromISO8601:[contact objectForKey:@"birthday"]];
    CFStringRef society = (__bridge CFStringRef)[contact objectForKey:@"society"];
    CFStringRef notes = (__bridge CFStringRef)[contact objectForKey:@"notes"];
    
    ABMultiValueRef phones = ABMultiValueCreateMutable(kABMultiStringPropertyType);
    NSArray *phoneList = [contact objectForKey:@"phoneList"];
    
    ABMultiValueRef mails = ABMultiValueCreateMutable(kABMultiStringPropertyType);
    NSArray *mailList = [contact objectForKey:@"mailList"];
    
    CFErrorRef error = NULL;
    
    if (!ABRecordSetValue(newContact, kABPersonFirstNameProperty, firstname, &error)) {
        NSLog(@"[addContact]: Couldn't update the firstname property.");
    }
    if (!ABRecordSetValue(newContact, kABPersonLastNameProperty, lastname, &error)) {
        NSLog(@"[addContact]: Couldn't update the lastname property.");
    }
    if (!ABRecordSetValue(newContact, kABPersonBirthdayProperty, birthday, &error)) {
        NSLog(@"[addContact]: Couldn't update the birthday property.");
    }
    if (!ABRecordSetValue(newContact, kABPersonOrganizationProperty, society, &error)) {
        NSLog(@"[addContact]: Couldn't update the society property.");
    }
    if (!ABRecordSetValue(newContact, kABPersonNoteProperty, notes, &error)) {
        NSLog(@"[addContact]: Couldn't update the notes property.");
    }
    
    for (NSDictionary *entry in phoneList) {
        // TODO : Label Check when implemented on client
        ABMultiValueAddValueAndLabel(phones, (__bridge CFStringRef)[entry valueForKey:@"value"], kABPersonPhoneOtherFAXLabel, NULL);
    } // TO DO : Create a .plist for JSONLabel match with ABRecordLabel
    
    if (ABMultiValueGetCount(phones)) {
        if (!ABRecordSetValue(newContact, kABPersonPhoneProperty, phones, &error)) {
            NSLog(@"[addContact]: Couldn't update the phone1 property.");
        }
    }
    for (NSDictionary *entry in mailList) {
        // TODO : Label Check when implemented on client
        ABMultiValueAddValueAndLabel(mails, (__bridge CFStringRef)[entry valueForKey:@"value"], kABOtherLabel, NULL);
    }
    if (ABMultiValueGetCount(mails)) {
        if (!ABRecordSetValue(newContact, kABPersonEmailProperty, mails, &error)) {
            NSLog(@"[addContact]: Couldn't update the mail1 property.");
        }
    }
    
    if (!ABAddressBookAddRecord(addressBook, newContact, &error)) {
        NSLog(@"[addContact]: Couldn't add contact to adress book.");
    }
    if (!ABAddressBookSave(addressBook, &error)) {
        NSLog(@"[addContact]: Couldn't save adress book.");
    }
    [contact setObject:[NSString stringWithFormat:@"%d", ABRecordGetRecordID(newContact)] forKey:@"id"];
    return contact;
}

- (void) updateContact:(NSArray *) data {
    NSMutableDictionary *contact = [data objectAtIndex:0];
    
    ABRecordID identifier = [[contact objectForKey:@"id"] integerValue];
    ABAddressBookRef addressBook = ABAddressBookCreateWithOptions(NULL, NULL);
    ABRecordRef person = ABAddressBookGetPersonWithRecordID(addressBook, identifier);
    CFErrorRef error = NULL;
    
    CFStringRef firstname = (__bridge CFStringRef)[contact objectForKey:@"firstname"];
    CFStringRef lastname = (__bridge CFStringRef)[contact objectForKey:@"lastname"];
    CFDateRef birthday = (__bridge CFDateRef)[dateConverter dateFromISO8601:[contact objectForKey:@"birthday"]];
    CFStringRef society = (__bridge CFStringRef)[contact objectForKey:@"society"];
    CFStringRef notes = (__bridge CFStringRef)[contact objectForKey:@"notes"];
    
    
    NSArray *phoneList = [contact objectForKey:@"phoneList"];
    NSArray *mailList = [contact objectForKey:@"mailList"];
    
    NSLog(@"[updateContact]: Contact object %@", person);
    if (person) {
        
        NSLog(@"[updateContact]: Currently updating contact id: %@", [contact objectForKey:@"id"]);
        
        if (!ABRecordSetValue(person, kABPersonFirstNameProperty, firstname, &error)) {
            NSLog(@"[updateContact]: Couldn't update the firstname property.");
        }
        if (!ABRecordSetValue(person, kABPersonLastNameProperty, lastname, &error)) {
            NSLog(@"[updateContact]: Couldn't update the lastname property.");
        }
        if (!ABRecordSetValue(person, kABPersonOrganizationProperty, society, &error)) {
            NSLog(@"[updateContact]: Couldn't update the society property.");
        }
        if (!ABRecordSetValue(person, kABPersonBirthdayProperty, birthday, &error)) {
            NSLog(@"[updateContact]: Couldn't update the birthday property.");
        }
        if (!ABRecordSetValue(person, kABPersonNoteProperty, notes, &error)) {
            NSLog(@"[updateContact]: Couldn't update the notes property.");
        }
        
        ABMultiValueRef phones = ABMultiValueCreateMutable(kABMultiStringPropertyType);
        for (NSDictionary *entry in phoneList) {
            // TODO : Label Check when implemented on client
            ABMultiValueAddValueAndLabel(phones, (__bridge CFStringRef)[entry valueForKey:@"value"], kABPersonPhoneOtherFAXLabel, NULL);
        }
        if (!ABRecordSetValue(person, kABPersonPhoneProperty, phones, &error)) {
            NSLog(@"[updateContact]: Couldn't update the phone property.");
        }
        ABMultiValueRef mails = ABMultiValueCreateMutable(kABMultiStringPropertyType);
        for (NSDictionary *entry in mailList) {
            // TODO : Label Check when implemented on client
            ABMultiValueAddValueAndLabel(mails, (__bridge CFStringRef)[entry valueForKey:@"value"], kABPersonPhoneOtherFAXLabel, NULL);
        }
        
        if (!ABRecordSetValue(person, kABPersonEmailProperty, mails, &error)) {
            NSLog(@"[updateContact]: Couldn't update the mail property.");
        }
        
        if (!ABAddressBookSave(addressBook, &error)) {
            NSLog(@"[updateContact]: Couldn't save contact to adress.");
        }
    }
}

- (void) deleteContact:(NSArray *) data {
    NSMutableDictionary *contact = [data objectAtIndex:0];
    
    ABRecordID identifier = [[contact objectForKey:@"id"] integerValue];
    ABAddressBookRef addressBook = ABAddressBookCreateWithOptions(NULL, NULL);
    ABRecordRef person = ABAddressBookGetPersonWithRecordID(addressBook, identifier);
    CFErrorRef error = NULL;
    
    
    if (!ABAddressBookRemoveRecord(addressBook, person, &error)) {
        NSLog(@"[deleteContact]: Couldn't remove contact from address book: %@", error);
    }
    if (!ABAddressBookSave(addressBook, &error)) {
        NSLog(@"[updateContact]: Couldn't save adress book: %@", error);
    }
}

@end
