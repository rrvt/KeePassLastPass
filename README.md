# KeePass/LastPass Extension

This is primarily a Last Pass Import/Export Extension to KeePass 1.40 (C++ Version).

I like Password Vaults and a few years ago the password vault I was using got a little confused and
deleted all my passwords.  I abandoned that vault and decided that the next vault I used would have a
backup.  I chose LastPass based on comments but I was determined to save the passwords on my personal
computer too.  So KeePass also received good marks and the source is available.  Being a veteran C++
SE I chose it in the C++ version.  Last Pass keeps changing its export function so this version will
process the latest version of its export file (Early 2022).  Since It might be necessary to
restore the passwords to LastPass there is an export function included in the KeePass/LastPass extension.
It will produce the same file as what was imported.  I have not attempted to load it into LastPass...

## Getting Started

Compiled with Visual Studio 2017 (VS17).  KeePass v1.40 requires the Boost version 1.78 Libraries:
https://www.boost.org/ and the WiX Toolset extension to VS17.
The boost library links may need adjusting in the project properties.

Do a difference between the latest KeePass (1.39 at the time of writing) and this code to get an idea
of the changes I made in KeePass.  Note the Project name:

  - Project KeePassLibC name was changed to KeePassLib
  - Project Deprecated was added as a static library
  - Project LastPass was added as a dll
  - Project Installer was added as a Wix installer (see WixApp)

### Prerequisites

I put the Boost directory at the same level as the KeePass directory.  The Boost Library must be compiled.
I chose the complete command:
```
   b2 --build-dir=D:\SandBox\boost.1.75\Build toolset=msvc --build-type=complete  stage >build.log 2>&1
```
The Extension "Wix Toolset visual Studio 2017 Extension" is also required.  It can be found in
"visual Studio 2017/Tools/Extensions and Updates".

Changes to the Installer may made by editing the "Product.wxs" file or obtaining the WixApp on Git.

### Installing

Run the KeePass.msi to install KeePass.

## Updates

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
done away with precompiled headers, period.  And, whoops, just setting the VS17 property to "Not Using
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
Furthermore, KeePass is built for many different compilers and Visual Studio 2017 deprecates GetVersion
and GetVersionEx so I've moved those two functions into a Deprecated Library and enclosed them in
pragmas to avoid the warning.

Modified the Interator a bit to model IterT in the Library even though the guts use KeePassLib to
actually return the entries.  Made the duplicate hunting a bit more stringent.  The user will need to
resolve the duplicates that are just close to each other.

## Built With

Visual Studio 2017 in C++.

## Authors

KeePass Authors for most of it.
Robert Van Tuyl for the LastPass Extension and some minor additions to the ini file functions.

Contact:  romeo romeo victor tango alpha tango sam whisky delta echo dot charlie oscar mike

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

The current and former KeePass Authors deserve a big thank you for this application.

