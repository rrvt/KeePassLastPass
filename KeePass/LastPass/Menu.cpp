// Last Pass Menu


#include "pch.h"
#include "Menu.h"
#include "Language.h"


static TCchar*   LPSection = _T("LastPass");
static const int DelIndex  = LP_ToggleDelState - BaseMenuItem;

Menu menu;                      // The one menu object


struct MenuCode {
int    code;
String txt;
int    icon;
int    flags;

  MenuCode(int c, TCchar* p, int icn, int flgs) : code(c), txt(p), icon(icn), flags(flgs) { }
  };


const MenuCode menuCodes[] = {
                         {LP_LastPass,          _T("LP_LastPass"),         62, KPMIF_POPUP_START },
                         {LP_ExportEntries,     _T("LP_ExportEntries"),    62, 0 },
                         {LP_ImportEntries,     _T("LP_ImportEntries"),    62, 0 },
                         {LP_ExpungeFile,       _T("LP_ExpungeFile"),      62, 0 },
                         {LP_Break,             _T("LP_Break"),             0, 0 },
                         {LP_ToggleDelState,    _T("LP_ToggleDelState"),    0, KPMIF_CHECKBOX },
                         {LP_RemoveReduntantPW, _T("LP_RemoveReduntantPW"),62, 0},
                         {LP_About,             _T("LP_About"),            62, 0 },
                         {LP_Test,              _T("LP_Test"),             62, 0 },
                         {LP_PopUpEnd,          _T("LP_PopUpEnd"),          0, KPMIF_POPUP_END }
                         };

static MenuCode* getMenuCode(int i);



/*
  delMenuX = i;   p = getMenuCode(delMenuX);  txt = p->txt;

  state = cfg->get((Tchar*) LPsection, txt, (bool) KPMIS_UNCHECKED);

  menu[i].set(translate(p->code, txt), 0, KPMIF_CHECKBOX, state);
*/


void Menu::init(IKpConfig* config) {
int           i;
int           n = noElements(menuCodes);
MenuCode*     p;
Tchar         buf[MAX_PATH];

  config->GetUserPath(buf, MAX_PATH);
  iniFile.getAppDataPath(buf);

  for (i = 0; i < n; i++) set(i);

  p = getMenuCode(DelIndex);

  items[DelIndex].dwState = iniFile.readInt(LPSection, p->txt, KPMIS_UNCHECKED);
  }


void Menu::set(int i) {
MenuCode* p = getMenuCode(i);   if (!p) return;
String    val;

  if (!language.find(p->code, p->txt, val)) return;

  if (val == _T("~")) val.clear();

  item[i].name = val;    item[i].code = p->code;

  items[i].set(item[i].name, p->icon, p->flags);
  }


int Menu::get(LPARAM commandID) {
int i;

  for (i = 0; i < nMenu; i++) if (items[i].dwCommandID == commandID) break;

  return item[i].code;
  }


void Menu::toggleDelState() {
DWORD&          dwState = items[DelIndex].dwState;
const MenuCode* p;

  dwState = dwState ^ KPMIS_CHECKED;

  p = getMenuCode(DelIndex);

  iniFile.writeInt(LPSection, p->txt, dwState != 0);
  }


bool Menu::getDelState() {return items[DelIndex].dwState != 0;}





MenuItem::MenuItem() {ZeroMemory(this, sizeof(MenuItem));}



MenuCode* getMenuCode(int i)
                      {return 0 <= i && i < noElements(menuCodes) ? (MenuCode*) &menuCodes[i] : 0;}


void MenuItem::set(TCchar* stg, DWORD icon, DWORD flags, DWORD state)
                 {lpCommandString = (LPTSTR) stg; dwFlags = flags; dwState = state; dwIcon = icon;}



