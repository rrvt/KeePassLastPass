// Last Pass Commands


#include "stdafx.h"
#include "Commands.h"
#include "About.h"
#include "Duplicates.h"
#include "GetPathDlg.h"
#include "KpIter.h"
#include "KpPort.h"
#include "LpPort.h"
#include "Menu.h"


void Commands::importLP() {
LpPort    lpPort;
String    path;
LpRecord* lpRcd;
int       i;
KpPort    kpPort(kp, kpdb);
PW_TIME   expire;

  if (!lpPort.openImport(path)) return;

  kp.GetNeverExpireTime(&expire);

  for (i = 0, lpRcd = lpPort.getNext(); lpRcd; i++, lpRcd = lpPort.getNext()) {

    if (!i) continue;

    kp.GetCurrentPwTime(&lpRcd->time);    lpRcd->expire = expire;

    kpPort.store(*lpRcd);
    }

  kp.UpdateUI();   kp.SetFileModified(true);

  if (menu.getDelState()) expungeFile(path);
  }



  // url,username,password,totp,extra,name,grouping,fav

void Commands::exportLP() {
LpPort      lpPort;
KpIter      iter(kpdb);
KpRecord*   rcd;

  if (!lpPort.openExport()) return;

  for (rcd = iter(); rcd; rcd = iter++) {

    kpdb.UnlockEntryPassword(rcd);

      lpPort.store(*rcd);

    kpdb.LockEntryPassword(rcd);
    }
  }



void Commands::expungeFile() {
PathDlgDsc dsc;
String     path;

  dsc = PathDlgDsc{_T("LastPass Export File"), _T(""), _T("csv"), _T("*.csv")};

  if (getPathDlg(dsc, path)) expungeFile(path);
  }



void Commands::toggleDelState() {
#if 1
  menu.toggleDelState();
#else
DWORD&          dwState = items[delMenuX].dwState;
const MenuCode* p;

  dwState = dwState ^ KPMIS_CHECKED;

  p = getMenuCode(delMenuX);

  cfg->set((Tchar*) LPsection, p->txt, dwState != 0);
#endif
  }



void Commands::removeRedundantPW() {
Duplicates delDup(kpdb);
bool       someDeleted;

  someDeleted = delDup();

  kp.UpdateUI();   kp.SetFileModified(true);
  }


void Commands::expungeFile(TCchar* path) {
String pth = path;
FileIO fi;
int    n;
int    i;
Tchar  ch;

   if (!fi.open(pth, FileIO::Read | FileIO::Write)) return;

   n = fi.getLength();

   for (i = 0, ch = ' '; i < n; i++) {fi.write(ch);   if (ch++ > 126) {fi.write(_T("\r\n")); ch = ' ';}}

   fi.close();

   _tremove(path);
  }


void Commands::about() {CAboutDlg dlg;   dlg.DoModal();}



void Commands::testStuff() {
  }




