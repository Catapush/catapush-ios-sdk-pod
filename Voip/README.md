## Catapush and VoIP

Catapush uses Voip Push Notification (PushKit framework) this guarantee high priority push and the ability to start the application also if is inactive. To use this type of Voip you *MUST* have a Voip functionality in your APP or the APP cannot pass the Apple review process on pubblication. If your application doesn't has already a Voip functionality you can add it quickly following this tutorial.

You can add to your application an easy help desk support functionality via Voip, the user can call the number to receive information or for customer support. With this Voip functionality you can request from Apple the approval for Voip Push Notification. 

To add a voice channel to your App you can use [Twilio](https://www.twilio.com/), in this tutorial you can see how to integrate it.

## Twilio Integration Guide
First of all you have to create an account on [Twilio](https://www.twilio.com/), then in the Twilio control panel, in the "Programmable Voice" section you have to create a "TwilML App". You have to set a Callback URL to manage the calls generated from the APPs. At each "TwilML App" is associated an "app_id", this value is used by the SDK to integrate in your APP.

When you receive a call from an APP you can choose what Twilio have to do, like forwading the call to a telephone exchange or reply directly with an IVR. It's easy with Twilio to create an IVR, you can do it creating a "Programmable Voice". In the callback specified on the creation of the "TwilML App" you have to setup a page that response with a correct TwilioML.

For example this is a simple TwilioML that only play a registered message:

```<Response>
<Say voice="woman">You have reached our customer support. All our operators are busy at the moment. Please leave your message and we will do our best to call you back as soon as possibile.
</Say>
<Record maxLength="60" playBeep="true" timeout="3" action="http://www.your-domain.com/recording-callback"/>
<Say voice="woman">I did not receive any message.</Say>
<Hangup/>
</Response>```

In this way you can register the request of the user and call back him later.

After the setup in the Twilio control panel you need to integrate the Twilio SDK to you APP. With CocoaPod you can simple use:

```pod 'TwilioSDK', '~>1.2 '```

Twilio has a dynamic authorization system, for each Twilio Client must be associated some capabilities that allow each client to make or receive calls. The generation of the capabilities is dinamyc and must be executed each time a Twilio Client open a connection, we have implemented this simple PHP script to generate the capabilities.

```
// Generate Twilio token
$accountSid = "INSERT_HERE_YOUR_ACCOUNT_SID"; // Your Account SID from  www.twilio.com/user/account
$authToken = "INSERT_HERE_YOUR_AUTH_TOKEN"; // Your Auth Token from www.twilio.com/user/account
$applicationSid = "INSERT_HERE_YOUR_APPLICATION_SID"; // Twilio Application Sid
$capability = new \Services_Twilio_Capability($accountSid, $authToken);
$capability->allowClientOutgoing($applicationSid);
$expireSeconds = 300;
$token = $capability->generateToken($expireSeconds);

// Send response
$responseData = array();
$responseData['twilio'] = array(
'token' => $token,
);
```
In this example only the outgoing capability is enabled so only the Client can call the backend server and not viceversa.

On the mobile application, after retriving the capabilities you can make the Voip call using the Twilio SDK

```
@property (strong,nonatomic) TCDevice *tdcDevice;
@property (strong,nonatomic) TCConnection *tdcConnection;

...

-(void) requestOutgoingColl {

NSMutableURLRequest *endpoint = @”http://webservice.com/token”
NSMutableURLRequest *request = [self nsMutableURLRequestWithPath:endpoint andHTTPMethod:@"POST"];
    
[NSURLConnection asyncRequest:request success:^(NSData *data, NSURLResponse *response) {
  NSDictionary *jsonResponse = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
  NSDictionary *twilio = jsonResponse[@"twilio"];
   if (twilio) {
     NSString *token = twilio[@"token"];
     if (token == nil) {
          // Handle Errors
      } else {
       self.tdcDevice = [[TCDevice alloc] initWithCapabilityToken:token delegate:self];
           self.tdcConnection = [self.tdcDevice connect:nil  delegate:self];
    }
    } else {
    // Handle Errors
    }        
        
}];
}
```
You can find a detailed tutorial on how to integrate Twilio in theire [Official Documentation](https://www.twilio.com/docs/quickstart/client/ios)
