// Iterator over the KeePass Data


#include "pch.h"
#include "KpIter.h"
#include "groups.h"
#include "KeePassSDK.h"
#include "KpRecord.h"


KpRecord* KpIter::operator() (Dir rev) {
  nEntry = db.GetEntryCount();   bkupID = groups.bkupID();   i = rev ? nEntry-1 : 0;

  return next(rev);
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


void KpIter::remove(Dir dir) {db.DeleteEntry(i); i += dir == Fwd ? -1 : 1;   adjust();}


int  KpIter::adjust() {return nEntry = db.GetEntryCount();}


