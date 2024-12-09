del /f /s /q dependencies 1>nul
rmdir /S /Q dependencies

del /f /s /q lib 1>nul
rmdir /S /Q lib

del /f /s /q obj 1>nul
rmdir /S /Q obj

del /f /s /q bin 1>nul
rmdir /S /Q bin

del Wyrd.sln