// Last Pass Password entry data


#pragma once
#include "FileIO.h"


class DBentry;

extern TCchar* NotesURL;

extern TCchar* FFLineOpen;
extern TCchar* FFLineClose;
extern TCchar* FavOpen;
extern TCchar* FavClose;
extern TCchar* ExtraOpen;
extern TCchar* ExtraClose;
extern TCchar* TimeOpen;
extern TCchar* TimeClose;


class LPentry {
IKpDatabase* kpdb;
bool         aNote;
public:

String grpName;                             // database entry contains fields with these concepts
String url;                                 // These are temporary fields used during import and
String title;                               // export.
String name;
String password;
String notes;
String fav;
String timeStamp;

String additional;                          // used to hold notes, fav and timestamp during add or

String errorField;
char   lastCh;

  LPentry(IKpDatabase* database) : kpdb(database), aNote(false), lastCh(0) {}
 ~LPentry();

  void clear();

  void set(TCHAR* src, TCchar* const opn, TCchar* const cls, String& s);

  bool readRecord(FileIn&  fi);
  bool isNote() {return aNote;}

  void writePW(   FileOut& fo);

  LPentry& operator= (DBentry& dbe);

private:

  void clr(String& s);
  LPentry() { }
  };

