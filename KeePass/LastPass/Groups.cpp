// Groups are used to organize KeePass passwords


#include "pch.h"
#include "Groups.h"
#include "KeePassSDK.h"


Groups groups;


void Groups::initialize() {
int cnt = kpdb->GetGroupCount();   if (cnt == nGroups) return;
int i;

  nGroups = cnt;

  for (i = 0; i < cnt; i++) {

    groups[i] = kpdb->GetGroup(i);

    if (groups[i].name == "Backup") backupID = groups[i].id;
    }
  }



DWORD Groups::getID(TCchar* grpName) {
int    i;
String s = grpName;
int    cnt;

  initialize();

  for (cnt = 0; cnt < 2; cnt++) {

    for (i = 0; i < nGroups; i++) if (groups[i].name == s) return groups[i].id;

    // Not found so add it to the list

    PW_GROUP grp;    ZeroMemory(&grp, sizeof(PW_GROUP));

    grp.pszGroupName = (Tchar*) s.str();

    kpdb->AddGroup(&grp);   initialize();
    }

  return 0;
  }


TCchar* Groups::getName(DWORD grpID) {
int i;

  initialize();

  for (i = 0; i < nGroups; i++) if (groups[i].id == grpID) return groups[i].name;

  return 0;
  }


Group& Group::operator= (PW_GROUP* pwg)
                           {if (pwg) {name = pwg->pszGroupName; id = pwg->uGroupId;} return *this;}

