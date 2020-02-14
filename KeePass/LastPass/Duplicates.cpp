// Duplicates


#include "stdafx.h"
#include "Duplicates.h"


void Duplicates::remove() {
DBIterator iter(&kpdb);
DBentryP*  dbeP;

  iter.passwordsOnly();  notes = false;

  for (dbeP = iter.startLoop(); dbeP; dbeP = iter.nextEntry()) {
    DBentry* dbe = dbeP->dbe;

    srchDestroy(*dbe);
    }

  iter.notesOnly();   notes = true;

  for (dbeP = iter.startLoop(); dbeP; dbeP = iter.nextEntry()) {
    DBentry* dbe = dbeP->dbe;

    srchDestroy(*dbe);
    }
  }



void Duplicates::srchDestroy(DBentry& entry) {
String     url = entry.url();
DBIterator iter(&kpdb);
DBentryP*  dupEP;
int        entryAddlLng;
int        dupEaddlLng;

  for (dupEP = iter.startLoop(url); dupEP; dupEP = iter.nextURL()) {
    DBentry* dupE  = dupEP->dbe;
    int      index = dupEP->index;

    if (*dupE == entry) continue;

    if (notes) {if (*dupE >= entry) continue;}  // true if titles, psswd length or names are not the same
    else        if (*dupE != entry) continue;   // true if names or passwords are not the same

    entryAddlLng = strlen(entry.pszAdditional);
    dupEaddlLng  = strlen(dupE->pszAdditional);

    if (dupE->uGroupId == entry.uGroupId) {

      if (entryAddlLng == dupEaddlLng) {
        PWtime* t = (PWtime*) &entry.tCreation;
        PWtime* u = (PWtime*) &dupE->tCreation;

        if (*t >= *u) {db.remove(index); iter.backup(1);}
        }

      else if (entryAddlLng > dupEaddlLng) {db.remove(index);  iter.backup(1);}
      }

    else {
      if (entryAddlLng == dupEaddlLng) {
        PWtime* t = (PWtime*) &entry.tCreation;
        PWtime* u = (PWtime*) &dupE->tCreation;
        if (*t >= *u) {db.remove(index);  iter.backup(1);}
        }

      else if (entryAddlLng > dupEaddlLng) {db.remove(index);  iter.backup(1);}
      }
    }
  }

