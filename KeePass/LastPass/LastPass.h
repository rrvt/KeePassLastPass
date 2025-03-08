// LastPass.h : main header file for the LastPass DLL


#pragma once
#include "Menu.h"


#define DllExp extern "C" __declspec(dllexport)

DllExp HRESULT KP_I_CREATEINSTANCE_DECL(REFIID riid, void** ppvObject, IKpUnknown* api);


class LastPass : public IKpPlugin {

ulong         refCount;

HINSTANCE     hInstDLL;
IKpAPI*       keePass;
IKpConfig*    cfg;
IKpDatabase*  kpdb;
IKpUtilities* utilities;
HWND          h;                        // Main Window Handle
String        appBasePath;

public:

  LastPass() : refCount(1), hInstDLL(0), keePass(0), cfg(0), kpdb(0), utilities(0), h(0) { }

 ~LastPass() {close();}

  bool       initialize(IKpUnknown* api);
  void       close();

  bool       getFileName(    Cchar* ext, String& path);
  bool       getOpenFileName(Cchar* ext, String& path);
  void       setBasePath();

  STDMETHODIMP_(HRESULT)       QueryInterface(REFIID riid, void** ppvObject);
  STDMETHODIMP_(ULONG)         AddRef();
  STDMETHODIMP_(ULONG)         Release();
  STDMETHODIMP_(BOOL)          OnMessage(DWORD dwCode, LPARAM lParamW, LPARAM lParamL);
  STDMETHODIMP_(LPCTSTR)       GetProperty(LPCTSTR lpName);
  STDMETHODIMP_(HRESULT)       SetProperty(LPCTSTR lpName, LPCTSTR lpValue);
  STDMETHODIMP_(DWORD)         GetMenuItemCount();
  STDMETHODIMP_(KP_MENU_ITEM*) GetMenuItems();

  Cchar*                       basePath() {return appBasePath.str();}
  HINSTANCE                    instDLL()  {return hInstDLL;}

private:

  void doCommand(LPARAM lParam, LPARAM);

  void importLP();
  void exportLP();

  void expungeFile();

  void toggleDelState() {menu.toggleDelState();}

  void removeRedundantPW();

  void testStuff();
  void about();

  friend IKpAPI*      keePss();
  friend IKpDatabase* database();
  friend HWND         mainWindowHandle();
  friend IKpAPI*      getAPI();
  friend HINSTANCE    instDLL();
  friend Cchar*       basePath();

  friend HRESULT KP_API KP_I_CREATEINSTANCE_DECL(REFIID riid,
                                                          void** ppvObject, IKpUnknown* keePass);
  };


extern LastPass lastPass;
