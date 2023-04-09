// Duplicates


#include "stdafx.h"
#include "Duplicates.h"
#include "KpIter.h"
#include "LpRecord.h"
#include "Picker.h"


bool Duplicates::operator() () {
KpIter    iter(kpdb);
KpRecord* rcd1;
KpRecord* rcd2;
Picker    dlg;
LpRecord  lpRcd1;
int       index2;
LpRecord  lpRcd2;
String    kpPw1;
String    kpPw2;
String    noteType1;
String    noteType2;
int       dlgRslt;
bool      rslt = false;

  for (rcd1 = iter(); rcd1; rcd1 = iter++) {

    kpdb.UnlockEntryPassword(rcd1);
      kpPw1 = rcd1->pszPassword;    lpRcd1 = *rcd1;
    kpdb.LockEntryPassword(rcd1);

    dlg.group1      = lpRcd1.grpName;
    dlg.url1        = lpRcd1.url;
    dlg.title1      = lpRcd1.title;
    dlg.userName1   = lpRcd1.userName;
    dlg.password1   = kpPw1;
    dlg.notes1      = prep(lpRcd1.notes, noteType1);
    dlg.timeStamp1  = lpRcd1.timeStamp;

if (lpRcd1.title.find(_T("verdrive")) >= 0 || lpRcd1.title.find(_T("verDrive")) >= 0) {
int x = 1;
}


    KpIter iter2 = iter;

    for (index2 = srchdupRcd(iter2, *rcd1); index2 >= 0; index2 = srchdupRcd(iter2, *rcd1)) {

      rcd2 = (KpRecord*) kpdb.GetEntry(index2);

      kpdb.UnlockEntryPassword(rcd2);
        kpPw2 = rcd2->pszPassword;   lpRcd2 = *rcd2;
      kpdb.LockEntryPassword(rcd2);

      dlg.group2      = lpRcd2.grpName;
      dlg.url2        = lpRcd2.url;
      dlg.title2      = lpRcd2.title;
      dlg.userName2   = lpRcd2.userName;
      dlg.password2   = kpPw2;
      dlg.notes2      = prep(lpRcd2.notes, noteType2);
      dlg.timeStamp2  = lpRcd2.timeStamp;

      if ((!noteType1.isEmpty() || !noteType2.isEmpty()) && noteType1 != noteType2) continue;

      dlg.cmpGrp = isEqual(lpRcd1.grpName,  lpRcd2.grpName);
      dlg.cmpURL = isEqual(lpRcd1.url,      lpRcd2.url);
      dlg.cmpTtl = isEqual(lpRcd1.title,    lpRcd2.title);
      dlg.cmpUsr = isEqual(lpRcd1.userName, lpRcd2.userName);
      dlg.cmpPsw = isEqual(kpPw1, kpPw2);
      dlg.cmpNts = isEqual(lpRcd1.notes,    lpRcd2.notes);

      dlgRslt = dlg.DoModal();

      if (dlgRslt == IDC_NoDelete) continue;

      if (dlgRslt == IDC_DelRcd1) {iter.remove();  rslt = true;   break;}

      if (dlgRslt == IDC_DelRcd2) {kpdb.DeleteEntry(index2);  iter.adjust();  rslt = true;  continue;}

      if (dlgRslt == IDC_Finish)   return rslt;

      if (dlgRslt == IDCANCEL)     return 0;
      }
    }

  return rslt;
  }


// Look for anything that might be a match, i.e. let the user decide

int Duplicates::srchdupRcd(KpIter& iter, KpRecord& rcd) {
KpRecord* kpRcd;

  for (kpRcd = iter++; kpRcd; kpRcd = iter++) {



String title = kpRcd->pszTitle;
if (title.find(_T("verdrive")) >=0 || title.find(_T("verDrive")) >= 0) {
int x = 1;
}

    if (kpRcd == &rcd) continue;

    if (_tcslen(kpRcd->pszTitle) && !_tcscmp(kpRcd->pszTitle, rcd.pszTitle)) return iter.curIndx();

    if (!kpRcd->uPasswordLen     || kpRcd->uPasswordLen != rcd.uPasswordLen) continue;
    if (!memcmp(kpRcd->pszPassword, rcd.pszPassword, kpRcd->uPasswordLen))   {
      if (_tcslen(kpRcd->pszUserName) && !_tcscmp(kpRcd->pszUserName, rcd.pszUserName))
                                                                                    return iter.curIndx();
      }
    }

  return -1;
  }


String& Duplicates::prep(String& src, String& noteType) {
static String stg;
Tchar         tch;
Tchar         tch1;
int           i;
int           n;
bool          isDone;

  stg.clear();   src.trim();  noteType.clear();

  for (i = 0, n = src.length(), isDone = false; i < n; i++) {
    tch = src[i];   tch1 = i+1 < n ? src[i+1] : 0;

    if (!isDone && tch == _T('\n')) {noteType = stg;  isDone = true;}

    if (tch == _T('\n')) stg += _T('\r');

    stg += tch;

    if (tch == _T('\r') && tch1 == _T('\n')) {stg += tch1; i++; continue;}

    if (tch == _T(',') && tch1 != _T(',')) stg += _T("\r\n");
    }

  if (noteType.find(_T("NoteType:")) < 0) noteType.clear();

  return stg;
  }


TCchar* Duplicates::isEqual(String& left, String& right) {
  if (left.isEmpty() && right.isEmpty()) return _T("");

  return left == right ? _T("=") : _T("!=");
  }



#if 0
String     title     = entry.pszTitle;
String     url       = entry.url();
String     groupName = groups.getName(entry.uGroupId);
String     userName  = entry.pszUserName;
String     pswd      = entry.pszPassword;
String     addtl     = entry.pszAdditional;
#endif


#if 0
    String   dtitle     = dup->pszTitle;
    String   durl       = dup->url();
    String   dgroupName = groups.getName(dup->uGroupId);
    String   duserName  = dup->pszUserName;
    String   dpswd      = dup->pszPassword;
    String   dAddtl     = dup->pszAdditional;
#endif

