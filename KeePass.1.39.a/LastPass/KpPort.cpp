// KeePass Database Port


#include "stdafx.h"
#include "KpPort.h"
#include "KpIter.h"
#include "KpRecord.h"
#include "LpRecord.h"


// Store by updating if the URL and two other features are the same
// Slightly different features are used for passwords and not passwords (i.e. additionals)

void KpPort::store(LpRecord& lpRcd) {
KpIter iter(kpdb);
KpRecord* kpRcd;
KpRecord  rcd;
bool      namesCmp;
bool      pwsCmp;
bool      titlesCmp;

if (!lpRcd.notes.isEmpty()) {
int x = 1;
}

  rcd = lpRcd;

  for (kpRcd = iter(); kpRcd; kpRcd = iter++) {

    if (lpRcd.url != kpRcd->pszURL) continue;

    namesCmp = compare(lpRcd.userName, kpRcd->pszUserName);
    pwsCmp   = comparePW(lpRcd, kpRcd);

    if (namesCmp && pwsCmp) {update(iter.curIndx(), rcd);   return;}

    titlesCmp = compare(lpRcd.title, kpRcd->pszTitle);

    if (!lpRcd.isNote()) {  // Is Password

      if (titlesCmp && (namesCmp || pwsCmp)) {update(iter.curIndx(), rcd);   return;}

      break;
      }

    // Is Note

    if (titlesCmp && (compare(lpRcd.additional, kpRcd->pszAdditional)) || pwsCmp)
                                                                 {update(iter.curIndx(), rcd);   return;}
    break;
    }

  kpdb.AddEntry(&rcd);
  }


bool KpPort::comparePW(LpRecord& lpRcd, KpRecord* kpRcd) {
String pw;

  if (lpRcd.password.isEmpty()) return false;

  kpdb.UnlockEntryPassword(kpRcd);
    pw = kpRcd->pszPassword;
  kpdb.LockEntryPassword(kpRcd);    return compare(lpRcd.password, pw);
  }




void KpPort::update(int index, KpRecord& kpRcd) {
BOOL x;

  kpdb.BackupEntry(kpdb.GetEntry(index), &x);
  kpdb.DeleteEntry(index);
  kpdb.AddEntry(&kpRcd);
  }

