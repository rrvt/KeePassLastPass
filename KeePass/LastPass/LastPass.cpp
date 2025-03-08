// LastPass.cpp : Defines the initialization routines for the DLL.
//

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//    any functions exported from this DLL which call into
//    MFC must have the AFX_MANAGE_STATE macro added at the
//    very beginning of the function.
//
//    For example:
//
//    extern "C" BOOL PASCAL EXPORT ExportedFunction()
//    {
//      AFX_MANAGE_STATE(AfxGetStaticModuleState());
//      // normal function body here
//    }
//
//    It is very important that this macro appear in each
//    function, prior to any calls into MFC.  This means that
//    it must appear as the first statement within the
//    function, even before any object variable declarations
//    as their constructors may generate calls into the MFC
//    DLL.
//
//    Please see MFC Technical Notes 33 and 58 for additional
//    details.
//

#include "pch.h"
#include "LastPass.h"
#include "Commands.h"
#include "Groups.h"
#include "Language.h"
#include "LastPassApp.h"


static TCchar* CmdLinePrefix = "lp_export.";


LastPass    lastPass;
IniFile     iniFile;



DllExp HRESULT KP_I_CREATEINSTANCE_DECL(REFIID riid, void** ppvObject, IKpUnknown* api) {

  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  *ppvObject = 0;     if (!api) return E_UNEXPECTED;                       // Require API

  if (riid != IID_IKpUnknown && riid != IID_IKpPlugin) return E_NOINTERFACE;

  if (!lastPass.initialize(api)) {Verify(false); return E_FAIL;}

  *ppvObject = &lastPass;   return S_OK;
  }


KP_EXPORT HRESULT KP_API KP_I_INITIALIZELIB_DECL(IKpUnknown*) {return S_OK;}



bool LastPass::initialize(IKpUnknown* api) {

  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  hInstDLL = theApp.m_hInstance;
  keePass  = (IKpAPI*) api;

  if (keePass->CreateInstance(CLSID_KpConfig,     IID_IKpConfig,    (void**) &cfg))   return false;
  if (keePass->QueryInstance( SCLSID_KpDatabase,  IID_IKpDatabase,  (void**) &kpdb))  return false;
  if (keePass->QueryInstance( SCLSID_KpUtilities, IID_IKpUtilities, (void**) &utilities))
                                                                                      return false;
  if (!cfg || !kpdb || !utilities) return false;

  h = keePass->GetMainWindowHandle();

  setBasePath();

  language.initialize(keePass->GetTranslationName());

  groups.setKpdb(kpdb);

  menu.init(cfg);                                           // Set up the menus

  return true;
  }


void LastPass::setBasePath() {
IKpCommandLine* pCmdLine;
int             pos;

  if (!keePass) return;

  if (keePass->QueryInstance(SCLSID_KpCommandLine,
                                    IID_IKpCommandLine, (void**) &pCmdLine) != S_OK) Verify(FALSE);
  appBasePath = pCmdLine->GetExecutablePath();

  VERIFY(pCmdLine->Release());

  pos = appBasePath.find_last_of('\\');

  appBasePath = appBasePath.substr(0,pos);
  }


STDMETHODIMP_(HRESULT) LastPass::QueryInterface(REFIID riid, void** ppvObject) {
#define __m_uInterfaceRefCount refCount
  KP_SUPPORT_INTERFACE(IID_IKpUnknown, IKpUnknown);
  KP_SUPPORT_INTERFACE(IID_IKpPlugin,  IKpPlugin);

  *ppvObject = 0;  return E_NOINTERFACE;
  }


STDMETHODIMP_(ULONG) LastPass::AddRef() {return ++refCount;}


STDMETHODIMP_(ULONG) LastPass::Release() {
AFX_MANAGE_STATE(AfxGetStaticModuleState());
ULONG cnt;

  if (refCount <= 0) { Verify(false); return 0;}

  cnt = --refCount;

  if (refCount <= 0) close();

  return cnt;
  }


void LastPass::close() {SAFE_RELEASE(cfg); SAFE_RELEASE(utilities); SAFE_RELEASE(kpdb);}


STDMETHODIMP_(BOOL) LastPass::OnMessage(DWORD dwCode, LPARAM lParamW, LPARAM lParamL) {

  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  switch(dwCode) {
    case KPM_DIRECT_EXEC  : doCommand(lParamW, lParamL); break;
    default               : break;
    }

  return TRUE;
  }



void LastPass::doCommand(LPARAM commandID, LPARAM) {
int      id = menu.get(commandID);
Commands commands(*keePass, *kpdb);


  groups.initialize();

  switch (id) {
    case LP_ExportEntries     : commands.exportLP();            break;
    case LP_ImportEntries     : commands.importLP();            break;
    case LP_ExpungeFile       : commands.expungeFile();         break;
    case LP_ToggleDelState    : commands.toggleDelState();      break;
    case LP_RemoveReduntantPW : commands.removeRedundantPW();   break;
    case LP_About             : commands.about();               break;
    case LP_Test              : commands.testStuff();           break;
    }
  }


STDMETHODIMP_(LPCTSTR) LastPass::GetProperty(LPCTSTR lpName) {

  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  if (lpName == NULL) return NULL;

  if (_tcscmp(lpName, KPPS_COMMANDLINEARGPREFIX) == 0) return CmdLinePrefix;

  return NULL;
  }


STDMETHODIMP_(HRESULT) LastPass::SetProperty(LPCTSTR, LPCTSTR) {

  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  return E_FAIL;
  }


STDMETHODIMP_(DWORD) LastPass::GetMenuItemCount() {return menu.noMenu();}


STDMETHODIMP_(KP_MENU_ITEM*) LastPass::GetMenuItems() {return menu.addr();}


