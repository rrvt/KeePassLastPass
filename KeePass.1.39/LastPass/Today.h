// Today using CTime



#pragma once
#include "stdafx.h"


class Today {

CTime dt;

public:

  Today() : dt(CTime::GetCurrentTime()) { }
 ~Today() {}

  void initialize() {dt = CTime::GetCurrentTime();}

  String getTime()   {CString s = dt.Format("%X");    return String(s);}
  String getDate()   {CString s = dt.Format("%x");    return String(s);}
  String dayOfWeek() {CString s = dt.Format("%a");    return String(s);}
  operator String () {CString s = dt.Format("%a %c"); return String(s);}
  };
