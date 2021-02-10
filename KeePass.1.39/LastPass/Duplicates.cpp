// Duplicates


#include "stdafx.h"
#include "Duplicates.h"
#include "Groups.h"


void Duplicates::operator() () {
DBIter iter(&kpdb);
DBentryP*  dbeP;

  iter.passwordsOnly();   for (dbeP = iter(); dbeP; dbeP = iter++) srchDestroy(iter, *dbeP->dbe);

  iter.notesOnly();       for (dbeP = iter(); dbeP; dbeP = iter++) srchDestroy(iter, *dbeP->dbe);
  }



void Duplicates::srchDestroy(DBIter iter, DBentry& entry) {
DBentryP*  dupP;

  iter.switchToUrl(entry.url());

  for (dupP = iter++; dupP; dupP = iter++) {
    DBentry* dup = dupP->dbe;   if (dup == &entry || *dup != entry) continue;

    iter.remove();
    }
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

