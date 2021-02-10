// Some Visual Studio 2017 Deprecated Functions


#include "StdAfx.h"
#include "Deprecated.h"

int version() {
  #pragma warning(push)
  #pragma warning(disable: 4996) // SCL warning
  return (int) GetVersion();
  #pragma warning(pop)
  }


void versionEx(OSVERSIONINFO& versionInfo) {

  ZeroMemory(&versionInfo, sizeof(OSVERSIONINFO));

  versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

  #pragma warning( push )
  #pragma warning( disable : 4996 )

    GetVersionEx(&versionInfo);

  #pragma warning( pop )
  }

