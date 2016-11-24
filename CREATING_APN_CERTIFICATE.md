Catapush uses native voIP Push Notification as a component of its reliable and secure push message delivery system.
Catapush is a simple, reliable and scalable delivery API for transactional push notifications for websites and applications. Ideal for sending data-driven transactional notifications including targeted e-commerce and personalized one-to-one messages.

Get delivery confirmation with real time status for each message sent and build intelligent messaging logic in your applications
Here are the instructions on how to set up the Apple APN push notification services

# Creating VoIP Services certificate
These are pre-requisites for setting up VoIP with Catapush.

Please follow this steps:
Log in to the [iOS Dev Center](https://developer.apple.com/membercenter/index.action) and click on the Certificates, Identifiers & Profiles.

* Go to Identifiers > App IDs.
* Add a new App ID clicking the "+" button.

Under App ID Description enter the Name of your application, for example: "My Application Name"

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/apns_01.png)

Under "App ID Suffix" make sure "Explicit App ID" is selected.

Enter the the Bundle ID of your app.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/apns_02.png)

Once you have created your application id, select "All" in the "Certificates" section on your left and click "+" button.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/voip_cert_00.png)

Make sure to select "VoIP Service Certificate" and press "Continue" button.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/voip_cert_01.png)

Select the "My Application Name" application created in previous step and press "Continue" button.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/voip_cert_02.png)

Now you should see the following page

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/voip_cert_03.png)

Before keep going on we ha have to create our Certificate Signing Request (CSR).
 
Step 2 - Generating a certificate request

The process of generating development and production certificates is the same.

You must generate a certificate request file so that you can use it to request a development SSL certificate later on. You can do this by using the Keychain Access application.

Click on Certificate Assistant, and then Request a Certificate From a Certificate Authority.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/csr_00.png)

Enter the required information and choose the Saved to disk option.

When you click Continue, you will be able to save the certificate request using the name suggested by the system.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/csr_01.png)

Now switch back to itunes and press the "Continue button". You should see the following page:

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/csr_02.png)

Upload the CSR generated and press the "Generate" button.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/csr_04.png)

After you’re done with the wizard, you can download the certificate which you created. The same process also applies to the production certificates. A double-click on the SSL certificate that you downloaded will start the installation process in the Keychain Access application. This certificate will be used by Catapush Push service so it can contact the APNs in order to send push notifications to your applications.


