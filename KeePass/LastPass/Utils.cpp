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


#if 0
////////////////////////////////////////////////////////
// Expand environment Ex routine, using basic_string  //
////////////////////////////////////////////////////////
String ExpandEnvironmentEx(String s) {
TCHAR *tcTokenStart=NULL;
TCHAR *tcTokenEnd=NULL;
TCHAR *tcHolder=NULL;
TCHAR *tcFragmentStart=NULL;
TCHAR *tcEnvString=NULL;
String strExpanded;
size_t len;
size_t requiredSize;
int state=0;

  if (s.empty()) return strExpanded;

//  LastPass_Settings * DBBckp_S=LastPass_Settings::GetInstance();

  tcHolder = tcFragmentStart = (TCHAR *)s.c_str();

  while( (tcHolder = _tcschr(tcFragmentStart, _T('%') ))  != NULL )
  {
    switch (++state) {
    case 1:
    tcTokenStart = tcHolder;
    len = (tcTokenStart - tcFragmentStart) >> shiftBy();
    if ( len > 0)
      strExpanded.append(tcFragmentStart, len );
    tcFragmentStart = tcTokenStart + sizeof (TCHAR);
    break;

    case 2:
    tcTokenEnd = tcHolder;
    len = (tcTokenEnd - tcFragmentStart) >> shiftBy();

    // Get Environment variable value in 2 steps
    // First, get the size of the required buffer
    getenv_s( &requiredSize, NULL, 0, String(tcFragmentStart).substr(0,len).c_str() );
    if (requiredSize > 0) {
      tcEnvString = (TCHAR *) malloc(requiredSize << shiftBy());
      // then, when the buffer is ready, copy the value of the variable into the buffer
      _tgetenv_s(&requiredSize, tcEnvString, requiredSize << shiftBy(),
          String(tcFragmentStart).substr(0,len).c_str());
    }

    if (tcEnvString != NULL)
      strExpanded.append(tcEnvString);
    else
      // len +2 because we have to copy both '%' signs
      strExpanded.append(String(tcTokenStart).substr(0, len+2));

    // and of course, free the variable for future use.
    free ( tcEnvString );
    tcFragmentStart = tcTokenEnd + sizeof (TCHAR);
    state = 0;
    break;
    }
  }

  if (state==1)
     tcFragmentStart -= sizeof (TCHAR);
  if (tcFragmentStart[0] != _T('\0'))
    //_tcsncat_s(dest, lBufSizeDest, tcFragment, lBufSizeDest - _tcslen(dest) );
    strExpanded.append(tcFragmentStart);
  return strExpanded;
}
#endif


#if 0
// ******************************************************
// The following code has been contributed by Bill Rubin
// modified by me to suit LastPass
// ******************************************************

void displayIgnoredMessage(HWND h, const String& argument, const String& reason) {

//  LastPass_Settings * DBBckp_S=LastPass_Settings::GetInstance();

const String message(String(_T("Command line argument \"")) + argument + _T("\" ") + reason + _T('.'));

  MessageBox(h, message.c_str(), PluginTitle, MB_ICONERROR);
  }

void ParseCommandLine() {

String          strBuf;
TCHAR           tcErrorMsg[200] = {0};
LPCTSTR         lpOptionValue;
TCHAR           tcNumAsStr[55]  = {0};
TCHAR*          tcNumStart      = NULL;
BOOL            bNegVal         = FALSE;
int             counter         = 0;
IKpCommandLine* pCmdLine;
IKpUtilities*   pUtils;

//  LastPass_Settings * DBBckp_S=LastPass_Settings::GetInstance();

  HWND h = mainWindowHandle();
  if (getAPI()->QueryInstance (SCLSID_KpCommandLine, IID_IKpCommandLine, (void**) &pCmdLine) != S_OK)
                                                                                          assert(FALSE);
  if (getAPI()->QueryInstance (SCLSID_KpUtilities, IID_IKpUtilities, (void**) &pUtils) != S_OK)
                                                                                          assert(FALSE);

  //_tcsncpy_s(DBBckp_S->tcAppBasePath, MAX_PATH, pCmdLine->GetExecutablePath(), _TRUNCATE);
  //tcNumStart = _tcsrchr ( DBBckp_S->tcAppBasePath, _T('\\') );
  //tcNumStart[0] = _T('\0');

  // For each command line argument 'dest:'

  while ((lpOptionValue=pCmdLine->GetMultiOption(String(CmdLinePrefix).append(_T("dest:")).c_str(),
                                                                                  counter++)) != NULL) {
    strBuf = ExpandEnvironmentEx(String(lpOptionValue));
    if (!pUtils->ValidatePath(strBuf.c_str(), KPVPF_REGULAR_NAME | KPVPF_DEFAULT))
      displayIgnoredMessage(h,
                    pCmdLine->GetTokenRel(String(CmdLinePrefix).append(_T("dest:")).c_str(),
                    counter-1),
                    _T("ignored because destination is invalid")
                    );
//    else
//      DBBckp_S->bDestCmdParam = TRUE;
    }

  // Check for a present command line argument 'cnt:'

  tcNumStart = (TCHAR*) pCmdLine->GetOption(String(CmdLinePrefix).append(_T("cnt:")).c_str());

  if (tcNumStart != NULL) {
    if (tcNumStart[0]==_T('-')) {bNegVal = TRUE; tcNumStart += sizeof(TCHAR);}

    while (tcNumStart[0]==_T('0')) tcNumStart += sizeof(TCHAR);

    int iCnt = _tstoi(tcNumStart);

    iCnt=(iCnt==0?1:iCnt);

    _itot_s(iCnt, tcNumAsStr, 55, 10);

    if ((_tcscmp(tcNumAsStr, tcNumStart) != 0) || bNegVal ){
      _sntprintf_s (tcErrorMsg, 200, 199,
        _T("partially ignored because option not entirely of integer type or negative.\n\"%s\" will be used instead"), tcNumAsStr);
      displayIgnoredMessage(h, pCmdLine->GetTokenRel(String(CmdLinePrefix).append(_T("cnt:")).c_str(), 0), tcErrorMsg );
      }

    if (iCnt>99) {
      displayIgnoredMessage(h, pCmdLine->GetTokenRel(String(CmdLinePrefix).append(_T("cnt:")).c_str(), 0),
        _T("ignored because it was out of range (>99).\n\"99\" will be used instead") );
      iCnt=99;
      }

//    DBBckp_S->iBackupCnt = iCnt;
//    DBBckp_S->bCountCmdParam = TRUE;
  }

  // Check if the OpenPrefix is given 'prfxo:'
  if ((lpOptionValue=pCmdLine->GetOption(String(CmdLinePrefix).append(_T("prfxo:")).c_str())) != NULL) {
    strBuf = ExpandEnvironmentEx(String(lpOptionValue));
    _tcsncpy_s(DBBckp_S->tcOpenPrfx, MAX_PATH, strBuf.c_str(), _TRUNCATE);
    DBBckp_S->bOpenPrfxCmdParam = TRUE;
  }

  // Check if the SavePrefix is given 'prfxs:'
  if ((lpOptionValue=pCmdLine->GetOption(String(CmdLinePrefix).append(_T("prfxs:")).c_str())) != NULL) {
    strBuf = ExpandEnvironmentEx(String(lpOptionValue));
    _tcsncpy_s(DBBckp_S->tcSavePrfx, MAX_PATH, strBuf.c_str(), _TRUNCATE);
    DBBckp_S->bSavePrfxCmdParam = TRUE;
  }

  // Check for a present command line argument 'prog:'
  if ((lpOptionValue=pCmdLine->GetOption(String(CmdLinePrefix).append(_T("prog:")).c_str())) != NULL) {
    strBuf = ExpandEnvironmentEx(String(lpOptionValue));
    if (!pUtils->ValidatePath(strBuf.c_str(), KPVPF_REGULAR_NAME | KPVPF_DEFAULT))
      displayIgnoredMessage(h, pCmdLine->GetTokenRel(String(CmdLinePrefix).append(_T("prog:")).c_str(), 0),
        _T("ignored because the program path is invalid"));
    else {
      _tcsncpy_s(DBBckp_S->tcStartPrg, MAX_PATH, strBuf.c_str(), _TRUNCATE);
      DBBckp_S->bPrgCmdParam = TRUE;
    }
  }

  // Check for a present command line argument 'show:'
  if ((lpOptionValue=pCmdLine->GetOption(String(CmdLinePrefix).append(_T("show:")).c_str())) != NULL) {
    int iShow = _tstoi(lpOptionValue);
    DBBckp_S->bShowWnd = (iShow > 0 ? TRUE : FALSE);
    if ((iShow != TRUE) && (iShow != FALSE)){
      _sntprintf_s (tcErrorMsg, 200, 199,
        _T("partially ignored because option not \"1\" or \"0\"\n\"%d\" will be used instead"), DBBckp_S->bShowWnd);
      displayIgnoredMessage(h, pCmdLine->GetTokenRel(String(CmdLinePrefix).append(_T("show:")).c_str(), 0), tcErrorMsg );
    }
    DBBckp_S->bWndCmdParam = TRUE;
  }
  // TODO: Dominik: somehow advice the user of ignored command line options
    // displayIgnoredMessage(h, ignored_argument, _T("ignored because option not recognized"));
  VERIFY(pCmdLine->Release());
  VERIFY(pUtils->Release());
}
#endif


#if 0
String GetDestFromCmd(const TCHAR *tcParam, int iParamInstance) {
LPCTSTR lpOptionValue;
String strBuf1, strBuf2;
IKpCommandLine *pCmdLine;
IKpUtilities *pUtils;

  LastPass_Settings * DBBckp_S=LastPass_Settings::GetInstance();

  if (DBBckp_S->g_pAPI->QueryInstance (SCLSID_KpCommandLine, IID_IKpCommandLine, (void**)&pCmdLine) != S_OK) assert(FALSE);
  if (DBBckp_S->g_pAPI->QueryInstance (SCLSID_KpUtilities, IID_IKpUtilities, (void**)&pUtils) != S_OK) assert(FALSE);

  lpOptionValue=pCmdLine->GetMultiOption(String(CmdLinePrefix).append(tcParam).c_str(), iParamInstance);
  if (lpOptionValue != NULL) {
    strBuf1 = ExpandEnvironmentEx(String(lpOptionValue));
    if (!pUtils->ValidatePath(strBuf1.c_str(), KPVPF_REGULAR_NAME | KPVPF_DEFAULT)) {
      displayIgnoredMessage(mainWindowHandle(),
        pCmdLine->GetTokenRel(String(CmdLinePrefix).append(tcParam).c_str(), 0),
        _T("ignored because destination is invalid"));
      strBuf1.clear();
    }
  }
  VERIFY(pCmdLine->Release());
  VERIFY(pUtils->Release());
  return strBuf1;
}
#endif