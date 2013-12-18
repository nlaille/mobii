//
//  MobiiPacketManager.m
//  Mobii
//
//  Created by Nicolas on 12/17/13.
//  Copyright (c) 2013 SmartBase. All rights reserved.
//

// Temporary
#import <AssetsLibrary/AssetsLibrary.h>
#import "Base64.h"

#import <CommonCrypto/CommonDigest.h>
#import "MobiiPacketManager.h"
#import "CJSONDeserializer.h"
#import "CJSONSerializer.h"

@interface MobiiPacketManager()
@property NSMutableArray *pictureArraySync;
@property MobiiISO8601Converter *dateConverter;
@end

@implementation MobiiPacketManager

@synthesize connectionManager;
@synthesize contactManager;
@synthesize calendarManager;
@synthesize deviceProfiler;
@synthesize delegate;

@synthesize firstLaunch;

@synthesize UID;
@synthesize UIDData;
@synthesize password;
@synthesize passCode;
@synthesize receivedPacket;

@synthesize pictureArraySync;
@synthesize dateConverter;



- (id)initWithConnexionToHost:(NSString *) host onTCPPort:(NSString *) tcp andUDPPort:(NSString *) udp withDelegate:(MobiiMainViewController *) controller {
    
    if (self = [super init]) {
        delegate = controller;
        connectionManager = [[MobiiConnectionManager alloc] initWithHost: host
                                                            onTCPPort: tcp
                                                           andUDPPort: udp
                                                          andDelegate: self];
        contactManager = [[MobiiContactManager alloc] init];
        calendarManager = [[MobiiCalendarManager alloc] init];
        deviceProfiler = [[MobiiDeviceProfiler alloc] init];
        
        [self loadUID];
    }
    return self;
}

- (void) loadUID {
    NSString *savedUID = [[NSUserDefaults standardUserDefaults] objectForKey:@"uid"];
    NSData *savedUIDData = [[NSUserDefaults standardUserDefaults] objectForKey:@"uid_data"];
    NSString *savedPassCode = [[NSUserDefaults standardUserDefaults] objectForKey:@"passCode"];
    
    if (!savedUID)//User ID not present on device is generated
    {
        firstLaunch = true;
        UID = [self generateUID];
        NSLog(@"[LoadUID]: Generated a new UID:%@", UID);
        [delegate displayUIDLabel];
        [self createPassCode];
    }
    else { //User ID exists on phone data is loaded from device
        firstLaunch = false;
        UID = [[NSMutableString alloc] initWithString:savedUID];
        UIDData = savedUIDData;
        passCode = savedPassCode;
        NSLog(@"[LoadUID]: Loaded a UID:%@ , UIDData: %@ and passCode: %@ from UserDefaults ", UID, UIDData, passCode);
        [delegate displayUIDLabel];
    }
}

- (NSString *) generateUID {
    
    NSMutableString *UIDString = [[NSMutableString alloc] init];
    
    unsigned char tmp[6];
    for (int i = 0; i < 6; i++) {
        tmp[i] = arc4random() % 255;
        [UIDString appendFormat:@"%02X", tmp[i]];
    }
    
    UIDData = [[NSData alloc] initWithBytes:tmp length:6];
    return UIDString;
}


- (void) createPassCode {
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Pass Code" message:@"Please create a passcode for authentication from a client app."  delegate:self cancelButtonTitle:@"OK"  otherButtonTitles:nil];
    [alert setAlertViewStyle:UIAlertViewStyleSecureTextInput];
//    [alertDictionary setObject:alert forKey:@"passcodeAlert"];
    
    [alert show];
}

- (void) alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    UITextField *input = [alertView textFieldAtIndex:0];
    
    if ([[input text] isEqualToString:@""]) {
        NSLog(@"[alertView](MobiiMainViewController): Cannot use an empty string as passcode");
        [self createPassCode];
    }
    else {
        [self savePasscode:[[input text] copy]];
        //            NSLog(@"[alertView]: Md5 Hash generated: %@", passCode);
    }
}

- (void) savePasscode:(NSString *) pass
{
    passCode = [self md5HexDigest:pass];
    [self saveObjectToUserDefaults:passCode forKey:@"passCode"];
    NSLog(@"[savePasscode]: Passcode:%@ was saved to user defaults", passCode);
}

- (NSString *) md5HexDigest:(NSString*)input {
    const char* str = [input UTF8String];
    unsigned char result[CC_MD5_DIGEST_LENGTH];
    CC_MD5(str, strlen(str), result);
    
    NSMutableString *ret = [NSMutableString stringWithCapacity:CC_MD5_DIGEST_LENGTH*2];
    for(int i = 0; i<CC_MD5_DIGEST_LENGTH; i++) {
        [ret appendFormat:@"%02x",result[i]];
    }
    return ret;
}

- (BOOL) saveObjectToUserDefaults:(NSObject*)value forKey:(NSString*)key {
    
    NSUserDefaults * usrdef = [NSUserDefaults standardUserDefaults];
    if (usrdef)
    {
        [usrdef setObject:value forKey:key];
        if (![usrdef synchronize])
        {
            NSLog(@"[saveObject]: Couldn't save %@ for key %@", value, key);
            return false;
        }
    }
    NSLog(@"[saveObject]: Successfully saved %@ for key %@", value, key);
    return true;
}

-(NSObject *) loadDataFromKey:(NSString*) key {
    NSObject *usrDef = [[NSUserDefaults standardUserDefaults] objectForKey:key];
    
    if (usrDef)
    {
        return usrDef;
    }
    return NULL;
}

- (void) serverAuthentication {
    if (firstLaunch) //Since key does not exist on phone uses generated ID and sends this ID to server
    {
        NSLog(@"[serverAuthentication]: First launch, sending UID for registration");
        [self deviceUIDRegistration];
    }
    else {// loads password and UID from phone and authenfication is done on behalf of the server
        NSLog(@"[serverAuthentication]: Not the first lauch, sending UID/password combo for authentication");
        //UID = [self loadDataFromKey:@"uid"];
        password = (NSString *)[self loadDataFromKey:@"password"];
        UIDData = (NSData *)[self loadDataFromKey:@"uid_data"];
        
        if (UID != NULL && password != NULL) {
            NSMutableDictionary *infos = [[NSMutableDictionary alloc] init];
            [infos setObject:UID forKey:@"uid"];
            [infos setObject:password forKey:@"password"];
            [self sendJSONPacket:(unsigned char*)"\x0\x0\x0\x0\x0\x0" :(unsigned char*)"\x0\x0\x0\x0\x0\x0" :'\x05' :'\x00' :infos];
        }
        else {
            NSLog(@"Couldn't load UID/password combo");
        }
    }
}

-(void) deviceUIDRegistration {
    NSMutableDictionary *dict = [[NSMutableDictionary alloc] init];
    
    // Get the device informations here...
    
    NSLog(@"[deviceUIDRegistration]: UID is: %@", UID);
    [dict setObject:UID forKey:@"uid"];
    [dict setObject:@"" forKey:@"imei"];
    [dict setObject:@"Apple Inc." forKey:@"manufacturer"];
    [dict setObject:@"iPhone" forKey:@"model"];
    // Todo: Format sendJsonPacket poperly
    NSLog(@"[deviceUIDRegistration]: Sending device infos : %@", dict);
    [self sendJSONPacket:(unsigned char*)"\x0\x0\x0\x0\x0\x0"
                        :(unsigned char*)"\x0\x0\x0\x0\x0\x0"
                        :'\x50'
                        :'\x10'
                        :dict];
}

-(void)sendJSONPacket:(unsigned char*)src :(unsigned char*)dest :(char)dispatch :(char)opcode :(NSMutableDictionary*)data {
    
    NSData *serializedData = [[CJSONSerializer serializer] serializeDictionary:data error:NULL];
    NSLog(@"[sendJSONPacket]: sending serialzed data : %@", serializedData);
    
    Packet *packet = [[Packet alloc] initWithSource:src
                                      toDestination:dest
                                         withAction:dispatch
                                          andOpCode:opcode
                                             ofSize:[serializedData length]
                                         andContent:serializedData];
    [packet display];
    [connectionManager writeDataToCurrentSocket:[packet getRawData]];
}

-(void) packetAnalysis {
    NSLog(@"[packetAnalysis]: Analysing packet ...");
    
    if (receivedPacket.dispatch == '\x20' && receivedPacket.opcode == '\x10') {
        NSLog(@"[packetAnalysis]: Packet type is \"Client Connection Request\"");
        //        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Connection Request" message:@"A Mobii Client App wants to connect..." delegate:self cancelButtonTitle:@"No" otherButtonTitles:@"Yes", nil];
        //        [alertDictionary setObject:alert forKey:@"connectionAlert"];
        //        [alert show];
        [self clientConnectionRequest];
        
    }
    else if (receivedPacket.dispatch == '\x30' && receivedPacket.opcode == '\x10') {
        NSLog(@"[packetAnalysis]: Packet type is \"JSON Request from Client\"");
        [self jsonClientRequest];
    }
    else if (receivedPacket.dispatch == '\x30' && receivedPacket.opcode == '\x01') {
        NSLog(@"[packetAnalysis]: Packet type is \"Connexion with Client Established\"");
        [self tryLocalNetworkConnection];
        [self sendSystemInfos];
        //[self holePunchingInfosRequest];
        
        //  check if server sent local network informations
        [connectionManager readDataFromServerSocket];
    }
    else if (receivedPacket.dispatch == '\x40' && receivedPacket.opcode == '\x00') {
        NSLog(@"[packetAnalysis]: Packet type is \"UDP HolePunching Infos\"");
        [self holePunch];
    }
    else if (receivedPacket.dispatch == '\x05' && receivedPacket.opcode == '\x11') {
        NSLog(@"[packetAnalysis]: Packet type is \"Server Authentication Succeded\"");
    }
    else if (receivedPacket.dispatch == '\x50' && receivedPacket.opcode == '\x11') // success
    {
        NSLog(@"[packetAnalysis]: Packet type is \"UID Registration Succeded\"");
        [self savePassAndUID:receivedPacket];
        [delegate displayUIDLabel];
        [self loadUID];
        [self serverAuthentication];
        
    }
    else if (receivedPacket.dispatch == '\x50' && receivedPacket.opcode == '\x12') // failure
    {
        NSLog(@"[packetAnalysis]: Packet type is \"UID Registration Failed\"");
        UID = [self generateUID];
        [self serverAuthentication];
    }
    else if (receivedPacket.dispatch == '\x30' && receivedPacket.opcode == '\x10')
    {
        NSLog(@"[packetAnalysis]: Packet type is \"Direct Connection Informations \"");
        // [self directConnectionToClient];
    }
    else {
        NSLog(@"[packetAnalysis]: Error : Packet type not supported.");
    }
}

- (void) jsonClientRequest {
    
    NSLog(@"[jsonClientRequest]: JSON Request...");
    NSError *error = nil;
    NSString *dataString = [[NSString alloc] initWithBytes:receivedPacket.data length:receivedPacket.size encoding:NSUTF8StringEncoding];
    NSData *jsonData = [dataString dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *dictionary = [[CJSONDeserializer deserializer] deserializeAsDictionary:jsonData error:&error];
    
    NSLog(@"[jsonClientRequest]: Deserialezed data : %@", dictionary);
    NSString *request = [dictionary objectForKey:@"action"];
    NSMutableDictionary *response = [[NSMutableDictionary alloc] init];
    if ([request isEqualToString:@"req_contacts"]) {
        [response setObject:[contactManager getAdressBookContacts] forKey:@"data"];
        [response setObject:@"contacts" forKey:@"action"];
    }
    else if ([request isEqualToString:@"upt_contacts"]) {
        [contactManager updateContact:[dictionary objectForKey:@"data"]];
    }
    else if ([request isEqualToString:@"new_contact"]) {
        [response setObject:@"valid_new_contact" forKey:@"action"];
        [response setObject:[NSArray arrayWithObject:[contactManager addContact:[dictionary objectForKey:@"data"]]] forKey:@"data"];
    }
    else if ([request isEqualToString:@"del_contact"]) {
        [contactManager deleteContact:[dictionary objectForKey:@"data"]];
    }
    else if ([request isEqualToString:@"req_calendar"]) {
        [response setObject:@"calendar" forKey:@"action"];
        [response setObject:[calendarManager getCalendarEvents] forKey:@"data"];
    }
    else if ([request isEqualToString:@"upt_events"]) {
        [calendarManager updateCalendarEvents:[dictionary objectForKey:@"data"]];
    }
    else if ([request isEqualToString:@"new_event"]) {
        [response setObject:@"valid_new_event" forKey:@"action"];
        [response setObject:[NSArray arrayWithObject:[calendarManager addNewEvent:[dictionary objectForKey:@"data"]]] forKey:@"data"];
    }
    else if ([request isEqualToString:@"del_event"]) {
        [calendarManager deleteEvent:[dictionary objectForKey:@"data"]];
    }
    else if ([request isEqualToString:@"req_files"]) {
        [self getDevicePictures];
    }
    else if ([request isEqualToString:@"req_this_image"]) {
        NSString * requestedImage = [dictionary objectForKey:@"id"];
        [self getImageFromID:requestedImage];
    }
    else {
        NSLog(@"[jsonClientRequest]: Action not supported");
    }
    if ([response count]) {
        NSLog(@"[jsonClientRequest]: response: %@", response);
        NSData *responseData = [[CJSONSerializer serializer] serializeDictionary:response error:NULL];
        NSLog(@"[jsonClientRequest]: responseData: %s", (char *)[responseData bytes]);
        NSLog(@"[jsonClientRequest]: data lenght : %d", [responseData length]);
        Packet *packet = [[Packet alloc] initWithSource:(unsigned char *)[UIDData bytes]
                                          toDestination:receivedPacket.source
                                             withAction:'\x30'
                                              andOpCode:'\x10'
                                                 ofSize:[responseData length]
                                             andContent:responseData];
        
        [packet display];
        [connectionManager writeDataToCurrentSocket:[packet getRawData]];
    }
    else {
        [connectionManager readDataFromCurrentSocket];
    }
}


-(void) clientConnectionRequest {
    Packet *packet = [[Packet alloc] init];
    
    
    packet.source = (unsigned char *)[UIDData bytes];
    packet.destination = receivedPacket.source;
    packet.size = 0;
    packet.data = nil;
    
    NSString *dataString = [[NSString alloc] initWithBytes:receivedPacket.data length:receivedPacket.size encoding:NSUTF8StringEncoding];
    NSData *jsonData = [dataString dataUsingEncoding:NSUTF8StringEncoding];
    NSError *error = nil;
    NSDictionary *dictionary = [[CJSONDeserializer deserializer] deserializeAsDictionary:jsonData error:&error];
    
    
    
    NSString *code = [dictionary objectForKey:@"code"];
    NSLog(@"[clientConnectionRequest]: Object for code = %@", code);
    NSLog(@"[clientConnectionRequest]: Registered code on the device = %@", passCode);
    
    if ([passCode isEqualToString:passCode]) {
        packet.dispatch = '\x10';
        packet.opcode = '\x10';
    }
    else {
        packet.dispatch = '\x10';
        packet.opcode = '\x11';
    }
    
    NSLog(@"[clientConnectionRequest]: response packet:");
    [packet display];
    
    [connectionManager writeDataToCurrentSocket:[packet getRawData]];
}

- (void) sendSystemInfos {
    NSLog(@"Sending system infos ...");
    NSMutableDictionary *infos = [[NSMutableDictionary alloc] init];
    
    [infos setObject:@"mobile_infos" forKey:@"action"];
    [infos setObject:[deviceProfiler getDeviceInfos] forKey:@"args"];
    [infos setObject:[deviceProfiler getSupportedFeatures] forKey:@"features"];
    
    NSData *infosData = [[CJSONSerializer serializer] serializeDictionary:infos error:NULL];
    NSLog(@"infosData : %s", (char *)[infosData bytes]);
    Packet *packet = [[Packet alloc] initWithSource:(unsigned char *)[UIDData bytes]
                                      toDestination:receivedPacket.source
                                         withAction:'\x30'
                                          andOpCode:'\x10'
                                             ofSize:[infosData length]
                                         andContent:infosData];
    
    [packet display];
    
    [connectionManager writeDataToCurrentSocket:[packet getRawData]];
}

-(void) savePassAndUID : (Packet*) packet// Saving password and UID sent back from server on to the phone
{
    NSError *error = nil;
    NSString *dataString = [[NSString alloc] initWithBytes:packet.data length:packet.size encoding:NSUTF8StringEncoding];
    NSData *jsonData = [dataString dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *dictionary = [[CJSONDeserializer deserializer] deserializeAsDictionary:jsonData error:&error];
    
    NSString *ID = [dictionary objectForKey:@"uid"];
    NSString *pwd = [dictionary objectForKey:@"password"];
    
    [self saveObjectToUserDefaults:ID forKey:@"uid"];
    [self saveObjectToUserDefaults:UIDData forKey:@"uid_data"];
    [self saveObjectToUserDefaults:pwd forKey:@"password"];
    NSLog(@"[savePassAndUID]: UID and password were successfully stored");
    
}


#pragma mark Networking Functions

- (void) tryLocalNetworkConnection {
    NSString *dataString = [[NSString alloc] initWithBytes:receivedPacket.data length:receivedPacket.size encoding:NSUTF8StringEncoding];
    NSData *jsonData = [dataString dataUsingEncoding:NSUTF8StringEncoding];
    NSError *error = nil;
    NSDictionary *dictionary = [[CJSONDeserializer deserializer] deserializeAsDictionary:jsonData error:&error];
    if (dictionary) {
        NSLog(@"[tryLocalNetworkConnection]: Connection established with server. Server response : %@", dictionary);
        NSString *clientIP = [NSString stringWithFormat:@"%@", [dictionary objectForKey:@"ip"]];
        NSString *clientPort = [NSString stringWithFormat:@"%@", [dictionary objectForKey:@"port"]];
        
        [connectionManager connectToClient:clientIP onPort:clientPort];
    }
}

- (void) holePunch {
//    NSError *error = nil;
//    NSString *dataString = [[NSString alloc] initWithBytes:receivedPacket.data length:receivedPacket.size encoding:NSUTF8StringEncoding];
//    NSData *jsonData = [dataString dataUsingEncoding:NSUTF8StringEncoding];
//    NSDictionary *dictionary = [[CJSONDeserializer deserializer] deserializeAsDictionary:jsonData error:&error];
//    
//    NSLog(@"[holePunch]: Received informations are : %@", dictionary);
//    NSString *clientIp = [dictionary objectForKey:@"ip"];
//    NSString *clientPort = [dictionary objectForKey:@"port"];
//    
//    NSData *data = [[NSData alloc] initWithBytes:"hello" length:5];
//    NSLog(@"[holePunch]: Binding the udpsocket on port:%d", [udpPort intValue]);
//    if (![udpSocket bindToPort:[udpPort intValue] error:&error]) {
//        NSLog(@"[holePunch]: error while binding: %@", error);
//    }
//    
//    NSLog(@"[holePunch]: udpSocket will send data using ip:%@ and port:%d", clientIp, [clientPort intValue]);
//    for (int i = 0; i < 20; i++) {
//        [udpSocket sendData:data toHost:clientIp port:[clientPort intValue] withTimeout:-1 tag:0];
//        usleep(500000);
//    }
//    NSLog(@"[holePunch]: udpSocket is bound on local port: %d and ipv4 port: %d", [udpSocket localPort],[udpSocket localPort_IPv4]);
//    if (![udpSocket beginReceiving:&error]) {
//        NSLog(@"[holePunch]: UDPSocket is not able to begin receive packet:%@", error);
//    }
}

- (void) holePunchingInfosRequest {
//    NSMutableDictionary *infos = [[NSMutableDictionary alloc] init];
//    NSError *error = nil;
//    [infos setObject:UID forKey:@"uid"];
//    [infos setObject:password forKey:@"password"];
//    
//    NSData *serializedData = [[CJSONSerializer serializer] serializeDictionary:infos error:NULL];
//    NSLog(@"[holePunchingInfosRequest]: sending serialzed data : %@", serializedData);
//    Packet *packet = [[Packet alloc] initWithSource:(unsigned char *)[UIDData bytes]
//                                      toDestination:(unsigned char *) receivedPacket.source
//                                         withAction:0x40
//                                          andOpCode:0x00
//                                             ofSize:[serializedData length]
//                                         andContent:serializedData];
//    [packet display];
//    [udpSocket sendData:[packet getRawData] toHost:host port:53789 withTimeout:-1 tag:0];
//    NSLog(@"[holePunchingInfosRequest]: udpsocket automatically bound to local port: %d and ipv4 port: %d", [udpSocket localPort], [udpSocket localPort_IPv4]);
//    udpPort = [NSString stringWithFormat:@"%d", [udpSocket localPort_IPv4]];
//    
//    [serverSocket readDataWithTimeout:-1 tag:0];
//    [udpSocket close];
}

- (void) serverConnectionSucceded {
    [delegate connectionSucceded];
    [self serverAuthentication];
    
}

- (void) serverConnectionClosed {
    [delegate connectionTerminated];
}

- (void) clientConnectionClosed {
    [delegate connectionTerminated];
}

- (void) packetReceived:(NSData *)data {
    receivedPacket = [[Packet alloc] initWithRawData:data];
    [self packetAnalysis];
}


#pragma mark Functions to move to a new Object

-(NSMutableArray *)getDevicePictures {
    ALAssetsLibrary * assestsLib = [[ALAssetsLibrary alloc] init];
    __block NSMutableArray * picArray = [[NSMutableArray alloc] init];
    __block NSMutableArray * folderList = [[NSMutableArray alloc] init];
    //NSNumber *imageID = [[NSNumber alloc]initWithInt:0];
    
    static BOOL containsPics = 0;
    __block int temp = 0;
    __block int temp2 = 0;
    NSMutableDictionary *response = [[NSMutableDictionary alloc] init];
    
    
    //   ALAssetsLibraryAssetForURLResultBlock resultBlock
    
    //        sync();
    //dispatch_async(dispatch_get_main_queue(), ^{
    [assestsLib enumerateGroupsWithTypes:ALAssetsGroupAll
     
                              usingBlock:^(ALAssetsGroup *group, BOOL * stop)
     {
         
         
         
         //fold
         NSMutableDictionary * folderDictionnary = [[NSMutableDictionary alloc] init];
         NSMutableArray * pictures =[[NSMutableArray alloc] init];
         
         //[pictures removeAllObjects];
         //[folderDictionnary setObject:
         //             [picArray removeAllObjects];
         [group enumerateAssetsUsingBlock:^(ALAsset * asset, NSUInteger index, BOOL *stop)
          {
              //
              if (asset)
              {
                  containsPics = true;
                  
                  ALAssetRepresentation *rep = [asset defaultRepresentation];
                  NSString * date = [dateConverter strFromISO8601:[asset valueForProperty:ALAssetPropertyDate]];
                  NSString * name = [asset valueForProperty:ALAssetPropertyAssetURL];
                  NSMutableDictionary *infos = [[NSMutableDictionary alloc] init];
                  
                  
                  [infos setObject:[NSString stringWithFormat:@"%@", date] forKey:@"date"];
                  NSString* myInt = [NSString stringWithFormat:@"%i", temp];
                  
                  
                  
                  [infos setObject:myInt forKey:@"id"];
                  [infos setObject:[NSString stringWithFormat:@"%@", name] forKey:@"path"];
                  [infos setObject:[NSString stringWithFormat:@"%@", [rep filename]] forKey:@"name"];
                  
                  [pictures addObject:infos];
                  
                  
                  
                  
                  temp++;
              }
          }
          ];
         //ALAssetRepresentation * fRep = [group ]
         NSLog(@"Albume name = %@", [group valueForProperty:ALAssetsGroupPropertyName]);
         
         if ([pictures count])
         {
             [folderDictionnary setObject:[NSString stringWithFormat:@"%@", [group valueForProperty:ALAssetsGroupPropertyName]] forKey:@"foldername"];
             [folderDictionnary setObject:[[NSMutableArray alloc] initWithArray:pictures] forKey:@"folderfiles"];
             [pictures removeAllObjects];
             if ([folderDictionnary count])
             {
                 [folderList addObject:[[NSMutableDictionary alloc] initWithDictionary:folderDictionnary]];
                 [folderDictionnary removeAllObjects];
                 
             }
         }
         if (group == nil)
         {
             [self sendFileListToClient:folderList :response];
             pictureArraySync = folderList;
         }
         ++temp2;
         
     }
                            failureBlock:^(NSError *error)
     {
         NSLog(@"Houston we have an error : %@ ", error);
     }
     ];
    
    
    //    NSLog(@"%@", picArray);
    
    return picArray;
}

-(void)sendFileListToClient:(NSMutableArray*)folderArray :(NSMutableDictionary*)response {
    [response setObject:@"filelist" forKey:@"action"];
    [response setObject:folderArray forKey:@"filelist"];
    
    
    
    NSLog(@"[getDevicePictures]: response: %@", response);
    NSData *responseData = [[CJSONSerializer serializer] serializeDictionary:response error:NULL];
    NSLog(@"[getDevicePictures]: responseData: %s", (char *)[responseData bytes]);
    NSLog(@"[getDevicePictures]: data lenght : %d", [responseData length]);
    Packet *packet = [[Packet alloc] initWithSource:(unsigned char *)[UIDData bytes]
                                      toDestination:receivedPacket.source
                                         withAction:'\x30'
                                          andOpCode:'\x10'
                                             ofSize:[responseData length]
                                         andContent:responseData];
    
    [packet display];

    [connectionManager writeDataToCurrentSocket:[packet getRawData]];
    pictureArraySync = folderArray;
}

-(UIImage*)getImageFromID:(NSString*)ID // L'image vaut null c'est le probleme !
{
    //
    NSMutableArray * picArray = pictureArraySync;
    NSString * mediaurl = NULL;
    NSLog(@"Value of Picture arrays - ---- -- - - - - -- - -- - - %i", [picArray count]);
    for (int i = 0 ; i < [picArray count]; ++i)
    {
        if ([[picArray objectAtIndex:i] objectForKey:@"folderfiles"])
        {
            NSMutableArray * filesArray = [[picArray objectAtIndex:i] objectForKey:@"folderfiles"];
            
            
            
            for (int j = 0 ; j < [filesArray count] ; ++j)
            {
                if ([[[filesArray objectAtIndex:j] objectForKey:@"id"] isEqualToString:ID])
                {
                    mediaurl = [[NSString alloc] initWithString:[[filesArray objectAtIndex:j] objectForKey:@"path"]];
                }
            }
        }
    }
    NSURL *url = [[NSURL alloc] initWithString:mediaurl];
    NSMutableDictionary *response = [[NSMutableDictionary alloc] init];
    
    
    
    __block UIImage * wantedImage = NULL;
    //    ALAssetsLibrary * library = [[ALAssetsLibrary alloc] init];
    
    ALAssetsLibrary *assetsLibrary = [[ALAssetsLibrary alloc] init];
    [assetsLibrary assetForURL:url resultBlock: ^(ALAsset *asset){
        ALAssetRepresentation *representation = [asset defaultRepresentation];
        CGImageRef imageRef = [representation fullResolutionImage];
        if (imageRef) {
            //UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 100, 100)];
            //imageView.image = [UIImage imageWithCGImage:imageRef scale:representation.scale orientation:representation.orientation];
            // ...
            wantedImage = [UIImage imageWithCGImage:representation.fullResolutionImage scale:representation.scale orientation:(UIImageOrientation)representation.orientation];
            
            if (wantedImage != NULL){
                
                if ([mediaurl rangeOfString:@"ext="].location != NSNotFound)
                {
                    NSString * type = [mediaurl substringFromIndex:[mediaurl rangeOfString:@"ext="].location + 4];
                    type = [type lowercaseString];
                    NSString * encodedString;
                    NSData * imageData = NULL;
                    
                    if ([type isEqualToString:@"jpg"] || [type isEqualToString:@"jpeg"])
                    {
                        imageData = UIImageJPEGRepresentation(wantedImage, 0.0);
                        
                        encodedString = [Base64 encode:(uint8_t *)[imageData bytes] length:[imageData length]];
                    }
                    else
                    {
                        Byte * buffer = (Byte*)malloc(representation.size);
                        NSUInteger buffered = [representation getBytes:buffer fromOffset:0.0 length:representation.size error:nil];
                        NSData * data = [NSData dataWithBytesNoCopy:buffer length:buffered freeWhenDone:YES];
                        encodedString = [Base64 encode:(uint8_t *)[data bytes] length:[data length]];
                    }
                    [response setObject:[NSString stringWithFormat:@"%@", @"PhonePic"] forKey:@"type"];
                    [response setObject:@"file" forKey:@"action"];
                    [response setObject:[NSString stringWithFormat:@"%i", [encodedString length]] forKey:@"size"];
                    [response setObject:[NSString stringWithFormat:@"%@", ID] forKey:@"id"];
                    [response setObject:[NSString stringWithFormat:@"%@", [representation filename]] forKey:@"name"];
                    [response setObject:[NSString stringWithFormat:@"%@", encodedString] forKey:@"data"];
                    NSData *responseData = [[CJSONSerializer serializer] serializeDictionary:response error:NULL];
                    Packet *packet = [[Packet alloc] initWithSource:(unsigned char *)[UIDData bytes]
                                                      toDestination:receivedPacket.source
                                                         withAction:'\x30'
                                                          andOpCode:'\x10'
                                                             ofSize:[responseData length]
                                                         andContent:responseData];
                    [connectionManager writeDataToCurrentSocket:[packet getRawData]];
                }
                
            }
        }
    } failureBlock: ^(NSError *error){
        NSLog(@"Failure ");
    }];
    return wantedImage;
}// Delegate Functions



@end
