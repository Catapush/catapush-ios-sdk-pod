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

## Author

Alessandro Chiarotto, alessandro@catapush.com

## License

catapush-ios-sdk-pod is available under the Commercial license. See the LICENSE file for more info.
