![Catapush Logo](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/catapush_logo.png)


# catapush-ios-sdk-pod

Catapush is a simple, reliable and scalable delivery API for transactional push notifications for websites and applications. Ideal for sending data-driven transactional notifications including targeted e-commerce and personalized one-to-one messages.

Get delivery confirmation with real time status for each message sent and build intelligent messaging logic in your applications

## Installation

Catapush iOS sdk is available through [CocoaPods](http://cocoapods.org). To install
it, simply add the following line to your Podfile:

```ruby
pod "catapush-ios-sdk-pod"
```

#QuickStart

Import ```Catapush.h``` into your application delegate as follows:
```ruby
#import "Catapush.h"
```
Get your App Key from [Catapush Dashboard](http://www.catapush.com/panel/dashboard) and insert it together with a couple of credentials of your choice into your application delegate:
```ruby
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    [Catapush setAppKey:@"YOUR_APP_KEY"];
    
    [Catapush setIdentifier:@"test" andPassword:@"test"];
    
    [Catapush setupCatapushStateDelegate:self andMessagesDispatcherDelegate:self];
    
    [Catapush registerUserNotification:self voIPDelegate:nil];
    
    NSError *error;
    [Catapush start:&error];
    
    if (error != nil) {
        // Handle error...
    }
    
    [self setupUI];
    [UNUserNotificationCenter currentNotificationCenter].delegate = self;

    return YES;
}

```

Note: Catapush DOES register user notification for you, so DO NOT register user notification by calling instance method of ```UIApplication```.

These are pre-requisites for setting up your application with Catapush.
### Create and configure the authentication key
This section describes how to generate an authentication key for an App ID enabled for Push Notifications. If you have an existing key, you can use that key instead of generating a new one.

To create an authentication key:
1) In your [Apple Developer Member Center](https://developer.apple.com/account), go to Certificates, Identifiers & Profiles, and select Keys.
2) Click the Add button (+) in the upper-right corner.
3) Enter a description for the APNs Auth Key.
4) Under Key Services, select the Apple Push Notifications service (APNs) checkbox, and click Continue.
5) Click Confirm and then Download. Save your key in a secure place. This is a one-time download, and the key cannot be retrieved later.

Once you have download it you have to configure your Catapush application.
1) Go to https://www.catapush.com/panel/apps/YOUR_APP_ID/platforms.
2) Click on iOS Token Based to enable it.
3) Fill iOS Team Id, iOS Key Id, iOS AuthKey and iOS Topic.

The iOS Team Id can be found here https://developer.apple.com/account/#/membership in "Membership Information" section.

The iOS Key Id can be retrieved here https://developer.apple.com/account/resources/authkeys/list, click on the key you have created and you can find it under "View Key Details" section.

The iOS AuthKey is the content of the key file.

Example:
```ruby
-----BEGIN PRIVATE KEY-----
...........................
          AUTH_KEY
...........................
-----END PRIVATE KEY-----
```

The iOS Topic is the bundle identifier of your iOS application.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/catapush_ios_token_based.png)

### Notification Service Extension
In order to process the push notification a Notification Service Extension is required.
Add a Notification Service Extension (in Xcode File -> New -> Target...) that extends CatapushNotificationServiceExtension
```ruby
@interface CatapushNotificationServiceExtension : UNNotificationServiceExtension
- (void)handleMessage:(MessageIP *) message;
- (void)handleError:(NSError *) error;
@end
```
Implements these two methods to customize the push notification.
Example:
```ruby
- (void)handleError:(NSError *) error{
    if (error.code == CatapushCredentialsError) {
        self.bestAttemptContent.body = @"User not logged in";
    }
    if (error.code == CatapushNetworkError) {
        self.bestAttemptContent.body = @"Newtork error";
    }
    if (error.code == CatapushNoMessagesError) {
        self.bestAttemptContent.body = @"No new message";
    }
}

- (void)handleMessage:(MessageIP *) message{
    if (message != nil) {
        self.bestAttemptContent.body = message.body.copy;
    }else{
        self.bestAttemptContent.body = @"No new message";
    }
    self.contentHandler(self.bestAttemptContent);
}
```
Example projects
[Obj-C](https://github.com/Catapush/catapush-ios-sdk-example)
[Swift](https://github.com/Catapush/catapush-ios-swift-sdk-example)

### AppGroups
Catapush need that the Notification Service Extension and the main application can share resources.
In order to do that you have to create and enable a specific app group for both the application and the extension.
The app and the extansion must be in the same app group.
![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/appgroup_1.png)
![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/appgroup_2.png)
![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/catapush_ios_token_based.png)

You should also add this information in the App plist and the Extension plist:
```ruby
    <key>Catapush</key>
    <dict>
        <key>AppGroup</key>
        <string>group.example.group</string>
    </dict>
```

##Push Notification
```ruby
#pragma mark - End developer user must  declare in order to let AOP to inject catapush library code
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
   // Custom code (can be empty)
}

#pragma mark - End developer user must  declare in order to let AOP to inject catapush library code
- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error {
    // Custom code (can be empty)
}

```

Catapush injects code on the above methods in order to intercept their calls once a notification is received.
The injected code will handle incoming push notification. You can insert custom code if you have specific needs or
you can let the implentations empty.



## Events Handling
In order to receive events, setup then two delegates ```<CatapushDelegate>``` and ```<MessagesDispatchDelegate>```, for instance your App Delegate itself :
```ruby
@interface AppDelegate () <CatapushDelegate, MessagesDispatchDelegate>
```
and then for instances in your application delegate ```application:didFinishLaunchingWithOption```:
```ruby
[Catapush setupCatapushStateDelegate:self andMessagesDispatcherDelegate:self];
```

```CatapushDelegate``` handles connection events, notifying the connection state, and ```MessagesDispatchDelegate``` deliver Messages to your App
Authentication

```CatapushDelegate``` is in charge of notifying the state of the connection or any errors related with the Library

If connection is successfully, this delegate is triggered:
```ruby
- (void)catapushDidConnectSuccessfully:(Catapush *)catapush
{
	UIAlertView *connectedAV = [[UIAlertView alloc] initWithTitle:@"Connected"
                                                          message:@"Catapush Connected!" delegate:self
                                                cancelButtonTitle:@"Ok" otherButtonTitles:nil];
	[connectedAV show];
}
```
Error handling comes with this delegate:
```ruby
- (void)catapush:(Catapush *)catapush didFailOperation:(NSString *)operationName withError:(NSError *)error
{

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
```
## Receiving Messages
```MessagesDispatchDelegate``` is the delegate in charge of messages dispatching. Messages are represented by a MessageIP object, and arrive in this delegate:

```ruby
-(void)libraryDidReceiveMessageIP:(MessageIP *)messageIP
{
    NSLog(@"Did receive IP Message with identifier: %@ and body: %@", messageIP.identifier, messageIP.body);
}
```
## Reading Messages
When you consume the received Messages, you can mark them as readed if user opened the Push Notification:
```ruby
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
You can send text messages to Catapuhs server using the following method:

```ruby
[Catapush sendMessageWithText:text];
```

and you can monitor the status of the message delivery with new two optional methods of the  ```MessagesDispatchDelegate```  protocol:
```ruby
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
# Multiuser
If your client app require a multi user experience you have to logout the current session, in particular calling this static method:
```ruby
+ (void)logoutStoredUser;
```
the SDK:
1) Disconnect XMPP Connection
2) Remove User data from local storage
3) Disable Push notifications removing device token from Catapush Server

This operation is asyncronous so if you need an immediate login of a new user use the block based method that gives you a completion block and a failure (in case of some occurred errors).
```ruby
[Catapush logoutStoredUserWithCompletion:^{
   // Configure a new user
    NSError *error;
    [Catapush start:&error];
    
    if (error != nil) {
        // Handle login/start error...
    }
} failure:^{
    // Handle logout error...
}];
```


# Advanced
Let Library knows when user read message in your own View invoking this method:
```ruby
[MessageIP sendMessageReadNotification:messageIP];
```
You can always query the current connection state of the Library by using an Observer over the connection state, with the following method:
```ruby
[[NSNotificationCenter defaultCenter] addObserver:self
                                         selector:@selector(checkLibraryStatus)
                                             name:kCatapushStatusChangedNotification
                                           object:nil];
```
is possible to get the current connection state with:
```ruby
[Catapush status];
```
that returns an enum of states:
```
DISCONNECTED
CONNECTING
CONNECTED
```
in your selector as the one declared above:
```ruby
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

##Messages Managers
Catapush iOS SDK manages the messages using the power of CoreData Framework. This allow an easy integration of the library and management of the ```MessageIP``` object life-cicle.

CatapushCoreData exposes:
```ruby
    managedObjectContext
    managedObjectModel
    persistentStoreCoordinator
```
These three objects can be used with the protocol.

For example to retrieve all messages stored and their changes, set up the ```NSFetchedResultsControllerDelegate``` with the desidered ```NSPredicate``` (see below).
```ruby
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
```ruby
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
```ruby
request.predicate = nil;
```
i.e.
```ruby
NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"MessageIP"];
request.predicate = nil;
request.sortDescriptors = @[[NSSortDescriptor sortDescriptorWithKey:@"sentTime" ascending:YES]];
```
if you need the unread messages just write:
```ruby
request.predicate = [NSPredicate predicateWithFormat:@"status = %i", MessageIP_NOT_READ];
```
i.e.
```ruby
NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"MessageIP"];
request.predicate = [NSPredicate predicateWithFormat:@"status = %i", MessageIP_NOT_READ];
request.sortDescriptors = @[[NSSortDescriptor sortDescriptorWithKey:@"sentTime" ascending:YES]];
```
When the objects that satisfy the defined NSPredicate you will be notifies in the NSFetchedResultsControllerDelegate methods as shown above.

## Notes
The contribution of the Catapush static library to IPA size is 650KB.
The size of the static library archive file, compiled with ENABLE_BITCODE = 1, is 60MB (it included differents architecture object files).


## Author

Alessandro Chiarotto, alessandro@catapush.com
Felice De Luca, felice@catapush.com
Matteo Corradin, matteo@catapush.com

## License

catapush-ios-sdk-pod is available under the Commercial license.See the LICENSE file for more info.
