// Standard Definitions
// rrvt 8/25/91
// Copyright (C) 1991 by Software Design & Engineering, Robert R. Van Tuyl, All rights reserved.

#pragma once

#include <string>
#include <tchar.h>
#include <cstringt.h>

typedef unsigned short   ushort;
typedef unsigned long    ulong;
typedef unsigned int     uint;
//typedef unsigned char    uchar;
typedef unsigned char    Byte;
typedef const    char    Cchar;
typedef          TCHAR   Tchar;
typedef const    TCHAR   TCchar;
typedef const   _TUCHAR  TUCchar;

typedef          __int64 int64;
typedef unsigned __int64 Uint64;

#define loop for (;;)

#define noElements(array)     (sizeof(array) / sizeof(array[0]))

#define pathSepChar _T('\\')
#define pathSepStg  _T("\\")
#define uniSepChar  _T('/')




