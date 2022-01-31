// LastPass Port


#include "stdafx.h"
#include "LpPort.h"
#include "GetPathDlg.h"


static TCchar* FormFill = _T("http://ff");


bool LpPort::openImport(String& path) {
PathDlgDsc dsc;

  dsc = PathDlgDsc{_T("LastPass Export File"), _T(""), _T("csv"), _T("*.csv")};
  i = 0;

  return lex.open(dsc, path);
  }


LpRecord* LpPort::getNext() {return readRecord(lex, lpRcd) ? &lpRcd : 0;}


bool LpPort::readRecord(CSVLexF& lex, LpRecord& rcd) {
int        field;
CSVtokCode code;
String     stg;

  rcd.clear();

  for (field = 0, code = lex.get_token(); code != EOFToken; field++, code = lex.get_token()) {

    CSVtok& tok  = lex.token;     if (tok.code == CommaToken) {lex.accept_token(); continue;}
    CSVtok& tok1 = lex.token1;

    switch (field) {
      case 0: rcd.url      = tok.name; break;
      case 1: rcd.userName = tok.name; break;
      case 2: rcd.password = tok.name; break;
      case 3: rcd.totp     = tok.name; break;
      case 4: rcd.notes    = tok.name; break;
      case 5: rcd.title    = tok.name; break;
      case 6: rcd.grpName  = tok.name; break;
      case 7: rcd.fav      = tok.name; break;
      }

    if (tok1.code >= EolToken) {

      rcd.setAdditional();

      if (field  < 5) {errorField.format("No Fields = %i", field); return false;}

      rcd.setAnote();

      lex.accept_two_tokens();   i++;   return true;
      }
    lex.accept_two_tokens();
    }

  return false;
  }


bool LpPort::openExport() {
PathDlgDsc dsc;

  dsc = PathDlgDsc{_T("LastPass Import File"), _T(""), _T("csv"), _T("*.csv")};

  if (!csv.open(dsc)) return false;

  csv << _T("url")      << _T(',');
  csv << _T("username") << _T(',');
  csv << _T("password") << _T(',');
  csv << _T("totp")     << _T(',');
  csv << _T("extra")    << _T(',');
  csv << _T("name")     << _T(',');
  csv << _T("grouping") << _T(',');
  csv << _T("fav")      << vCrlf;

  return true;
  }


void LpPort::store(KpRecord& kpRcd) {
LpRecord lpRcd;


  lpRcd = kpRcd;

  if (lpRcd.url == FormFill) return;

  csv << lpRcd.url      << _T(',');
  csv << lpRcd.userName << _T(',');
  csv << lpRcd.password << _T(',');
  csv << lpRcd.totp     << _T(',');
  csv << lpRcd.notes    << _T(',');
  csv << lpRcd.title    << _T(',');
  csv << lpRcd.grpName  << _T(',');
  csv << lpRcd.fav      << vCrlf;
  }




#if 0
void LpPort::writePW(FileIO& fo) {
#if 1
#else
  fo.write(url);      fo.write(_T(','));
  fo.write(userName); fo.write(_T(','));
  fo.write(password); fo.write(_T(','));
  fo.write(totp);     fo.write(_T(','));
  fo.write(notes);    fo.write(_T(','));
  fo.write(title);    fo.write(_T(','));
  fo.write(grpName);  fo.write(_T(','));
  fo.write(fav);      fo.write(_T('\n'));
#endif
  }


void LpPort::importPW() {
#if 1
#else
Database db(kpdb);
int      index;

  // url, Name and Passord Match
  if (db.findNmPswdMatch(PswdsOnly, lpe, index))  {DBentry dbe;   dbe = lpe;  dbe.update(index); return;}

  // url, Name and Title Match => password change?
  if (db.findNmTtlMatch(PswdsOnly, lpe, index))   {DBentry dbe;   dbe = lpe;  dbe.update(index); return;}

  // url, Title and Password Match
  if (db.findTtlPswdMatch(PswdsOnly, lpe, index)) {DBentry dbe;   dbe = lpe;  dbe.update(index); return;}

  // Otherwise just add new entry...
                                        DBentry dbe;   dbe = lpe;  dbe.add();
#endif
  }



void LpPort::importNote() {
#if 1
#else
Database db(kpdb);
int      index;

  if (db.findTitleNotesMatch(NotesOnly, lpe, index))
                                                  {DBentry dbe;   dbe = lpe;  dbe.update(index); return;}

  // url, Title and Password Match
  if (db.findTtlPswdMatch(NotesOnly, lpe, index)) {DBentry dbe;   dbe = lpe;  dbe.update(index); return;}

  // Otherwise just add new entry...
                                           DBentry dbe;   dbe = lpe;  dbe.add();
#endif
  }
#endif
