// Access to KeePass Database


#include "stdafx.h"
#include "Database.h"
#include "Groups.h"
#include "LastPass.h"




// find kpdb entry where url, name and password the same as last pass entry

bool Database::findNmPswdMatch(LPentry& lpe, int& index) {
DBIterator iter(kpdb);
DBentryP*  dbep;

  if (lpe.name.isEmpty() || lpe.password.isEmpty()) return false;

  for (dbep = iter.startLoop(lpe.url); dbep; dbep = iter.nextURL()) {
    DBentry* dbe = dbep->dbe;

    if (*dbe != lpe) continue;
    index = dbep->index; return true;
    }

  return false;
  }


// find kpdb entry where url, titles and name the same as last pass entry

bool Database::findNmTtlMatch(LPentry& lpe, int& index) {
DBIterator iter(kpdb);
DBentryP*  dbep;

  if (lpe.title.isEmpty() || lpe.name.isEmpty()) return false;

  for (dbep = iter.startLoop(lpe.url); dbep; dbep = iter.nextURL()) {
    DBentry* dbe = dbep->dbe;

    if (*dbe <= lpe) continue;

    index = dbep->index; return true;
    }

  return false;
  }


// find kpdb entry where url, titles and passwords the same as last pass entry

bool Database::findTtlPswdMatch(LPentry& lpe, int& index) {
DBIterator iter(kpdb);
DBentryP*  dbep;

  if (lpe.title.isEmpty() || lpe.password.isEmpty()) return false;

  for (dbep = iter.startLoop(lpe.url); dbep; dbep = iter.nextURL()) {
    DBentry* dbe = dbep->dbe;

    if (*dbe >= lpe) continue;                  // Continue when title and password are not the same

    index = dbep->index; return true;
    }

  return false;
  }


// find kpdb entry where url, titles and notess the same as last pass entry

bool Database::findTitleNotesMatch(LPentry& lpe, int& index) {
DBIterator iter(kpdb);
DBentryP*  dbep;

  if (lpe.title.isEmpty() || lpe.notes.isEmpty()) return false;

  for (dbep = iter.startLoop(lpe.url); dbep; dbep = iter.nextURL()) {
    DBentry* dbe = dbep->dbe;

    if (*dbe < lpe) continue;                  // Continue when titles or notes are not the same

    index = dbep->index; return true;
    }

  return false;
  }


DBentryP* DBIterator::startLoop() {i = -1;   nEntry = kpdb->GetEntryCount(); return nextEntry();}


DBentryP* DBIterator::nextEntry() {
DBentry* dbe;

  while (++i < nEntry) {

    dbe = (DBentry*) kpdb->GetEntry(i);       if (!dbe) break;

    if (dbe->uGroupId == groups.bkupID()) continue;

    if (noteOnly ^ !strcmp(dbe->pszURL, NotesURL)) continue;

    dbep.dbe = dbe; dbep.index = i; return &dbep;
    }

  return 0;
  }


DBentryP* DBIterator::startLoop(TCchar* url) {

  this->url = url;    noteOnly = this->url == NotesURL;

  i = -1;   nEntry = kpdb->GetEntryCount(); return nextURL();
  }


DBentryP* DBIterator::nextURL() {
DBentry* dbe;

  while (++i < nEntry) {

    i = kpdb->Find(url, true, PWMF_URL, i);   if (i > nEntry) break;

    dbe = (DBentry*) kpdb->GetEntry(i);       if (!dbe) break;

    if (dbe->uGroupId == groups.bkupID()) continue;

    if (noteOnly ^ !strcmp(dbe->pszURL, NotesURL)) continue;

    dbep.dbe = dbe; dbep.index = i; return &dbep;
    }

  return 0;
  }


DBentry& DBentry::operator= (LPentry& lpe) {

  if (lpe.grpName.isEmpty()) lpe.grpName = lpe.isNote() ? _T("Notes") : _T("General");

  uGroupId      = groups.getID(lpe.grpName);
  pszTitle      = const_cast<LPTSTR>(lpe.title.str());
  pszURL        = const_cast<LPTSTR>(lpe.url.str());
  pszUserName   = const_cast<LPTSTR>(lpe.name.str());
  pszPassword   = const_cast<LPTSTR>(lpe.password.str());
  uPasswordLen  = static_cast<DWORD>(_tcslen(pszPassword));
  pszAdditional = const_cast<LPTSTR>(lpe.additional.str());

  keePss()->GetCurrentPwTime(&tCreation);  tLastMod = tLastAccess = tCreation;
  keePss()->GetNeverExpireTime(&tExpire);

  return *this;
  }


#if 0
void DBentry::convert(LPentry& lpe) {
#if 0
  lpe.additional  = FavOpen;
  lpe.additional += lpe.fav;
  lpe.additional += FavClose;

  lpe.additional += ExtraOpen;
  lpe.additional += lpe.notes;
  lpe.additional += ExtraClose;

  lpe.additional += TimeOpen;
  lpe.additional += lpe.timeStamp;
  lpe.additional += TimeClose;
#endif

  uGroupId      = groups.getID(lpe.grpName);
  pszTitle      = const_cast<LPTSTR>(lpe.title.str());
  pszURL        = const_cast<LPTSTR>(lpe.url.str());
  pszUserName   = const_cast<LPTSTR>(lpe.name.str());
  pszPassword   = const_cast<LPTSTR>(lpe.password.str());
  uPasswordLen  = static_cast<DWORD>(_tcslen(pszPassword));
  pszAdditional = const_cast<LPTSTR>(lpe.additional.str());

  keePss()->GetCurrentPwTime(&tCreation);  tLastMod = tLastAccess = tCreation;
  keePss()->GetNeverExpireTime(&tExpire);
  }
#endif



bool DBentry::isBackupGrp() {return uGroupId == groups.bkupID();}



String DBentry::getFFLine() {
String s     = pszAdditional;
int    first = s.find(FFLineOpen) + _tcslen(FFLineOpen);
int    last  = s.find(FFLineClose);

  return s.substr(first, last-first);
  }


void DBentry::update(int index) {
IKpDatabase* kpdb = database();
BOOL         x;

  kpdb->BackupEntry(kpdb->GetEntry(index), &x);
  kpdb->DeleteEntry(index);
  kpdb->AddEntry(this);
  }


void DBentry::add() {database()->AddEntry(this);}


bool DBentry::operator==(DBentry& dbe) {
BYTE* p = uuid;
BYTE* q = dbe.uuid;
int   i;
int   n = sizeof(uuid);

  for (i = 0; i < n; i++) if (p[i] != q[i]) return false;

  return true;
  }


// compare important fields of record, returns true if any field not equal:
// true if names or passwords are not the same
// Use this to further refine a url search (i.e. assumes url's match)

bool DBentry::operator!=(DBentry& dbe) {
  if (uPasswordLen    !=    dbe.uPasswordLen)               return true;
  if (strcmp(pszUserName,   dbe.pszUserName))               return true;
  if (memcmp(pszPassword,   dbe.pszPassword, uPasswordLen)) return true;

  return false;
  }


// true if titles, password length or names are not the same

bool DBentry::operator>=(DBentry& dbe) {

  if (strcmp(pszTitle, dbe.pszTitle))         return true;
  if (uPasswordLen    !=    dbe.uPasswordLen) return true;
  if (strcmp(pszUserName,   dbe.pszUserName)) return true;

  return false;
  }


// true if names or passwords are not the same

bool DBentry::operator!=(LPentry& lpe) {
String pw;

  if (lpe.name != pszUserName) return true;

  database()->UnlockEntryPassword(this);
    pw = pszPassword;
  database()->LockEntryPassword(this);

  return pw != lpe.password;
  }


// True if titles or notes are not the same

bool DBentry::operator<  (LPentry& lpe) {
String notes;

  if (strcmp(lpe.title, pszTitle)) return true;

  lpe.set(pszAdditional, ExtraOpen, ExtraClose,  notes);

  if (lpe.notes != notes) return true;

  return false;
  }



// true if titles or passwords are not the same

bool DBentry::operator>=(LPentry& lpe) {
String pw;

  if (lpe.title != pszTitle) return true;
  database()->UnlockEntryPassword(this);
    pw = pszPassword;
  database()->LockEntryPassword(this);

  return pw != lpe.password;
  }



bool PWtime::operator>=(PWtime& t) {
  if (shYear   > t.shYear)   return true;
  if (shYear   < t.shYear)   return false;
  if (btMonth  > t.btMonth)  return true;
  if (btMonth  < t.btMonth)  return false;
  if (btDay    > t.btDay)    return true;
  if (btDay    < t.btDay)    return false;
  if (btHour   > t.btHour)   return true;
  if (btHour   < t.btHour)   return false;
  if (btMinute > t.btMinute) return true;
  if (btMinute < t.btMinute) return false;
  if (btSecond > t.btSecond) return true;
  if (btSecond < t.btSecond) return false;

  return true;
  }




#if 0
bool DBentry::formFillOnly() {
  if (uGroupId == groups.bkupID())      return false;
//  if (!strcmp(pszURL, FFurl))    return true;
  return false;
  }
#endif
#if 0
bool Database::find(Cchar* url, DBentry*& dbe) {

  this->url = url;   i = DWORD(-1);   nEntry = kpdb->GetEntryCount();

  return findNext(dbe);
  }


bool Database::findNext(DBentry*& dbe) {

  while (++i < nEntry) {

    i = kpdb->Find(url, true, PWMF_URL, i);   if (i > nEntry) break;

    dbe = (DBentry*) kpdb->GetEntry(i);       if (!dbe) break;

    if (dbe->uGroupId == groups.bkupID()) continue;
    if (allowNotes    ^ !strcmp(dbe->pszURL, NotesURL)) continue;
//    if (filterFormFill ^ !strcmp(dbe->pszURL, FFurl))    continue;

    dbe->index = i; return true;
    }

  dbe = 0; return false;
  }
#endif
#if 0
// Copy all relevant fields from a database entry to a VsblEntry.

void Database::copyData(DBentry* dbe, LPentry& lpe) {
//void LPentry::initialize(IKpDatabase* kpdb, PW_ENTRY* p) {

  lpe.url      = dbe->pszURL;
  lpe.title    = dbe->pszTitle;
  lpe.name     = dbe->pszUserName;
  kpdb->UnlockEntryPassword(dbe);
    lpe.password = dbe->pszPassword;
  kpdb->LockEntryPassword(dbe);
  lpe.set(dbe->pszAdditional, FavOpen,   FavClose,    lpe.fav);
  lpe.set(dbe->pszAdditional, ExtraOpen, ExtraClose,  lpe.notes);
  lpe.set(dbe->pszAdditional, TimeOpen,  TimeClose,   lpe.timeStamp);
  lpe.grpName  = groups.getName(dbe->uGroupId);
  }
#endif


//bool DBentry::isNote() {return !strcmp(pszURL, NotesURL);}


//void Database::remove(int index) {kpdb->DeleteEntry(index);}

