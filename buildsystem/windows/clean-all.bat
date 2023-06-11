del /f /s /q dependencies 1>nul
rmdir -r -f dependencies

del /f /s /q lib 1>nul
rmdir -r -f lib

del /f /s /q obj 1>nul
rmdir -r -f obj

del /f /s /q bin 1>nul
rmdir -r -f bin

del Wyrd.sln