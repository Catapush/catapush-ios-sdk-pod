# Change Log
All notable changes to this project will be documented in this file.
`catapush-ios-sdk-pod` adheres to [Semantic Versioning](http://semver.org/).

---

## [2.0.5] (02/02/2020)
Bugfix and stability improvement.

## [2.0.4] (29/01/2020)
Fixed error with mixed case username.


## [2.0.3] (11/01/2020)

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


## [2.0.2] (15/12/2019)
Added the possibility to force a logout, it will remove user data from local storage and close the XMPP connection even if the call to Catapush Server fail.

Added the function ```isCatapushNotificationRequest``` to check if a ```UNNotificationRequest``` is from Catapush.

New Catapush Error code (```CatapushFileProtectionError```), it will be returned if the user recieve a push notification but the device was not unlocked for at least one time after the boot.
User data is stored in an encrypted format on disk with ```NSFileProtectionCompleteUntilFirstUserAuthentication``` policy.

https://developer.apple.com/documentation/foundation/nsfileprotectioncompleteuntilfirstuserauthentication

## [2.0.1] (22/11/2019)
New push management without VoIP