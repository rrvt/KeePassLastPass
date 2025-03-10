/*
  KeePass Password Safe - The Open-Source Password Manager
  Copyright (C) 2003-2025 Dominik Reichl <dominik.reichl@t-online.de>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef ___NEW_GUI_COMMON___
#define ___NEW_GUI_COMMON___

#include "../../KeePassLibCpp/SysDefEx.h"
#include "BCMenu.h"
#include "XPStyleButtonST.h"
#include "AutoRichEditCtrlFx.h"
#include "GlobalWindowManager.h"
#include "QualityProgressCtrl.h"
#include <string>
#include <vector>

#define HCMBX_SEPARATOR _T("----------------")
#define HCMBX_CLEARLIST TRL("Clear Search History")

// #define CR_BACK RGB(216,216,235) /* Blue */
// #define CR_BACK RGB(240,236,224) /* WinXP */
#define CR_BACK (RGB(208,208,208)) /* Win2k */
#define CR_FRONT (RGB(0,0,0))

#ifndef LVS_EX_GRIDLINES
	#define LVS_EX_GRIDLINES        0x00000001
	#define LVS_EX_SUBITEMIMAGES    0x00000002
	#define LVS_EX_CHECKBOXES       0x00000004
	#define LVS_EX_TRACKSELECT      0x00000008
	#define LVS_EX_HEADERDRAGDROP   0x00000010
	#define LVS_EX_FULLROWSELECT    0x00000020
	#define LVS_EX_ONECLICKACTIVATE 0x00000040
	#define LVS_EX_TWOCLICKACTIVATE 0x00000080
#endif

#ifndef LVS_EX_FLATSB
	#define LVS_EX_FLATSB           0x00000100
	#define LVS_EX_REGIONAL         0x00000200
	#define LVS_EX_INFOTIP          0x00000400
	#define LVS_EX_UNDERLINEHOT     0x00000800
	#define LVS_EX_UNDERLINECOLD    0x00001000
	#define LVS_EX_MULTIWORKAREAS   0x00002000
#endif

#define LVS_EX_SI_MENU (/*LVS_EX_FLATSB|*/LVS_EX_ONECLICKACTIVATE|LVS_EX_UNDERLINEHOT)
#define LVS_EX_SI_REPORT (/*(LVS_EX_FLATSB)*/0)

#ifndef LVBKIF_SOURCE_NONE
#define LVBKIF_SOURCE_NONE      0x00000000
#define LVBKIF_SOURCE_HBITMAP   0x00000001
#define LVBKIF_SOURCE_URL       0x00000002
#define LVBKIF_SOURCE_MASK      0x00000003
#define LVBKIF_STYLE_NORMAL     0x00000000
#define LVBKIF_STYLE_TILE       0x00000010
#define LVBKIF_STYLE_MASK       0x00000010
#define LVBKIF_FLAG_TILEOFFSET  0x00000100
#define LVBKIF_TYPE_WATERMARK   0x10000000

typedef struct tagLVBKIMAGE
{
    ULONG ulFlags;
    HBITMAP hbm;
    LPTSTR pszImage;
    UINT cchImageMax;
    int xOffsetPercent;
    int yOffsetPercent;
} LVBKIMAGE, FAR *LPLVBKIMAGE;
#endif

#ifndef LWM_SETBKIMAGE
#define LVM_SETBKIMAGEA (LVM_FIRST + 68)
#define LVM_SETBKIMAGEW (LVM_FIRST + 138)
#define LVM_GETBKIMAGEA (LVM_FIRST + 69)
#define LVM_GETBKIMAGEW (LVM_FIRST + 139)
#ifdef UNICODE
#define LVM_SETBKIMAGE LVM_SETBKIMAGEW
#define LVM_GETBKIMAGE LVM_GETBKIMAGEW
#else
#define LVM_SETBKIMAGE LVM_SETBKIMAGEA
#define LVM_GETBKIMAGE LVM_GETBKIMAGEA
#endif
#endif

#ifndef SB_SETTIPTEXT
#define SB_SETTIPTEXTA		(WM_USER+16)
#define SB_SETTIPTEXTW		(WM_USER+17)
#ifdef UNICODE
#define SB_SETTIPTEXT SB_SETTIPTEXTW
#else
#define SB_SETTIPTEXT SB_SETTIPTEXTA
#endif
#endif

#ifndef SBT_TOOLTIPS
#define SBT_TOOLTIPS 0x0800
#endif

#ifndef ListView_SetBkImage
#define ListView_SetBkImage(hwnd, plvbki) \
    (BOOL)SNDMSG((hwnd), LVM_SETBKIMAGE, 0, (LPARAM)(plvbki))
#define ListView_GetBkImage(hwnd, plvbki) \
    (BOOL)SNDMSG((hwnd), LVM_GETBKIMAGE, 0, (LPARAM)(plvbki))
#endif

#ifndef CDRF_NOTIFYSUBITEMDRAW
#define CDRF_NOTIFYSUBITEMDRAW 0x00000020
#define CDDS_SUBITEM           0x00020000
#endif

#ifndef TCM_HIGHLIGHTITEM
#define TCM_HIGHLIGHTITEM (TCM_FIRST + 51)
#endif

typedef struct tagNMLVCUSTOMDRAWX
{
    NMCUSTOMDRAW nmcd;
    COLORREF clrText;
    COLORREF clrTextBk;
    int iSubItem;
} NMLVCUSTOMDRAWX, *LPNMLVCUSTOMDRAWX;

#ifndef COLOR16
typedef USHORT COLOR16;
#endif

typedef struct _MY_TRIVERTEX
{
    LONG    x;
    LONG    y;
    COLOR16 Red;
    COLOR16 Green;
    COLOR16 Blue;
    COLOR16 Alpha;
} MY_TRIVERTEX, *MY_PTRIVERTEX, *MY_LPTRIVERTEX;

typedef struct _MY_GRADIENT_TRIANGLE
{
    ULONG Vertex1;
    ULONG Vertex2;
    ULONG Vertex3;
} MY_GRADIENT_TRIANGLE, *MY_PGRADIENT_TRIANGLE, *MY_LPGRADIENT_TRIANGLE;

typedef struct _MY_GRADIENT_RECT
{
    ULONG UpperLeft;
    ULONG LowerRight;
} MY_GRADIENT_RECT, *MY_PGRADIENT_RECT, *MY_LPGRADIENT_RECT;

#ifndef GRADIENT_FILL_RECT_H
#define GRADIENT_FILL_RECT_H    0x00000000
#define GRADIENT_FILL_RECT_V    0x00000001
#define GRADIENT_FILL_TRIANGLE  0x00000002
#define GRADIENT_FILL_OP_FLAG   0x000000ff
#endif

// No balloon support
#ifndef TTS_BALLOON
#define TTS_BALLOON 0
#endif

#ifndef BCM_SETSHIELD
#define BCM_SETSHIELD 0x160C
#endif

#ifndef WDA_MONITOR
#define WDA_NONE    0x00000000
#define WDA_MONITOR 0x00000001
#endif

void NewGUI_Init();
void NewGUI_CleanUp();

BOOL* NewGUI_GetPreventScreenCapturePtr();

void NewGUI_InitDialog(CDialog* pDlg, bool bTranslate = true);

COLORREF NewGUI_GetBgColor();
COLORREF NewGUI_GetBtnColor();
// COLORREF NewGUI_LightenColor(COLORREF crColor, double dblFactor);

void NewGUI_SetImgButtons(BOOL bImageButtons);
// void NewGUI_Button(void *pButton, int nBitmapIn = -1, int nBitmapOut = -1, BOOL bForceImage = FALSE);
void NewGUI_XPButton(CXPStyleButtonST& rButton, int nBitmapIn, int nBitmapOut, BOOL bForceImage = FALSE);
void NewGUI_ToolBarButton(void *pButton, int nBitmapIn = -1, int nBitmapOut = -1);

void NewGUI_SetThemeHelper(void *pThemeHelper);

void NewGUI_ConfigQualityMeter(CQualityProgressCtrl *pWnd);
void NewGUI_ShowQualityMeter(CQualityProgressCtrl *pProgress, CWnd *pStatic,
	LPCTSTR pszPassword);

void NewGUI_TranslateCWnd(CWnd *pWnd);

BOOL CALLBACK NewGUI_TranslateWindowCb(HWND hWnd, LPARAM lParam);

void NewGUI_ConfigSideBanner(void *pBanner, void *pParentWnd);

BOOL NewGUI_GetHeaderOrder(HWND hwListCtrl, INT *pOrder, INT nColumnCount);
BOOL NewGUI_SetHeaderOrder(HWND hwListCtrl, INT *pOrder, INT nColumnCount);

void NewGUI_MakeHyperLink(void *pXHyperLink);

void NewGUI_DisableHideWnd(CWnd *pWnd);
void NewGUI_MoveWnd(CWnd *pWnd, long lMoveRightPixels, long lMoveDownPixels,
	CWnd *pParent);
void NewGUI_Resize(CWnd *pWnd, long lAddX, long lAddY, CWnd *pParent);

void NewGUI_SetWin32Banner();
void NewGUI_SetBannerColors(COLORREF crStart, COLORREF crEnd, COLORREF crText);
void NewGUI_SetBannerParams(bool bBannerFlip);

BOOL NewGUI_RemoveMenuCommand(BCMenu *pMenu, UINT uCommandID);
void NewGUI_RemoveInvalidSeparators(BCMenu *pMenu, BOOL bIsTopLevel);

BCMenu *NewGUI_GetBCMenu(CMenu *pMenu);

CSize NewGUI_GetWndBasePosDiff(CWnd *pWnd1, CWnd *pWnd2);

void NewGUI_SetCueBanner_TB(HWND hTextBox, LPCTSTR lpText);
void NewGUI_SetCueBanner_CB(HWND hComboBox, LPCTSTR lpText);

void NewGUI_ComboBox_GetInfo(HWND hComboBox, HWND* phComboBox,
	HWND* phEditBox, HWND* phListBox);
bool NewGUI_ComboBox_HasFocus(HWND hComboBox, HWND hCurrentFocus);

void NewGUI_ComboBox_UpdateHistory(CComboBox& comboBox,
	const std::basic_string<TCHAR>& strNew,
	std::vector<std::basic_string<TCHAR> >* pvHistoryItems,
	size_t dwMaxHistoryItems);

BOOL NewGUI_SetIcon(BCMenu& rMenu, UINT uCommand, int nResourceID);

int CALLBACK NgslCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
void NewGUI_SortList(CListCtrl* pListCtrl);

void NewGUI_AppendToRichEditCtrl(CAutoRichEditCtrlFx* pCtrl, LPCTSTR lpAppend,
	bool bScrollToBottom);

void NewGUI_DeselectAllItems(CListCtrl* pCtrl);
void NewGUI_EnableSortHeader(CListCtrl* pCtrl, bool bEnable);

void NewGUI_SetShield(CButton& rButton, BOOL bSetShield);

int NewGUI_Scale(int nPixels, CWnd* pParentWindow);
CSize NewGUI_Scale(const CSize& rSize, CWnd* pParentWindow);

COLORREF NewGUI_ColorToGrayscale(COLORREF clr);
COLORREF NewGUI_ColorMid(COLORREF clrA, COLORREF clrB);

void NewGUI_InitGDIPlus();
void NewGUI_TerminateGDIPlus();

void NewGUI_PumpMessages(HWND hWnd = NULL, UINT wMsgFilterMin = 0,
	UINT wMsgFilterMax = 0);

INT_PTR NewGUI_DoModal(CDialog* pDlg);

SIZE NewGUI_GetWindowContentSize(HWND hWnd);

// void NewGUI_ResetFocus(CWnd* pToFocus);

BOOL NewGUI_GetNonClientMetrics(NONCLIENTMETRICS* p);

void NewGUI_CustomizeWindow(CWnd* pWnd, bool bInit);

#endif // ___NEW_GUI_COMMON___
