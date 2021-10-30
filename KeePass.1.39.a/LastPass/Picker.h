// Pick items to delete


#pragma once
#include "Resource.h"


// Picker dialog

class Picker : public CDialogEx {

  DECLARE_DYNAMIC(Picker)

public:

CString group1;
CString url1;
CString title1;
CString userName1;
CString password1;
CString notes1;
CString timeStamp1;

CString cmpGrp;
CString cmpURL;
CString cmpTtl;
CString cmpUsr;
CString cmpPsw;
CString cmpNts;

CString group2;
CString url2;
CString title2;
CString userName2;
CString password2;
CString notes2;
CString timeStamp2;

  Picker(CWnd* pParent = nullptr);   // standard constructor

  virtual ~Picker();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_PickerDlg };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnNodelete();
  afx_msg void OnDelRcd1();
  afx_msg void OnDelRcd2();
  afx_msg void OnCancelAll();
  afx_msg void OnFinish();
};
