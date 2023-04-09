// KeePass Database Port


#pragma once

class  LpRecord;
class  KpRecord;
struct IKpAPI;
struct IKpDatabase;


class KpPort {

IKpAPI&      kp;
IKpDatabase& kpdb;

public:

  KpPort(IKpAPI& keePass, IKpDatabase& db) : kp(keePass), kpdb(db) { }
 ~KpPort() { }

  void store(LpRecord& lpRcd);

private:

  bool compare(String& lpStg, TCchar* kpStg) {return !lpStg.isEmpty() && lpStg == kpStg;}
  bool comparePW(LpRecord& lpRcd, KpRecord* kpRcd);

  void update(int index, KpRecord& kpRcd);
  };

