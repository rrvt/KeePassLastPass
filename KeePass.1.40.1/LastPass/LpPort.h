// LastPass Port


#pragma once
#include "CSVLexF.h"
#include "CSVOutF.h"
#include "LpRecord.h"

class KpRecord;


class LpPort {

CSVLexF  lex;
int      i;
LpRecord lpRcd;

CSVOutF  csv;

String   errorField;

public:

  LpPort() { }
 ~LpPort() { }

  bool      openImport(String& path);    // Start loop for importing LastPass records, return Path
  LpRecord* getNext();

  bool      openExport();
  void      store(KpRecord& kpRcd);

  void      close() {lex.close();}

private:

  String parseNote(String& stg);
  String parseCC(  String& stg);
  String parseWiFi(String& stg);
  String parseDB(  String& stg);
  String parseAddr(String& stg);
  String parseBank(String& stg);

  String extract(TCchar* beg, TCchar* end, String& src, String& prefix, String& suffix);

  void importPW();
  void importNote();

  bool readRecord(CSVLexF& lex, LpRecord& rcd);
  };


