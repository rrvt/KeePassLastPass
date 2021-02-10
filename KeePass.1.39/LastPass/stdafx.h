/* Plugin Framework for KeePass
Copyright 2009 Bill Rubin <rubin@contractor.net>
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt) */

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN          // Exclude rarely-used stuff from Windows headers
#endif


#include <SDKDDKVer.h>

#if 0

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER                // Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0601         // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT          // Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0601   // Change this to the appropriate value to target Windows 2000 or later.
#endif

#ifndef _WIN32_WINDOWS        // Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0601 // Change this to the appropriate value to target Windows Me or later.
#endif
#endif


#include <windows.h>
#include <atltime.h>


#include <comutil.h>
#include <string>
#include <tchar.h>

// TODO: reference additional headers your program requires here

#include "std.h"

using namespace std;

typedef basic_string<Tchar> tstring;

#include "Strings.h"

#include "SDK/KpSDK.h"


// Badly defined in PwManager.h which can't be included in plugins for strange reasons...

// Field flags (for example in Find function)
// These flags must be disjoint to PWMS_* flags
#ifndef PWMF_TITLE
#define PWMF_TITLE              1
#define PWMF_USER               2
#define PWMF_URL                4
#define PWMF_PASSWORD           8
#define PWMF_ADDITIONAL        16
#define PWMF_GROUPNAME         32
#define PWMF_CREATION          64
#define PWMF_LASTMOD          128
#define PWMF_LASTACCESS       256
#define PWMF_EXPIRE           512
#define PWMF_UUID            1024
#endif
