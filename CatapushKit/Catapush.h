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

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_8_0
#import <PushKit/PushKit.h>
#endif

#define CATAPUSH_ERROR_DOMAIN @"MessagingIPError"
#define kCatapushStatusChangedNotification @"CatapushStatusChangedNotification"


/**
 * Catapush Error code returned by the Catapush's start/1 method.
 *
 */
enum {
    CatapushCredentialsError
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
    GENERIC = -11000
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

- (bool)hasMedia;
- (bool)isMediaReady;

//Media Preview
- (bool)hasMediaPreview;
- (UIImage *)mediaPreview;

- (void)downloadMediaWithCompletionHandler:(void (^)(NSError *error, NSData *data))completionHandler;

@end

/******************************************
 ***            DISPATCHER              ***
 ******************************************/


@protocol VoIPNotificationDelegate <NSObject>

-(void) didReceiveIncomingPushWithPayload:(PKPushPayload *)payload;

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
+ (MessageIP *)sendMessageWithText:(NSString *)text andImage:(UIImage *)image;
+ (MessageIP *)sendMessageWithText:(NSString *)text andData:(NSData *)data ofType:(NSString *)mediaType;

/**
 *  Registers for notifying the user with the following options:
 *  UIUserNotificationTypeAlert | UIUserNotificationTypeBadge | UIUserNotificationTypeSound.
 *
 *  @param voIPNotificationDelegate reference in order to receive voip notificatin payload.
 *  If set to null, the library will present the notification to the user using default alert, otherwise it just call
 *  the method defined in VoIPNotificationDelegate protocol without presenting the notification to the user.
 */
+ (void) registerUserNotification:(UIResponder *) appDelegate voIPDelegate:(id<VoIPNotificationDelegate> ) voipNotificationDelegate;


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

+(void) start:(NSError **) error;


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
 *  This method can be used with both 'Regular Push' or 'VoIP Push'.
 *  With 'Regular Push' call this method in didRegisterForRemoteNotificationsWithDeviceToken: passing the received 'deviceToken'.
 *  With 'VoIP Push' use this method in pushRegistry:didUpdatePushCredentials:forType: passing as parameter 'credentials.token'.
 *
 *  @param deviceToken NSData received in device didRegisterForRemoteNotificationsWithDeviceToken: with Standard Push Service or 'credentials.token' received in pushRegistry:didUpdatePushCredentials:forType: with VoIP Push Service.
 */
+ (void)registerForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken;

/**
 * Logout the user:
 * 1) Disconnect XMPP Connection
 * 2) Remove User data from local storage
 * 3) Disable Push notifications removing device token from Catapush Server
 *
 */
+ (void)logoutStoredUser;

/**
 * Return aps environment string included in mobile provisioning profile entitlements
 */
+ (NSString *) apsEnvironment:(NSError **) error;

@end


/******************************************
 ***   CATAPUSH REMOTE NOTIFICATIONS    ***
 ******************************************/

/**
 * Interface for Catapush Push Notifications.
 * @since 1.0.0
 *
 */
@interface CatapushRemoteNotifications : NSObject <NSURLSessionDataDelegate, NSURLSessionDownloadDelegate, NSURLSessionTaskDelegate>

/**
 *  Tells Catapush library that a push notification arrived and that indicates there is a message to be acked to avoid the fallback SMS.\n Call this method in application:didReceiveRemoteNotificationfetchCompletionHandler of the UIApplication Delegate when app receives a Catapush Push Notification (this methods checks the sender as well, so can be called for every received Push Notification).
 *
 *  @param application       app UIApplication Delegate
 *  @param userInfo          Push Notificaition userInfo
 *  @param completionHandler Background mode fetchCompletionHandler
 */
+ (void)application:(UIApplication *)application
    didReceiveRemoteNotification:(NSDictionary *)userInfo
          fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler;

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_8_0
+ (void)pushRegistry:(PKPushRegistry *)registry
didReceiveIncomingPushWithPayload:(PKPushPayload *)payload
             forType:(NSString *)type;
#endif

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
