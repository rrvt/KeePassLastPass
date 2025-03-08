// Last Pass Menu


#pragma once
#include "IniFile.h"
#include "KeePassSDK.h"
#include "resource.h"


/*struct KP_MENU_ITEM {
  /// Flags (enabled state, checkbox, popup, etc).
  /// - If KPMIF_CHECKBOX is set, the menu item has a checkbox.
  /// - If KPMIF_DISABLED is set, the menu item is disabled (grayed out).
  /// - If KPMIF_POPUP_START is set, the menu item has a popup menu (all menu
  ///   items following the current one will be shown in the popup).
  /// - If KPMIF_POPUP_END is set, the menu item is invisible and a popup
  ///   menu is closed (all following menu items are shown in the parent menu).

  DWORD dwFlags; // KPMIF_XXX flags

  /// State (checkbox, etc).
  /// If KPMIS_CHECKED is set, the checkbox is checked.

  DWORD dwState; // KPMIS_XXX flags

  DWORD dwIcon;

  LPTSTR lpCommandString; ///< The menu item's text.
  }  // Some more fields but these are the only ones settable by LastPass
*/

class MenuItem : public KP_MENU_ITEM {
public:

  MenuItem();

  void set(TCchar* stg, DWORD icon, DWORD flags = 0, DWORD state = 0);

  void test(int i);
  };


struct MenuAddm {
int    code;
String name;

  MenuAddm() : code(0) { }
 ~MenuAddm() { }
  };


class Menu {

static const int nMenu = EndMenuItem - BaseMenuItem;

IniFile    iniFile;
MenuItem   items[nMenu+1];             // One more for POPUP_END
MenuAddm   item[nMenu+1];

public:

  Menu() { }    // cfg(0)
 ~Menu() { }

  void init(IKpConfig* config);

  void set(int i);
  int  get(LPARAM commandID);             // Returns the Resource ID for the commandID

  void toggleDelState();
  bool getDelState();

  MenuItem& operator[] (int i) {return 0 <= i && i <= nMenu ? items[i] : *(MenuItem*)0;}

  int       noMenu() {return nMenu;}
  MenuItem* addr()   {return items;}
  };


extern Menu menu;
