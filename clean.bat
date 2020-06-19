rmdir /Q /S .\externalbuild\bin
rmdir /Q /S .\externalbuild\bin-int

del /s /q /f .\externalbuild\*.vcxproj
del /s /q /f .\externalbuild\*.vcxproj.user
del /s /q /f .\externalbuild\*.vcxproj.filters
del /s /q /f .\externalbuild\*.sln

del /s /q /f .\*.vcxproj
del /s /q /f .\*.vcxproj.user
del /s /q /f .\*.vcxproj.filters
del /s /q /f .\*.sln
