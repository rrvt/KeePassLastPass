/*
  KeePass Password Safe - The Open-Source Password Manager
  Copyright (C) 2003-2021 Dominik Reichl <dominik.reichl@t-online.de>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef ___KPAPIIMPL_H___
#define ___KPAPIIMPL_H___

#include "../../KeePassLibCpp/SDK/KpSDK.h"
#include "../Util/PrivateConfigEx.h"

#pragma pack(1)

class CKpApiImpl : public IKpAPI3
{
private:
  CKpApiImpl();

public:
  static CKpApiImpl& Instance();

  KP_DECL_IUNKNOWN;

  STDMETHODIMP CreateInstance(REFCLSID rclsid, REFIID riid, void** ppvObject);
  STDMETHODIMP CreateInstanceEx(REFCLSID rclsid, REFIID riid, void** ppvObject,
    LPARAM lParamW, LPARAM lParamL);
  STDMETHODIMP QueryInstance(REFCLSID rclsid, REFIID riid, void** ppvObject);

  STDMETHODIMP DeleteObject(void* pObject);
  STDMETHODIMP DeleteArray(void* pArray);

  STDMETHODIMP_(DWORD) GetVersion();
  STDMETHODIMP_(LPCTSTR) GetVersionStr();
  STDMETHODIMP_(DWORD) GetCharSize();
  STDMETHODIMP_(DWORD) GetPointerSize();
  STDMETHODIMP_(DWORD) GetDebugLevel();

  STDMETHODIMP_(HWND) GetMainWindowHandle();
  STDMETHODIMP_(HIMAGELIST) GetClientIconsImageList();

  STDMETHODIMP_(LPCTSTR) GetTranslationName();
  STDMETHODIMP_(LPCTSTR) Translate(LPCTSTR lpDefString);

  STDMETHODIMP GetNeverExpireTime(PW_TIME* pBuf);
  STDMETHODIMP GetCurrentPwTime(PW_TIME* pBuf);

  STDMETHODIMP_(BOOL) IsFileOpen();
  STDMETHODIMP_(LPCTSTR) GetAbsoluteDatabasePath();

  STDMETHODIMP_(DWORD) GetEntryListItemCount();
  STDMETHODIMP_(DWORD) GetSelectedGroupId();

  STDMETHODIMP_(BOOL) IsFileModified();
  STDMETHODIMP SetFileModified(BOOL bModified);

  STDMETHODIMP_(BOOL) IsDisplayingDialog();
  STDMETHODIMP SetDisplayDialog(BOOL bDialogDisplayed);

  STDMETHODIMP EntryListEnsureVisible(INT nItem, BOOL bPartialOK);

  STDMETHODIMP UpdateToolBar();
  STDMETHODIMP UpdateGroupTree();
  STDMETHODIMP UpdateEntryList();
  STDMETHODIMP UpdateUI();

  STDMETHODIMP SetStatusBarText(LPCTSTR lpText);

  STDMETHODIMP ShowDialog(DWORD dwDialogId, DWORD dwVersion, void* pConfig,
    INT* pnResult, void* pOutput);

  STDMETHODIMP AutoType(PW_ENTRY* pEntry, LONG_PTR lFlags);

  STDMETHODIMP AddImportMenuItem(LPCTSTR lpText, UINT uId, INT nIconId);

  STDMETHODIMP_(BOOL) IsInitialCommandLineFile();

  STDMETHODIMP GetProperty(DWORD dwID, void* pOutValue);
  STDMETHODIMP SetProperty(DWORD dwID, void* pNewValue);

  STDMETHODIMP_(BOOL) IsFileLocked();
  STDMETHODIMP LockFile(BOOL bLock);

  STDMETHODIMP_(BOOL) IsFileReadOnly();

  STDMETHODIMP NotifyUserActivity();

  STDMETHODIMP ParseAndOpenUrlWithEntryInfo(LPCTSTR lpUrl, PW_ENTRY* pEntry);

  STDMETHODIMP_(BOOL) CanSort();
  STDMETHODIMP SortListIfAutoSort();
  STDMETHODIMP SortList(DWORD dwByField, BOOL bAutoSortCall);

  STDMETHODIMP EntryListSaveView();
  STDMETHODIMP EntryListRestoreView();

  STDMETHODIMP GroupTreeSaveView(BOOL bSaveSelection);
  STDMETHODIMP GroupTreeRestoreView();

  STDMETHODIMP_(BOOL) RemoveSearchGroup();

  STDMETHODIMP UpdateTitleBar();
  STDMETHODIMP UpdateTrayIcon();
  STDMETHODIMP UpdateGuiToManager();
  STDMETHODIMP UpdateCachedGroupIDs();

  STDMETHODIMP_(BOOL) IsUnsafeAllowed(HWND hWndParent);

  STDMETHODIMP Find(DWORD dwFindGroupId);
  STDMETHODIMP QuickFind(LPCTSTR lpText);

  STDMETHODIMP ShowToolBar(BOOL bShow);

  STDMETHODIMP_(UINT) GetControlMessageID();

  STDMETHODIMP_(BOOL) IsInMiniMode();

private:
  KP_DECL_STDREFIMPL;
};

#pragma pack()

#pragma pack(1)

class CKpConfigImpl : public IKpConfig
{
public:
  CKpConfigImpl(BOOL bRequireWriteAccess);

  KP_DECL_IUNKNOWN;

  STDMETHODIMP_(BOOL) Get(LPCTSTR pszField, LPTSTR pszValue);
  STDMETHODIMP_(BOOL) Set(LPCTSTR pszField, LPCTSTR pszValue);

  STDMETHODIMP_(BOOL) GetBool(LPCTSTR pszField, BOOL bDefault);
  STDMETHODIMP_(BOOL) SetBool(LPCTSTR pszField, BOOL bValue);

// Van Tuyl Additions

  STDMETHODIMP_(bool) set(TCHAR* section, const TCHAR* pszField, const TCHAR* pszValue)
                                                   {return m_pConfig->set(section, pszField, pszValue);}
  STDMETHODIMP_(bool) set(TCHAR* section, const TCHAR* pszField, bool bValue)
                                                   {return m_pConfig->set(section, pszField, bValue);}
  STDMETHODIMP_(bool) get(TCHAR* section, const TCHAR* pszField,       TCHAR* pszValue)
                                                   {return m_pConfig->get(section, pszField, pszValue);}
  STDMETHODIMP_(bool) get(TCHAR* section, const TCHAR* pszField, std_string&  val)
                                                   {return m_pConfig->get(section, pszField, val);}
  STDMETHODIMP_(bool) get(TCHAR* section, const TCHAR* pszField, bool bDefault)
                                                   {return m_pConfig->get(section, pszField, bDefault);}
  STDMETHODIMP_(bool) del(TCHAR* section, const TCHAR* pszField)
                                                   {return m_pConfig->del(section, pszField);}
// End Van Tuyl Additions


  STDMETHODIMP GetUserPath(LPTSTR lpBuf, DWORD dwBufMaxChars);

private:
  KP_DECL_STDREFIMPL;

  CPrivateConfigEx* m_pConfig;
};

#pragma pack()

void KPMI_SetMainDialog(void* pMainPwSafeDlg);
void* KPMI_GetMainDialog();

#endif // ___KPAPIIMPL_H___
