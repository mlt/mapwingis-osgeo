@echo off

rem Adjust the following as necessary
set MAPWINDOW_DIR=%~dp0..\..\..\MapWindow4Dev
set WORK_DIR=%~dp0_temp
set OUT_DIR=%~dp0_out

rmdir /S /Q "%WORK_DIR%" "%OUT_DIR%"
xcopy "%MAPWINDOW_DIR%\Bin" "%WORK_DIR%\apps\MapWindow" /d /s /q /i
mkdir "%WORK_DIR%\bin" "%OUT_DIR%\x86"
copy /Y "%~dp0MapWindow.cmd" "%WORK_DIR%\bin\MapWindow.cmd"

rem Note thatwhen building MapWindow with COM isolation, OCX is copied automatically
rem Perhaps it can be symlinked to OCX in \OSGeo4W\bin\ on NTFS

pushd "%WORK_DIR%"
cmake -E tar cpjf "%OUT_DIR%\x86\mapwindow.tar.bz2" apps bin
popd
call size_md5.cmd "%OUT_DIR%\x86\mapwindow.tar.bz2"


pushd C:\OSGeo4W
cmake -E tar cpjf "%OUT_DIR%\x86\mapwingis-debug.tar.bz2" bin\MapWinGIS_d.ocx bin\MapWinGIS_d.pdb
cmake -E tar cpjf "%OUT_DIR%\x86\mapwingis-release.tar.bz2" bin\MapWinGIS.ocx
popd

call size_md5.cmd "%OUT_DIR%\x86\mapwingis-debug.tar.bz2"
call size_md5.cmd "%OUT_DIR%\x86\mapwingis-release.tar.bz2"

rmdir /S /Q "%WORK_DIR%"

for /f "delims=" %%x in ('cscript /nologo ts.vbs') do echo setup-timestamp: %%x > timestamp
copy timestamp + /b *.ini "%OUT_DIR%\x86\setup.ini"
