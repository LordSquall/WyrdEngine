echo off

call buildsystem\windows\externaltools\premake5.exe vs2022

msbuild Wyrd.sln -m
