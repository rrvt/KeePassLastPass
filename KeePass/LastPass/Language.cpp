// Language Processing


#include "pch.h"
#include "Language.h"
#include "LastPass.h"


static TCchar* Section = _T("LastPass");


Language language;



void Language::initialize (TCchar* langFile) {
String path;

  path =  lastPass.basePath(); path += _T("\\Plugins\\");
  path += langFile;            path += _T("_LastPass.plng");

  if (GetFileAttributes(path) == INVALID_FILE_ATTRIBUTES) {opened = false; return;}

  iniFl.setPath(path);   opened = true;
  }


bool Language::find(int id, TCchar* txtID, String& val) {

  if (iniFl.readString(Section, txtID, val)) return true;

  Tchar buf[128];

  if (LoadString(lastPass.instDLL(), id, buf, sizeof(buf)) > 0) {val = buf; return true;}

  return false;
  }

