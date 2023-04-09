// Picker.cpp : implementation file
//

#include "stdafx.h"
#include "Picker.h"


// Picker dialog

IMPLEMENT_DYNAMIC(Picker, CDialogEx)

Picker::Picker(CWnd* pParent ) : CDialogEx(IDD_PickerDlg, pParent), group1(_T("")), url1(_T("")),
                                 title1(_T("")), userName1(_T("")), password1(_T("")), notes1(_T("")),
                                 timeStamp1(_T("")), group2(_T("")), url2(_T("")), title2(_T("")),
                                 userName2(_T("")), password2(_T("")), notes2(_T("")),
                                 timeStamp2(_T("")), cmpGrp(_T(""))
  , cmpURL(_T(""))
  , cmpTtl(_T(""))
  , cmpPsw(_T(""))
  , cmpNts(_T(""))
  , cmpUsr(_T(""))
{ }

Picker::~Picker() { }

void Picker::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_Group1, group1);
  DDX_Text(pDX, IDC_URL1, url1);
  DDX_Text(pDX, IDC_Title1, title1);
  DDX_Text(pDX, IDC_UserName1, userName1);
  DDX_Text(pDX, IDC_Password1, password1);
  DDX_Text(pDX, IDC_Notes1, notes1);
  DDX_Text(pDX, IDC_TimeStamp1, timeStamp1);

  DDX_Text(pDX, IDC_CmpGrp, cmpGrp);
  DDX_Text(pDX, IDC_CmpURL, cmpURL);
  DDX_Text(pDX, IDC_CmpTtl, cmpTtl);
  DDX_Text(pDX, IDC_CmpUsr, cmpUsr);
  DDX_Text(pDX, IDC_CmpPsw, cmpPsw);
  DDX_Text(pDX, IDC_CmpNts, cmpNts);

  DDX_Text(pDX, IDC_Group2, group2);
  DDX_Text(pDX, IDC_URL2, url2);
  DDX_Text(pDX, IDC_Title2, title2);
  DDX_Text(pDX, IDC_UserName2, userName2);
  DDX_Text(pDX, IDC_Password2, password2);
  DDX_Text(pDX, IDC_Notes2, notes2);
  DDX_Text(pDX, IDC_TimeStamp2, timeStamp2);
  }


BEGIN_MESSAGE_MAP(Picker, CDialogEx)
  ON_BN_CLICKED(IDC_NoDelete, &Picker::OnNodelete)
  ON_BN_CLICKED(IDC_DelRcd1, &Picker::OnDelRcd1)
  ON_BN_CLICKED(IDC_DelRcd2, &Picker::OnDelRcd2)
  ON_BN_CLICKED(IDCANCEL, &Picker::OnCancelAll)
  ON_BN_CLICKED(IDC_Finish, &Picker::OnFinish)
END_MESSAGE_MAP()


// Picker message handlers


void Picker::OnNodelete()  {EndDialog(IDC_NoDelete);}
void Picker::OnDelRcd1()   {EndDialog(IDC_DelRcd1);}
void Picker::OnDelRcd2()   {EndDialog(IDC_DelRcd2);}
void Picker::OnCancelAll() {CDialogEx::OnCancel();}
void Picker::OnFinish()    {EndDialog(IDC_Finish);}
