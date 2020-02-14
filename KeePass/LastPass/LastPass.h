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
#include "LPentry.h"
#include "MyMenu.h"


KP_EXPORT HRESULT KP_API KP_I_CREATEINSTANCE_DECL(REFIID riid, void** ppvObject, IKpUnknown* pAPI);


extern TCchar* ModuleName;
extern TCchar* LPsection;


class LastPass : public IKpPlugin {

KP_DECL_STDREFIMPL;                               // ULONG __m_uInterfaceRefCount

HINSTANCE     hInstDLL;
IKpAPI*       keePass;
IKpConfig*    cfg;
IKpDatabase*  kpdb;
IKpUtilities* utilities;
HWND          h;                        // Main Window Handle
MyMenu        myMenu;
String        appBasePath;

public:

  LastPass() : keePass(0), cfg(0), kpdb(0), utilities(0), h(0) {KP_IMPL_CONSTRUCT}

 ~LastPass();

  bool       initialize();
  void       close();

  void       doCommand(LPARAM lParamW, LPARAM lParamL);

  bool       getFileName(    Cchar* ext, String& path);
  bool       getOpenFileName(Cchar* ext, String& path);
  void       setBasePath();

  KP_DECL_IUNKNOWN;               // STDMETHODIMP         QueryInterface(REFIID riid, void** ppvObject);
                                  // STDMETHODIMP_(ULONG) AddRef();
                                  // STDMETHODIMP_(ULONG) Release();

  STDMETHODIMP_(BOOL)          OnMessage(DWORD dwCode, LPARAM lParamW, LPARAM lParamL);
  STDMETHODIMP_(LPCTSTR)       GetProperty(LPCTSTR lpName);
  STDMETHODIMP                 SetProperty(LPCTSTR lpName, LPCTSTR lpValue);
  STDMETHODIMP_(DWORD)         GetMenuItemCount();
  STDMETHODIMP_(KP_MENU_ITEM*) GetMenuItems();

private:

  void importLP();
  void exportLP();

  void expungeFile();

  void toggleDelState() {myMenu.toggleDelState();}

  void removeRedundantPW();

  void testStuff();
  void about();

  friend IKpAPI*      keePss();
  friend IKpDatabase* database();
  friend HWND         mainWindowHandle();
  friend IKpAPI*      getAPI();
  friend HINSTANCE    instDLL();
  friend Cchar*       basePath();

  friend BOOL WINAPI    DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
  friend HRESULT KP_API KP_I_CREATEINSTANCE_DECL(REFIID riid, void** ppvObject, IKpUnknown* keePass);

private:

  void importPW(LPentry& lpe);
  void importNote(LPentry& lpe);
  };


//String GetNextDestination(IKpConfig *pCfg, BOOL bReset);


IKpAPI*         keePss();
IKpDatabase*    database();
HWND            mainWindowHandle();
HINSTANCE       instDLL();
int             shiftBy();
Cchar*          basePath();

extern TCchar*  PluginTitle;
extern TCchar*  CmdLinePrefix;

