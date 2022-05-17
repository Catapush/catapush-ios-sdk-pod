![Catapush Logo](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/catapush_logo.png)


Catapush is a simple, reliable and scalable delivery API for transactional push notifications for websites and applications. Ideal for sending data-driven transactional notifications including targeted e-commerce and personalized one-to-one messages.

Get delivery confirmation with real time status for each message sent and build intelligent messaging logic in your applications


Table of Contents
=================

   * [Table of Contents](#table-of-contents)
   * [Setup](#setup)
      * [Create Catapush Application and get an App Key](#create-catapush-application-and-get-an-app-key)
      * [Create and configure the authentication key](#create-and-configure-the-authentication-key)
      * [Add a Notification Service Extension](#add-a-notification-service-extension)
      * [App Groups](#app-groups)
      * [AppDelegate](#appdelegate)
      * [Push Notification](#push-notification)
   * [Example projects](#example-projects)
   * [Events Handling](#events-handling)
      * [Connection Events](#connection-events)
      * [Error Handling](#error-handling)
         * [Main app](#main-app)
         * [Error codes](#error-codes)
         * [Service](#service)
      * [Receiving Messages](#receiving-messages)
      * [Reading Messages](#reading-messages)
      * [Sending Messages](#sending-messages)
      * [Fetching an attachment](#fetching-an-attachment)
      * [Get optional data](#get-optional-data)
   * [Multiuser](#multiuser)
      * [Forced logout](#forced-logout)
   * [Advanced](#advanced)
      * [Messages Managers](#messages-managers)
      * [Logging](#logging)
      * [Manual library integration when using use_frameworks!](#manual-library-integration-when-using-use_frameworks)
      * [Handle badge counts](#handle-badge-counts)
   * [Troubleshooting](#troubleshooting)
      * [Raw notification](#raw-notification)
               * [Wrong configuration](#wrong-configuration)
               * [Crash](#crash)
               * [Server side override](#server-side-override)
      * [Disable Notification Service during debug](#disable-notification-service-during-debug)
   * [Extra](#extra)
      * [Use the Console.app to access logs](#use-the-consoleapp-to-access-logs)
      * [Retrieve MessageIP after tap on a notification](#retrieve-messageip-after-tap-on-a-notification)
   * [Notes](#notes)
   * [Author](#author)
   * [License](#license)

# Setup
Catapush iOS sdk is available through [CocoaPods](http://cocoapods.org). To install it, simply add the following line to your Podfile:

```objectivec
pod "catapush-ios-sdk-pod"
```

> **WARNING**: Since our library isn't a framework you cannot use `use_frameworks!` in your Podfile, if you have to use this flag you have to include the library manually [following this steps!](#manual-library-integration-when-using-use_frameworks)

The minimun iOS version is 10.

These are pre-requisites for setting up your application with Catapush:

## Create Catapush Application and get an App Key
1. Get your App Key from  [Catapush Dashboard](http://www.catapush.com/)  from the left menu in "Your APP" -> App details
2.  Create the first user from "Your APP" -> User

## Create and configure the authentication key
This section describes how to generate an authentication key for an App ID enabled for Push Notifications. If you have an existing key, you can use that key instead of generating a new one.

To create an authentication key:
1) In your [Apple Developer Member Center](https://developer.apple.com/account), go to Certificates, Identifiers & Profiles, and select Keys.
2) Click the Add button (+) or click the "Create a key" button.
<img src="https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/auth_key_1.png">
3) Enter a description for the APNs Auth Key.
4) Under Key Services, select the Apple Push Notifications service (APNs) checkbox, and click Continue.
<img src="https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/auth_key_2.png">
5) Click Register and then download and save your key in a secure place. This is a one-time download, and the key cannot be retrieved later.
<img src="https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/auth_key_3.png">

Once you have download it you have to configure your Catapush application.
1) Go to https://www.catapush.com/panel/apps/YOUR_APP_ID/platforms.
2) Click on iOS Token Based to enable it.
3) Fill iOS Team Id, iOS Key Id, iOS AuthKey and iOS Topic.

The iOS **Team Id** can be found here https://developer.apple.com/account/#/membership in "Membership Information" section.

The iOS **Key Id** can be retrieved here https://developer.apple.com/account/resources/authkeys/list, click on the key you have created and you can find it under "View Key Details" section.

The iOS **AuthKey** is the content of the key file.

Example:
```
-----BEGIN PRIVATE KEY-----
...........................
          AUTH_KEY
...........................
-----END PRIVATE KEY-----
```

The iOS Topic is the bundle identifier of your iOS application.

<img src="https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/catapush_ios_token_based.png" width="200">

## Add a Notification Service Extension
In order to process the push notification a Notification Service Extension is required.
Add a Notification Service Extension (in Xcode File -> New -> Target...) that extends ```CatapushNotificationServiceExtension```
```objectivec
@interface CatapushNotificationServiceExtension : UNNotificationServiceExtension
- (void)handleMessage:(MessageIP * _Nullable) message withContentHandler:(void (^_Nullable)(UNNotificationContent * _Nullable))contentHandler  withBestAttemptContent: (UNMutableNotificationContent* _Nullable) bestAttemptContent;
- (void)handleError:(NSError * _Nonnull) error withContentHandler:(void (^_Nullable)(UNNotificationContent * _Nullable))contentHandler  withBestAttemptContent: (UNMutableNotificationContent* _Nullable) bestAttemptContent;
@end
```

Example:

**NotificationService.h**

```objectivec
#import "Catapush.h"

@interface NotificationService : CatapushNotificationServiceExtension

@end
```
**NotificationService.m**
```objectivec
#import "NotificationService.h"

@interface NotificationService ()

@end

@implementation NotificationService

- (void)didReceiveNotificationRequest:(UNNotificationRequest *)request withContentHandler:(void (^)(UNNotificationContent * _Nonnull))contentHandler {
    [super didReceiveNotificationRequest:request withContentHandler:contentHandler];
}

- (void)handleError:(NSError * _Nonnull) error withContentHandler:(void (^_Nullable)(UNNotificationContent * _Nullable))contentHandler  withBestAttemptContent: (UNMutableNotificationContent* _Nullable) bestAttemptContent{
    if (contentHandler != nil && bestAttemptContent != nil){
        if (error.code == CatapushCredentialsError) {
            bestAttemptContent.body = @"User not logged in";
        }
        if (error.code == CatapushNetworkError) {
            bestAttemptContent.body = @"Network error";
        }
        if (error.code == CatapushNoMessagesError) {
            bestAttemptContent.body = @"No new message";
        }
        if (error.code == CatapushFileProtectionError) {
            bestAttemptContent.body = @"Unlock the device at least once to receive the message";
        }
        if (error.code == CatapushConflictErrorCode) {
            bestAttemptContent.body = @"Connected from another resource";
        }
        if (error.code == CatapushAppIsActive) {
            bestAttemptContent.body = @"Please open the app to read the message";
        }
        contentHandler(bestAttemptContent);
    }
}

- (void)handleMessage:(MessageIP * _Nullable) message withContentHandler:(void (^_Nullable)(UNNotificationContent * _Nullable))contentHandler  withBestAttemptContent: (UNMutableNotificationContent* _Nullable) bestAttemptContent{
    if (contentHandler != nil && bestAttemptContent != nil){
        if (message != nil) {
            bestAttemptContent.body = message.body.copy;
        }else{
            bestAttemptContent.body = @"No new message";
        }
        contentHandler(bestAttemptContent);
    }
}

@end
```

```CatapushFileProtectionError```will be returned if the user receive a push notification but the device was not unlocked for at least one time after the boot.
User data is stored in an encrypted format on disk with ```NSFileProtectionCompleteUntilFirstUserAuthentication``` policy.

https://developer.apple.com/documentation/foundation/nsfileprotectioncompleteuntilfirstuserauthentication

>**WARNING**: Make sure the deployment target of the Service is the same as the deployment target of the app.
<img src="https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/deployment_target.png">

## App Groups
Catapush need that the Notification Service Extension and the main application can share resources.
In order to do that you have to create and enable a specific app group for both the application and the extension.
The app and the extension must be in the same app group.
<img src="https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/appgroup_1.png">
<img src="https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/appgroup_2.png">

You should also add this information in the App plist and the Extension plist (```group.example.group``` should match the one you used for example ```group.catapush.test``` in the screens):
```objectivec
    <key>Catapush</key>
    <dict>
        <key>AppGroup</key>
        <string>group.example.group</string>
    </dict>
```

## AppDelegate

Bind the App Life-Cycle Events with the corresponding Catapush methods:
```objectivec
+ (void)applicationDidEnterBackground:(UIApplication *)application;
+ (void)applicationDidBecomeActive:(UIApplication *)application; 
+ (void)applicationWillEnterForeground:(UIApplication *)application withError:(NSError **) error;
+ (void)applicationWillTerminate:(UIApplication *)application;
```

Setup the **Catapush App Key** and the **user credentials**.
```objectivec
    [Catapush setAppKey:@"YOUR_APP_KEY"];
    [Catapush setIdentifier:@"test" andPassword:@"test"];
```

Conforms to ```UNUserNotificationCenterDelegate```
```objectivec
- (void)userNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void (^)(void))completionHandler{
    //Here you can handle tap on the push notification
    completionHandler();
}

- (void)userNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions))completionHandler{
    //UNNotificationPresentationOptionNone is required to avoid alert when the app is in foreground
    completionHandler(UNNotificationPresentationOptionNone);
}
```


Full example:
```objectivec
#import "AppDelegate.h"
#import "Catapush.h"

@interface AppDelegate () <CatapushDelegate,MessagesDispatchDelegate, UNUserNotificationCenterDelegate>

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    [Catapush setAppKey:@"YOUR_APP_KEY"];
    [Catapush setIdentifier:@"test" andPassword:@"test"];
    [Catapush setupCatapushStateDelegate:self andMessagesDispatcherDelegate:self];
    [Catapush registerUserNotification:self];
    NSError *error;
    [Catapush start:&error];
    if (error != nil) {
        // Handle error...
    }
    [UNUserNotificationCenter currentNotificationCenter].delegate = self;
    return YES;
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    [Catapush applicationDidEnterBackground:application];
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    
    NSError *error;
    [Catapush applicationWillEnterForeground:application withError:&error];
    if (error != nil) {
        // Handle error...
    }
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    [Catapush applicationDidBecomeActive:application];
}

- (void)applicationWillTerminate:(UIApplication *)application {
    [Catapush applicationWillTerminate:application];
}

#pragma mark Standard Push Notification Delegate
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
   // Custom code (can be empty)
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error {
    // Custom code (can be empty)
}

- (void)catapushDidConnectSuccessfully:(Catapush *)catapush
{
    UIAlertView *connectedAV = [[UIAlertView alloc] initWithTitle:@"Connected"
                                                          message:@"Catapush Connected!" delegate:self
                                                cancelButtonTitle:@"Ok" otherButtonTitles:nil];
    [connectedAV show];
}

- (void)catapush:(Catapush *)catapush didFailOperation:(NSString *)operationName withError:(NSError *)error {
    if ([error.domain isEqualToString:CATAPUSH_ERROR_DOMAIN]) {
        switch (error.code) {
            case WRONG_AUTHENTICATION:
                break;
            case INVALID_APP_KEY:
                break;
            case USER_NOT_FOUND:
                break;
            case GENERIC:
                break;
            default:
                break;
        }
    }
    
    NSString *errorMsg = [NSString stringWithFormat:@"The operation %@ is failed with error:\n%@", operationName, [error localizedDescription]];
    
    UIAlertView *flowErrorAlertView = [[UIAlertView alloc] initWithTitle:@"Error"
                                                                 message:errorMsg
                                                                delegate:self
                                                       cancelButtonTitle:@"Ok"
                                                       otherButtonTitles:nil];
    [flowErrorAlertView show];
}

-(void)libraryDidReceiveMessageIP:(MessageIP *)messageIP {
    [MessageIP sendMessageReadNotification:messageIP];
    [[Catapush allMessages] enumerateObjectsUsingBlock:^(MessageIP *m, NSUInteger idx, BOOL * stop) {
        NSLog(@"Message: \(%@)",m.body);
    }];
}

- (void)userNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void (^)(void))completionHandler{
    //Here you can handle tap on the push notification
    completionHandler();
}

- (void)userNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions))completionHandler{
    //To avoid alert when the app is in foreground
    completionHandler(UNNotificationPresentationOptionNone);
}

@end
```

## Push Notification
```objectivec
#pragma mark - End developer user must  declare in order to let AOP to inject catapush library code
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
   // Custom code (can be empty)
}
```

Catapush injects code on the above method in order to retrieve the device token once is available.

You can check if a ```UNNotificationRequest``` is from Catapush by calling:
```[Catapush isCatapushNotificationRequest:request]```
This is usefull if you have to manage custom push notification that have ```mutable-content : 1``` in the payload.
Example (in your ```UNNotificationServiceExtension```):
```objectivec
- (void)didReceiveNotificationRequest:(UNNotificationRequest *)request withContentHandler:(void (^)(UNNotificationContent * _Nonnull))contentHandler {
    if (![Catapush isCatapushNotificationRequest:request]) {
        // Custom code to properly handle the notification
    }else{
        [super didReceiveNotificationRequest:request withContentHandler:contentHandler];
    }
}
```

# Example projects
[Obj-C](https://github.com/Catapush/catapush-ios-sdk-example)
[Swift](https://github.com/Catapush/catapush-ios-swift-sdk-example)


# Events Handling
In order to receive events, setup then two delegates ```<CatapushDelegate>``` and ```<MessagesDispatchDelegate>```, for instance your App Delegate itself :
```objectivec
@interface AppDelegate () <CatapushDelegate, MessagesDispatchDelegate>
```
and then for instances in your application delegate ```application:didFinishLaunchingWithOption```:
```objectivec
[Catapush setupCatapushStateDelegate:self andMessagesDispatcherDelegate:self];
```

```CatapushDelegate``` handles connection events, notifying the connection state, and ```MessagesDispatchDelegate``` deliver Messages to your App
Authentication

## Connection Events
```CatapushDelegate``` is in charge of notifying the state of the connection or any errors related with the Library

If connection is successfully, this delegate is triggered:
```objectivec
- (void)catapushDidConnectSuccessfully:(Catapush *)catapush
{
    UIAlertView *connectedAV = [[UIAlertView alloc] initWithTitle:@"Connected"
                                                          message:@"Catapush Connected!" delegate:self
                                                cancelButtonTitle:@"Ok" otherButtonTitles:nil];
    [connectedAV show];
}
```
## Error Handling
### Main app
Error handling comes with this delegate:
```objectivec
- (void)catapush:(Catapush *)catapush didFailOperation:(NSString *)operationName withError:(NSError *)error;
```

### Error codes


The callback might be executed with the following error codes:

| Error code | Description | Suggested strategy |
|-|-|-|
| -10404 = INVALID_APP_KEY | The app key is incorrect. | Check the app id and retry. |
| -11404 = USER_NOT_FOUND | Identifier or password not set | Please check if you have provided a valid username and password to Catapush via this method:<br><br>[Catapush setIdentifier:username andPassword:password]; |
| -10403 = WRONG_AUTHENTICATION | Credentials error | Please verify your identifier and password validity. The user might have been deleted from the Catapush app (via API or from the dashboard) or the password has changed.<br><br>You should not keep retrying, delete the stored credentials.<br>Provide a new identifier to this installation to solve the issue. |
| -11000 = GENERIC | Internal error of the remote messaging service | An unexpected internal error on the remote messaging service has occurred.<br>This is probably due to a temporary service disruption.<br>Please try again in a few minutes. |
| 11 = XMPP_MULTIPLE_LOGIN | The same user identifier has been logged on another device, the messaging service will be stopped on this device | Please check that you are using a unique identifier for each device, even on devices owned by the same user.<br>If you received this error it means that you are using the same username/password on different devices.<br>Provide a new identifier to this installation to solve the issue. |
| 14011 = API_UNAUTHORIZED | The credentials has been rejected | Please verify your identifier and password validity. The user might have been deleted from the Catapush app (via API or from the dashboard) or the password has changed.<br><br>You should not keep retrying, delete the stored credentials.<br>Provide a new identifier to this installation to solve the issue. |
| 15001 = API_INTERNAL_ERROR | Internal error of the remote messaging service | An unexpected internal error on the remote messaging service has occurred.<br>This is probably due to a temporary service disruption.<br>Please try again in a few minutes. |
| 24001 = REGISTRATION_BAD_REQUEST | Internal error of the remote messaging service | An unexpected internal error on the remote messaging service has occurred.<br>This is probably due to a temporary service disruption.<br>Please try again in a few minutes. |
| 24031 = REGISTRATION_FORBIDDEN_WRONG_AUTH | Wrong auth | Please verify your identifier and password validity. The user might have been deleted from the Catapush app (via API or from the dashboard) or the password has changed.<br><br>You should not keep retrying, delete the stored credentials.<br>Provide a new identifier to this installation to solve the issue. |
| 24041 = REGISTRATION_NOT_FOUND_APPLICATION | Application not found | You appplication is not found or not active.<br>You should not keep retrying. |
| 24042 = REGISTRATION_NOT_FOUND_USER | User not found | The user has been probably deleted from the Catapush app (via API or from the dashboard).<br><br>You should not keep retrying.<br>Provide a new identifier to this installation to solve the issue. |
| 25001 = REGISTRATION_INTERNAL_ERROR | Internal error of the remote messaging service | An unexpected internal error on the remote messaging service has occurred.<br>This is probably due to a temporary service disruption.<br>Please try again in a few minutes. |
| 34001 = OAUTH_BAD_REQUEST | Internal error of the remote messaging service | An unexpected internal error on the remote messaging service has occurred.<br>This is probably due to a temporary service disruption.<br>Please try again in a few minutes. |
| 34002 = OAUTH_BAD_REQUEST_INVALID_CLIENT | Internal error of the remote messaging service | An unexpected internal error on the remote messaging service has occurred.<br>This is probably due to a temporary service disruption.<br>Please try again in a few minutes. |
| 34003 = OAUTH_BAD_REQUEST_INVALID_GRANT | Internal error of the remote messaging service | An unexpected internal error on the remote messaging service has occurred.<br>This is probably due to a temporary service disruption.<br>Please try again in a few minutes. |
| 35001 = OAUTH_INTERNAL_ERROR | Internal error of the remote messaging service | An unexpected internal error on the remote messaging service has occurred.<br>This is probably due to a temporary service disruption.<br>Please try again in a few minutes. |
| 44031 = UPDATE_PUSH_TOKEN_FORBIDDEN_WRONG_AUTH | Credentials error | Please verify your identifier and password validity. The user might have been deleted from the Catapush app (via API or from the dashboard) or the password has changed.<br><br>You should not keep retrying, delete the stored credentials.<br>Provide a new identifier to this installation to solve the issue. |
| 44032 = UPDATE_PUSH_TOKEN_FORBIDDEN_NOT_PERMITTED | Credentials error | Please verify your identifier and password validity. The user might have been deleted from the Catapush app (via API or from the dashboard) or the password has changed.<br><br>You should not keep retrying, delete the stored credentials.<br>Provide a new identifier to this installation to solve the issue. |
| 44041 = UPDATE_PUSH_TOKEN_NOT_FOUND_CUSTOMER | Application error | You appplication is not found or not active.<br>You should not keep retrying. |
| 44042 = UPDATE_PUSH_TOKEN_NOT_FOUND_APPLICATION | Application not found | You appplication is not found or not active.<br>You should not keep retrying. |
| 44043=UPDATE_PUSH_TOKEN_NOT_FOUND_USER | User not found | Please verify your identifier and password validity. The user might have been deleted from the Catapush app (via API or from the dashboard) or the password has changed.<br><br>You should not keep retrying, delete the stored credentials.<br>Provide a new identifier to this installation to solve the issue. |
| 45001 = UPDATE_PUSH_TOKEN_INTERNAL_ERROR | Internal error of the remote messaging service when updating the push token. | Nothing, it's handled automatically by the sdk.<br>An unexpected internal error on the remote messaging service has occurred.<br>This is probably due to a temporary service disruption. |
| 55001 = UPLOAD_FILE_ERROR | There was a problem while sending an attachment.<br><br>The device is not connected to the internet or it might be blocked by a firewall or the remote messaging service might be temporarily disrupted. | You could check underlyingError to obtain the error just for debug.<br>if (error.userInfo != nil) {<br>  NSError* underlyingError = error.userInfo[NSUnderlyingErrorKey];<br>}<br>                 <br>Please try to send the message again in a few minutes. |
| 54001 = UPLOAD_FILE_MAX_SIZE_EXCEEDED | The attachment is too big, it must not be larger than 8MB. | Please reduce the attachment size. |
| 10 = NETWORK_ERROR | The SDK couldn’t establish a connection to the Catapush remote messaging service.<br>The device is not connected to the internet or it might be blocked by a firewall or the remote messaging service might be temporarily disrupted. | Please check your internet connection and try to reconnect again. |
| 13 = INTERNAL_NETWORK_ERROR | The SDK couldn’t establish a connection to the Catapush remote messaging service.<br>The device is not connected to the internet or it might be blocked by a firewall or the remote messaging service might be temporarily disrupted. | Nothing, it's handled automatically by the sdk. Just for debug.<br><br>You could check underlyingError to obtain the error just for debug.<br>if (error.userInfo != nil) {<br>  NSError* underlyingError = error.userInfo[NSUnderlyingErrorKey];<br>} |
| 12 = PUSH_TOKEN_UNAVAILABLE | Push token is not available. | Nothing, it's handled automatically by the sdk. |
| 14 = DISK_IS_FULL | No space left on device. | Remove temp files and/or notify the user. |


### Service
Error can be handled in this method:
```objectivec
- (void)handleError:(NSError * _Nonnull) error withContentHandler:(void (^_Nullable)(UNNotificationContent * _Nullable))contentHandler  withBestAttemptContent: (UNMutableNotificationContent* _Nullable) bestAttemptContent;
```

| Error code                  | Description                                                                                                                                                                                                                              | Suggested strategy                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       |
|-----------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| CatapushCredentialsError    | App key, username or password not set                                                                                                                                                                                                    | Check the app id, the username and the password and retry.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               |
| CatapushNetworkError        | The SDK couldn’t establish a connection to the Catapush remote messaging service.<br>The device is not connected to the internet or it might be blocked by a firewall or the remote messaging service might be temporarily disrupted.    | Check underlyingError if any.<br>if (error.userInfo != nil) {<br>    NSError* underlyingError = error2.userInfo[NSUnderlyingErrorKey];<br>}<br><br>The underlyingError code can be one of these: https://github.com/Catapush/catapush-ios-sdk-pod#error-handling                                                                                                                                                                                                                                                                                                                                                                         |
| CatapushNoMessagesError    | No message received                                                                                                                                                                                                                      | No new messages, this could happen if the message was already handled.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   |
| CatapushFileProtectionError | The user receive a push notification but the device was not unlocked for at least one time after the boot.<br>User data is stored in an encrypted format on disk with ```NSFileProtectionCompleteUntilFirstUserAuthentication``` policy. | Check https://developer.apple.com/documentation/foundation/nsfileprotectioncompleteuntilfirstuserauthentication for more information.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    |
| CatapushConflictErrorCode   | The same user identifier has been logged on another device, or another process.                                                                                                                                                          | Please check that you are using a unique identifier for each device, even on devices owned by the same user.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             |
| CatapushAppIsActive         | The application is active, the notification will be handled by the main app.<br>In rare cases where the the main application goes into background a few microseconds later, a notification may still be displayed.                       | In rare cases where the the main application goes into background a few microseconds later, a notification may still be displayed and is not possibile to handle the notification so it's advisable to change the notification text to handle this rare case like this:<br>```bestAttemptContent.body = @"Please open the app to read the message";```<br><br>Otherwise it will be handled in the main app here:<br>```- (void)userNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions))completionHandler;``` |


## Receiving Messages
```MessagesDispatchDelegate``` is the delegate in charge of messages dispatching. Messages are represented by a MessageIP object, and arrive in this delegate:

```objectivec
-(void)libraryDidReceiveMessageIP:(MessageIP *)messageIP
{
    NSLog(@"Did receive IP Message with identifier: %@ and body: %@", messageIP.identifier, messageIP.body);
}
```
## Reading Messages
When you consume the received Messages, you can mark them as readed if user opened the Push Notification:
```objectivec
-(void)libraryDidReceiveMessageIP:(MessageIP *)messageIP
{
    NSLog(@"Did receive IP Message with identifier: %@ and body: %@", messageIP.identifier, messageIP.body);
    UIAlertView *connectedAV = [[UIAlertView alloc] initWithTitle:@"MyApp"
                                                          message:messageIP.body
                                                         delegate:self
                                                cancelButtonTitle:@"Ok"
                                                otherButtonTitles:nil];

    [connectedAV show];
    [MessageIP sendMessageReadNotification:messageIP];
}
```
## Sending Messages
You can send text messages to Catapush server using the following method:

```objectivec
[Catapush sendMessageWithText:text];
```

and you can monitor the status of the message delivery with new two optional methods of the  ```MessagesDispatchDelegate```  protocol:
```objectivec
- (void)libraryDidSendMessage:(MessageIP *)message;
- (void)libraryDidFailToSendMessage:(MessageIP *)message;
```
the delivery status of a message is stored in the ```status``` property of MessageIP class. It's possible value are list by the enum:
```
typedef NS_ENUM(NSInteger, MESSAGEIP_STATUS)
{
    MessageIP_NOT_READ = 0,
    MessageIP_READ = 1,
    MessageIP_READ_SENT_ACK = 2,

    MessageIP_NOT_SENT = 3,
    MessageIP_SENT = 4,
    MessageIP_SENDING = 5
};
```
In case a delivery of message fails you can re-send the message using its ```messageID```:
```
+ (MessageIP *)sendMessageWithMessageId:(NSString *) messageId;
```

You can also send a message with an image, a file, or you can specify a [channel](https://www.catapush.com/docs-api?php#channels) or a replyTo (the id of the message to which the reply refers).

```
+ (MessageIP *)sendMessageWithText:(NSString *)text andChannel:(NSString *) channel;
+ (MessageIP *)sendMessageWithText:(NSString *)text andImage:(UIImage *)image;
+ (MessageIP *)sendMessageWithText:(NSString *)text andChannel:(NSString *) channel andImage:(UIImage *)image;
+ (MessageIP *)sendMessageWithText:(NSString *)text andData:(NSData *)data ofType:(NSString *)mediaType;
+ (MessageIP *)sendMessageWithText:(NSString *)text andChannel:(NSString *) channel andData:(NSData *)data ofType:(NSString *)mediaType;
+ (MessageIP *)sendMessageWithText:(NSString *)text replyTo:(NSString *) messageId;
+ (MessageIP *)sendMessageWithText:(NSString *)text andChannel:(NSString *) channel replyTo:(NSString *) messageId;
+ (MessageIP *)sendMessageWithText:(NSString *)text andImage:(UIImage *)image replyTo:(NSString *) messageId;
+ (MessageIP *)sendMessageWithText:(NSString *)text andChannel:(NSString *) channel andImage:(UIImage *)image replyTo:(NSString *) messageId;
+ (MessageIP *)sendMessageWithText:(NSString *)text andData:(NSData *)data ofType:(NSString *)mediaType replyTo:(NSString *) messageId;
+ (MessageIP *)sendMessageWithText:(NSString *)text andChannel:(NSString *) channel andData:(NSData *)data ofType:(NSString *)mediaType replyTo:(NSString *) messageId;
```
## Fetching an attachment
MessageIP questi sono i vari metodi e proprietà:
```
@interface MessageIP : NSManagedObject
// ....
@property (readonly) NSData *  mm;
@property (readonly) NSString * mmType;
@property (readonly) NSString * filename;
- (bool)hasMedia;
- (bool)isMediaReady;
- (bool)hasMediaPreview;
- (UIImage * _Nullable)imageMediaPreview;
- (void)downloadMediaWithCompletionHandler:(void (^)(NSError *error,
NSData *data))completionHandler;
// ....
@end
```
```
- (bool)hasMedia;
```
True if the message contains a media.
```
- (bool)isMediaReady;
```
True if the media has already been downloaded.
```
@property (readonly) NSData *  mm;
```
Contains the media if has already been downloaded.
```
@property (readonly) NSString * mmType;
```
The media type of the media (e.g. image/png, application/msword)
```
@property (readonly) NSString * filename;
```
The media filename.
```
- (void)downloadMediaWithCompletionHandler:(void (^)(NSError *error,
NSData *data))completionHandler;
```
Allows you to download a media if it isn't already been downloaded.
The callback contains the downloaded media and eventually the error (e.g. network error).

If the media had already been downloaded, invoking this method will have no effect.

## Get optional data
If a message contains [optional data](https://www.catapush.com/docs-api?php#2.1-post---send-a-new-message), you can get them by this method.
```
@interface MessageIP : NSManagedObject
//....
- (NSDictionary *)optionalData;
//....
@end
```

# Multiuser
If your client app require a multi user experience you have to logout the current session, in particular calling this static method:
```objectivec
+ (void)logoutStoredUser;
```
the SDK:
1) Disconnect XMPP Connection
2) Remove User data from local storage
3) Disable Push notifications removing device token from Catapush Server

This operation is asyncronous so if you need an immediate login of a new user use the block based method that gives you a completion block and a failure (in case of some occurred errors).
```objectivec
[Catapush logoutStoredUserWithCompletion:^{
   // Configure a new user
    NSError *error;
    [Catapush start:&error];
    
    if (error != nil) {
        // Handle login/start error...
    }
} failure:^{

    /*
     *
     * XMPP NOT disconnected
     * User data NOT removed from local storage
     * Device token NOT removed from Catapush Server
     *
     */

}];
```

## Forced logout

You can force the logout to clear user data and close the connection even if the call to Catapush Server fails (for example no internet connection available, timeout).

```objectivec
    [Catapush logoutStoredUser:true];


    [Catapush logoutStoredUser:true withCompletion:^{
        /*
         * Success
         *
         * XMPP disconnected
         * User data removed from local storage
         * Removed device token from Catapush Server
         *
         */
    } failure:^{
        /*
         * Device token NOT removed from Catapush Server (no internet connection available, timeout ...)
         *
         * XMPP disconnected
         * User data removed from local storage
         *
         */
    }];
```

# Advanced
Let Library knows when user read message in your own View invoking this method:
```objectivec
[MessageIP sendMessageReadNotification:messageIP];
```
You can always query the current connection state of the Library by using an Observer over the connection state, with the following method:
```objectivec
[[NSNotificationCenter defaultCenter] addObserver:self
                                         selector:@selector(checkLibraryStatus)
                                             name:kCatapushStatusChangedNotification
                                           object:nil];
```
is possible to get the current connection state with:
```objectivec
[Catapush status];
```
that returns an enum of states:
```
DISCONNECTED
CONNECTING
CONNECTED
```
in your selector as the one declared above:
```objectivec
- (void)checkLibraryStatus
{
 NSString *statusString;
    switch ([Catapush status]) {
        case CONNECTED:
            statusString = @"Connected";
           break;
        case DISCONNECTED:
            statusString = @"Disconnected";
           break;
        case CONNECTING:
            statusString = @"Connecting..";
            break;              
      }

    NSLog(@"%@", statusString);
}
```

## Messages Managers
Catapush iOS SDK manages the messages using the power of CoreData Framework. This allow an easy integration of the library and management of the ```MessageIP``` object life-cicle.

CatapushCoreData exposes:
```objectivec
    managedObjectContext
    managedObjectModel
    persistentStoreCoordinator
```
These three objects can be used with the protocol.

For example to retrieve all messages stored and their changes, set up the ```NSFetchedResultsControllerDelegate``` with the desidered ```NSPredicate``` (see below).
```objectivec
@property (nonatomic, strong) NSFetchedResultsController *fetchedResultsController; //handles all Messages
@property (nonatomic, strong) NSManagedObjectContext *managedObjectContext;

- (void)perfomFetch
{
    self.fetchedResultsController.delegate = self;

    NSError *error;
    BOOL success = [self.fetchedResultsController performFetch:&error];
    //Now all messages are stored in [self.fetchedResultsController objectAtIndexPath:indexPath];
}

- (NSFetchedResultsController *)fetchedResultsController{

if (!_fetchedResultsController) {

    NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"MessageIP"];
    request.predicate = nil;
    request.sortDescriptors = @[[NSSortDescriptor sortDescriptorWithKey:@"sentTime" ascending:YES]];

    _fetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:request
                                                                    managedObjectContext:self.managedObjectContext
                                                                      sectionNameKeyPath:nil
                                                                               cacheName:nil];
}

return _fetchedResultsController;
}

- (NSManagedObjectContext *)managedObjectContext{

if (!_managedObjectContext) {
    _managedObjectContext = [CatapushCoreData managedObjectContext];
}

return _managedObjectContext;
}
```
then observe the MessageIP changes:
```
    Insertion
    Updating
    Deletion
```

with the ```NSFetchedResultsControllerDelegate``` methods
```objectivec
#pragma mark - NSFetchedResultsControllerDelegate

- (void)controllerWillChangeContent:(NSFetchedResultsController *)controller
{
//If needed, prepare for content changes
}


- (void)controller:(NSFetchedResultsController *)controller
  didChangeSection:(id <NSFetchedResultsSectionInfo>)sectionInfo
           atIndex:(NSUInteger)sectionIndex
     forChangeType:(NSFetchedResultsChangeType)type
{
    switch(type)
    {
        case NSFetchedResultsChangeInsert:
        //New object fetched
        break;

        case NSFetchedResultsChangeDelete:
        //Object deleted
        break;
        default:
        break;
    }
}

- (void)controller:(NSFetchedResultsController *)controller
didChangeObject:(id)anObject
atIndexPath:(NSIndexPath *)indexPath
forChangeType:(NSFetchedResultsChangeType)type
newIndexPath:(NSIndexPath *)newIndexPath
{
    switch(type)
    {
        case NSFetchedResultsChangeInsert:
        //New object fetched
        break;

        case NSFetchedResultsChangeDelete:
        //Object deleted
        break;

        case NSFetchedResultsChangeUpdate:
        //Object updated
        break;

        case NSFetchedResultsChangeMove:
        //Object changes index in the array
        break;
    }
}

- (void)controllerDidChangeContent:(NSFetchedResultsController *)controller
{
    //After the changes of the fetched content
}
```
Using ```NSFetchedResultsController``` is easy to fetch messages that satisfy a condition. For Example when creating NSFetchRequest to fetch all messages just set:
```objectivec
request.predicate = nil;
```
i.e.
```objectivec
NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"MessageIP"];
request.predicate = nil;
request.sortDescriptors = @[[NSSortDescriptor sortDescriptorWithKey:@"sentTime" ascending:YES]];
```
if you need the unread messages just write:
```objectivec
request.predicate = [NSPredicate predicateWithFormat:@"status = %i", MessageIP_NOT_READ];
```
i.e.
```objectivec
NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"MessageIP"];
request.predicate = [NSPredicate predicateWithFormat:@"status = %i", MessageIP_NOT_READ];
request.sortDescriptors = @[[NSSortDescriptor sortDescriptorWithKey:@"sentTime" ascending:YES]];
```
When the objects that satisfy the defined NSPredicate you will be notifies in the NSFetchedResultsControllerDelegate methods as shown above.

##  Logging
To enable logging to the console:
```objectivec
[Catapush enableLog:true];
```

To disable:
```objectivec
[Catapush enableLog:false];
```

## Manual library integration when using use_frameworks!
Since our library isn't a framework you cannot use ```use_frameworks!``` in your Podfile, if you have to use this flag you have to include the library manually following this steps:

> **ATTENTION**: this steps doesn't works in our example project, you have to create a new empty project or include it in your project

1. git clone https://github.com/Catapush/catapush-ios-sdk-pod.git
2. open your project in XCode
3. From Finder drag this files in your project root, when asked select "Copy items if needed" and add them to **both app and the service target**.
    * catapush-ios-sdk-pod/CatapushKit/libCatapushLib.a
    * catapush-ios-sdk-pod/CatapushKit/Catapush.h
    * catapush-ios-sdk-pod/CatapushKit/CatapushLibBundle.bundle

4. Add Dependencies **for both target (app + service)**
From Project settings, select your target, and go to the "Build Phases" tab. Under "Link Binary With Libraries" use the + button to add the following Frameworks to your project:
    * Security.framework
    * CFNetwork.framework
    * SystemConfiguration.framework
    * CoreData.framework
    * libresolv

5.  **Swift only**: from XCode create a new File -> header, call it ```<projectname>-Bridging-Header.h``` and add this instruction like in this [Bridging-Header.h example](https://raw.githubusercontent.com/Catapush/catapush-ios-swift-sdk-example/master/catapush-ios-swift-sdk-example-Bridging-Header.h)
```objectivec
#import "Catapush.h"
```

6. Set Build Settings
Go ahead to "Build Settings", select "All" tab
    * Find "Linking" section and fill "Other Linker Flags" with: -ObjC -lxml2
    * **Swift only**: find "Swift Compiler - General" and fill "Objective-C Bridging Header" with: ```<projectname>-Bridging-Header.h```

## Handle badge counts
You can manage the badge by setting it, for example, to the number of messages still unread and reset them when the user opens the app.

```objectivec
//NotificationService

override func handleMessage(_ message: MessageIP?, withContentHandler contentHandler: ((UNNotificationContent?) -> Void)?, withBestAttempt bestAttemptContent: UNMutableNotificationContent?) {
    if let contentHandler = contentHandler, let bestAttemptContent = bestAttemptContent {
        if (message != nil) {
            bestAttemptContent.body = message!.body;
        }else{
            bestAttemptContent.body = NSLocalizedString("no_message", comment: "");
        }
        
        //Set the badge to the value equal to the number of messages still to be read
        let request = NSFetchRequest<NSFetchRequestResult>(entityName: "MessageIP")
        request.predicate = NSPredicate(format: "status = %i", MESSAGEIP_STATUS.MessageIP_NOT_READ.rawValue)
        request.includesSubentities = false
        do {
            let msgCount = try CatapushCoreData.managedObjectContext().count(for: request)
            bestAttemptContent.badge = NSNumber(value: msgCount)
        } catch _ {
        }
            
        contentHandler(bestAttemptContent);
    }
}
```

```objectivec
//AppDelegate

func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
    // Override point for customization after application launch.
    FirebaseApp.configure()
    Catapush.enableLog(false)
    Catapush.setAppKey(Const.appKey)
    Catapush.registerUserNotification(self)
    let firstTime = Defaults[\.firstTime]
    if firstTime {
        Catapush.logoutStoredUser(true)
        Defaults[\.firstTime] = false
    }
    Defaults[\.didFinishLaunchingWithOptions] = true

    application.applicationIconBadgeNumber = 0;
    UNUserNotificationCenter.current().delegate = self
        
    return true
}
```
# Troubleshooting
## Raw notification
The notification service extension is responsible to populate the notification content that will be presented to the user, see [Add a Notification Service Extension](https://github.com/Catapush/catapush-ios-sdk-pod#add-a-notification-service-extension).

Normally the service will receive the message and populate the notification content with the content of the message or present the proper error message to the user (ex. user not logged in, message already received...)

Sometimes however it can happen that the notification content pass through the Service unaltered, unmodified.
This could happen for two reasons:
1) Wrong configuration
2) Crash

##### Wrong configuration
In this case the notification service extension will be completelly ignored and the notification will pass throught unmodified.

The common reason is the wrong deployment target of the Service.
Check if the deployment target of the Service is the same as the deployment target of the app, you could do that by following theese steps:
[Add a Notification Service Extension](https://github.com/Catapush/catapush-ios-sdk-pod#add-a-notification-service-extension)

##### Crash
If the notification service extension crashes for whatever reason:
1) It will not stop the Notification to be shown to the user.
2) The unaltered push notification will be presented to the user.

To catch the crash you could implement [Firebase Crashlytics](https://firebase.google.com/docs/crashlytics?hl=en) that is a lightweight, realtime crash reporter, or you could track them manually by adding an UncaughtExceptionHandler to handle Objective-C exceptions and SignalHandlers.
Here is an example that works both with objective-c and swift:
[CrashEye](https://github.com/zixun/CrashEye/blob/master/CrashEye/Classes/CrashEye.swift)

##### Server side override
When you send a message you could set a custom message to prevent showing raw message to the user by populating the devicesData.ios.alert field. ([Api docs](https://www.catapush.com/docs-api?php#2.1-post---send-a-new-message))
This field is the text that is showed to the user if there is some unexpected behaviour with the Notification Service Extension and for some reason iOS ignore the Notification Extension and just display the text of the native push notification.

## Disable Notification Service during debug
In order to debug the library correctly, it is necessary to disable the action of the notification service to prevent it from handling the message during debugging.
That is necessary because if a breakpoint is triggered while receiving a message, the Notification Service get called and think that the application is closed because it cannot communicate with the main app.
In order to handle this case we can bypass all the logic inside the Notification Service by overriding the didReceive method.

```objectivec
- (void)didReceiveNotificationRequest:(UNNotificationRequest *)request withContentHandler:(void (^)(UNNotificationContent * _Nonnull))contentHandler {
    contentHandler(request.content);
}
```

```objectivec
override func didReceive(_ request: UNNotificationRequest, withContentHandler contentHandler: @escaping (UNNotificationContent) -> Void) {
    contentHandler(request.content)
}
```

# Extra

## Use the Console.app to access logs
Console is a log viewer developed by Apple Inc. and included with macOS. It allows users to search through all of the system's logged messages.

You can open the console via Spotlight or directly from the terminal with this command:
```bash
% open /System/Applications/Utilities/Console.app
```

Choose the desired device on the left to access its real time log.

If you have enabled logging (https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/README.md#logging) you could easily filter them by searching for the ```--- CATAPUSHLIB ---``` prefix in the upper right corner.


## Retrieve MessageIP after tap on a notification
If your need to access the MessageIP after tap on a notification you have to link the notification to the specific MessageIP.

The identifier property of a UNNotificationRequest object, that is the unique identifier for the notification request, could be used to create this link.

Create a link between a MessageIP and a Notification using the messageIP identifier and the request identifier and save it in the shared UserDefaults in order to allow the main app to access the data.

NotificationService (save the link):
```objectivec
@implementation NotificationService

- (void)handleMessage:(MessageIP * _Nullable) message withContentHandler:(void (^_Nullable)(UNNotificationContent * _Nullable))contentHandler  withBestAttemptContent: (UNMutableNotificationContent* _Nullable) bestAttemptContent{
    if (contentHandler != nil && bestAttemptContent != nil){
        if (message != nil) {
            bestAttemptContent.body = message.body.copy;
            //We will save the link in the user defaults shared by both the app and the extension: https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/README.md#app-groups
            NSUserDefaults* ud = [[NSUserDefaults alloc] initWithSuiteName:[[[NSBundle mainBundle] objectForInfoDictionaryKey:@"Catapush"] objectForKey:@"AppGroup"]];
            NSString* pendingMessagesKey = @"pendingMessages";
            NSDictionary* pendingMessages = [ud objectForKey:pendingMessagesKey];
            NSMutableDictionary* newPendingMessages;
            if (pendingMessages == nil) {
                newPendingMessages = [[NSMutableDictionary alloc] init];
            }else{
                newPendingMessages = [pendingMessages mutableCopy];
            }
            [newPendingMessages setValue:message.messageId forKey:[self.receivedRequest identifier]];
            [ud setValue:newPendingMessages forKey:pendingMessagesKey];
            [ud synchronize];
        }else{
            bestAttemptContent.body = @"No new message";
        }
        contentHandler(bestAttemptContent);
    }
}

@end
```
AppDelegate (retrieve the MessageIP):
```objectivec
- (void)userNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void (^)(void))completionHandler{
    //tap on the notification
    NSUserDefaults* ud = [[NSUserDefaults alloc] initWithSuiteName:[[[NSBundle mainBundle] objectForInfoDictionaryKey:@"Catapush"] objectForKey:@"AppGroup"]];
    NSString* pendingMessagesKey = @"pendingMessages";
    NSDictionary* pendingMessages = [ud objectForKey:pendingMessagesKey];
    if (pendingMessages != nil && [pendingMessages objectForKey:response.notification.request.identifier] != nil) {
        NSPredicate *predicate = [NSPredicate predicateWithFormat:@"messageId == %@", [pendingMessages objectForKey:response.notification.request.identifier]];
        NSArray *matches = [Catapush messagesWithPredicate:predicate];
        if (matches.count){
            //Exists one occurence of that Message IP
            MessageIP* messageIP = [matches firstObject];
        }
    }
    completionHandler();
}
```

# Notes
The contribution of the Catapush static library to IPA size is 650KB.
The size of the static library archive file, compiled with ENABLE_BITCODE = 1, is 60MB (it included differents architecture object files).


# Author

Alessandro Chiarotto, alessandro@catapush.com
Felice De Luca, felice@catapush.com
Matteo Corradin, matteo@catapush.com

# License

catapush-ios-sdk-pod is available under the Commercial license.See the LICENSE file for more info.
