// Some Visual Studio 2017 Deprecated Functions


#include "StdAfx.h"
#include "Deprecated.h"


CurrentOS currentOS;

#if 0
int versionDprX() {
  #pragma warning(push)
  #pragma warning(disable: 4996) // SCL warning
  return (int) GetVersion();
  #pragma warning(pop)
  }


void versionExDprX(OSVERSIONINFO& versionInfo) {

  ZeroMemory(&versionInfo, sizeof(OSVERSIONINFO));

  versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

  #pragma warning( push )
  #pragma warning( disable : 4996 )

    GetVersionEx(&versionInfo);

  #pragma warning( pop )
  }
#endif


/*
Windows 1.0                 --  1.04
Windows 2.0                 --  2.11

Windows 3.0                 --  3
Windows NT 3.1              --  3.10.528
Windows for Workgroups 3.11 --  3.11
Windows NT Workstation 3.5  --  3.5.807
Windows NT Workstation 3.51 --  3.51.1057

Windows 95                  --  4.0.950
Windows NT Workstation 4.0  --  4.0.1381
Windows 98                  --  4.1.1998
Windows 98 Second Edition   --  4.1.2222
Windows Me                  --  4.90.3000

Windows 2000 Professional   --  5.0.2195
Windows XP                  --  5.1.2600

Windows Vista               --  6.0.6000
Windows 7                   --  6.1.7600
Windows 8.1                 --  6.3.9600

Windows 10                  -- 10.0.10240

List of all Windows OS Version Numbers –
msigeek https://www.msigeek.com/442/windows-os-version-numbers


typedef struct _OSVERSIONINFOW {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    WCHAR  szCSDVersion[ 128 ];     // Maintenance string for PSS usage
} OSVERSIONINFOW, *POSVERSIONINFOW, *LPOSVERSIONINFOW, RTL_OSVERSIONINFOW, *PRTL_OSVERSIONINFOW;
*/


CurrentOS::CurrentOS() : is(NilWin) {
OSVERSIONINFO info = {sizeof(info)};

  #pragma warning( push )
  #pragma warning( disable : 4996 )

    GetVersionEx(&info);

  #pragma warning( pop )

  maj = info.dwMajorVersion;
  mnr = info.dwMinorVersion;
  bld = info.dwBuildNumber;

  switch (maj) {
    case  1 :
    case  2 : is = NilWin; break;
    case  3 : switch (mnr) {
                case  0 : is = Win3;   break;
                case 10 : is = WinNT;  break;
                case 11 : is = Win31;  break;
                case  5 : is = WinNT1; break;
                default : is = WinNT2; break;
                }
              break;

    case  4 : switch (mnr) {
                case  0 : is = bld >=  950 && bld < 1381 ? Win95 : WinNT4;  break;
                case  1 : is = bld >= 1998 && bld < 2222 ? Win98 : Win98Snd; break;
                default : is = WinMe;                                        break;
                }
              break;
    case  5 : is = mnr == 0 ? Win2K : WinXP;
    case  6 : switch (mnr) {
                case  0 : is = WinVista; break;
                case  1 : is = Win7;     break;
                default : is = Win81;    break;
                }
              break;

    case 10 : is = Win10; break;
    case  7 :
    case  8 :
    case  9 :
    default : is = NilWin; break;
    }
  }

