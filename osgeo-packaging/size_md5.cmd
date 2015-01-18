@echo off

if not "%~1"=="" (
  copy /Y %~nx1.template %~nx1.ini
  for /f %%i in ('cmake -E md5sum %1') do echo  %~z1 %%i >> %~nx1.ini
)
