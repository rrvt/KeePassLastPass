# KeePass/LastPass Extension

This is primarily a Last Pass Import/Export Extension to KeePass 1.41 (C++ Version).

I like Password Vaults and a few years ago the password vault I was using got a little confused and
deleted all my passwords.  I abandoned that vault and decided that the next vault I used would have
a backup.  I chose LastPass based on comments but I was determined to save the passwords on my
personal computer too.  So KeePass also received good marks and the source is available.  Being a
veteran C++ SE I chose it in the C++ version.  Last Pass keeps changing its export function so this
version will process the latest version of its export file (Early 2022).  Since It might be
necessary to restore the passwords to LastPass there is an export function included in the
KeePass/LastPass extension.  It will produce the same file as what was imported.  I have not
attempted to load it into LastPass...

## Getting Started

Compiled with Visual Studio 2022 (VS22).  KeePass v1.41 requires the Boost version 1.81 Libraries:
https://www.boost.org/ and the WiX Toolset extension to VS22.
The boost library links may need adjusting in the project properties.

Do a difference between the latest KeePass (1.39 at the time of writing) and this code to get an
idea of the changes I made in KeePass.  Note the Project name:

  - Project Deprecated was added as a static library
  - Project KeePassLibC name was changed to KeePassLib
  - Project Installer was added as a Wix installer (see WixApp)
  - Project LastPass was added as a dll
  - Project LibraryMB was added as a static library

### Prerequisites

I put the Boost directory (v 1.81) at the same level as the KeePass directory.  The Boost Library
must be compiled.  I chose the complete command:
```
   b2 --build-dir=D:\SandBox\boost\Build toolset=msvc --build-type=complete  stage >build.log 2>&1
```
The Extension "Wix Toolset visual Studio 2022 Extension" is also required.  It can be found in
"visual Studio 2022/Tools/Extensions and Updates".

Changes to the Installer may made by editing the "Product.wxs" file or obtaining the WixApp on Git.

### Installing

Run the KeePass.msi to install KeePass.

## Updates

### 9/17/25

Update KeePass 1.43.1.

### Update 3/7/25

Downloaded KeePass Version 1.43.  Removed/resolved compile errors/warnings in VS 2022.  Integrated
LastPass import/export commands.

### Update 1/1/25

Removed redundant classes from LastPass Export/Import commands.  Cleanup source.

### Update 12/29/24

Downloaded Version 1.42, modified it to remove warnings and errors and added LastPass toolbar
commands.

### Update 1/7/24

### Update 2/17/24

Reorganized the Library so that it compiles once into Debug and Release directories under the
Library directory.

Library and other changes.

### Update 12/25/23

Update LibraryMB from the Library and subsequent ripple changes.

### Update 4/11/23

Update Boost to 1.81 and recompiled Boost.  Renamed Boost from boost.1.78 to just boost.  Recompiled
KeePass with the new Boost.  Tested the LastPass import and it seemed fine.  Did not test exporting
from KeePass to LastPass because KeePass remains the backup as yet unused as backup.  I have used it
from time to time as it is sometimes easier than LastPass.

### Update 4/8/23

KeePass checks for updates and 1.41 was announced by KeePass.  So I downloaded the new version.  The
real problem is that there are substantial changes to both allow it to compile under Windows compiler
(Visual Studio 2022) and to provide an interface to KeePass database from functions that will import and
export from and to LastPass.

The solution was to use SlickEdit's difference to find all the differences between the new version and
this version.  Of course it would show my changes and the KeePass 1.41 changes.  The new KeePass changes
were made in this version, built and installed.  I'll check the LastPass Import/Export later.

### Update 6/12/22

Primarily an update from the KeePass web site.  No changes were made to the LastPass add on.

Made one interesting addition to the Depricated
function: GetVersionEx.  I moved it to the CurrentOS class and it initialize on startup.  The addition
is:  if GetVersionEx returns Win8.1 as the OS (that is the maximum that it can return) a further look
in the Registry will yield whether the OS is Win10 or Win11.  Not an earth shattering addition but
maybe important down the road...

### Update 4/15/22

Big adventure in February when I started moving to a new Win11 and Visual Studio 2022.  I now
feel that this version is working in the new environment.  There were several changes made to the app
in order to compile properly on VS22.

There were additional warnings and errors due to the new compiler.  They have been fixed or suppressed.

### Update 1/30/22

Download the latest version of the c++ version of KeyPass, Version 1.40 and the current version of boost,
version 1.78.  Studied all the #pragma warnings and commented out those that were not needed.  moved
Deprecated, LibraryMB and LastPass directories from the last version to this version.  Fixed some minor
issues.

#### Precompiled Headers

When the applications are really large, precompiled headers makes a lot of sense.  However, from timt to
time I have discovered that changing a header did not always get reflected in the next build.  So I have
done away with precompiled headers, period.  And, whoops, just setting the VS22 property to "Not Using
Precompiled Headers" is not enough to make the files (i.e. somethin.pch) disappear from the build.  And
they can be large.  So I wrote a program to parse the project.vcxproj file and remove all references to
pch files.  It turned out that some project file references had references to individual pch files.  These
were removed.  Some simplification of the file is also accomplished at the same time.

### Update 11/13/21

But the major changes are in the library:

  - Expandable and ExpandableP -- moved the functions out of the class to make the compilation smaller,
also fixed one bug (or feature) in the ExpandableP insert sorted function that would throw out entries
that were already in in the array.
  - FileIO -- Changed the type of path from "String&" to "TCchar*".  It turns out that automatic
conversion from String to TCchar* is easy, but the reverse is is not easy.  Also added a function to
get the length of a file given a path.
  - FineName -- Added a function to get the directory name of from a path
  - NewAllocator -- Corrected the debug dependency from if to ifdef
  - ToolBar -- Added data for drop down menus

### Update 10/31/21

Changed Resources (i.e. xxx.rc data) access so that it will work with a dll.  Changed the logic in the
About Box code.  Some other library changes.  Some corresponding changes in the application.

### Update 10/30/21

Boost has moved to version 1.77, so I compiled that version, again at the same level as KeePass in the
directory tree.  The following script is used to compile boost:

```
rem first argument is the configuration (i.e. Debug or Release)
rem second argument is path to openSSL directory base (e.g. D:\Sources\OpenSSL.3.1\)
rem third argument is "static" to indicate a static library (rather than a dll library)

rem   "C:\Program Files\JPSoft\TCMD25\tcc" /C BuildForWin "%bn" "%rp" static/shared

echo on

set Config=%1
set Path=%2
set Dir=%Path%%Config%\


iff "%Config%" == "Debug" then
  set Variant=debug
else
  set Variant=release
endiff

iff "%3" == "static" then
  set Static=static
else
  set Static=shared
endiff


set Dirs=--build-dir=%Dir% --stagedir=%Dir%
set Options=--build-type=complete toolset=msvc link=%Static% address-model=32 runtime-link=shared
set Exclusions=--without-graph_parallel --without-mpi --without-python

b2 stage %Dirs% %Options% %Variant% %Exclusions% >build.log 2>&1

quit
```

The script may be called as follows to produce a Release version of the library:

```
tcc  /C BuildForWin Release D:\Sources\boost.1.77\ static
```

The script is run in jpsoftware's Take Command (i.e. tcc). Their websit is https://jpsoft.com/.

LastPass export and interanl structures seem to change from time to time so I've tried to keep up.
This version has moved sensitive numbers (e.g. credit card number/Security Code) into the password field
in an effort to hide those details better.  During export they are replaced in the notes where they were
found.  Note:  When two different entities need to be placed in the password field they are separated by
"<~>".

### Update 2/9/21

KeePass update this January to 1.39.  However, I had made some additions to KeePass to facilitate
adding a LastPass PlugIn.  So those needed to be forwarded from 1.38 version.  This was done using
SlickEdit's difference engine.
Furthermore, KeePass is built for many different compilers and Visual Studio 2022 deprecates GetVersion
and GetVersionEx so I've moved those two functions into a Deprecated Library and enclosed them in
pragmas to avoid the warning.

Modified the Interator a bit to model IterT in the Library even though the guts use KeePassLib to
actually return the entries.  Made the duplicate hunting a bit more stringent.  The user will need to
resolve the duplicates that are just close to each other.

## Built With

Visual Studio 2022 in C++.

## Authors

KeePass Authors for most of it.
Robert Van Tuyl for the LastPass Extension and some minor additions to the ini file functions.

Contact:  romeo romeo victor tango alpha tango sam whisky delta echo dot charlie oscar mike

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

The current and former KeePass Authors deserve a big thank you for this application.

