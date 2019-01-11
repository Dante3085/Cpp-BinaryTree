@echo off

pushd ..\build
cl /ZI /EHsc ..\code\BinaryTree.cpp
echo.
echo RUNNING BinaryTree.exe ...
BinaryTree.exe
popd