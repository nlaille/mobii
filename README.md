Mobii Server
=====

Welcome to Mobii! We hope you will enjoy contributing to this project.


Requirements
---------

Mobii server is based on the Boost Libraries, in order to ensure a complete cross-platform compatibility among various operating systems, notably Microsoft Windows, MacOS X and Linux.

You will need the following tools in order to start coding:

-	CMake
-	Boost Libraries (1.52.0)
-	Soci 3.2.0 (http://soci.sourceforge.net/doc.html)
-	OpenSSL (for secure socket)
-	A database (MySQL, PostgreSQL, Oracle, ...)
-	An IDE (Visual Studio 2010, vim, ...)

Compiling the server
---------

Before proceeding, please make sure you have a fully functional development environment (see above).
Then use CMake to generate the solution and compile it.
You might need to add some extra informations when you use CMake (use -D)

	cd build
	cmake .. (-DBOOST_DIR=/usr/local/lib/boost/)
	make
	
Some extra dll are included into build directory for windows.
	
RTFM!
---------

We are going to deploy a detailed documentation within a few weeks. We are strongly advising you to read it once it becomes available.

Besides the server application itself, it will explain you the philosophy and the technical architecture which drives the entire platform.

Authors
---------

Before it was released to the general public, Mobii has been developed as a part of a student project, whose original members are (in alphabetical order):

-	Geoffrey AUBERT (Android app)
-	Nicolas DE-THORE (iPhone app, team leader)
-	Camille GARDET (Windows Phone, team leader)
-	Sébastien GUILLERM (Android app, team leader)
-	Nicolas LAILLE (Server application, project leader)
-	Pierre LE (Client application, team leader)
-	Lucas McKENNA (iPhone app)
-	Clément MORISSARD (Client application)

