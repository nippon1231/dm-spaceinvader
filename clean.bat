@echo off
echo Nettoyage du projet...

if exist out rmdir /s /q out
if exist rom.bin del rom.bin
if exist *.o del *.o
if exist res\resources.s del res\resources.s

echo Nettoyage terminé!
pause
