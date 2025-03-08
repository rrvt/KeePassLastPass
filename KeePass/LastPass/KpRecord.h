// KeePass Record


#pragma once
#include "KeePassSDK.h"


class LpRecord;


extern TCchar* NotesURL;


class KpRecord : public PW_ENTRY {
public:

  KpRecord() {ZeroMemory(this, sizeof(KpRecord));}
 ~KpRecord() { }

  KpRecord& operator=  (LpRecord& rcd);       // Assignment

  bool      operator== (KpRecord& rcd);
  };

