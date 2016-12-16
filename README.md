![Catapush Logo](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/catapush_logo.png)


# catapush-ios-sdk-pod

Catapush is a simple, reliable and scalable delivery API for transactional push notifications for websites and applications. Ideal for sending data-driven transactional notifications including targeted e-commerce and personalized one-to-one messages.

Get delivery confirmation with real time status for each message sent and build intelligent messaging logic in your applications

## Installation

catapush-ios-sdk-pod is available through [CocoaPods](http://cocoapods.org). To install
it, simply add the following line to your Podfile:

```ruby
pod "catapush-ios-sdk-pod"
```

#QuickStart

Import ```CatapushHeaders.h``` into your application delegate as follows:
```ruby
#import "CatapushHeaders.h"
```
Get your App Key from [Catapush Dashboard](http://www.catapush.com/panel/dashboard) and insert it together with a couple of credentials of your choice into your application delegate:
```ruby
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    [Catapush setAppKey:@"YOUR_APP_KEY"];

    // Register for push notification Standard or VoIP based on capabilites setting in Xcode Project
    [Catapush registerUserNotification:self voIPDelegate:nil];

    [Catapush startWithIdentifier: @"test" andPassword:@"test"];

    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    [Catapush applicationDidEnterBackground:application];
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    [Catapush applicationWillEnterForeground:application];
}
```
The method ```registerUserNotification/2``` requests registration for remote notification. If VoIP background mode in an app
is enabled in XCode capabilites, then the method requests a registration for VoIP notification.

Note: Catapush DOES register user notification for you, so DO NOT register user notification by calling instance method  ```registerUserNotificationSettings/1``` of ```UIApplication```.

##Enabling Voice Over IP Push Notifications
Set the following capabilites in your XCode project:

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/capabilities_remote_xcode.png)

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/capabilities_xcode.png)

If VoIP is enabled, Catapush library will display alert message and play default sound when a notification is received
(you don't need to write code for showing alert message).
If you want to use standard notification just select ```Remote Notification``` (and unselect Voip).

The 2nd argument of the method ```registerUserNotification/2``` is a ```VoIPNotificationDelegate``` delegate.
The protocol ```VoIPNotificationDelegate``` has one method ```didReceiveIncomingPushWithPayload:(PKPushPayload *)payload``` called when a notification is received.  You can implement this method, and write your custom code, but  Catapush
library will not display any alert or play a sound when a notification is received.

##Certificate, App Id, and Entitlements
These are pre-requisites for setting up VoIP with Catapush.
* Make sure your app has an explicit app id and push entitlements in Apple's Developer Portal.
* Create a VoIP Push Certificate. This can be re-used for development and production.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/voip_cert.png)

* Import the VoIP Push Certificate into Keychain Access and export as a .p12 file.
* Upload the exported .p12 file into your Catapush Application ("Platform" menu item).


##Standard Push Notification
If you are going to use VoIP notification you can omit the following method declarations:
```ruby
#pragma mark - End developer user must  declare in order to let AOP to inject catapush library code
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    // Custom End user code, can be empty
}

# pragma mark
- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
    // Custom End user code, can be empty
}

# pragma mark End developer user must be declare in order to let AOP to inject catapush library code
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    // Custom End user code, can be empty
}

# pragma mark End developer user Must be declared in order to let AOP to inject catapush library code
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler
{
    // Custom End user code, can be empty
}

```

Catapush injects code on the above methods in order to intercept their calls once a (standard) notification is received.
The injected code will handle incoming push notification. You can insert custom code if you have specific needs or
you can let the implentations empty.






##Events Handling
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
##Receiving Messages
```MessagesDispatchDelegate``` is the delegate in charge of messages dispatching. Messages are represented by a MessageIP object, and arrive in this delegate:

```ruby
-(void)libraryDidReceiveMessageIP:(MessageIP *)messageIP
{
    NSLog(@"Did receive IP Message with identifier: %@ and body: %@", messageIP.identifier, messageIP.body);
}
```
##Reading Messages
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
##Sending Messages
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


#Advanced
Let Library knows when user read message in your own View invoking this method:
```ruby
[MessageIP sendMessageReadNotification:messageIP];
```
You can always query the current connection state of the Library by using an Observer over the connection state, with folling method:
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

##Notes
The contribution of the Catapush static library to IPA size is 650KB.
The size of the static library archive file, compiled with ENABLE_BITCODE = 1, is 60MB (it included differents architecture object files).

## Author

Alessandro Chiarotto, alessandro@catapush.com
Felice De Luca, felice@catapush.com

## License

catapush-ios-sdk-pod is available under the Commercial license.See the LICENSE file for more info.
