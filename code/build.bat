@echo off

pushd ..\build
cl /ZI /EHsc ..\code\main.cpp
popd