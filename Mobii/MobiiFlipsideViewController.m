//
//  MobiiFlipsideViewController.m
//  Mobii
//
//  Created by 4ppolo on 16/11/12.
//  Copyright (c) 2012 SmartBase. All rights reserved.
//

#import <CommonCrypto/CommonDigest.h>

#import "MobiiFlipsideViewController.h"

@interface MobiiFlipsideViewController ()

@property NSString *passcode;
@property BOOL passcodeChanged;
@end

@implementation MobiiFlipsideViewController

@synthesize passcode;
@synthesize passcodeChanged;
@synthesize message;

- (void)viewDidLoad
{
    [super viewDidLoad];
    passcodeChanged = NO;
    [message setText:@"The password was successfully changed."];
    
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Actions

- (IBAction)enterNewPassword:(id)sender
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Pass Code" message:@"Please create new a passcode for authentication from a client app."  delegate:self cancelButtonTitle:@"OK"  otherButtonTitles:nil];
    [alert setAlertViewStyle:UIAlertViewStyleSecureTextInput];
//    [alertDictionary setObject:alert forKey:@"passcodeAlert"];
    
    [alert show];
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    UITextField *input = [alertView textFieldAtIndex:0];
    
    if ([[input text] isEqualToString:@""]) {
        NSLog(@"[alertView](MobiiFlibViewController): Cannot use an empty string as passcode");
        [self enterNewPassword:self];
    }
    else {
        [self.delegate savePasscode:[input text]];
        [message setAlpha:0];
        [UIView animateWithDuration:1 animations:^{
            [message setAlpha:1];
        }];

    }

}

- (IBAction)done:(id)sender
{
    [self.delegate flipsideViewControllerDidFinish:self];
}

@end
