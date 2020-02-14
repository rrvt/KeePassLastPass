@ECHO off

REM Tailor the backup based on what computers do we work on
IF  %COMPUTERNAME%==WORKLAPTOP GOTO Section-WorkLaptop
IF  %COMPUTERNAME%==WORKLAPTOP GOTO Section-MyPersonalLaptop
GOTO Default

:Section-WorkLaptop
ECHO Use FTP Upload on company machine
REM WARNING: This method stores the name/password in clear-text format. Consider this a security risk.
REM Nevertheless, the backed up DB is strongly encrypted

REM Set the user name
echo user MyUserName> ftpcmd.dat
REM Set the password
echo MyPassword>> ftpcmd.dat
echo bin>> ftpcmd.dat
echo put %1>> ftpcmd.dat
echo quit>> ftpcmd.dat
REM Change mypersonal.ftpserver.com to your desired ftp server with path
ftp -n -s:ftpcmd.dat mypersonal.ftpserver.com/private/mykeepassDB
del ftpcmd.dat
GOTO :EOF
 
:Section-MyPersonalLaptop
REM setting the right drive and working directory
%~d1
cd %~p1

REM Dont back up on my laptop @ work
IF %COMPUTERNAME%=="PANTHERX60" ECHO "No backup" 


REM do the actual work...
FOR /F "tokens=1-5 delims=:./ " %%I IN ('ECHO %~t1') DO SET mydate=%%I%%J%%K%%L%%M

ren %~n1%~x1 %mydate%-%~n1%~x1
REM echo APP started for: %1 at 'ECHO %!~t1' > %1.log
GOTO :EOF

:default
REM Do here some default stuff
ECHO Doing the default branch - no match with predefined computer names"
GOTO :EOF
