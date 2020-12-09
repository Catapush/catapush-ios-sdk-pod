![Catapush Logo](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/catapush_logo.png)

# Catapush iOS SDK Changelog


#### 2.1.6 (09/12/2020)
Bugfix to avoid a crash that could occur due to concurrency.

JPEG compression when sending an image attachment.

#### 2.1.5 (19/10/2020)
Optimization for logoutStoredUser function.

#### 2.1.4 (05/10/2020)
Bugfix to avoid crash that could occur on network error.

Fix double notification for the ```Disconnected``` state while observing the connection state. 

#### 2.1.3 (21/08/2020)
Bugfix to avoid crash in notification service and stability improvement.

New Catapush Error code (CatapushAppIsActive), it will be returned if the user recieve a push notification and the main application is active.

#### 2.1.2 (05/08/2020)
Bugfix to avoid crash in notification service and stability improvement.

#### 2.1.1 (30/06/2020)
Improved oauth management.

#### 2.1.0 (08/06/2020)
Error code revisited.

https://github.com/Catapush/catapush-ios-sdk-pod#error-handling

#### 2.0.9 (20/05/2020)
Catapush credentials are now stored in the keychain.

#### 2.0.8 (07/05/2020)
Improved connection and retry management.

Improved update of an expired session.

#### 2.0.7] (01/04/2020)
ReplyTo support.

Optional Data support.

Improved native token update for better performances.

#### 2.0.6 (08/03/2020)
Channel support.

Bug fix concurrency oauth call.

Improved manage of 404 Mobile user not found.

Bug fix crash if cannot update time offset with NTP.

Fix delivered message not sent when multiple messages is downloaded.

#### 2.0.5 (02/02/2020)
Bugfix and stability improvement.

#### 2.0.4 (29/01/2020)
Fixed error with mixed case username.


#### 2.0.3 (11/01/2020)

Now you can enable logging for debugging purpose.
To enable logging to the console:
```ruby
[Catapush enableLog:true];
```

To disable:
```ruby
[Catapush enableLog:false];
```


New Catapush Error code (```CatapushConflictErrorCode```), it will be returned if the user recieve a push notification but the same is already connected or is connecting with another device concurrently.


#### 2.0.2 (15/12/2019)
Added the possibility to force a logout, it will remove user data from local storage and close the XMPP connection even if the call to Catapush Server fail.

Added the function ```isCatapushNotificationRequest``` to check if a ```UNNotificationRequest``` is from Catapush.

New Catapush Error code (```CatapushFileProtectionError```), it will be returned if the user recieve a push notification but the device was not unlocked for at least one time after the boot.
User data is stored in an encrypted format on disk with ```NSFileProtectionCompleteUntilFirstUserAuthentication``` policy.

https://developer.apple.com/documentation/foundation/nsfileprotectioncompleteuntilfirstuserauthentication

#### 2.0.1 (22/11/2019)
New push management without VoIP
