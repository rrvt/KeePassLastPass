/*
  Copyright (c) 2009, Matyas Bene <notabenem@hotmail.com>
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Neither the name of ReichlSoft nor the names of its contributors may be
    used to endorse or promote products derived from this software without
    specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/


#include "stdafx.h"
#include "Utils.h"
#include "LastPass.h"


#ifdef _DEBUG
#define VERIFY(f)          assert(f)
#else
#define VERIFY(f)          ((void)(f))
#endif


static TCHAR lastPassMenu[EndMenuItem-BaseMenuItem][MaxStrLen + 1] = {0};
static TCHAR path[MAX_PATH];



LPCTSTR InitTranslationEngine (LPCTSTR lpName) {
TCHAR*          tcIndex1;
TCHAR*          tcIndex2;
TCHAR           tcLangName[50];

  // Construct the language file name

  _tcsncpy_s(tcLangName, 50, lpName, _TRUNCATE);

  //Check whether the file exists. If not, try to shorten the name

  while (_tcslen(tcLangName) > 0) {

    _sntprintf_s(path, MAX_PATH, _T("%s\\%s_LastPass.plng"), basePath(), tcLangName);

    if (GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES) return path;

    tcIndex1 = _tcsrchr(tcLangName, _T('_'));   if (!tcIndex1) tcIndex1 = tcLangName;
    tcIndex2 = _tcsrchr(tcLangName, _T('-'));   if (!tcIndex2) tcIndex2 = tcLangName;

    tcIndex1 = tcIndex1 > tcIndex2 ? tcIndex1   : tcIndex2;   tcIndex1[0] = _T('\0');
    }

  return NULL;
  }


LPTSTR translate(UINT uid, LPCTSTR tcStrKeyName) {
int    i = uid-BaseMenuItem;
TCHAR* p = lastPassMenu[i];
TCHAR* pos;
                                                            //lastPassMenu[i][0]
  if (!*p) {

    // First, check if the corresponding language file is available
    // If the file does not exist, get it from the string table (rc file)

    if (GetPrivateProfileString(_T("LastPass"), tcStrKeyName, NULL, p, MaxStrLen+1, path) == 0)
                                                              LoadString(instDLL(), uid, p, MaxStrLen+1);
    // Replacing newlines chars with new line

    pos = _tcsstr(p, _T("\\n"));

    while (pos != NULL) {pos[0]=_T('\r');   pos[1]=_T('\n');   pos=_tcsstr(pos,_T("\\n"));}
    }

  return p;
  }


TCHAR* loadString(UINT uid) {
TCHAR  buf[1024];
int    n = LoadString(GetModuleHandle(0), uid, buf, noElements(buf)) + 1;
TCHAR* p;

  if (!n) return 0;

  p = new TCHAR[n];

  if (p) _tcscpy_s(p, n, buf); return p;
  }


