/* Plugin Framework for KeePass
Copyright 2009 Bill Rubin <rubin@contractor.net>
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt) */

#pragma once

#define DebugMemoryLeaks

#ifdef DebugMemoryLeaks
#define _CRTDBG_MAP_ALLOC
#endif

#include "targetver.h"

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN          // Exclude rarely-used stuff from Windows headers
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _AFX_ALL_WARNINGS

#include <afxwin.h>
#include <afxext.h>
#include <afxmt.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxdialogex.h>
#include <afxdisp.h>
#include <afxole.h>

//#include <windows.h>
//#include <atltime.h>

#include <comutil.h>
#include <string>
#include <tchar.h>

// TODO: reference additional headers your program requires here

#include "std.h"

using namespace std;

typedef basic_string<Tchar> tstring;

#include "Strings.h"

#include "NewAllocator.h"


#ifdef _DEBUG
#define Verify(f)          assert(f)
#else
#define Verify(f)          ((void)(f))
#endif

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
