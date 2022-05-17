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

@property (nonatomic, retain) NSNumber * status;

@property (readonly) NSNumber * type; //Outgoing or Incoming

@property (readonly) NSString * messageId;
@property (readonly) NSString * sender;
@property (readonly) NSDate * sentTime;
@property (readonly) NSString * body;


+ (void)sendMessageReadNotification:(MessageIP *)message;
+ (void)sendMessageReadNotificationWithId:(NSString *)messageId;

//Media
@property (readonly) NSData *  mm;
@property (readonly) NSString * mmType;
@property (readonly) NSString * filename;

//Channel
@property (readonly) NSString * channel;
//ReplyTo
@property (readonly) NSString * originalMessageId;

- (bool)hasMedia;
- (bool)isMediaReady;

//Media Preview
- (bool)hasMediaPreview;
- (UIImage * _Nullable)imageMediaPreview;

- (void)downloadMediaWithCompletionHandler:(void (^)(NSError *error, NSData *data))completionHandler;

- (NSDictionary *)optionalData;

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
- (void)libraryDidReceiveMessageIP:(MessageIP *)messageIP;

//Outgoing Messages Delegate
@optional
- (void)libraryDidSendMessage:(MessageIP *)message;
- (void)libraryDidFailToSendMessage:(MessageIP *)message;
@end

/******************************************
 ***              CATAPUSH              ***
 ******************************************/


@class Catapush;


@protocol CatapushDelegate <NSObject>

- (void)catapushDidConnectSuccessfully:(Catapush *)catapush;
- (void)catapush:(Catapush *)catapush didFailOperation:(NSString *)operationName withError:(NSError *)error;

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
+ (NSArray *)allMessages;
+ (NSArray *)messagesWithPredicate:(NSPredicate *)predicate;

+ (void)removeAllMessages;
+ (void)removeMessage:(MessageIP *)message;
+ (MessageIP *)sendMessageWithText:(NSString *)text;
+ (MessageIP *)sendMessageWithMessageId:(NSString *) messageId;
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

/**
 *  Catapush will hook for didRegisterForRemoteNotificationsWithDeviceToken
 *  in order to receive the device token
 *  and registers for notifying the user with the following options:
 *  UIUserNotificationTypeAlert | UIUserNotificationTypeBadge | UIUserNotificationTypeSound.
 */
+ (void)registerUserNotification:(UIResponder *) appDelegate;

/**
 *  Catapush will hook for didRegisterForRemoteNotificationsWithDeviceToken
 *  in order to receive the device token
 *  @param appDelegate appDelegate
 *  @param withRequest if true, registers for notifying the user with the following options UIUserNotificationTypeAlert | UIUserNotificationTypeBadge | UIUserNotificationTypeSound
 */
+ (void)registerUserNotification:(UIResponder *) appDelegate withRequest:(bool) withRequest;


//Library Status
+ (BOOL)isConnected;

/**
 *  This Methods connects the library to the host to receive Messages.
 *  It needs indentifier, password
 *  
 * @param identifier user identifer
 * @param password user password
 */

+ (void)startWithIdentifier:(NSString *)identifier andPassword:(NSString *)password __attribute((deprecated(("startWithIdentifier/2 has been deprecated, use setIdentifier/2 and start/1 instead."))));
/**
 *  Save username and password of the user.
 *  This method must be called before calling start/0 method.
 *
 *  @param identifier identifier of the user
 *  @param password password of the user
 *
 */
 
+(void) setIdentifier:(NSString *) username andPassword:(NSString *) password;
/**
 *  Return the identifier of the user. The method returns nil if the Capush status is equals to LOGOUT or if the indetifer has 
 *  never been set (using setIdentifier/2).
 *
 *  @return identifier of the user
 */
+ (NSString *) userIdentifier;

/**
 * Return the password of the user. The method returns nil if the Capush status is equals to LOGOUT or if the password has
 * never been set using setIdentifier/2
 *
 *  @return password of the user
 */
+ (NSString *) userPassword;

/**
 *  This Methods connects the library to the host to receive Messages.
 *  It needs the indentifier and the password of the user (see setIndetifier/2 to set user credentials).
 *  The optional error return indicates problem that was encountered (for instance, the absence indentifier and/or password).
 * 
 * @param error, nil if no problem was encountered.
 */

+ (void) start:(NSError **) error;


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
+ (void)setupCatapushStateDelegate:(id<CatapushDelegate>)stateDelegate
        andMessagesDispatcherDelegate:(id<MessagesDispatchDelegate>)messagesDelegate;


/**
 *  Key to register App with the Catapush service
 *
 *  @param appKey app key
 */
+ (void)setAppKey:(NSString *)appKey;



+ (void)applicationDidEnterBackground:(UIApplication *)application;
+ (void)applicationDidBecomeActive:(UIApplication *)application;
+ (void)applicationWillEnterForeground:(UIApplication *)application __attribute((deprecated(("applicationWillEnterForeground/1 has been deprecated, use applicationWillEnterForeground/2 instead."))));;
+ (void)applicationWillEnterForeground:(UIApplication *)application withError:(NSError **) error;
+ (void)applicationWillTerminate:(UIApplication *)application;


/**
 *  Register the library to the Push Notification Service.
 *  Call this method in didRegisterForRemoteNotificationsWithDeviceToken: passing the received 'deviceToken'.
 *
 *  @param deviceToken NSData received in device didRegisterForRemoteNotificationsWithDeviceToken.
 */
+ (void)registerForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken;

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
+ (void)logoutStoredUser:(bool) forced withCompletion:(void (^)(void))success failure:(void (^)(void))failure;
+ (void)logoutStoredUser;
+ (void)logoutStoredUserWithCompletion:(void (^)(void))success failure:(void (^)(void))failure;

/**
 * Return aps environment string included in mobile provisioning profile entitlements
 */
+ (NSString *) apsEnvironment:(NSError **) error;

/**
 *  Return true if the UNNotificationRequest is from Catapush
 *
 *  @return true if the UNNotificationRequest is from Catapush
*/
+ (bool)isCatapushNotificationRequest:(UNNotificationRequest *)request;

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
+ (NSManagedObjectContext *)managedObjectContext;
+ (NSManagedObjectModel *)managedObjectModel;
+ (NSPersistentStoreCoordinator *)persistentStoreCoordinator;

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
