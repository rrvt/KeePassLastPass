// Iterator over the KeePass Data


#pragma once

class  KpRecord;
struct IKpDatabase;


class KpIter {
IKpDatabase& db;
int          nEntry;
int          i;
DWORD        bkupID;

public:

enum Dir     {Fwd, Rev};

  KpIter(IKpDatabase& kpdb) : db(kpdb), nEntry(0), i(0) { }
  KpIter(KpIter& iter) : db(iter.db), nEntry(iter.nEntry), i(iter.i), bkupID(iter.bkupID) { }
 ~KpIter() { }

  KpRecord* operator() (Dir rev = Fwd);          //  = PswdsOnly

  KpRecord* operator++ (int);
  KpRecord* operator-- (int);
  int       curIndx() {return i;}

  bool      isLast();
  bool      isFirst();

  void      remove(Dir dir = Fwd);        // {db.DeleteEntry(i); i += dir == Fwd ? -1 : 1;   adjust();}
  int       adjust();                     // {return nEntry = db.GetEntryCount();}

private:

  KpRecord* next(Dir rev = Fwd);
  KpIter() : db(*(IKpDatabase*)0) { }
  };

