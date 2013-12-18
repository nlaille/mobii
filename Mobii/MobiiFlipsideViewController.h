//
//  MobiiFlipsideViewController.h
//  Mobii
//
//  Created by 4ppolo on 16/11/12.
//  Copyright (c) 2012 SmartBase. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MobiiFlipsideViewController;

@protocol MobiiFlipsideViewControllerDelegate
- (void)flipsideViewControllerDidFinish:(MobiiFlipsideViewController *)controller;
/**
 Save the new passcode to UserDefaults.
 @param pass the NSString value of the user input.
 @returns void
 */
- (void) savePasscode:(NSString *)pass;
@end

@interface MobiiFlipsideViewController : UIViewController

/**
 Delegate property so the FlipSideViewController is able to send the new passcode to MainViewController for storage.
 */
@property (weak, nonatomic) id <MobiiFlipsideViewControllerDelegate> delegate;

/**
 Label for confirmation when changing the passcode.
 */
@property IBOutlet UILabel* message;

- (IBAction)enterNewPassword:(id)sender;
- (IBAction)done:(id)sender;

@end
