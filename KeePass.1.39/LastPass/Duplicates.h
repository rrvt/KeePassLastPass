// Duplicates


#pragma once
#include "Database.h"


class Duplicates {
IKpDatabase& kpdb;
Database     db;

public:

  Duplicates(IKpDatabase* keePassdb) : kpdb(*keePassdb), db(&kpdb) { }
 ~Duplicates() { }

  void operator() ();

private:

  void srchDestroy(DBIter iter, DBentry& entry);
  };

