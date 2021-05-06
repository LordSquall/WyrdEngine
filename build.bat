:: Run Code generation
:: NOTE: requires testtransform and msbuild to the system path
::"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\msbuild.exe" codegen\models\CodeGenModels.csproj
::"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\TextTransform.exe" codegen\api\PlayerAPI.tt


call toolchain\premake\premake5.exe vs2019

PAUSE