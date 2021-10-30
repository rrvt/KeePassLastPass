// LastPass Application -- MFC required class which does very little in KeePass


#pragma once


class LastPassApp : public CWinApp {
public:

  LastPassApp() { }

  virtual BOOL InitInstance();

  DECLARE_MESSAGE_MAP()

  friend class LastPass;
  };


extern LastPassApp theApp;

