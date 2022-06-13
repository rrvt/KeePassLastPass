// Iterator over the KeePass Data


#include "stdafx.h"
#include "KpIter.h"
#include "groups.h"
#include "KpRecord.h"


KpRecord* KpIter::operator() (Dir rev) {
  nEntry = db.GetEntryCount();   bkupID = groups.bkupID();   i = rev ? nEntry-1 : 0;   return next(rev);
  }


KpRecord* KpIter::operator++ (int) {i++; return next();}
KpRecord* KpIter::operator-- (int) {i--; return next();}


KpRecord* KpIter::next(Dir rev) {
int       delta = rev ? -1 : 1;
KpRecord* rcd;

  for (; -1 < i && i < nEntry; i += delta) {

    rcd = (KpRecord*) db.GetEntry(i);   if (!rcd) continue;

    if (rcd->uGroupId == bkupID) continue;

    return rcd;
    }

  return 0;
  }


