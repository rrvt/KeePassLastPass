// CSV Output (takes care of internal commas, quotes, etc...

/*
The template for loading, storing and accessing CSV files is contained in several files:
  *  CSVLex   -- A Lexical Analyser (FSM) for reading CSV files
  *  CSVOut   -- Contains a class for outputing to a CSV file (managing commas and quotes withing
                 fields)
  *  CSVRcdB  -- A base file for your CSV Record to provide basic functionality
  *  CSVRcdsT -- Contains a template for creating a container for CSV Records.  May be used as a
                 base class for extending functionality with a subclass.

This class provides a simple mechansim for outputing to a CSV file.  It allos the use of the "<<"
(streaming) operator and handles comma and quotes withing the items output.  Each item output is
considered to be a separate field in the CSV file.
*/


#pragma once
#include "Date.h"
#include "FileIO.h"
#include "ManipT.h"

class PathDlgDsc;


extern TCchar Comma;


class CSVOutF;

typedef ManipT<CSVOutF> CSVManip;

class CSVOutF {
FileIO fo;

public:

  CSVOutF() {initialize();}

  bool     open(PathDlgDsc& dsc);

  CSVOutF& operator<< (String&    s)     {fo.write(quotes(s));              return *this;}
  CSVOutF& operator<< (TCchar*    p)     {fo.write(quotes(p));              return *this;}
  CSVOutF& operator<< (Tchar     ch)     {fo.write(ch);                     return *this;}
  CSVOutF& operator<< (int        x)     {String s = x;        fo.write(s); return *this;}
  CSVOutF& operator<< (Date&     dt)     {String s; dt >> s;   fo.write(s); return *this;}

  CSVOutF& operator<< (CSVManip& m)      {return m.func(*this);}

  void    crlf()                        {fo.crlf();}

private:

  static String& quotes(TCchar* p);

  void initialize();

  static CSVOutF& doCrlf(CSVOutF& n) {n.crlf(); return n;}
  };


extern CSVManip vCrlf;       // add to stream to terminate a line on display: ar << "xyz" << vCrlf;

