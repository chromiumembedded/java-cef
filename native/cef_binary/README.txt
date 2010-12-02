Chromium Embedded Framework (CEF) Binary Distribution
-------------------------------------------------------------------------------

CEF Revision:        149
Chromium Revision:   66269
Date:                November 23, 2010

This distribution contains all files necessary to build an application using
CEF.  Please read the included LICENSE.txt file for licensing terms and
restrictions.


CONTENTS
--------

build       Contains libcef.dll and other DLLs required to run CEF-based
            applications.  Also acts as the build target for the included
            cefclient sample application.

cefclient   Contains the cefclient sample application configured to build
            using the files in this distribution.

include     Contains all required CEF and NPAPI-related header files.  Read
            the include/npapi/NPAPI-README.txt file for more information about
            the NPAPI-related header files.

lib         Contains the libcef.lib and libcef_dll_wrapper.lib library files
            that all CEF-based C++ applications must link against.


USAGE
-----

Visual Studio 2005: Open the cefclient/cefclient2005.sln solution and build.
Visual Studio 2008: Open the cefclient/cefclient2008.sln solution and build.

Please visit the CEF Website for additional usage information.

http://code.google.com/p/chromiumembedded
