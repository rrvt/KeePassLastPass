// file Input/Output


#pragma once



class Special {
int    i;
int    lng;
String s;

public:

  Special() : lng(0), i(0) {}
 ~Special() {}

  Special&  operator=  (Tchar  ch) {s = ch;  i = 0; lng = 1; return *this;}
  Special&  operator+= (Tchar  ch) {s += ch;        lng++;   return *this;}
  Tchar     operator() (Tchar& ch) {return ch = i < lng ? s[i++] : 0;}

  TCchar*   str() {return s.str();}
  };


class ReadBuf {

String pth;
String module;
char*  p;
char*  endBuf;
char*  buf;

public:

  ReadBuf(TCchar* modName) : module(modName), buf(0), p(0), endBuf(0) { }
 ~ReadBuf() {if (buf) delete[] buf;}

  bool open(String& path);

  Tchar operator() () {return p < endBuf ? *p++ : 0;}
  Tchar peek()        {return p < endBuf ? *p   : 0;}
  void  expunge();

private:

  ReadBuf() { }
  };


class FileIn {
ReadBuf rbuf;
Special special;

bool    delFile;

public:

       FileIn(TCchar* moduleName, bool deleteFile);
      ~FileIn() {if (delFile) rbuf.expunge();}

  bool open(String& path)  {return rbuf.open(path);}

  Tchar nextElement(String& stg);


  Tchar next() {Tchar ch; return special(ch) ? ch : rbuf();}
  Tchar peek() {Tchar ch; return special(ch) ? ch : rbuf.peek();}

  Tchar getSpecial();

private:

  FileIn() : rbuf(_T("")) {}
  };


class WriteBuf {
String module;
FILE*  fp;

char*  p;
char*  eBuf;
char*  buf;

char   lastCh;

public:

  WriteBuf(TCchar* moduleName);
 ~WriteBuf() {close();}

  bool open(String& path);

  WriteBuf& operator<< (Tchar ch);             // Adds '\r' when necessary

private:

  void put(Tchar ch) {if (p >= eBuf) flush();   *p++ = lastCh = ch;}
  void flush();
  void close();

  WriteBuf() {}
  };


class FileOut {

WriteBuf wb;

public:

  FileOut(TCHAR* moduleName) : wb(moduleName) {}
 ~FileOut() { }

  bool open(String& path) {return wb.open(path);}

  void write(String& s);                          // Add Quotes as necessary before writing
  void write(TCchar* s);
  void put(  Tchar   ch);

private:

  void quote() {wb << _T('"');}

  FileOut() : wb(_T("")) {}
  };
