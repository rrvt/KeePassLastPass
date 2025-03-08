DEL ..\KeePass.ncb
DEL /A:H ..\KeePass.suo

RMDIR /S /Q KeePassLibC
RMDIR /S /Q KeePassMsi
RMDIR /S /Q WinGUI_Bin
RMDIR /S /Q WinGUI_Distrib

DEL /A:H ..\Ext\KeePassMsi\KeePassMsi.suo

DEL ..\KeePassLibC\KeePassLib.aps
DEL ..\KeePassLibC\KeePassLib.ncb
DEL /A:H ..\KeePassLibC\KeePassLib.suo
DEL /Q ..\KeePassLibC\*.user

DEL ..\WinGUI\PwSafe.aps
DEL ..\WinGUI\PwSafe.ncb
DEL /A:H ..\WinGUI\PwSafe.suo
DEL /Q ..\WinGUI\*.user

CLS