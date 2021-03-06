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

#ifndef ___TIME_PARSER_H___
#define ___TIME_PARSER_H___


BOOL    ParseVarDateEx(LPCTSTR lpTimeString, PW_TIME* pStoreBuf);
PW_TIME IsoStringToPwTimeEx(const std::basic_string<TCHAR>& rString, PW_TIME* pTimeIfFail);

#endif
