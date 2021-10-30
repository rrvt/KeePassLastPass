// Groups are used to organize KeePass passwords


#pragma once
#include "Expandable.h"
#include "KeePassSDK.h"     //"SDK/KpSDK.h"


class Group {
String name;
DWORD  id;

public:

  Group() { }
  Group(Group& g) {name = g.name; id = g.id;}
 ~Group() { }

  Group& operator= (Group& g) {name = g.name; id = g.id; return *this;}
  Group& operator= (PW_GROUP* pwg)
                                {if (pwg) {name = pwg->pszGroupName; id = pwg->uGroupId;} return *this;}
  friend class Groups;
  };


class Groups {
IKpDatabase*          kpdb;
DWORD                 backupID;
int                   nGroups;
Expandable<Group, 16> groups;

public:

  Groups() : kpdb(0), nGroups(0), backupID(0) {}
 ~Groups() { }

  void   setKpdb(IKpDatabase* db) {kpdb = db;}
  void   initialize();

  Cchar* getName(DWORD grpID);
  DWORD  getID(TCchar* grpName);

  DWORD  bkupID() {initialize(); return backupID;}
private:

//Groups() : kpdb(*(IKpDatabase*)0) { }
  };


extern Groups groups;
