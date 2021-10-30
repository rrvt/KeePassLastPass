// Language Processing


#pragma once
#include "IniFile.h"


class Language {

bool    opened;
IniFile iniFl;

public:

  Language() : opened(false) { }
 ~Language() { }


  void initialize (TCchar* langFile);
  bool find(int id, TCchar* txtID, String& val);
  };


extern Language language;
