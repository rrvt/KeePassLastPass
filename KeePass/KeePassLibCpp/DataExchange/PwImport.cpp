/*
  KeePass Password Safe - The Open-Source Password Manager
  Copyright (C) 2003-2025 Dominik Reichl <dominik.reichl@t-online.de>

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

#include "StdAfx.h"
#include "PwImport.h"

#include "../Util/MemUtil.h"
#include "../Util/StrUtil.h"
#include "../Util/TranslateEx.h"

static TCHAR g_pNullString[4] = { 0, 0, 0, 0 };

CPwImport::CPwImport()
{
}

CPwImport::~CPwImport()
{
}

char *CPwImport::FileToMemory(const TCHAR *pszFile, unsigned long *pFileSize)
{
	ASSERT(pszFile != NULL); if(pszFile == NULL) return NULL;
	if(_tcslen(pszFile) == 0) return NULL;

	FILE *fp = NULL;
	_tfopen_s(&fp, pszFile, _T("rb"));
	if(fp == NULL) return NULL;

	fseek(fp, 0, SEEK_END);
	unsigned long uFileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *pData = new char[uFileSize + 32];
	if(pData == NULL) { fclose(fp); fp = NULL; return NULL; }
	memset(&pData[uFileSize], 0, 32);

	fread(pData, 1, uFileSize, fp);
	fclose(fp); fp = NULL;

	if(pFileSize != NULL) *pFileSize = uFileSize; // Store file size
	return pData;
}

DWORD CPwImport::ImportCsvToDb(const TCHAR *pszFile, CPwManager *pMgr, DWORD dwGroupId)
{
	unsigned long uFileSize, i, j = 0;
	char *pData;
	char *pProcessed;
	char ch;
	BOOL bInField = FALSE;
	BOOL bUTF8 = FALSE;
	DWORD dwFieldCounter = 0;

	ASSERT(pszFile != NULL); if(pszFile == NULL) return 0;
	ASSERT(pMgr != NULL); if(pMgr == NULL) return 0;

	if((dwGroupId == DWORD_MAX) || (dwGroupId == 0)) return 0;

	pData = CPwImport::FileToMemory(pszFile, &uFileSize);
	if(pData == NULL) return FALSE;

	pProcessed = new char[uFileSize + 32];
	if(pProcessed == NULL) { SAFE_DELETE_ARRAY(pData); return 0; }
	memset(&pProcessed[uFileSize], 0, 32);

	// Last character must not be an escape character
	if(pData[uFileSize - 1] == '\\') pData[uFileSize - 1] = 0;

	if(uFileSize > 3)
	{
		if((static_cast<unsigned char>(pData[0]) == 0xEF) &&
			(static_cast<unsigned char>(pData[1]) == 0xBB) &&
			(static_cast<unsigned char>(pData[2]) == 0xBF))
		{
			j += 3; // Skip UTF-8 initialization characters
			bUTF8 = TRUE;
		}
	}

	if(bUTF8 == FALSE) bUTF8 = _IsUTF8String((const UTF8_BYTE *)pData);

	for(i = j; i < uFileSize; i++)
	{
		ch = pData[i];

		if((bInField == FALSE) && (ch == '\n'))
		{
			if((dwFieldCounter % 5) != 0)
			{
				SAFE_DELETE_ARRAY(pProcessed);
				SAFE_DELETE_ARRAY(pData);
				return (dwFieldCounter / 5);
			}
		}

		if(ch == 0) continue;
		else if((ch == '\\') && (pData[i+1] != 0))
		{
			i++; // Skip escape character
			pProcessed[j] = pData[i]; // Write escaped symbol
			j++; // Increase write counter
		}
		else if((bInField == FALSE) && (ch == ',') && (pData[i+1] == ','))
		{
			pProcessed[j] = 0; j++; dwFieldCounter++;
		}
		else if((bInField == FALSE) && (ch == ',') && (pData[i+1] == '\r'))
		{
			pProcessed[j] = 0; j++; dwFieldCounter++;
		}
		else if((bInField == FALSE) && (ch == ',') && (pData[i+1] == '\n'))
		{
			pProcessed[j] = 0; j++; dwFieldCounter++;
		}
		else if(ch == '\"')
		{
			if(bInField == TRUE)
			{
				pProcessed[j] = 0; j++; dwFieldCounter++;

				bInField = FALSE;
			}
			else bInField = TRUE;
		}
		else
		{
			if(bInField == TRUE)
			{
				pProcessed[j] = ch; j++;
			}
		}
	}
	if(bInField == TRUE) { SAFE_DELETE_ARRAY(pData); SAFE_DELETE_ARRAY(pProcessed); return (DWORD_MAX - 1); }

	m_pLastMgr = pMgr;
	m_dwLastGroupId = dwGroupId;
	_AddStringStreamToDb(pProcessed, j, bUTF8);

	SAFE_DELETE_ARRAY(pProcessed);
	SAFE_DELETE_ARRAY(pData);
	return DWORD_MAX;
}

BOOL CPwImport::ImportCWalletToDb(const TCHAR *pszFile, CPwManager *pMgr)
{
	char *pData;
	CString strTitle, strURL, strUserName, strPassword, strNotes;
	DWORD uFileSize, i = DWORD_MAX, b;
	CString str;
	CString strLastCategory = _T("General");
	DWORD dwLastGroupId = 0;
	BOOL bInNotes = FALSE;

	ASSERT(pMgr != NULL);

	pData = CPwImport::FileToMemory(pszFile, &uFileSize);
	if(pData == NULL) return FALSE;

	strTitle.Empty(); strURL.Empty(); strUserName.Empty();
	strPassword.Empty(); strNotes.Empty();

	if((uFileSize > 3) && (static_cast<unsigned char>(pData[0]) == 0xEF) &&
		(static_cast<unsigned char>(pData[1]) == 0xBB) &&
		(static_cast<unsigned char>(pData[2]) == 0xBF))
		i += 3; // Skip UTF-8 initialization characters

	bool bUnicodeMode = false;
	if((uFileSize > 2) && (static_cast<unsigned char>(pData[0]) == 0xFF) &&
		(static_cast<unsigned char>(pData[1]) == 0xFE))
	{
		i += 2; // Skip Unicode initialization characters
		bUnicodeMode = true;
	}

	while(true) // Processing the file
	{
		str.Empty();

		while(true) // Loading one line to CString
		{
			++i;
			if(i >= uFileSize) break;

			const char chReadCx = pData[i];

			if(bUnicodeMode) ++i;

			if(chReadCx == '\n') break;
			if(chReadCx != '\r') str += chReadCx;
		}

		// Add the entry
		if(((str.Left(1) == _T("[")) && (str.Right(1) == _T("]"))) ||
			(str == DEF_CW_CATEGORY) || (str == DEF_CW_CATEGORY_NEW) || (i >= uFileSize))
		{
			CString strTrimmedNotes = strNotes;
			strTrimmedNotes = strTrimmedNotes.Trim();

			if((strTitle.IsEmpty() == FALSE) || (strUserName.IsEmpty() == FALSE) ||
				(strURL.IsEmpty() == FALSE) || (strPassword.IsEmpty() == FALSE) ||
				(strTrimmedNotes.IsEmpty() == FALSE))
			{
				strTitle = strTitle.Trim();
				strURL = strURL.Trim();
				strUserName = strUserName.Trim();
				strPassword = strPassword.Trim();
				strNotes = strNotes.Trim();

				PW_TIME tNow;
				_GetCurrentPwTime(&tNow);

				PW_ENTRY pwTemplate;
				memset(&pwTemplate, 0, sizeof(PW_ENTRY));
				pwTemplate.pszAdditional = const_cast<LPTSTR>((LPCTSTR)strNotes);
				pwTemplate.pszPassword = const_cast<LPTSTR>((LPCTSTR)strPassword);
				pwTemplate.pszTitle = const_cast<LPTSTR>((LPCTSTR)strTitle);
				pwTemplate.pszURL = const_cast<LPTSTR>((LPCTSTR)strURL);
				pwTemplate.pszUserName = const_cast<LPTSTR>((LPCTSTR)strUserName);
				pwTemplate.tCreation = tNow; CPwManager::GetNeverExpireTime(&pwTemplate.tExpire);
				pwTemplate.tLastAccess = tNow; pwTemplate.tLastMod = tNow;
				pwTemplate.uGroupId = dwLastGroupId;
				pwTemplate.uImageId = _GetPreferredIcon(strTitle);
				pwTemplate.uPasswordLen = strPassword.GetLength();

				if(pwTemplate.uGroupId != 0) pMgr->AddEntry(&pwTemplate);
			}

			strTitle.Empty(); strURL.Empty(); strUserName.Empty();
			strPassword.Empty(); strNotes.Empty();
			bInNotes = FALSE;
		}

		if(i >= uFileSize) break;

		if((str.Left(1) == _T("[")) && (str.Right(1) == _T("]")))
		{
			strTitle = str;
			strTitle = strTitle.Left(strTitle.GetLength() - 1);
			strTitle = strTitle.Right(strTitle.GetLength() - 1);
			continue;
		}

		if(bInNotes == TRUE)
		{
			if(strNotes.GetLength() != 0) strNotes += _T("\r\n");

			if((str != DEF_CW_CATEGORY) && (str != DEF_CW_CATEGORY_NEW))
				strNotes += str;
		}

		const std::basic_string<TCHAR> strFxCategory = _T("Category: ");
		const std::basic_string<TCHAR> strFxFolder = _T("Folder: ");
		const bool bStartsCat = (str.Left(static_cast<int>(strFxCategory.size())) ==
			strFxCategory.c_str());
		const bool bStartsFld = (str.Left(static_cast<int>(strFxFolder.size())) ==
			strFxFolder.c_str());

		if(bStartsCat || bStartsFld)
		{
			strLastCategory = str.Right(str.GetLength() - static_cast<int>(bStartsCat ?
				strFxCategory.size() : strFxFolder.size()));
			strLastCategory = strLastCategory.Trim();

			while(true)
			{
				strLastCategory = strLastCategory.Left(strLastCategory.GetLength() - 1);
				if(strLastCategory.GetLength() == 0) break;
				if(strLastCategory.Right(1) == _T("("))
				{
					strLastCategory = strLastCategory.Left(strLastCategory.GetLength() - 2);
					break;
				}
				if(strLastCategory.GetLength() == 0) break;
			}

			if(strLastCategory.GetLength() == 0)
				strLastCategory = TRL("General");
			dwLastGroupId = pMgr->GetGroupId((LPCTSTR)strLastCategory);
			if(dwLastGroupId == DWORD_MAX)
			{
				PW_GROUP pwT;
				PW_TIME tNow;
				_GetCurrentPwTime(&tNow);
				memset(&pwT, 0, sizeof(PW_GROUP));
				pwT.pszGroupName = (TCHAR *)(LPCTSTR)strLastCategory;
				pwT.tCreation = tNow; CPwManager::GetNeverExpireTime(&pwT.tExpire);
				pwT.tLastAccess = tNow; pwT.tLastMod = tNow;
				pwT.uGroupId = 0; // 0 = create new group ID
				pwT.uImageId = _GetPreferredIcon((LPCTSTR)strLastCategory);
				pMgr->AddGroup(&pwT);
				dwLastGroupId = pMgr->GetGroupId((LPCTSTR)strLastCategory);
			}
			ASSERT(dwLastGroupId != DWORD_MAX);
		}

		if((str.Left(6) == _T("Notes:")) && (bInNotes == FALSE))
		{
			bInNotes = TRUE;
			str = str.Right(str.GetLength() - 6);
			if(str.GetLength() != 0) strNotes = str;
			continue;
		}

		b=0;
		if(str.Left(9) == _T("User ID: ")) {strUserName = str.Right(str.GetLength() - 9); b=1;}
		if(str.Left(7) == _T("Login: ")) {strUserName = str.Right(str.GetLength() - 7); b=1;}
		if(str.Left(10) == _T("Access #: ")) {strUserName = str.Right(str.GetLength() - 10); b=1;}
		if(str.Left(8) == _T("System: ")) {strUserName = str.Right(str.GetLength() - 8); b=1;}
		if(str.Left(9) == _T("Content: ")) {strUserName = str.Right(str.GetLength() - 9); b=1;}
		if(strUserName.GetLength() == 0)
		{
			if(str.Left(6) == _T("Date: ")) {strUserName = str.Right(str.GetLength() - 6); b=1;}
			if(str.Left(8) == _T("Issuer: ")) {strUserName = str.Right(str.GetLength() - 8); b=1;}
			if(str.Left(8) == _T("Number: ")) {strUserName = str.Right(str.GetLength() - 8); b=1;}
			if(str.Left(9) == _T("Network: ")) {strUserName = str.Right(str.GetLength() - 9); b=1;}
			if(str.Left(11) == _T("Ftp login: ")) {strUserName = str.Right(str.GetLength() - 11); b=1;}
		}

		if(str.Left(5) == _T("URL: ")) {strURL = str.Right(str.GetLength() - 5); b=1;}
		if(str.Left(10) == _T("Web site: ")) {strURL = str.Right(str.GetLength() - 10); b=1;}
		if(strURL.GetLength() == 0)
		{
			if(str.Left(19) == _T("Registered e-mail: ")) {strURL = str.Right(str.GetLength() - 19); b=1;}
		}

		if(str.Left(10) == _T("Password: ")) {strPassword = str.Right(str.GetLength() - 10); b=1;}
		if(strPassword.GetLength() == 0)
		{
			if(str.Left(5) == _T("PIN: ")) {strPassword = str.Right(str.GetLength() - 5); b=1;}
		}

		if((b == 0) && (bInNotes == FALSE))
		{
			if(strNotes.GetLength() != 0) strNotes += _T("\r\n");

			if((str != DEF_CW_CATEGORY) && (str != DEF_CW_CATEGORY_NEW))
				strNotes += str;
		}
	}

	SAFE_DELETE_ARRAY(pData);
	return TRUE;
}

BOOL CPwImport::ImportPVaultToDb(const TCHAR *pszFile, CPwManager *pMgr)
{
	char *pData;
	CString strTitle, strURL, strUserName, strPassword, strNotes;
	DWORD uFileSize, i, b;
	CString str;
	CString strLastCategory = _T("General");
	DWORD dwLastGroupId = 0;
	BOOL bInNotes = FALSE;

	ASSERT(pMgr != NULL);

	pData = CPwImport::FileToMemory(pszFile, &uFileSize);
	if(pData == NULL) return FALSE;

	strTitle.Empty(); strURL.Empty(); strUserName.Empty();
	strPassword.Empty(); strNotes.Empty();

	i = DWORD_MAX;

	if(uFileSize > 3)
		if((static_cast<unsigned char>(pData[0]) == 0xEF) &&
			(static_cast<unsigned char>(pData[1]) == 0xBB) &&
			(static_cast<unsigned char>(pData[2]) == 0xBF))
			i += 3; // Skip UTF-8 initialization characters

	while(true) // Processing the file
	{
		str.Empty();

		while(true) // Loading one line to CString
		{
			++i;
			if(i >= uFileSize) break;

			if(pData[i] == '\n') break;
			if(pData[i] != '\r') str += pData[i];
		}

		// Add the entry
		if((str == DEF_PV_SEPENTRY) || (i >= uFileSize) || (str.Left(12) == DEF_PV_CATEGORY))
		{
			if((strTitle.IsEmpty() == FALSE) || (strUserName.IsEmpty() == FALSE) ||
				(strURL.IsEmpty() == FALSE) || (strPassword.IsEmpty() == FALSE))
			{
				strTitle = strTitle.Trim();
				strURL = strURL.Trim();
				strUserName = strUserName.Trim();
				strPassword = strPassword.Trim();
				strNotes = strNotes.Trim();

				PW_ENTRY pwTemplate;
				PW_TIME tNow;

				_GetCurrentPwTime(&tNow);
				memset(&pwTemplate, 0, sizeof(PW_ENTRY));
				pwTemplate.pszAdditional = const_cast<LPTSTR>((LPCTSTR)strNotes);
				pwTemplate.pszPassword = const_cast<LPTSTR>((LPCTSTR)strPassword);
				pwTemplate.pszTitle = const_cast<LPTSTR>((LPCTSTR)strTitle);
				pwTemplate.pszURL = const_cast<LPTSTR>((LPCTSTR)strURL);
				pwTemplate.pszUserName = const_cast<LPTSTR>((LPCTSTR)strUserName);
				pwTemplate.tCreation = tNow; CPwManager::GetNeverExpireTime(&pwTemplate.tExpire);
				pwTemplate.tLastAccess = tNow; pwTemplate.tLastMod = tNow;
				pwTemplate.uGroupId = dwLastGroupId;
				pwTemplate.uImageId = _GetPreferredIcon(strTitle);
				pwTemplate.uPasswordLen = strPassword.GetLength();

				pMgr->AddEntry(&pwTemplate);
			}

			strTitle.Empty(); strURL.Empty(); strUserName.Empty();
			strPassword.Empty(); strNotes.Empty();
			bInNotes = FALSE;
		}

		if(i >= uFileSize) break;

		if(bInNotes == TRUE)
		{
			if(strNotes.GetLength() != 0) strNotes += _T("\r\n");
			strNotes += str.Right(str.GetLength() - 14);
		}

		if((str.Left(12) == DEF_PV_CATEGORY) && (str.Right(12) == DEF_PV_CATEGORY))
		{
			strLastCategory = str.Right(str.GetLength() - 12);
			strLastCategory = strLastCategory.Left(strLastCategory.GetLength() - 12);
			strLastCategory = strLastCategory.Trim();

			if(strLastCategory.GetLength() == 0)
				strLastCategory = TRL("General");
			dwLastGroupId = pMgr->GetGroupId((LPCTSTR)strLastCategory);
			if(dwLastGroupId == DWORD_MAX)
			{
				PW_GROUP pwT;
				PW_TIME tNow;
				_GetCurrentPwTime(&tNow);
				memset(&pwT, 0, sizeof(PW_GROUP));
				pwT.pszGroupName = (TCHAR *)(LPCTSTR)strLastCategory;
				pwT.tCreation = tNow; CPwManager::GetNeverExpireTime(&pwT.tExpire);
				pwT.tLastAccess = tNow; pwT.tLastMod = tNow;
				pwT.uGroupId = 0; // 0 = create new group ID
				pwT.uImageId = _GetPreferredIcon((LPCTSTR)strLastCategory);
				pMgr->AddGroup(&pwT);
				dwLastGroupId = pMgr->GetGroupId((LPCTSTR)strLastCategory);
			}
			ASSERT(dwLastGroupId != DWORD_MAX);
		}

		if((str.Left(9) == _T("Comments:")) && (bInNotes == FALSE))
		{
			bInNotes = TRUE;
			str = str.Right(str.GetLength() - 9);
			if(str.GetLength() != 0) strNotes = str;
			continue;
		}

		if(str.Left(9) == _T("Account: ")) {strTitle = str.Right(str.GetLength() - 9); b=1;}

		b=0;
		if(str.Left(11) == _T("User Name: ")) {strUserName = str.Right(str.GetLength() - 11); b=1;}

		if(str.Left(11) == _T("Hyperlink: ")) {strURL = str.Right(str.GetLength() - 11); b=1;}

		if(str.Left(10) == _T("Password: ")) {strPassword = str.Right(str.GetLength() - 10); b=1;}

		if((b == 0) && (bInNotes == FALSE))
		{
			if(strNotes.GetLength() != 0) strNotes += _T("\r\n");
			strNotes += str;
		}
	}

	SAFE_DELETE_ARRAY(pData);

	return TRUE;
}

BOOL CPwImport::ImportPwSafeToDb(const TCHAR *pszFile, CPwManager *pMgr)
{
	char *pData;
	unsigned long uFileSize, i;
	int nField, j;
	CString strGroup, strTitle, strUserName, strPassword, strNotes;
	DWORD dwGroupId;
	BOOL bInNotes = FALSE;
	CString str3, str4, str5, str6;

	ASSERT(pszFile != NULL); if(pszFile == NULL) return FALSE;
	ASSERT(pMgr != NULL); if(pMgr == NULL) return FALSE;

	pData = CPwImport::FileToMemory(pszFile, &uFileSize);
	if(pData == NULL) return FALSE;

	nField = 0;
	i = 0; j = 0;
	strGroup.Empty(); strTitle.Empty(); strUserName.Empty(); strPassword.Empty(); strNotes.Empty();

	if(uFileSize > 3)
		if((static_cast<unsigned char>(pData[0]) == 0xEF) &&
			(static_cast<unsigned char>(pData[1]) == 0xBB) &&
			(static_cast<unsigned char>(pData[2]) == 0xBF))
			i += 3; // Skip UTF-8 initialization characters

	while(true)
	{
		if((pData[i] == '\t') && (bInNotes == FALSE))
		{
			nField++;

			if(nField == 1)
			{
				j = strGroup.ReverseFind(_T('.'));

				// Thanks to Andrew D. Bond for the following improvement
				// Slightly enhanced by D. Reichl to detect some more URLs

				// -- Andrew D. Bond
				// Introduction: The Password Safe export format is rather flawed,
				// since it uses the '.' character as the group / title separator.
				// However, '.' is not likely to appear in the context of titles
				// that include 'com', 'org', 'edu', etc, (domain names) or even
				// 'zip' (password protected zip files)

				// Slightly-smarter import: if the '.' we just found is followed
				// by 'com', 'org', 'edu', etc, (domain names) or even
				// 'zip' (password protected zip files) then figure this is part
				// of the title and try the group / title splitting again.
				// Example:
				//  strGroup = "websites.someSite.com" // j == 17 // <- INCORRECT
				//
				// Overall, these fixes do make things much better.
				// However, they will still not be able to handle entries where
				// the title has _other_ "."'s in it.
				// For example, when the title is:
				//  subdomain.domain.com
				//  mySoftware version 8.5
				//	domain.tv (replace .tv with any domain suffix not included below)

				if((strGroup.GetLength() >= 4) && (j != -1))
					str3 = strGroup.Right(3);
				else
					str3 = _T("   ");
				if((strGroup.GetLength() >= 5) && (j != -1))
					str4 = strGroup.Right(4);
				else
					str4 = _T("    ");
				if((strGroup.GetLength() >= 6) && (j != -1))
					str5 = strGroup.Right(5);
				else
					str5 = _T("     ");
				if((strGroup.GetLength() >= 7) && (j != -1))
					str6 = strGroup.Right(6);
				else
					str6 = _T("      ");

				str3 = str3.MakeLower();
				str4 = str4.MakeLower();
				str5 = str5.MakeLower();
				str6 = str6.MakeLower();

				if((str4 == _T(".com")) || (str4 == _T(".org")) || (str4 == _T(".edu"))
					|| (str4 == _T(".net")) || (str4 == _T(".zip"))
					|| (str3 == _T(".uk")) || (str3 == _T(".de")) || (str3 == _T(".ch"))
					|| (str3 == _T(".at")) || (str3 == _T(".it")))
				{
					// Fix for double-point URLs like .co.uk
					if(str6 == _T(".co.uk")) j -= 3;

					strTitle = strGroup.Right(strGroup.GetLength() - j);
					strGroup = strGroup.Left(j);
					j = strGroup.ReverseFind(_T('.'));
				}

				if(j != -1)
				{
					strTitle = strGroup.Right(strGroup.GetLength() - j - 1) + strTitle;
					strGroup = strGroup.Left(j);
				}
				else
				{
					// No '.' was found, or it is part of a ".com" in the title.
					// Since Password Safe
					// **requires** a title but not a group name, this means
					// that a group name was not specified (a common
					// occurrence). We should assign a logical group name
					// and store the title we just identified. 
					strTitle = strGroup + strTitle;
					strGroup = TRL("Imported from Password Safe");
				}
			}
		}
		else if((pData[i] == '\"') && (bInNotes == FALSE) && (nField == 3))
		{
			bInNotes = TRUE;
		}
		else if((pData[i] == '\"') && (bInNotes == TRUE) && (nField == 3))
		{
			bInNotes = FALSE;

			if(strNotes.GetLength() != 0)
			{
				if(strNotes.GetAt(0) == _T('\"')) strNotes = strNotes.Right(strNotes.GetLength() - 1);
				if(strNotes.Right(1) == _T("\"")) strNotes = strNotes.Left(strNotes.GetLength() - 1);
			}
		}
		else if((pData[i] == '\r') && (bInNotes == FALSE))
		{
			// Ignore all \r
		}
		else if((pData[i] == '\n') && (bInNotes == FALSE))
		{
			dwGroupId = pMgr->GetGroupId((LPCTSTR)strGroup);
			if(dwGroupId == DWORD_MAX)
			{
				PW_GROUP pwT;
				PW_TIME tNow;
				_GetCurrentPwTime(&tNow);
				memset(&pwT, 0, sizeof(PW_GROUP));
				pwT.pszGroupName = (TCHAR *)(LPCTSTR)strGroup;
				pwT.tCreation = tNow; CPwManager::GetNeverExpireTime(&pwT.tExpire);
				pwT.tLastAccess = tNow; pwT.tLastMod = tNow;
				pwT.uGroupId = 0; // 0 = create new group ID
				pwT.uImageId = _GetPreferredIcon((LPCTSTR)strGroup);
				pMgr->AddGroup(&pwT);
				dwGroupId = pMgr->GetGroupId((LPCTSTR)strGroup);
			}
			ASSERT(dwGroupId != DWORD_MAX);

			PW_ENTRY pwTemplate;
			PW_TIME tNow;

			memset(&pwTemplate, 0, sizeof(PW_ENTRY));
			_GetCurrentPwTime(&tNow);
			pwTemplate.pszAdditional = const_cast<LPTSTR>((LPCTSTR)strNotes);
			pwTemplate.pszPassword = const_cast<LPTSTR>((LPCTSTR)strPassword);
			pwTemplate.pszTitle = const_cast<LPTSTR>((LPCTSTR)strTitle);
			pwTemplate.pszURL = g_pNullString;
			pwTemplate.pszUserName = const_cast<LPTSTR>((LPCTSTR)strUserName);
			pwTemplate.tCreation = tNow; CPwManager::GetNeverExpireTime(&pwTemplate.tExpire);
			pwTemplate.tLastAccess = tNow; pwTemplate.tLastMod = tNow;
			pwTemplate.uImageId = _GetPreferredIcon((LPCTSTR)strTitle);
			pwTemplate.uPasswordLen = strPassword.GetLength();
			pwTemplate.uGroupId = dwGroupId;
			// UUID is zero -> create new UUID

			pMgr->AddEntry(&pwTemplate);

			strGroup.Empty(); strTitle.Empty(); strUserName.Empty();
			strPassword.Empty(); strNotes.Empty();
			nField = 0;
		}
		else
		{
			if(nField == 0) strGroup += pData[i];
			else if(nField == 1) strUserName += pData[i];
			else if(nField == 2) strPassword += pData[i];
			else if(nField == 3) strNotes += pData[i];
		}

		i++;
		if(i >= uFileSize) break;
	}

	EraseCString(&strGroup); EraseCString(&strTitle); EraseCString(&strUserName);
	EraseCString(&strPassword); EraseCString(&strNotes);

	SAFE_DELETE_ARRAY(pData);
	return TRUE;
}

void CPwImport::_AddStringStreamToDb(const char *pStream, unsigned long uStreamSize, BOOL bUTF8)
{
	DWORD s;
	char *pTitle = NULL, *pUserName = NULL, *pPassword = NULL, *pURL = NULL, *pNotes = NULL;
	char *p = (char *)pStream;
	char *pEnd = (char *)pStream + uStreamSize;
	TCHAR *tszTitle = NULL, *tszUserName = NULL, *tszPassword = NULL, *tszURL = NULL, *tszNotes = NULL;

	ASSERT(pStream != NULL);

	while(true)
	{
		if(p >= pEnd) break;
		pTitle = p;
		s = szlen(p);
		p += s + 1;

		if(p >= pEnd) break;
		pUserName = p;
		s = szlen(p);
		p += s + 1;

		if(p >= pEnd) break;
		pPassword = p;
		s = szlen(p);
		p += s + 1;

		if(p >= pEnd) break;
		pURL = p;
		s = szlen(p);
		p += s + 1;

		if(p >= pEnd) break;
		pNotes = p;
		s = szlen(p);
		p += s + 1;

		if((strcmp(pTitle, "Account") != 0) && (strcmp(pPassword, "Password") != 0))
		{
			PW_ENTRY pwTemplate;
			PW_TIME tNow;

			if(bUTF8 == TRUE)
			{
				tszTitle = _UTF8ToString((UTF8_BYTE *)pTitle);
				tszUserName = _UTF8ToString((UTF8_BYTE *)pUserName);
				tszPassword = _UTF8ToString((UTF8_BYTE *)pPassword);
				tszURL = _UTF8ToString((UTF8_BYTE *)pURL);
				tszNotes = _UTF8ToString((UTF8_BYTE *)pNotes);
			}
			else
			{
				tszTitle = (TCHAR *)pTitle;
				tszUserName = (TCHAR *)pUserName;
				tszPassword = (TCHAR *)pPassword;
				tszURL = (TCHAR *)pURL;
				tszNotes = (TCHAR *)pNotes;
			}

			memset(&pwTemplate, 0, sizeof(PW_ENTRY));
			_GetCurrentPwTime(&tNow);
			pwTemplate.pszAdditional = tszNotes;
			pwTemplate.pszPassword = tszPassword;
			pwTemplate.pszTitle = tszTitle;
			pwTemplate.pszURL = tszURL;
			pwTemplate.pszUserName = tszUserName;
			pwTemplate.tCreation = tNow; CPwManager::GetNeverExpireTime(&pwTemplate.tExpire);
			pwTemplate.tLastAccess = tNow; pwTemplate.tLastMod = tNow;
			pwTemplate.uGroupId = m_dwLastGroupId;
			pwTemplate.uImageId = _GetPreferredIcon(tszTitle);
			pwTemplate.uPasswordLen = static_cast<DWORD>(_tcslen(tszPassword));
			// UUID is zero -> create new UUID

			m_pLastMgr->AddEntry(&pwTemplate);

			if(bUTF8 == TRUE)
			{
				SAFE_DELETE_ARRAY(tszTitle);
				SAFE_DELETE_ARRAY(tszUserName);
				SAFE_DELETE_ARRAY(tszPassword);
				SAFE_DELETE_ARRAY(tszURL);
				SAFE_DELETE_ARRAY(tszNotes);
			}
		}
	}
}

unsigned long CPwImport::_GetPreferredIcon(LPCTSTR pszGroup)
{
	CString str = pszGroup;

	if(str.Find(_T("Windows")) != -1) return 38;
	if(str.Find(_T("System")) != -1) return 38;

	if(str.Find(_T("Network")) != -1) return 3;
	if(str.Find(_T("Connection")) != -1) return 3;
	if(str.Find(_T("VPN")) != -1) return 3;
	if(str.Find(_T("LAN")) != -1) return 3;

	if(str.Find(_T("Internet")) != -1) return 1;
	if(str.Find(_T("internet")) != -1) return 1;
	if(str.Find(_T("Web")) != -1) return 1;
	if(str.Find(_T("www")) != -1) return 1;
	if(str.Find(_T("WWW")) != -1) return 1;
	if(str.Find(_T("http")) != -1) return 1;
	if(str.Find(_T("HTTP")) != -1) return 1;
	if(str.Find(_T("ftp")) != -1) return 1;
	if(str.Find(_T("FTP")) != -1) return 1;

	if(str.Find(_T("eMail")) != -1) return 19;
	if(str.Find(_T("mail")) != -1) return 19;
	if(str.Find(_T("Post")) != -1) return 19;
	if(str.Find(_T("@")) != -1) return 19;

	if(str.Find(_T("Homebanking")) != -1) return 37;
	if(str.Find(_T("Banking")) != -1) return 37;
	if(str.Find(_T("Archive")) != -1) return 37;
	if(str.Find(_T("Collection")) != -1) return 37;
	if(str.Find(_T("Cash")) != -1) return 37;

	if(str.Find(_T("Explorer")) != -1) return 9;
	if(str.Find(_T("Netscape")) != -1) return 9;
	if(str.Find(_T("Mozilla")) != -1) return 9;
	if(str.Find(_T("Opera")) != -1) return 9;

	return 0;
}
