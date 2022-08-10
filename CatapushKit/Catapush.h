//
//  Catapush.h
//  Catapush Lib
//
//
//  Copyright (c) 2015 Catapush Srl. All rights reserved.
//

/*
 CatapushLib needs the following frameworks:
 
 Security.framework
 CFNetwork.framework
 libresolv.dylib
 SystemConfiguration.framework
 CoreData.framework
 PushKit.framework <If available>
 
 Then needs the '-ObjC -lXml2' other linker flag in project Build Configuration
 
 Add Catapush
    libCatapushLib.a
    Catapush.h
    CatapushLibBundle.bundle
 
 Make sure Copy items if needed is checked.

 */

#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>
#import <UserNotifications/UserNotifications.h>

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_8_0
#import <PushKit/PushKit.h>
#endif

#define CATAPUSH_ERROR_DOMAIN @"com.catapush.error"
#define kCatapushStatusChangedNotification @"CatapushStatusChangedNotification"

/**
 * Catapush Error code returned by the Catapush's start/1 method and CatapushNotificationServiceExtension handleError method.
 *
 */
enum {
    CatapushCredentialsError,
    CatapushNetworkError,
    CatapushNoMessagesError,
    CatapushFileProtectionError,
    CatapushConflictErrorCode,
    CatapushAppIsActive
};

/**
 * Delivery status of a message.
 *
 */
typedef NS_ENUM(NSInteger, MESSAGEIP_STATUS)
{
    MessageIP_NOT_READ = 0,
    MessageIP_READ = 1,
    MessageIP_READ_SENT_ACK = 2,
    
    MessageIP_NOT_SENT = 3,
    MessageIP_SENT = 4,
    MessageIP_SENDING = 5
};

/**
 * Message type of message.
 *
 */
typedef NS_ENUM(NSInteger, MESSAGEIP_TYPE)
{
    MessageIP_TYPE_OUTGOING = 0,
    MessageIP_TYPE_INCOMING = 1
};



typedef NS_ENUM(NSInteger, CatapushErrorCode)
{
    INVALID_APP_KEY = -10404,
    USER_NOT_FOUND = -11404,
    WRONG_AUTHENTICATION = -10403,
    GENERIC = -11000,
    XMPP_MULTIPLE_LOGIN = 11,
    API_UNAUTHORIZED = 14011,
    API_INTERNAL_ERROR = 15001,
    REGISTRATION_BAD_REQUEST = 24001,
    REGISTRATION_FORBIDDEN_WRONG_AUTH = 24031,
    REGISTRATION_NOT_FOUND_APPLICATION = 24041,
    REGISTRATION_NOT_FOUND_USER = 24042,
    REGISTRATION_INTERNAL_ERROR = 25001,
    OAUTH_BAD_REQUEST = 34001,
    OAUTH_BAD_REQUEST_INVALID_CLIENT = 34002,
    OAUTH_BAD_REQUEST_INVALID_GRANT = 34003,
    OAUTH_INTERNAL_ERROR = 35001,
    UPDATE_PUSH_TOKEN_FORBIDDEN_WRONG_AUTH = 44031,
    UPDATE_PUSH_TOKEN_FORBIDDEN_NOT_PERMITTED = 44032,
    UPDATE_PUSH_TOKEN_NOT_FOUND_CUSTOMER = 44041,
    UPDATE_PUSH_TOKEN_NOT_FOUND_APPLICATION = 44042,
    UPDATE_PUSH_TOKEN_NOT_FOUND_USER = 44043,
    UPDATE_PUSH_TOKEN_INTERNAL_ERROR = 45001,
    UPLOAD_FILE_ERROR = 55001,
    UPLOAD_FILE_MAX_SIZE_EXCEEDED = 54001,
    NETWORK_ERROR = 10,
    PUSH_TOKEN_UNAVAILABLE = 12,
    INTERNAL_NETWORK_ERROR = 13,
    DISK_IS_FULL = 14
};




/******************************************
 ***            MESSAGE IP             ***
 ******************************************/

@interface MessageIP : NSManagedObject

@property (nonatomic, retain) NSNumber * _Nonnull status;

@property (readonly) NSNumber * _Nonnull type; //Outgoing or Incoming

@property (readonly) NSString * _Nonnull messageId;
@property (readonly) NSString * _Nullable sender;
@property (readonly) NSDate * _Nonnull sentTime;
@property (readonly) NSString * _Nonnull body;


+ (void)sendMessageReadNotification:(MessageIP * _Nonnull)message;
+ (void)sendMessageReadNotificationWithId:(NSString * _Nonnull)messageId;

//Media
@property (readonly) NSData * _Nullable mm;
@property (readonly) NSString * _Nullable mmType;
@property (readonly) NSString * _Nullable filename;

//Channel
@property (readonly) NSString * _Nullable channel;
//ReplyTo
@property (readonly) NSString * _Nullable originalMessageId;

- (bool)hasMedia;
- (bool)isMediaReady;

//Media Preview
- (bool)hasMediaPreview;
- (UIImage * _Nullable)imageMediaPreview;

- (void)downloadMediaWithCompletionHandler:(void (^_Nullable)(NSError * _Nullable error, NSData * _Nullable data))completionHandler;

- (NSDictionary * _Nullable)optionalData;

@end

/**
 *  Protocol to deal with received Messages
 */
@protocol MessagesDispatchDelegate <NSObject>

/**
 *  Method called when the library receives a new MessageIp
 *
 *  @param messageIP new received MessageIP
 */
- (void)libraryDidReceiveMessageIP:(MessageIP * _Nullable)messageIP;

//Outgoing Messages Delegate
@optional
- (void)libraryDidSendMessage:(MessageIP * _Nullable)message;
- (void)libraryDidFailToSendMessage:(MessageIP * _Nullable)message;
@end

/******************************************
 ***              CATAPUSH              ***
 ******************************************/


@class Catapush;


@protocol CatapushDelegate <NSObject>

- (void)catapushDidConnectSuccessfully:(Catapush * _Nonnull)catapush;
- (void)catapush:(Catapush * _Nonnull)catapush didFailOperation:(NSString * _Nullable)operationName withError:(NSError * _Nullable)error;

@end


/**
 *  Main Catapush Library Interface
 */
@interface Catapush : NSObject

typedef NS_ENUM(NSInteger, CatapushStatus)
{
    DISCONNECTED = -1,
    CONNECTING = 0,
    CONNECTED = 1
};

+ (CatapushStatus) status;


//Messages
+ (NSArray * _Nonnull)allMessages;
+ (NSArray * _Nonnull)messagesWithPredicate:(NSPredicate *_Nullable)predicate;

+ (void)removeAllMessages;
+ (void)removeMessage:(MessageIP * _Nullable)message;
+ (MessageIP * _Nonnull)sendMessageWithText:(NSString * _Nullable)text;
+ (MessageIP * _Nullable)sendMessageWithMessageId:(NSString * _Nonnull) messageId;
+ (MessageIP * _Nonnull)sendMessageWithText:(NSString * _Nullable)text andChannel:(NSString * _Nullable) channel;
+ (MessageIP * _Nonnull)sendMessageWithText:(NSString * _Nullable)text andImage:(UIImage * _Nullable)image;
+ (MessageIP * _Nonnull)sendMessageWithText:(NSString * _Nullable)text andChannel:(NSString * _Nullable) channel andImage:(UIImage * _Nullable)image;
+ (MessageIP * _Nonnull)sendMessageWithText:(NSString * _Nullable)text andData:(NSData * _Nullable)data ofType:(NSString * _Nullable)mediaType;
+ (MessageIP * _Nonnull)sendMessageWithText:(NSString * _Nullable)text andChannel:(NSString * _Nullable) channel andData:(NSData * _Nullable)data ofType:(NSString * _Nullable)mediaType;
+ (MessageIP * _Nonnull)sendMessageWithText:(NSString * _Nullable)text replyTo:(NSString * _Nullable) messageId;
+ (MessageIP * _Nonnull)sendMessageWithText:(NSString * _Nullable)text andChannel:(NSString * _Nullable) channel replyTo:(NSString * _Nullable) messageId;
+ (MessageIP * _Nonnull)sendMessageWithText:(NSString * _Nullable)text andImage:(UIImage * _Nullable)image replyTo:(NSString * _Nullable) messageId;
+ (MessageIP * _Nonnull)sendMessageWithText:(NSString * _Nullable)text andChannel:(NSString * _Nullable) channel andImage:(UIImage * _Nullable)image replyTo:(NSString * _Nullable) messageId;
+ (MessageIP * _Nonnull)sendMessageWithText:(NSString * _Nullable)text andData:(NSData * _Nullable)data ofType:(NSString * _Nullable)mediaType replyTo:(NSString * _Nullable) messageId;
+ (MessageIP * _Nonnull)sendMessageWithText:(NSString * _Nullable)text andChannel:(NSString * _Nullable) channel andData:(NSData * _Nullable)data ofType:(NSString * _Nullable)mediaType replyTo:(NSString * _Nullable) messageId;

/**
 *  Catapush will hook for didRegisterForRemoteNotificationsWithDeviceToken
 *  in order to receive the device token
 *  and registers for notifying the user with the following options:
 *  UIUserNotificationTypeAlert | UIUserNotificationTypeBadge | UIUserNotificationTypeSound.
 */
+ (void)registerUserNotification:(UIResponder * _Nonnull) appDelegate;

/**
 *  Catapush will hook for didRegisterForRemoteNotificationsWithDeviceToken
 *  in order to receive the device token
 *  @param appDelegate appDelegate
 *  @param withRequest if true, registers for notifying the user with the following options UIUserNotificationTypeAlert | UIUserNotificationTypeBadge | UIUserNotificationTypeSound
 */
+ (void)registerUserNotification:(UIResponder * _Nonnull) appDelegate withRequest:(bool) withRequest;


//Library Status
+ (BOOL)isConnected;

/**
 *  This Methods connects the library to the host to receive Messages.
 *  It needs indentifier, password
 *
 * @param identifier user identifer
 * @param password user password
 */

+ (void)startWithIdentifier:(NSString * _Nonnull)identifier andPassword:(NSString * _Nonnull)password __attribute((deprecated(("startWithIdentifier/2 has been deprecated, use setIdentifier/2 and start/1 instead."))));
/**
 *  Save username and password of the user.
 *  This method must be called before calling start/0 method.
 *
 *  @param username identifier of the user
 *  @param password password of the user
 *
 */
 
+(void) setIdentifier:(NSString * _Nullable) username andPassword:(NSString * _Nullable) password;
/**
 *  Return the identifier of the user. The method returns nil if the Capush status is equals to LOGOUT or if the indetifer has
 *  never been set (using setIdentifier/2).
 *
 *  @return identifier of the user
 */
+ (NSString * _Nullable) userIdentifier;

/**
 * Return the password of the user. The method returns nil if the Capush status is equals to LOGOUT or if the password has
 * never been set using setIdentifier/2
 *
 *  @return password of the user
 */
+ (NSString * _Nullable) userPassword;

/**
 *  This Methods connects the library to the host to receive Messages.
 *  It needs the indentifier and the password of the user (see setIndetifier/2 to set user credentials).
 *  The optional error return indicates problem that was encountered (for instance, the absence indentifier and/or password).
 *
 */

+ (void) start:(NSError *_Nullable*_Nullable) error;


/**
 *  Disconnect XMPP Connection, Push Notifications are still enabled
 *
 */
+ (void)stop;

/**
 *  Call this method before start stream with 'start' method.
 Tells the Catapush Library which are stateDelegate and messagesDelegate
 *
 *
 *  @param stateDelegate    who is delegate to know the Catapush connection status
 *  @param messagesDelegate who is delegate to be informed when a new message has been received
 */
+ (void)setupCatapushStateDelegate:(id<CatapushDelegate> _Nullable)stateDelegate
     andMessagesDispatcherDelegate:(id<MessagesDispatchDelegate> _Nullable)messagesDelegate;


/**
 *  Key to register App with the Catapush service
 *
 *  @param appKey app key
 */
+ (void)setAppKey:(NSString * _Nullable)appKey;



+ (void)applicationDidEnterBackground:(UIApplication * _Nonnull)application;
+ (void)applicationDidBecomeActive:(UIApplication * _Nonnull)application;
+ (void)applicationWillEnterForeground:(UIApplication * _Nonnull)application __attribute((deprecated(("applicationWillEnterForeground/1 has been deprecated, use applicationWillEnterForeground/2 instead."))));;
+ (void)applicationWillEnterForeground:(UIApplication * _Nonnull)application withError:(NSError *_Nullable*_Nullable) error;
+ (void)applicationWillTerminate:(UIApplication * _Nonnull)application;


/**
 *  Register the library to the Push Notification Service.
 *  Call this method in didRegisterForRemoteNotificationsWithDeviceToken: passing the received 'deviceToken'.
 *
 *  @param deviceToken NSData received in device didRegisterForRemoteNotificationsWithDeviceToken.
 */
+ (void)registerForRemoteNotificationsWithDeviceToken:(NSData * _Nonnull)deviceToken;

/**
 * Logout the user:
 * 1) Disconnect XMPP Connection
 * 2) Remove User data from local storage
 * 3) Disable Push notifications removing device token from Catapush Server
 *
 * @param forced  If TRUE it will remove user data from local storage even if the call to Catapush Server fails. default value is FALSE
 *
 * To change a user (logout and login) it's necessary to use the block version method (in the completion you should i.e. start a new session) otherwise you can experiment a race condition.
*/
+ (void)logoutStoredUser:(bool) forced;
+ (void)logoutStoredUser:(bool) forced withCompletion:(void (^_Nullable)(void))success failure:(void (^_Nullable)(void))failure;
+ (void)logoutStoredUser;
+ (void)logoutStoredUserWithCompletion:(void (^_Nullable)(void))success failure:(void (^_Nullable)(void))failure;

/**
 * Return aps environment string included in mobile provisioning profile entitlements
 */
+ (NSString * _Nonnull) apsEnvironment:(NSError *_Nullable*_Nullable) error;

/**
 *  Return true if the UNNotificationRequest is from Catapush
 *
 *  @return true if the UNNotificationRequest is from Catapush
*/
+ (bool)isCatapushNotificationRequest:(UNNotificationRequest * _Nonnull)request;

/**
 *  This method can be used to enable or disable logs
 *
 *  @param enabled bool
 */
+ (void)enableLog:(bool) enabled;

@end



/******************************************
 ***        CATAPUSH CORE DATA          ***
 ******************************************/

@interface CatapushCoreData : NSObject
+ (NSManagedObjectContext * _Nonnull)managedObjectContext;
+ (NSManagedObjectModel * _Nonnull)managedObjectModel;
+ (NSPersistentStoreCoordinator * _Nonnull)persistentStoreCoordinator;

+ (void)saveContext;
+ (void)cleanDatabase;
+ (void)flushDatabase;
@end

/**************************************************************
***        CATAPUSH NotificationServiceExtension        ***
 ****************************************************************/

@interface CatapushNotificationServiceExtension : UNNotificationServiceExtension
- (void)handleMessage:(MessageIP * _Nullable) message withContentHandler:(void (^_Nullable)(UNNotificationContent * _Nullable))contentHandler  withBestAttemptContent: (UNMutableNotificationContent* _Nullable) bestAttemptContent;
- (void)handleError:(NSError * _Nonnull) error withContentHandler:(void (^_Nullable)(UNNotificationContent * _Nullable))contentHandler  withBestAttemptContent: (UNMutableNotificationContent* _Nullable) bestAttemptContent;
@end
