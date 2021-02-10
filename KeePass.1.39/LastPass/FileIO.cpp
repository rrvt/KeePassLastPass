// file Input/Output


#include "stdafx.h"
#include "fileIO.h"
#include "LastPass.h"




FileIn::FileIn(TCchar* moduleName, bool deleteFile) : rbuf(moduleName), delFile(deleteFile) {}


char FileIn::nextElement(String& stg) {
char ch;
bool bInString = false;

  stg.clear();

  for (ch = next(); ch; ch = next()) {

    if (ch == '&') ch = getSpecial();

    if (ch == '\r') continue;

    if (ch == '\"') {
      if (peek() == '\"') {stg += ch; next();}
      else                 bInString = !bInString;
      continue;
      }

    if (!bInString && (ch == ',' || ch == '\n')) break;

    stg += ch;
    }

  return ch;
  }


// Get Special Characters used in html, i.e. &xxx; => X, where xxx is a description of the character X.
// If xxx may be two or three characters, otherwise the string return as special is &xxxY where Y is not
// a semicolon (';')

char FileIn::getSpecial() {
int i;

  special = _T('&');

  for (i = 0; i < 4; i++) {
    char ch = rbuf.peek();   if (ch == 0 || ch == _T('&')) break;     // EOF

    ch = rbuf();

    if (ch == ';') {
      if (!_tcsicmp(special.str(), "&amp")) {special = _T('&'); break;}
      if (!_tcsicmp(special.str(), "&lt"))  {special = _T('<'); break;}
      if (!_tcsicmp(special.str(), "&gt"))  {special = _T('>'); break;}
      }

    special += ch;
    }

  return next();
  }



bool ReadBuf::open(String& path) {
FILE* fp;
int   bufSize;

  if (path.empty()) return false;

  _tfopen_s(&fp, path, _T("rb"));   if (!fp) return false;

  fseek(fp, 0, SEEK_END);   bufSize = static_cast<int>(ftell(fp));   fseek(fp, 0, SEEK_SET);

  buf = new char[bufSize + 1];    if (!buf) {fclose(fp); fp = 0;  return false;}

  fread(buf, 1, bufSize, fp); buf[bufSize] = 0;   fclose(fp);

  p = buf;   endBuf = buf + bufSize;   pth = path;   return true;
  }



void ReadBuf::expunge() {
FILE* fp;
int   bufSize = endBuf - buf;
char  ch;

  if (!buf) return;

  if (!_tfopen_s(&fp, pth.str(), _T("r+b"))) {

    for (p = buf, ch = ' '; p < endBuf; p++) {*p = ch;   if (ch++ > 126) ch = ' ';}

    if ((int) fwrite(buf, 1, bufSize, fp) < bufSize)
      MessageBox(mainWindowHandle(), _T("Unable to write to csv file."), module, MB_ICONWARNING | MB_OK);

    fclose(fp);
    }

  remove(pth.str());  pth.clear();
  }



// Add Quotes as necessary before writing

void FileOut::write(String& s) {
bool addQts = s.find(_T('"'))  != String::npos ||
              s.find(_T(','))  != String::npos ||
              s.find(_T('\n')) != String::npos;

  if (addQts) quote();   write(s.str());   if (addQts) quote();
  }


void FileOut::write(TCchar* s) {if (s) while (*s) put(*s++);}


void FileOut::put(Tchar ch) {if (ch == _T('"')) wb << _T('"');   wb << _T(ch);}




const int BufSize = 512;


WriteBuf::WriteBuf(TCchar* moduleName) : module(moduleName), fp(0), p(0), eBuf(0), buf(0), lastCh(0) {}


bool WriteBuf::open(String& path) {

  if (path.empty()) return false;

  _tfopen_s(&fp, path, _T("wb"));

  if (fp == NULL) {

    MessageBox(
      GetActiveWindow(),
        _T("File access error: cannot open output file!\r\n"
        "\r\nMake sure you have the rights to access this file."), module, MB_ICONWARNING | MB_OK
        );

    return false;
    }

  buf = new char[BufSize];

  if (buf == NULL) {
    fclose(fp); fp = 0;
    MessageBox(
      GetActiveWindow(), _T("Memory error: not enough free memory available!"),
      module, MB_ICONWARNING | MB_OK
      );

    return false;
    }

  p = buf; eBuf = buf + BufSize; return true;
  }



WriteBuf& WriteBuf::operator<< (Tchar ch) {

  if (ch == _T('\n')) {
//    if (lastCh == _T('\n')) return *this;            // suppress multiple end of lines
    if (lastCh != _T('\r')) put(_T('\r'));
    }

  put(_T(ch));  return *this;
  }


void WriteBuf::close() {

  if (!fp) return;

  flush(); fclose(fp); fp = 0;

  if (buf) {delete[] buf; p = eBuf = buf = 0;}
  }


void WriteBuf::flush() {
size_t n = p - buf;

  if (!fp) return;

  if (fwrite(buf, n, 1, fp) != 1) {
    MessageBox(GetActiveWindow(), _T("Write error"), module, MB_ICONWARNING | MB_OK);
    fclose(fp); fp = 0; return;
    }

  p = buf;
  }

