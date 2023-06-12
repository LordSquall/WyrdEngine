echo off

call buildsystem\windows\externaltools\premake5.exe vs2019

msbuild Wyrd.sln -m
