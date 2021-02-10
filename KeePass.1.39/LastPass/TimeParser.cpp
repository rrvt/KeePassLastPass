/*
  VariousImport - KeePass Plugin
  Copyright (C) 2008-2009 Dominik Reichl <dominik.reichl@t-online.de>

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

#include "stdafx.h"
#include "TimeParser.h"

BOOL ParseVarDateEx(LPCTSTR lpTimeString, PW_TIME* pStoreBuf)
{
  if(lpTimeString == NULL) return FALSE;
  if(pStoreBuf == NULL) return FALSE;
  if(_tcslen(lpTimeString) > 19) return FALSE;

  TCHAR tsz[22];
  _tcscpy_s(tsz, _countof(tsz), lpTimeString);

  PW_TIME t;
  ZeroMemory(&t, sizeof(PW_TIME));

  int nFormat = -1;
  if((tsz[2] == '.') && (tsz[5] == '.'))
  {
    tsz[2] = tsz[5] = 0;
    nFormat = 0;
  }
  else if((tsz[4] == '.') && (tsz[7] == '.'))
  {
    tsz[4] = tsz[7] = 0;
    nFormat = 1;
  }

  BOOL bSuccess = FALSE;
  switch(nFormat)
  {
  case 0:
    tsz[10] = 0;
    t.shYear = static_cast<USHORT>(_ttoi(&tsz[6]));
    t.btMonth = static_cast<BYTE>(_ttoi(&tsz[3]));
    t.btDay = static_cast<BYTE>(_ttoi(tsz));
    bSuccess = TRUE;
    break;

  case 1:
    tsz[10] = 0;
    t.shYear = static_cast<USHORT>(_ttoi(tsz));
    t.btMonth = static_cast<BYTE>(_ttoi(&tsz[5]));
    t.btDay = static_cast<BYTE>(_ttoi(&tsz[8]));
    bSuccess = TRUE;
    break;

  default:
    break;
  }

  if((tsz[13] == ':') && (tsz[16] == ':'))
  {
    tsz[13] = tsz[16] = 0;
    t.btHour = static_cast<BYTE>(_ttoi(&tsz[11]));
    t.btMinute = static_cast<BYTE>(_ttoi(&tsz[14]));
    t.btSecond = static_cast<BYTE>(_ttoi(&tsz[17]));
  }

  if(bSuccess == TRUE) *pStoreBuf = t;
  return bSuccess;
}

PW_TIME IsoStringToPwTimeEx(const std::basic_string<TCHAR>& rString, PW_TIME* pTimeIfFail)
{
  PW_TIME t;
  ZeroMemory(&t, sizeof(PW_TIME));

  PW_TIME tFail = ((pTimeIfFail != NULL) ? *pTimeIfFail : t);

  LPCTSTR lp = rString.c_str();
  if(_tcslen(lp) != 19) return tFail;

  if((lp[4] != _T('-')) || (lp[7] != _T('-')) || (lp[10] != _T('T')) ||
    (lp[13] != _T(':')) || (lp[16] != _T(':')))
    return tFail;

  TCHAR tsz[6];
  memset(tsz, 0, sizeof(TCHAR) * 6);
  _tcsncpy_s(tsz, 6, lp, 4);
  t.shYear = static_cast<USHORT>(_ttoi(tsz));
  memset(tsz, 0, sizeof(TCHAR) * 6);
  _tcsncpy_s(tsz, 6, lp + 5, 2);
  t.btMonth = static_cast<BYTE>(_ttoi(tsz));
  memset(tsz, 0, sizeof(TCHAR) * 6);
  _tcsncpy_s(tsz, 6, lp + 8, 2);
  t.btDay = static_cast<BYTE>(_ttoi(tsz));
  memset(tsz, 0, sizeof(TCHAR) * 6);
  _tcsncpy_s(tsz, 6, lp + 11, 2);
  t.btHour = static_cast<BYTE>(_ttoi(tsz));
  memset(tsz, 0, sizeof(TCHAR) * 6);
  _tcsncpy_s(tsz, 6, lp + 14, 2);
  t.btMinute = static_cast<BYTE>(_ttoi(tsz));
  memset(tsz, 0, sizeof(TCHAR) * 6);
  _tcsncpy_s(tsz, 6, lp + 17, 2);
  t.btSecond = static_cast<BYTE>(_ttoi(tsz));

  return t;
}
