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
#include "LastPass.h"
#include "Duplicates.h"
#include "Groups.h"
#include <assert.h>


#ifdef _DEBUG
#define VERIFY(f)          assert(f)
#else
#define VERIFY(f)          ((void)(f))
#endif


TCchar* ModuleName    = "Last Pass Import";
TCchar* PluginTitle   = "Last Pass Export Plugin";
TCchar* CmdLinePrefix = "lp_export.";
TCchar* LPsection     = _T("LastPass");

static LastPass  lastPass;
static HINSTANCE dllInstance;

IKpAPI*      keePss() {          return lastPass.keePass;}
IKpDatabase* database() {        return lastPass.kpdb;}
HWND         mainWindowHandle() {return lastPass.h;}
HINSTANCE    instDLL() {         return lastPass.hInstDLL;}
Cchar*       basePath() {        return lastPass.appBasePath.str();}

int          shiftBy() {         return (sizeof(TCHAR) == 2 ? 1 : 0);}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {

  UNREFERENCED_PARAMETER(fdwReason);  UNREFERENCED_PARAMETER(lpvReserved);

  dllInstance = hinstDLL;   return TRUE;
  }


KP_EXPORT HRESULT KP_API KP_I_INITIALIZELIB_DECL(IKpUnknown* pAPI)
                                                            {UNREFERENCED_PARAMETER(pAPI); return S_OK;}


KP_EXPORT HRESULT KP_API KP_I_CREATEINSTANCE_DECL(REFIID riid, void** ppvObject, IKpUnknown* keePass) {

  *ppvObject = NULL;

  if (keePass == NULL) return E_UNEXPECTED;                       // Require API

  if (riid != IID_IKpUnknown && riid != IID_IKpPlugin) return E_NOINTERFACE;

  lastPass.hInstDLL = dllInstance;
  lastPass.keePass  = (IKpAPI*) keePass;

  if (!lastPass.initialize()) {assert(false); return E_FAIL;}

  *ppvObject = riid == IID_IKpPlugin ? &lastPass : (IKpUnknown*) &lastPass;

  return S_OK;
  }


STDMETHODIMP_(DWORD)         LastPass::GetMenuItemCount() {return myMenu.nMenu;}
STDMETHODIMP_(KP_MENU_ITEM*) LastPass::GetMenuItems()     {return myMenu.menu;}


bool LastPass::initialize() {

  if (keePass->CreateInstance(CLSID_KpConfig,     IID_IKpConfig,    (void**) &cfg))       return false;
  if (keePass->QueryInstance( SCLSID_KpDatabase,  IID_IKpDatabase,  (void**) &kpdb))      return false;
  if (keePass->QueryInstance( SCLSID_KpUtilities, IID_IKpUtilities, (void**) &utilities)) return false;

  if (!cfg || !kpdb || !utilities) return false;

  h = keePass->GetMainWindowHandle();

  setBasePath();

  InitTranslationEngine(keePass->GetTranslationName());       // Initialize the translation engine

  groups.setKpdb(kpdb);

  myMenu.init(cfg);                                           // Set up the menus

  return true;
  }


void LastPass::setBasePath() {
IKpCommandLine* pCmdLine;
int             pos;

  if (!keePass) return;

  if (keePass->QueryInstance(SCLSID_KpCommandLine, IID_IKpCommandLine, (void**) &pCmdLine) != S_OK)
                                                                                          assert(FALSE);
  appBasePath = pCmdLine->GetExecutablePath();

  VERIFY(pCmdLine->Release());

  pos = appBasePath.find_last_of('\\');
  appBasePath = appBasePath.substr(0,pos);
  }


LastPass::~LastPass() {close();}


void LastPass::close() {SAFE_RELEASE(utilities); SAFE_RELEASE(kpdb);}



STDMETHODIMP_(ULONG) LastPass::AddRef() { return ++__m_uInterfaceRefCount; }


STDMETHODIMP_(ULONG) LastPass::Release() {
ULONG cnt;

  if (__m_uInterfaceRefCount <= 0) { assert(false); return 0;}

  cnt = --__m_uInterfaceRefCount;

  if (__m_uInterfaceRefCount == 0) close();       // {delete this; lastPass = 0;}

  return cnt;
  }


STDMETHODIMP LastPass::QueryInterface(REFIID riid, void** ppvObject) {
  KP_SUPPORT_INTERFACE(IID_IKpUnknown, IKpUnknown);
  KP_SUPPORT_INTERFACE(IID_IKpPlugin, IKpPlugin);

  *ppvObject = NULL;  return E_NOINTERFACE;
  }



KP_EXPORT HRESULT KP_API KP_I_RELEASELIB_DECL(IKpUnknown* pAPI) {
  UNREFERENCED_PARAMETER(pAPI);

  #ifdef _MYDEBUG
  fclose(DBBckp_S->fout);
  #endif

  return S_OK;
  }


STDMETHODIMP_(LPCTSTR) LastPass::GetProperty(LPCTSTR lpName) {
  if (lpName == NULL) return NULL;

  if (_tcscmp(lpName, KPPS_COMMANDLINEARGPREFIX) == 0) return CmdLinePrefix;

  return NULL;
  }


STDMETHODIMP LastPass::SetProperty(LPCTSTR lpName, LPCTSTR lpValue)
                      {UNREFERENCED_PARAMETER(lpName);  UNREFERENCED_PARAMETER(lpValue);  return E_FAIL;}


STDMETHODIMP_(BOOL) LastPass::OnMessage(DWORD dwCode, LPARAM lParamW, LPARAM lParamL) {

  switch(dwCode) {
    case KPM_DIRECT_EXEC  : doCommand(lParamW, lParamL); break;
    default               : break;
    }

  return TRUE;
  }



void LastPass::doCommand(LPARAM lParam, LPARAM) {
int        menuX;
DWORD      cmdID = lParam;

  for (menuX = 1; menuX < myMenu.nMenu; menuX++) if (myMenu[menuX].dwCommandID == cmdID) break;

  switch (menuX) {
    case LP_ExportEntries     - BaseMenuItem: exportLP();       break;
    case LP_ImportEntries     - BaseMenuItem: importLP();         break;
    case LP_ExpungeFile       - BaseMenuItem: expungeFile();         break;
    case LP_ToggleDelState    - BaseMenuItem: toggleDelState();      break;
    case LP_RemoveReduntantPW - BaseMenuItem: removeRedundantPW();   break;
    case LP_About             - BaseMenuItem: about();               break;
    case LP_Test              - BaseMenuItem: testStuff();           break;
    default                                 :                        break;
    }
  }


void LastPass::importLP() {
FileIn   fi(ModuleName, myMenu.getDelState());
String   path;
LPentry  lpe(kpdb);
int      i;
//Database db(kpdb);
//DWORD    index;

  if (!getOpenFileName(_T("csv"), path) || !fi.open(path)) return;

  for (i = 0; lpe.readRecord(fi); i++) {
    if (i == 0) continue;

    if (lpe.isNote()) importNote(lpe);
    else              importPW(lpe);
    }

  keePass->UpdateUI();   keePass->SetFileModified(true);
  }


void LastPass::importPW(LPentry& lpe) {
Database db(kpdb);
int      index;

  // url, Name and Passord Match
  if (db.findNmPswdMatch(lpe, index))       {DBentry dbe;   dbe = lpe;  dbe.update(index);}

  // url, Name and Title Match => password change?
  else if (db.findNmTtlMatch(lpe, index))   {DBentry dbe;   dbe = lpe;  dbe.update(index);}

  // url, Title and Password Match
  else if (db.findTtlPswdMatch(lpe, index)) {DBentry dbe;   dbe = lpe;  dbe.update(index);}

  // Otherwise just add new entry...
  else                                      {DBentry dbe;   dbe = lpe;  dbe.add();}
  }



void LastPass::importNote(LPentry& lpe) {
Database db(kpdb);
int      index;

  if (db.findTitleNotesMatch(lpe, index)) {DBentry dbe;   dbe = lpe;  dbe.update(index);}

  // Otherwise just add new entry...
  else                                   {DBentry dbe;   dbe = lpe;  dbe.add();}
  }


void LastPass::about() {MessageBox(h, translate(XL_ABOUT, _T("XL_ABOUT")),
                                                              PluginTitle, MB_OK | MB_ICONINFORMATION);}



void LastPass::exportLP() {
DBIterator        iter(kpdb);
DBentryP*         dbeP;
String            path;
FileOut fo(_T("Last Pass"));
LPentry           lpe(kpdb);

  if (!getFileName("csv", path) || !fo.open(path)) return;

  fo.write("url,username,password,extra,name,grouping,fav\n");

  for (dbeP = iter.startLoop(); dbeP; dbeP = iter.nextEntry()) {
    DBentry* dbe   = dbeP->dbe;

    if (!strcmp(dbe->pszURL, _T("http://ff"))) continue;

    lpe = *dbe;   lpe.writePW(fo);
    }

  iter.notesOnly();

  for (dbeP = iter.startLoop(); dbeP; dbeP = iter.nextEntry()) {
    DBentry* dbe   = dbeP->dbe;

    lpe = *dbe;   lpe.writePW(fo);
    }

  fo.put(_T('\n'));
  }


void LastPass::expungeFile() {
FileIn fi(_T("Last Pass"), true);
String path;

  if (!getOpenFileName(_T("*"), path)) return;

  fi.open(path); return;
  }



bool LastPass::getFileName(Cchar* ext, String& path) {
BOOL        bOldDisplayDialog;
const DWORD dwBufSize = (MAX_PATH * 4);
TCHAR       lpBuf[dwBufSize + 2];
HRESULT     hr;

  bOldDisplayDialog = keePass->IsDisplayingDialog();

    keePass->SetDisplayDialog(TRUE);

    hr = utilities->ShowFileDialog(false, ext, lpBuf, dwBufSize);

  keePass->SetDisplayDialog(bOldDisplayDialog);

  if (hr != S_OK) return false;

  path = lpBuf;   return true;
  }




bool LastPass::getOpenFileName(Cchar* ext, String& path) {
BOOL        bOldDisplayDialog;
const DWORD dwBufSize = (MAX_PATH * 4);
TCHAR       lpBuf[dwBufSize + 2];
HRESULT     hr;

  bOldDisplayDialog = keePass->IsDisplayingDialog();

    keePass->SetDisplayDialog(TRUE);

    hr = utilities->ShowFileDialog(TRUE, ext, &lpBuf[0], dwBufSize);

  keePass->SetDisplayDialog(bOldDisplayDialog);

  if (hr != S_OK) return false;

  path = lpBuf;   return true;
  }



void LastPass::removeRedundantPW() {Duplicates dups(kpdb);  dups.remove();}




void LastPass::testStuff() {
DBIterator iter(kpdb);
DBentryP*  dbeP;
int        noEntries = 0;
int        noNotes   = 0;
int        n         = 0;

  for (dbeP = iter.startLoop(); dbeP; dbeP = iter.nextEntry()) {
    noEntries++;
    }

  iter.notesOnly();

  for (dbeP = iter.startLoop(); dbeP; dbeP = iter.nextEntry()) {
    n++;  if (dbeP->dbe->isNote()) noNotes++;
    }
  }



#if 0
void LastPass::importFormFills() {
Import import;
String   path;

  if (!getOpenFileName(_T("csv"), path)) return;

  import.importFormFillsFile(path, myMenu.getDelState());
  }


void LastPass::getFormFillLabels() {
Import          import;
String            path;
LPform            lpForm;       //keePass
String            outPath;
FileOut fileOut(_T("Last Pass"));

  if (!getOpenFileName(_T("csv"), path)) return;

  import.getFormFillsLabels(path, myMenu.getDelState(), lpForm);

  if (lpForm.labelsOK()) return;

  if (!getFileName("csv", outPath)) return;

  if (!fileOut.open(outPath)) return;

  lpForm.writeLabels(fileOut);
  }
#endif



#if 0
void LastPass::exportFormFills() {
String            path;
FileOut fileOut(_T("Last Pass"));
DWORD             i;
DWORD             nEntries;

  if (!getFileName("csv", path)) return;

  if (!fileOut.open(path)) return;

  nEntries = kpdb->GetEntryCount();

  for (i = 0; i < nEntries; i++) {
    VsblEntry vsblEntry;   vsblEntry.entry.initialize(kpdb, kpdb->GetEntry(i));

    if (!vsblEntry.entry.grpName.compare("Backup")) continue;                         // do not backup
#if 0
    if (!vsblEntry.entry.url.compare(FFurl) && vsblEntry.entry.title == "~Heading~") {
      vsblEntry.exportFormFill(fileOut);   break;
      }
#endif
    }

  for (i = 0; i < nEntries; i++) {
    VsblEntry vsblEntry;   vsblEntry.entry.initialize(kpdb, kpdb->GetEntry(i));

    if (!vsblEntry.entry.grpName.compare("Backup")) continue;                         // do not backup
#if 0
    if (!vsblEntry.entry.url.compare(FFurl) && vsblEntry.entry.title != "~Heading~")
                                                                      vsblEntry.exportFormFill(fileOut);
#endif
    }
  }


void LastPass::exportFFNew() {
FileOut fo(_T("Last Pass"));
String            path;
Database          db(kpdb);
int               n = db.size();
int               i;
DBentry*          dbe;
LPentry           lpe;
String            line;

  if (!getFileName("csv", path) || !fo.open(path)) return;

//  db.setFilterFormFill(false);

  for (i = 0; i < n; i++) {

    dbe = db.get(i);  if (!dbe) continue;                             //|| !dbe->formFillOnly()

    if (!strcmp(dbe->pszTitle, "~Heading~")) {line = dbe->getFFLine() + "\n";  fo.writeRaw(line); break;}
    }

  for (i = 0; i < n; i++) {

    dbe = db.get(i);  if (!dbe) continue;                           //|| !dbe->formFillOnly()

    if (strcmp(dbe->pszTitle, "~Heading~")) {line = dbe->getFFLine() + "\n";  fo.writeRaw(line);}
    }
  }
#endif


#if 0
void LastPass::importEntries() {
Import import;
String   path;

  if (!getOpenFileName(_T("csv"), path)) return;

  import.importFile(path, myMenu.getDelState());
  }
#endif
#if 0
void LastPass::exportEntries() {
String            path;
FileOut fileOut(_T("Last Pass"));
DWORD             i;
DWORD             nEntries;

  if (!getFileName("csv", path)) return;

  if (!fileOut.open(path)) return;

  fileOut.write("url,username,password,extra,name,grouping,fav\n");

  nEntries = kpdb->GetEntryCount();

  for (i = 0; i < nEntries; i++) {
    VsblEntry vsblEntry;

    vsblEntry.entry.initialize(kpdb, kpdb->GetEntry(i));
    if (!vsblEntry.entry.grpName.compare("Backup")) continue;                         // do not backup
//    if (vsblEntry.entry.url.compare(FFurl)) vsblEntry.exportEntry(fileOut);
    }
  }
#endif
#if 0
    // url, Name and Passord Match
    if (db.findNmPswdMatch(lpe, index))       {DBentry dbe;   dbe = lpe;  dbe.update(index);}

    // url, Name and Title Match => password change?
    else if (db.findNmTtlMatch(lpe, index))   {DBentry dbe;   dbe = lpe;  dbe.update(index);}
    // url, Title and Password Match
    else if (db.findTtlPswdMatch(lpe, index)) {DBentry dbe;   dbe = lpe;  dbe.update(index);}
    // Otherwise just add new entry...
    else                                      {DBentry dbe;   dbe = lpe;  dbe.add();}
#endif
#if 0
void LastPass::suppressDups(DBentry& entry) {
String     url = entry.url();
DBIterator iter(kpdb);
DBentryP*  dupEP;
int        entryAddlLng;
int        dupEaddlLng;
Database   db(kpdb);

  for (dupEP = iter.startLoop(url); dupEP; dupEP = iter.nextURL()) {
    DBentry* dupE  = dupEP->dbe;
    int      index = dupEP->index;

    if (*dupE == entry) continue;

    if (!strcmp(entry.pszURL, NotesURL)) {if (*dupE >= entry) continue;}
    else if (*dupE != entry) continue;

    entryAddlLng = strlen(entry.pszAdditional);
    dupEaddlLng  = strlen(dupE->pszAdditional);

    if (dupE->uGroupId == entry.uGroupId) {

      if (entryAddlLng == dupEaddlLng) {
        PWtime* t = (PWtime*) &entry.tCreation;
        PWtime* u = (PWtime*) &dupE->tCreation;

        if (*t >= *u) {db.remove(index); iter.backup(1);}
        }

      else if (entryAddlLng > dupEaddlLng) {db.remove(index);  iter.backup(1);}
      }

    else {
      if (entryAddlLng == dupEaddlLng) {
        PWtime* t = (PWtime*) &entry.tCreation;
        PWtime* u = (PWtime*) &dupE->tCreation;
        if (*t >= *u) {db.remove(index);  iter.backup(1);}
        }

      else if (entryAddlLng > dupEaddlLng) {db.remove(index);  iter.backup(1);}
      }
    }
  }
#endif
#if 0
DBIterator iter(kpdb);
DBentryP*  dbeP;

  for (dbeP = iter.startLoop(); dbeP; dbeP = iter.nextEntry()) {
    DBentry* dbe = dbeP->dbe;

    suppressDups(*dbe);
    }

  iter.notesOnly();

  for (dbeP = iter.startLoop(); dbeP; dbeP = iter.nextEntry()) {
    DBentry* dbe = dbeP->dbe;

    suppressDups(*dbe);
    }
#endif

