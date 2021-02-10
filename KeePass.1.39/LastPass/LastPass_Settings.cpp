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
#include "LastPass_Settings.h"
#include <stdio.h>


LastPass_Settings* LastPass_Settings::GetInstance() {static LastPass_Settings inst;  return &inst;}


LastPass_Settings::LastPass_Settings(void) : bDestCmdParam(FALSE),   bPrgCmdParam (FALSE),
                                             bCountCmdParam (FALSE), bWndCmdParam (FALSE),
                                             iBackupCnt (0),         bShowWnd (FALSE),
                                             bStartExtPrg (FALSE),   bOpenPrfxCmdParam (FALSE),
                                             bSavePrfxCmdParam (FALSE) {

  tcPluginTitle = LastPassPluginName;   shiftBy =(sizeof(TCHAR) == 2 ? 1 : 0);

  ZeroMemory(tcStartPrg, MAX_PATH << shiftBy);   ZeroMemory(tcAppBasePath, MAX_PATH << shiftBy);

  #ifdef _MYDEBUG
    if((fout = fopen("debug.txt", "a+"))==NULL)
                {MessageBox(NULL, _T("Could not open \"debug.txt\"."),_T("ERROR"), MB_OK); return FALSE;}
  #endif
  }

