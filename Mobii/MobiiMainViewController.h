//
//  MobiiMainViewController.h
//  Mobii
//
//  Created by 4ppolo on 16/11/12.
//  Copyright (c) 2012 SmartBase. All rights reserved.
//

#import "MobiiFlipsideViewController.h"
#import "Connection.h"
#import "ConnectionDelegate.h"

@interface MobiiMainViewController : UIViewController <MobiiFlipsideViewControllerDelegate, UITextFieldDelegate>

/**
 Connection button outlet.
 */
@property IBOutlet UIButton *connectButton;
/**
 UID Label outlet.
 */
@property IBOutlet UILabel *deviceIDLabel;
/**
 Not used yet. Will be use to give more details about the connection estates.
 */
@property IBOutlet UILabel *statusLabel;
/**
 Used as a switch to choose the connection button label.
 */
@property BOOL connected;

/**
 Display the UIDLabel with a 1second animation.
 @returns void
 */
- (void) displayUIDLabel;
/**
 Call back to change the connection button label on connection success event.
 @returns void
 */
- (void) connectionSucceded;
/**
 Call back to change the connection button label on connection failure event.
 @returns void
 */
- (void) connectionTerminated;

@end
