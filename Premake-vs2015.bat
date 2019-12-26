@echo off
external\bin\premake5.exe --file=premake/static.lua vs2015 [LinkLibraryDependencies]
if %ERRORLEVEL% NEQ 0 PAUSE