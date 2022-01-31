

#include "stdafx.h"
#include "KpRecord.h"
#include "Groups.h"
#include "LpRecord.h"


TCchar* NotesURL = _T("http://sn");             // flag in url for a note




KpRecord& KpRecord::operator= (LpRecord& rcd) {

  if (rcd.grpName.isEmpty()) rcd.grpName = rcd.isNote() ? _T("Notes") : _T("General");

  uGroupId      = groups.getID(rcd.grpName);
if (!uGroupId) {
int x = 1;
}
  pszTitle      = const_cast<LPTSTR>(rcd.title.str());
  pszURL        = const_cast<LPTSTR>(rcd.url.str());
  pszUserName   = const_cast<LPTSTR>(rcd.userName.str());
  pszPassword   = const_cast<LPTSTR>(rcd.password.str());
  uPasswordLen  = static_cast<DWORD>(_tcslen(pszPassword));
  pszAdditional = const_cast<LPTSTR>(rcd.additional.str());

  tCreation     = rcd.time;
  tLastMod      = rcd.time;
  tLastAccess   = rcd.time;
  tExpire       = rcd.expire;

  return *this;
  }


bool KpRecord::operator== (KpRecord& rcd) {
  if (_tcscmp(pszTitle,      rcd.pszTitle))                  return false;
  if (_tcscmp(pszUserName,   rcd.pszUserName))               return false;
  if (uPasswordLen    !=     rcd.uPasswordLen)               return false;
  if (memcmp(pszPassword,    rcd.pszPassword, uPasswordLen)) return false;

  return true;
  }

