@echo off
pushd %~dp0\..\..\
call Tools\Premake\premake5.exe gmake2
popd
pause