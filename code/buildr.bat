@echo off

pushd ..\build
cl /ZI /EHsc ..\code\main.cpp
echo.
main.exe
popd