// Duplicates


#pragma once


class  KpIter;
class  KpRecord;
struct IKpDatabase;


class Duplicates {
IKpDatabase& kpdb;

public:

  Duplicates(IKpDatabase& db) : kpdb(db) { }
 ~Duplicates() { }

  bool operator() ();

private:

  int     srchdupRcd(KpIter& iter, KpRecord& rcd);
  String& prep(String& src, String& noteType);
  TCchar* isEqual(String& left, String& right);
  };

