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

#pragma once
#include "SDK/KpSDK.h"
#include <windows.h>
#include <tchar.h>


#define CMDLINE_PREFIX     _T("lp_export.")
#define LastPassPluginName _T("Last Pass Export Plugin")


class LastPass_Settings {
public:

#if 0
// Global variables holding the configuration
IKpAPI*      g_pAPI;
BOOL         bDestCmdParam;
BOOL         bPrgCmdParam;
BOOL         bCountCmdParam;
BOOL         bWndCmdParam;
BOOL         bOpenPrfxCmdParam;
BOOL         bSavePrfxCmdParam;
int          iBackupCnt;
BOOL         bShowWnd;
BOOL         bStartExtPrg;
TCHAR        tcStartPrg[MAX_PATH];
int          shiftBy;
const TCHAR* tcPluginTitle;
HINSTANCE    hInstDLL;
TCHAR        tcAppBasePath[MAX_PATH];
TCHAR        tcSavePrfx[MAX_PATH];
TCHAR        tcOpenPrfx[MAX_PATH];

#ifdef _MYDEBUG
FILE* fout;
#endif

static LastPass_Settings* GetInstance();

private:

  LastPass_Settings(void);
 ~LastPass_Settings(void) {}



  LastPass_Settings(const LastPass_Settings& );                       // Not defined, to prevent copying
  LastPass_Settings& operator=(const LastPass_Settings& other);
#endif
  };
