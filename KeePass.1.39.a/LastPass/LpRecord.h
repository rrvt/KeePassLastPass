// Last Pass Password entry data


#pragma once
#include "KpRecord.h"



extern TCchar* FFLineOpen;
extern TCchar* FFLineClose;
extern TCchar* FavOpen;
extern TCchar* FavClose;
extern TCchar* ExtraOpen;
extern TCchar* ExtraClose;
extern TCchar* ToTpOpen;
extern TCchar* ToTpClose;
extern TCchar* TimeOpen;
extern TCchar* TimeClose;


class LpRecord {
bool   aNote;
public:

String  grpName;                             // database entry contains fields with these concepts
String  url;                                 // These are temporary fields used during import and
String  title;                               // export.
String  userName;
String  password;
String  totp;
String  notes;
String  fav;
String  additional;                          // used to hold notes, fav and timestamp during add or
String  timeStamp;

PW_TIME time;
PW_TIME expire;

  LpRecord() : aNote(false) {}
 ~LpRecord();

  void clear();

  void setAdditional();

  void setAnote() {aNote = url == NotesURL;}

  bool isNote() {return aNote;}

  LpRecord& operator= (KpRecord& kpRcd);

private:

  void parseNote();

  void parseCC();
  void parseWiFi();
  void parseDB();
  void parseAddr();
  void parseBank();

  String extract(TCchar* beg, TCchar* end, String& src, String& prefix, String& suffix);

  void   restoreNote();
  void   restoreCC();
  void   restoreWiFi();
  void   restoreDB();
  void   restoreAddr();
  void   restoreBank();

  bool   nextItem(String& src, String& item);
  void   insert(TCchar* beg, String& stg);
  void   set(   TCchar* opn, TCchar* cls, String& dst);

  void   clr(String& s);
  };

