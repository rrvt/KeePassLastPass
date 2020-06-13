# KeePass/LastPass Extension

This is primarily a Last Pass Import/Export Extension to KeePass 1.38 (C++ Version).

I like Password Vaults and a couple of years ago the password vault I was using got a little confused and
deleted all my passwords.  I abandoned that vault and decided that the next vault I used would have a
backup.  I chose LastPass based on comments but I was determined to save the passwords on my personal
computer too.  So KeePass also received good marks and the source is available.  Being a veteran C++
SE I chose it in the C++ version.  Last Pass keeps changing its export function so this version will
process the latest version of its export file (Early 2020).  Since It might be necessary to restore the
passwords to LastPass there is an export function included in the KeePass/LastPass extension.  It will
produce the same file as what was imported.  I have not attempted to load it into LastPass...

## Getting Started

Compiled with Visual Studio 2017 (VS17).  It requires the Boost Libraries:  https://www.boost.org/ and
the WiX Toolset extension to VS17.  The boost library links may need adjusting in the project properties.

Do a difference between the latest KeePass (1.38 at the time of writing) and this code to get an idea
of the changes I made in KeePass.  Note the directory name changes:
  - WinGUI was changed to KeePass
  - KeePassLibC was changed to KeePassLib

### Prerequisites

I put the Boost directory at the same level as the KeePass directory.  The Boost Library must be compiled.
I chose the complete command:
```
   b2 --build-dir=D:\SandBox\boost_1_72\Build toolset=msvc --build-type=complete  stage >build.log 2>&1
```
The Extension "Wix Toolset visual Studio 2017 Extension" is also required.  It can be found in
"visual Studio 2017/Tools/Extensions and Updates".

Changes to the Installer may made by editing the "Product.wxs" file or obtaining the WixApp on Git.

### Installing

Run the KeePassInstaller.msi to install KeePass.

## Built With

Visual Studio 2017 in C++.

## Authors

KeePass Authors for most of it.
Robert Van Tuyl for the LastPass Extension and some minor additions to the ini file functions.  Other
changes in KeePass code were made to suppress noisy warnings (mostly about version but a few others too).

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

The current and former KeePass Authors deserve a big thank you for this application.

