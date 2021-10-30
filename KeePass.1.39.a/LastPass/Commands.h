// Last Pass Commands


#pragma once

struct IKpAPI;
struct IKpDatabase;


class Commands {

IKpAPI&       kp;
IKpDatabase&  kpdb;

public:

  Commands(IKpAPI& keePass, IKpDatabase& kpData) : kp(keePass), kpdb(kpData) { }
 ~Commands() { }

  void exportLP();
  void importLP();
  void expungeFile();
  void toggleDelState();
  void removeRedundantPW();
  void about();
  void testStuff();

private:

  void expungeFile(TCchar* path);

  Commands() : kp(*(IKpAPI*)0), kpdb(*(IKpDatabase*)0) { }
  };

