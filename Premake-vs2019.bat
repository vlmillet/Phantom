@echo off
premake\premake5.exe --file=Phantom.lua vs2019
if %ERRORLEVEL% NEQ 0 PAUSE