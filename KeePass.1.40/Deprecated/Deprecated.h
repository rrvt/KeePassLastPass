// Some Visual Studio 2017 Deprecated Functions



#pragma once


class CurrentOS {

int  maj;
int  mnr;
int  bld;

public:

enum OS {NilWin,   Win3,  WinNT, Win31, WinNT1,   WinNT2, Win95, WinNT4, Win98,
         Win98Snd, WinMe, Win2K, WinXP, WinVista, Win7,   Win81, Win10};

OS is;

  CurrentOS();
 ~CurrentOS() { }
  };


extern CurrentOS currentOS;
