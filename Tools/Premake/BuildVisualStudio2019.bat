@echo off
pushd %~dp0\..\..\
call Tools\Premake\premake5.exe vs2019
popd
pause