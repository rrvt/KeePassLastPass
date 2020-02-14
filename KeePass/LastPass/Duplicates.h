// Duplicates


#pragma once
#include "Database.h"


class Duplicates {
IKpDatabase& kpdb;
Database     db;

bool         notes;

public:

  Duplicates(IKpDatabase* keePassdb) : kpdb(*keePassdb), db(&kpdb), notes(false) { }
 ~Duplicates() { }

  void remove();

private:

  void srchDestroy(DBentry& entry);
  };

