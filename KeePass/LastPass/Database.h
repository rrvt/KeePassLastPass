// Access to KeePass Database


#pragma once
#include "Expandable.h"
#include "LPentry.h"



class PWtime : public PW_TIME {
public:

  PWtime() {}

  bool operator>=(PWtime& t);
  };


class DBentry : public PW_ENTRY {
public:

  DBentry() {ZeroMemory(this, sizeof(DBentry));}

  String   url() {return pszURL;}

//  void     convert(LPentry& lpe);

  bool     isNote() {return !strcmp(pszURL, NotesURL);}

  String   getFFLine();

  void     update(int index);
  void     add();

  DBentry& operator=  (LPentry& lpe);       // Assignment

  bool     operator== (DBentry& dbe);       // true if record IDs (16 byte value) are the same

  bool     operator!= (DBentry& dbe);       // true if names or passwords are not the same
  bool     operator>= (DBentry& dbe);       // true if titles, password length or names are not the same

  bool     operator!= (LPentry& lpe);       // true if names or passwords are not the same

  bool     operator<= (LPentry& lpe) {return lpe.title != pszTitle || lpe.name != pszUserName;}
                                            // true if titles or names are not the same
  bool     operator<  (LPentry& lpe);       // True if names or notes are not the same

  bool     operator>= (LPentry& lpe);       // true if titles or passwords are not the same

protected:

  bool     isBackupGrp();

  friend class DBentryP;
  };


class DBentryP {
public:

int      index;
DBentry* dbe;

  DBentryP() : index(0), dbe(0) {}
 ~DBentryP() {}
  };



class DBIterator {
IKpDatabase* kpdb;
String       url;
int          nEntry;
int          i;
bool         noteOnly;

DBentryP     dbep;

public:


  DBIterator(IKpDatabase* keepassdb) : kpdb(keepassdb), nEntry(0), i(0), noteOnly(false) { }

  void      notesOnly()     {noteOnly = true;}         // Default is Passwords Only
  void      passwordsOnly() {noteOnly = false;}

  // The loops return either password records or note records depending on the setting of noteOnly
  DBentryP* startLoop();
  DBentryP* nextEntry();
  void      backup(int n) {i -= n;}

  DBentryP* startLoop(TCchar* url);
  DBentryP* nextURL();
  };


class Database {
IKpDatabase* kpdb;
String       url;

public:

  Database(IKpDatabase* database) : kpdb(database) {}

  bool     findNmPswdMatch(  LPentry& lpe, int& index);  // url, name, password
  bool     findNmTtlMatch(   LPentry& lpe, int& index);  // url, title, name
  bool     findTtlPswdMatch( LPentry& lpe, int& index);  // url=NotesURL, title, password

  bool     findTitleNotesMatch(LPentry& lpe, int& index); // title, notes

  void     remove(int index) {kpdb->DeleteEntry(index);}

private:

  Database() { }
  };

