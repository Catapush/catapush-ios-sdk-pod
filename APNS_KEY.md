## Create and configure the authentication key
This section describes how to generate an authentication key for an App ID enabled for Push Notifications. If you have an existing key, you can use that key instead of generating a new one.

To create an authentication key:
1) In your [Apple Developer Member Center](https://developer.apple.com/account), go to Certificates, Identifiers & Profiles, and select Keys.
2) Click the Add button (+) or click the "Create a key" button.
<img src="https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/auth_key_1.png">
3) Enter a description for the APNs Auth Key.
4) Under Key Services, select the Apple Push Notifications service (APNs) checkbox, and click Continue.
<img src="https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/auth_key_2.png">
5) Click Register and then download and save your key in a secure place. This is a one-time download, and the key cannot be retrieved later.
<img src="https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/auth_key_3.png">

Once you have download it you have to configure your Catapush application.
1) Go to https://www.catapush.com/panel/apps/YOUR_APP_ID/platforms.
2) Click on iOS Token Based to enable it.
3) Fill iOS Team Id, iOS Key Id, iOS AuthKey and iOS Topic.

The iOS **Team Id** can be found here https://developer.apple.com/account/#/membership in "Membership Information" section.

The iOS **Key Id** can be retrieved here https://developer.apple.com/account/resources/authkeys/list, click on the key you have created and you can find it under "View Key Details" section.

The iOS **AuthKey** is the content of the key file.

Example:
```
-----BEGIN PRIVATE KEY-----
...........................
          AUTH_KEY
...........................
-----END PRIVATE KEY-----
```

The iOS Topic is the bundle identifier of your iOS application.

<img src="https://github.com/Catapush/catapush-ios-sdk-pod/blob/master/images/catapush_ios_token_based.png" width="200">
