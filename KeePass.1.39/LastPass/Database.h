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



class DBIter {
IKpDatabase* kpdb;
String       url;
bool         urlSrch;
int          nEntry;
int          i;
bool         noteOnly;

DBentryP     dbep;

public:

enum Dir {Fwd, Rev};

  DBIter(IKpDatabase* keepassdb) : kpdb(keepassdb), urlSrch(false), nEntry(0), i(0),
                                                                                      noteOnly(false) { }
  DBIter(DBIter& iter) {copy(iter);}

  DBIter& operator= (DBIter& iter) {copy(iter); return *this;}

  void      notesOnly()         {noteOnly = true;}         // Default is Passwords Only
  void      passwordsOnly()     {noteOnly = false;}
  void      switchToUrl(TCchar* url) {this->url = url; urlSrch = true;}

  // The loops return either password records or note records depending on the setting of noteOnly

  DBentryP* operator() (Dir rev = Fwd);
  DBentryP* operator() (TCchar* url);                 // Forward Search only

  DBentryP* operator++ (int);
  DBentryP* operator-- (int);
  DBentryP* current();

  bool      isLast();
  bool      isFirst();

  void      remove(Dir rev = Fwd) {if (kpdb) kpdb->DeleteEntry(i); i = rev ? i - 1 : i + 1;}

private:

  DBentryP* next(Dir rev = Fwd);
  void      copy(DBIter& iter);
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

//  void     remove(int index) {kpdb->DeleteEntry(index);}

private:

  Database() { }
  };

