// Last Pass Menu


#pragma once
#include "resource.h"
#include "Utils.h"


class MenuItem : public KP_MENU_ITEM {
public:

  MenuItem();

  void setItem(int i);

  void set(LPTSTR stg, DWORD icon, DWORD flags = 0, DWORD state = 0);

  void test(int i);
  };


class MyMenu {

IKpConfig* cfg;
int        delMenuX;

public:

static const int nMenu = EndMenuItem - BaseMenuItem;
static MenuItem  menu[nMenu+1];

  MyMenu() : cfg(0), delMenuX(0) {}

  void init(IKpConfig* config);

  void toggleDelState();
  bool getDelState() {return menu[delMenuX].dwState != 0;}

  MenuItem& operator[] (int i) {return menu[i];}
  };

