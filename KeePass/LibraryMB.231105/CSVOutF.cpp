// CSV Output (takes care of internal commas, quotes, etc...


#include "stdafx.h"
#include "CSVOutF.h"
#include "GetPathDlg.h"


static TCchar quote = _T('"');
       TCchar Comma = _T(',');


bool CSVOutF::open(PathDlgDsc& dsc) {
String path;
  return getSaveAsPathDlg(dsc, path) && fo.open(path, FileIO::Write);
  }


String& CSVOutF::quotes(TCchar* p) {
static String s;
bool   addQuotes = false;

  s = p;

  if (s.find(quote) >= 0) {
    String t;
    int    n = s.length();
    int    i;

    for (i = 0; i < n; i++) {Tchar ch = s[i];   if (ch == quote) t += quote;   t += ch;}

    s = t; addQuotes = true;
    }

  if (s.find(_T(',')) >= 0) addQuotes = true;

  if (addQuotes) s = quote + s + quote;

  return s;
  }



CSVManip vCrlf;                  // add to stream to terminate a line on display: dsp << "xyz" << vCrlf;

void CSVOutF::initialize() {vCrlf.n = this; vCrlf.func = CSVOutF::doCrlf;}


