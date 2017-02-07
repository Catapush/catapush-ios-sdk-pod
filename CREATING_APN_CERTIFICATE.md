# Creating VoIP Push notifications certificate

Catapush use VoIP Push Notifications (PushKit) as a component of its reliable and secure push message delivery system, to setup the system correctly you have to create a certificate on [iOS Dev Center](https://developer.apple.com/membercenter/index.action), export it and upload on Catapush control panel.

Log in to the [iOS Dev Center](https://developer.apple.com/membercenter/index.action) and click on the **"Certificates, Identifiers & Profiles"**.

* Go to **Identifiers** > **App IDs**.
* Add a new App ID clicking the **"+"** button.

Under App ID Description enter the Name of your application, for example: "My Application Name"

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/apns_01.png)

Under **"App ID Suffix"** make sure **"Explicit App ID"** is selected.

Enter the the **Bundle ID** of your app.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/apns_02.png)

Once you have created your application id, select **"All"** in the **"Certificates"** section on your left and click **"+"** button.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/voip_cert_00.png)

Make sure to select **"VoIP Service Certificate"** and press **"Continue"** button.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/voip_cert_01.png)

Select the **"My Application Name"** application created in previous step and press **"Continue"** button.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/voip_cert_02.png)

Now you should see the following page

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/voip_cert_03.png)

Before keep going on we ha have to create our **Certificate Signing Request (CSR)**.
 
The process of generating development and production certificates is the same.

You must generate a certificate request file so that you can use it to request the certificate, you can do it by using the **Keychain Application**.

Click on **Certificate Assistant**, and then **Request a Certificate From a Certificate Authority**.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/csr_00.png)

Enter the required information and choose the **"Saved to disk"** option.

When you click **"Continue"**, you will be able to save the certificate request using the name suggested by the system.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/csr_01.png)

Now switch back to iOS Dev Center and press the **"Continue"** button. You should see the following page:

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/csr_02.png)

Upload the CSR generated and press the **"Generate"** button.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/csr_04.png)

After you're done the wizard, you can download the certificate which you have created. A double-click on the SSL certificate that you downloaded will start the installation process in the Keychain Access application. This certificate will be used by Catapush Push service so it can contact the APNs in order to send push notifications to your applications.

Finally you have to upload the .p12 certificate file on Catapush Dashboard.
Launch the Keychain Assistant from your local Mac and from the login section, filter by the **"Certificates"** category. You will see an entry called "VoIP Services: your.bundle.id" with the bundle of your application. Expand this entry, select both the certificates (VoIP Services: your.bundle.id) and the private key beneath it, then right click on them and choose **"Export 2 items"**

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/p12_00.png)

Choose ".p12" file format from the dropdown menu and save the file somewhere where you can access it. You could enter the password for exporting (the field can be left empty, but if you choose to enter the password then the same password must be entered on the Catapush Dashboard).

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/p12_01.png)

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/p12_02.png)

Now log in to Catapush Dashboard and upload the ".p12" generate file in the "Platform" section of your applcation.

![alt tag](https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/dashboard_00.png)

Now you can start sending push notifications
