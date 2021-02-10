// Last Pass Menu


#include "stdafx.h"
#include "MyMenu.h"
#include "LastPass.h"


struct MenuCode {
int    code;
String txt;
  MenuCode(int c, TCHAR* p) : code(c), txt(p) { }
  };


const MenuCode menuCodes[] = {{LP_LastPass,          _T("LP_LastPass")},
                              {LP_ExportEntries,     _T("LP_ExportEntries")},
                              {LP_ImportEntries,     _T("LP_ImportEntries")},
                              {LP_ExpungeFile,       _T("LP_ExpungeFile")},
                              {XL_Menu4,             _T("XL_Menu4")},
                              {LP_ToggleDelState,    _T("LP_ToggleDelState")},
                              {LP_RemoveReduntantPW, _T("LP_RemoveReduntantPW")},
                              {LP_About,             _T("LP_About")},
                              {LP_Test,              _T("LP_Test")},
                              };

static const MenuCode* getMenuCode(int i);



MenuItem MyMenu::menu[nMenu + 1];   // One more for POPUP_END
const int nMenuLoop = LP_ToggleDelState - BaseMenuItem - 1;       // -1 for Break


void MyMenu::init(IKpConfig* config) {
int             i = 0;
const MenuCode* p = getMenuCode(i);
DWORD           state;
const TCHAR*    txt;

  cfg = config;

  menu[i].set(translate(p->code, p->txt), 62, KPMIF_POPUP_START);                  // _TRN(LP_LastPass)

  for (i++; i < nMenuLoop; i++) menu[i].setItem(i);

  menu[i++].set("", 0);

  delMenuX = i;   p = getMenuCode(delMenuX);  txt = p->txt;

  state = cfg->get((Tchar*) LPsection, txt, (bool) KPMIS_UNCHECKED);

  menu[i].set(translate(p->code, txt), 0, KPMIF_CHECKBOX, state);

  for (i++; i < nMenu; i++) menu[i].setItem(i);

  menu[i].set("", 0, KPMIF_POPUP_END);
  }


void MyMenu::toggleDelState() {
DWORD&          dwState = menu[delMenuX].dwState;
const MenuCode* p;

  dwState = dwState ^ KPMIS_CHECKED;

  p = getMenuCode(delMenuX);

  cfg->set((Tchar*) LPsection, p->txt, dwState != 0);
  }





MenuItem::MenuItem() {ZeroMemory(this, sizeof(MenuItem));}



void MenuItem::setItem(int i) {
const MenuCode* p = getMenuCode(i);   if (!p) return;

  set(translate(p->code, p->txt), 62);
  }



const MenuCode* getMenuCode(int i) {return 0 <= i && i < noElements(menuCodes) ? &menuCodes[i] : 0;}


void MenuItem::set(LPTSTR stg, DWORD icon, DWORD flags, DWORD state)
                              {lpCommandString = stg; dwFlags = flags; dwState = state; dwIcon = icon;}



