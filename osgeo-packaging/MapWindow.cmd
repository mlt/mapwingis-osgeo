@echo off
call "%~dp0\o4w_env.bat"
path %OSGEO4W_ROOT%\apps\MapWindow;%PATH%
set GDAL_FILENAME_IS_UTF8=YES
start "MapWindow" /B MapWindow.exe %*
