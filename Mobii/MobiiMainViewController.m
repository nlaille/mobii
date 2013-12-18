//
//  MobiiMainViewController.m
//  Mobii
//
//  Created by 4ppolo on 16/11/12.
//  Copyright (c) 2012 SmartBase. All rights reserved.
//

#import "MobiiMainViewController.h"
#import "MobiiPacketManager.h"


@interface MobiiMainViewController ()
@property NSString *host;
@property NSString *port;
@property NSString *udpPort;

@property MobiiPacketManager *packetManager;
@end

@implementation MobiiMainViewController

@synthesize host;
@synthesize port;
@synthesize udpPort;
@synthesize connected;

@synthesize connectButton;
@synthesize deviceIDLabel;
@synthesize statusLabel;
@synthesize packetManager;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    host = @"163.5.84.178";
    //host = @"10.224.9.178";
    
    port = @"3689";
    udpPort = @"53795";
    
    connected = false;
    packetManager = [[MobiiPacketManager alloc] initWithConnexionToHost:host onTCPPort:port andUDPPort:udpPort withDelegate:self];
   [deviceIDLabel setText:[packetManager UID]];
}

- (void) displayUIDLabel {
    [UIView animateWithDuration:1 animations:^{
        [deviceIDLabel setAlpha:1];
    }];
}

-(IBAction)connect:(id)sender {
    if (connected == FALSE) {        
        [[packetManager connectionManager] connectToServer];
        [connectButton setTitle:@"Connecting..." forState:UIControlStateNormal];
    }
    else {
        [[packetManager connectionManager] disconnectFormClient];
        [[packetManager connectionManager] disconnectFromServer];
            [connectButton setTitle:@"Disconnecting..." forState:UIControlStateNormal];
    }
}

- (void) connectionSucceded {
    if (connected == FALSE) {
        connected = TRUE;
    }
    [connectButton setTitle:@"Disconnect" forState:UIControlStateNormal];
}

- (void) connectionTerminated {
    if (connected == TRUE) {
        connected = FALSE;
    }
    [connectButton setTitle:@"Connect" forState:UIControlStateNormal];
    
}

- (void) savePasscode:(NSString *) pass
{
    [packetManager savePasscode:pass];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Flipside View

- (void)flipsideViewControllerDidFinish:(MobiiFlipsideViewController *)controller
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([[segue identifier] isEqualToString:@"showAlternate"]) {
        [[segue destinationViewController] setDelegate:self];
    }
}

@end