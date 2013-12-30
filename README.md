Mobii
=========

Welcome to Mobii! We hope you will enjoy contributing to this project.

Introducing Mobii
=========

Mobii is an all-in-one application which allows you to remotely gain access to your datas, whether the phone is deep into your pocket or kilometers away at home.

To put it in a nutshell, Mobii works with a mobile app which has to be installed on the remote smartphone and a client application to be run on a conventional computer.

Features
=========

It is far from easy to offer an equivalent experience on three different mobile platforms. This is why Mobii's features are restricted to what we consider to be the most essential features one may expect to see.

These features are:
- SMS messages synchronization, including reception and sending
- Contacts management
- Agenda synchronization
- Files and pictures

How does it work
=========

Mobii is based on a rather conventional client-server architecture. The main goal of the server application is to provide a "bridge" between a client device and the remote phone to which the user wants to connect. Datas are transfered through the server, using a SSL tunnel, thus ensuring datas privacy.

Mobii's everywhere
=========

The mobility world is not a lean environment where all smartphones run the same operating system on the same hardware. As we wanted to offer our service to the largest public possible, we naturally developed Mobii as a multi-platform and nearly universal solution.

For the mobile app part, the 3 platforms currently supported are Android, iOS and Windows Phone.

Regarding the client application, another trio of platforms are supported: Windows, MacOS X and Linux.

Get started
=========

Some instructions below may be incomplete or unclear. Mobii is (and probably will remain) an under-development solution, and its components have not been completely user-proofed yet.

1. Install the Mobii app on your smartphone. Please note that they are not deployed on any official market (yet). Thus you will either need to compile the app on your favorite platform by yourself or find a pre-compiled package.
2. Run the app on the smartphone, and allow it to run in background if necessary. Once run, the app shows a string of characters in the middle of the screen; this is the Unique Identifier which you will need to enter in the client.
3. Download the client application (to compile by yourself or as a pre-compiled package), then run it.
4. Configure the server settings. To do so, open the "Properties" window, represented by a gear on the upper toolbar in the main window. Then, go to the server panel and type in the IP address and the port on which your instance of the Mobii server is running. Once done, click on "OK" to validate the changes.
5. In the main window, click on the "New Connection" tab (may be in French; in such case: "Nouvelle connexion").
6. In the first text box at the bottom of the window (the Unique Identifier field), write down the 16 characters which you saw when you ran the mobile app.
7. The second text box below the Unique Identifier field, you may have to enter a password before connecting to the phone. Depending on the platform, you may either have to enter a password upon the first start of the mobile app, or to let the password field empty. Of course, you can change the password from the mobile app at any time.
8. Click on "Connect" (French: "Connexion").
9. If everything goes right and with some luck, you should be connected to your smartphone. Now, you are free to explore the features of the Mobii solution. Enjoy!

Participate in the project
=========

You want to contribute to the Mobii project? Awesome!

This project is divided in multiple branchs, one for each mobile platform, another one for the client application and a last one for the server part. Please read the README file specific to the branch you are interested in.
