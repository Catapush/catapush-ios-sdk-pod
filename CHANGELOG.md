![Catapush Logo](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/catapush_logo.png)

# Catapush iOS SDK Changelog

### 2.2.4 (02/04/2024)
##### Added
- Added PrivacyInfo.xcprivacy file.
- logoutStoredUser deprecated, use logoutStoredUserWithCompletion

##### Integration changes required
The minimun iOS version is 12

### 2.2.3 (12/09/2023)
##### Fixed
- Fixed an issue to avoid unnecessary requests.

##### Integration changes required
None

### 2.2.2 (22/11/2022)
##### Added
- Fix a concurrency issue that can cause multiple read request
- Fix an issue that can cause a missing delivery message

##### Integration changes required
None

### 2.2.1 (13/10/2022)
##### Fixed
- Fixed an issue to avoid unnecessary requests.

##### Integration changes required
None

### 2.2.0 (27/09/2022)
##### Fixed
- Compatibility update for iOS 16 and Xcode 14.

##### Integration changes required
The minimun iOS version is 11

### 2.1.18 (10/08/2022)
##### Fixed
- Fixed nullability warning when compiling for swift.

##### Integration changes required
Need to change the signature of methods to handle the new nullability.

### 2.1.17 (17/05/2022)
##### Fixed
- Added a new error code when disk space runs out. (DISK_IS_FULL)

##### Integration changes required
None

### 2.1.16 (11/01/2022)
##### Fixed
- Fixed a bug that could cause a null to be sent to the server.

##### Integration changes required
None

### 2.1.15 (12/10/2021)
##### Fixed
- Bugfix to avoid a crash that could occur due to concurrency after logout.
- Fixed a bug that could cause a crash during NTP sync.

##### Integration changes required
None

### 2.1.14 (29/06/2021)
##### Fixed
- Bugfix to avoid a crash that could occur due to concurrency.

##### Integration changes required
None

### 2.1.13 (25/05/2021)
##### Fixed
- Fixed a bug to allow new customers to connect to the correct endpoint.
- Changed name of mediaPreview property of the MessageIP to imageMediaPreview.

##### Added
- UPLOAD_FILE_MAX_SIZE_EXCEEDED error if the attachment is too big (8MB).

##### Integration changes required
Replace usage of mediaPreview property of the MessageIP with imageMediaPreview.

### 2.1.12 (20/04/2021)
##### Fixed
- Fixed an error that could lead to a push notification message not being displayed due to concurrency.
- Fixed an error that can cause a crash on stop.
- Fixed crash on core data NSException due to disk full ([NSPersistentStoreCoordinator _coordinator_you_never_successfully_opened_the_database_disk_full:]).

##### Integration changes required
None

### 2.1.11 (23/02/2021)
##### Fixed
- Fixed an error that could lead to a loop when couldn't establish a connection.
- Fixed an error that can cause a crash during NTP sync.

##### Integration changes required
None

### 2.1.10 (16/02/2021)
##### Added
- Added the possibility to manually handle the push notification register request passing false as ```withRequest``` flag in the method:
```+ (void)registerUserNotification:(UIResponder *) appDelegate withRequest:(bool) withRequest;```
- Added 2 new error codes:
```INTERNAL_NETWORK_ERROR``` that represents network errors that are handled automatically by the framework and don't require a retry.
```UPLOAD_FILE_ERROR``` that represents an error during file upload.

##### Integration changes required
None

### 2.1.9 (03/02/2021)
##### Fixed
- Fixed a bug that prevented login during the update of the SDK.

##### Integration changes required
None

### 2.1.8 (25/01/2021)
##### Fixed
- logoutStoredUser function will now also remove the stored credential from the keychain.

##### Integration changes required
None

### 2.1.7 (22/12/2020)
##### Fixed
- Improved speed when a new message is received.

##### Integration changes required
None

### 2.1.6 (09/12/2020)
##### Fixed
- Bugfix to avoid a crash that could occur due to concurrency.

##### Added
- JPEG compression when sending an image attachment.

##### Integration changes required
None

### 2.1.5 (19/10/2020)
##### Fixed
- Optimization for logoutStoredUser function.

##### Integration changes required
None

### 2.1.4 (05/10/2020)
##### Fixed
- Bugfix to avoid crash that could occur on network error.
- Fix double notification for the ```Disconnected``` state while observing the connection state. 

##### Integration changes required
None

### 2.1.3 (21/08/2020)
##### Fixed
- Bugfix to avoid crash in notification service and stability improvement.
- New Catapush Error code (```CatapushAppIsActive```), it will be returned if the user recieve a push notification and the main application is active.

##### Integration changes required
- CatapushNotificationServiceExtension signature changed, now includes contentHandler and bestAttemptContent, the implementation should be updated accordingly and you must use these parameters to properly handle the notification.

- A new error code (```CatapushAppIsActive```) has been added and needs to be handled.
When a push notification is received, the main application may already be running and active, in these cases the Service does not intervene and the application will manage the notification by invoking this method:
```- (void) userNotificationCenter: (UNUserNotificationCenter *) center willPresentNotification: (UNNotification *) notification withCompletionHandler: (void (^) (UNNotificationPresentationOptions)) completionHandler;```
In rare cases it may happen that, if the application were to go in the background a fraction of a second after receiving the notification, and then after informing the service that it was active, the native notification that we pass to Apple is shown.
In this case you can, for example, change the message to this: ```bestAttemptContent.body = @ "Please open the app to read the message";``` or a similar message.
This error was already handled internally in previous versions, now it has been exposed in order to manage rare cases in which a notification is received from an open APP but which will go into the background in a little while (a few microseconds) and avoid showing the notification native passed to Apple.

### 2.1.2 (05/08/2020)
##### Fixed
- Bugfix to avoid crash in notification service and stability improvement.

##### Integration changes required
None

### 2.1.1 (30/06/2020)
##### Fixed
- Improved oauth management.

##### Integration changes required
None

### 2.1.0 (08/06/2020)
##### Fixed
- Error code revisited.
https://github.com/Catapush/catapush-ios-sdk-pod#error-handling

##### Integration changes required
None

### 2.0.9 (20/05/2020)
##### Added
- Catapush credentials are now stored in the keychain.

##### Integration changes required
None

### 2.0.8 (07/05/2020)
##### Fixed
- Improved connection and retry management.
- Improved update of an expired session.

##### Integration changes required
None

### 2.0.7 (01/04/2020)
##### Added
- ReplyTo support.
- Optional Data support.

##### Fixed
- Improved native token update for better performances.

##### Integration changes required
None

### 2.0.6 (08/03/2020)
##### Added
- Channel support.

##### Fixed
- Bug fix concurrency oauth call.
- Improved manage of 404 Mobile user not found.
- Bug fix crash if cannot update time offset with NTP.
- Fix delivered message not sent when multiple messages is downloaded.

##### Integration changes required
None

### 2.0.5 (02/02/2020)
##### Fixed
- Bugfix and stability improvement.

##### Integration changes required
None

### 2.0.4 (29/01/2020)
##### Fixed
- Fixed error with mixed case username.

##### Integration changes required
None

### 2.0.3 (11/01/2020)
##### Added
- Now you can enable logging for debugging purpose.
```[Catapush enableLog:true];```
```[Catapush enableLog:false];```

- New Catapush Error code (```CatapushConflictErrorCode```), it will be returned if the user recieve a push notification but the same is already connected or is connecting with another device concurrently.

##### Integration changes required
None

### 2.0.2 (15/12/2019)
##### Added
- Added the possibility to force a logout, it will remove user data from local storage and close the XMPP connection even if the call to Catapush Server fail.
- Added the function ```isCatapushNotificationRequest``` to check if a ```UNNotificationRequest``` is from Catapush.
- New Catapush Error code (```CatapushFileProtectionError```), it will be returned if the user recieve a push notification but the device was not unlocked for at least one time after the boot.
User data is stored in an encrypted format on disk with ```NSFileProtectionCompleteUntilFirstUserAuthentication``` policy.
https://developer.apple.com/documentation/foundation/nsfileprotectioncompleteuntilfirstuserauthentication

##### Integration changes required
None

### 2.0.1 (22/11/2019)
- New push management without VoIP
