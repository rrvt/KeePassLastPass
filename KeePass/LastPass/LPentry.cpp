// Password LPentry data


#include "stdafx.h"
#include "LPentry.h"
#include "Database.h"
#include "Groups.h"
#include <random>




TCchar* NotesURL    = _T("http://sn");             // flag in url for a note

TCchar* FavOpen     = _T("<Fav~>");
TCchar* FavClose    = _T("</Fav~>\n");
TCchar* ExtraOpen   = _T("<Extra~>\n");
TCchar* ExtraClose  = _T("</Extra~>\n");
TCchar* TimeOpen    = _T("<Time~>");
TCchar* TimeClose   = _T("</Time~>\n");
TCchar* FFLineOpen  = _T("<FFLine~>");
TCchar* FFLineClose = _T("</FFLine~>\n");


LPentry::~LPentry() {
  clr(grpName);  clr(url); clr(title); clr(name); clr(password); clr(notes); clr(fav); clr(additional);
  clr(errorField);
  }





void LPentry::clear() {
  grpName.clear(); url.clear(); title.clear(); name.clear(); password.clear(); notes.clear(); fav.clear();
  additional.clear();  errorField.clear(); lastCh = 0;
  }


bool LPentry::readRecord(FileIn& fi) {
int    field;
char   ch;
String stg;

  clear();

  for (field = 0; (ch = fi.nextElement(stg)) != 0; field++) {

    if (ch != ',' && ch != '\n') {lastCh = ch; errorField = stg; return false;}

    switch (field) {
      case 0: url      = stg; break;
      case 1: name     = stg; break;
      case 2: password = stg; break;
      case 3: notes    = stg; break;
      case 4: title    = stg; break;
      case 5: grpName  = stg; break;
      case 6: fav      = stg; break;
      }

    if (ch == '\n') {

      if (field  < 5) {lastCh = -1; errorField.format("No Fields = %i", field); return false;}

      additional  = FavOpen;     additional += fav;     additional += FavClose;
      additional += ExtraOpen;   additional += notes;   additional += ExtraClose;

      aNote = url == NotesURL;

      return true;
      }
    }

  return false;
  }


LPentry& LPentry::operator= (DBentry& dbe) {

  url   = dbe.pszURL;
  title = dbe.pszTitle;
  name  = dbe.pszUserName;

  kpdb->UnlockEntryPassword(&dbe);   password = dbe.pszPassword;   kpdb->LockEntryPassword(&dbe);

  set(dbe.pszAdditional, FavOpen,   FavClose,    fav);
  set(dbe.pszAdditional, ExtraOpen, ExtraClose,  notes);
  set(dbe.pszAdditional, TimeOpen,  TimeClose,   timeStamp);

  grpName  = groups.getName(dbe.uGroupId);   return *this;
  }



void LPentry::set(TCHAR* src, TCchar* const opn, TCchar* const cls, String& s) {
String t      = src;
int        opnPos = t.find(opn);
int        endPos = t.find(cls);
int        count;

  if (opnPos < 0 || endPos < 0) {s.clear(); return;}

  opnPos += _tcslen(opn); count = endPos - opnPos;

  if (count <= 0) {s.clear(); return;}

  s = t.substr(opnPos, count);
  }



void LPentry::clr(String& s) {
uniform_int_distribution<int> rand(32, 127);
random_device                 rd;
mt19937                       gen(rd());
int n = s.length();
int i;
TCHAR ch = s[0] - (TCHAR) n;

  for (i = 0; i < n; i++) {
    ch = (char) rand(gen);
    s[i] = ch++;
    }
  }





void LPentry::writePW(FileOut& fo) {
  fo.write(url);     fo.put(_T(','));
  fo.write(name);    fo.put(_T(','));
  fo.write(password);fo.put(_T(','));
  fo.write(notes);   fo.put(_T(','));
  fo.write(title);   fo.put(_T(','));
  fo.write(grpName); fo.put(_T(','));
  fo.write(fav);     fo.put(_T('\n'));
  }



#if 0
void exportStg(String& s,  FileOut& fo) {
bool addQts = s.find(',')  != String::npos ||
              s.find('"')  != String::npos ||
              s.find('\n') != String::npos;

  if (addQts) fo.quote();
  fo.write(s);
  if (addQts) fo.quote();
  }
#endif
