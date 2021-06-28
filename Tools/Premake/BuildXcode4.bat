@echo off
pushd %~dp0\..\..\
call Tools\Premake\premake5.exe xcode4
popd
pause