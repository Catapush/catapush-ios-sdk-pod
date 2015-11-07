![Catapush Logo](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/catapush_logo.png)


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
Get your App Key from [Catapush Dashboard](http://www.catapush.com/panel/dashboard) and insert it together with a couple of credentials of your choice into your application delegate ```application:didFinishLaunchingWithOption:```:
```ruby
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    [Catapush setAppKey:@"YOUR_APP_KEY"];
    [Catapush startWithIdentifier: @"test" andPassword:@"test"];
    return YES;
}
```
in the same file, fill following method with:
```ruby
- (void)applicationDidEnterBackground:(UIApplication *)application
{
    [Catapush applicationDidEnterBackground:application];
}
```
and this with:
```ruby
- (void)applicationWillEnterForeground:(UIApplication *)application
{
    [Catapush applicationWillEnterForeground:application];
}
```

##Events Handling
In order to receive events, setup then two delegates ```<CatapushDelegate>``` e ```<MessagesDispatchDelegate>```, for instance your App Delegate itself :
```ruby
@interface AppDelegate () <CatapushDelegate, MessagesDispatchDelegate>
```
and then for instances in your application delegate ```application:didFinishLaunchingWithOption```:
```ruby
[Catapush setupCatapushStateDelegate:self
       andMessagesDispatcherDelegate:self];
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




## Author

Alessandro Chiarotto, alessandro@catapush.com

## License

catapush-ios-sdk-pod is available under the Commercial license. See the LICENSE file for more info.
