@echo off
setlocal enabledelayedexpansion

echo ============================================
echo Compilation de Galaga pour Mega Drive
echo ============================================
echo.

REM Vérifier SGDK
if "%GDK%"=="" (
    echo ERREUR: Variable GDK non definie!
    echo Installez SGDK et configurez la variable GDK
    pause
    exit /b 1
)

echo SGDK: %GDK%
echo.

REM Vérifier Java
java -version >nul 2>&1
if errorlevel 1 (
    echo ERREUR: Java n'est pas installe ou pas dans le PATH!
    echo Telechargez Java depuis https://www.java.com
    pause
    exit /b 1
)

REM Créer les dossiers de sortie
if not exist out mkdir out
if not exist out\src mkdir out\src
if not exist out\res mkdir out\res

echo [1/4] Compilation des ressources avec rescomp...
java -jar "%GDK%\bin\rescomp.jar" res\resources.res out\res\resources.s
if errorlevel 1 (
    echo ERREUR: rescomp a echoue!
    echo Verifiez que vos sprites PNG sont corrects
    pause
    exit /b 1
)
echo   OK - resources.s genere

echo [2/4] Assemblage des ressources...
"%GDK%\bin\m68k-elf-as.exe" -m68000 --register-prefix-optional out\res\resources.s -o out\res\resources.o
if errorlevel 1 (
    echo ERREUR: Assemblage des ressources a echoue!
    pause
    exit /b 1
)
echo   OK - resources.o cree

echo [3/4] Compilation des fichiers C...
set "CFLAGS=-m68000 -Wall -Wextra -O3 -fno-builtin -fomit-frame-pointer"
set "INCLUDES=-I. -I%GDK%\inc -Iinc -Ires"

for %%f in (src\*.c) do (
    echo   Compilation: %%~nf.c
    "%GDK%\bin\m68k-elf-gcc.exe" !CFLAGS! !INCLUDES! -c %%f -o out\src\%%~nf.o
    if errorlevel 1 (
        echo ERREUR: Compilation de %%f a echoue!
        pause
        exit /b 1
    )
)
echo   OK - Tous les fichiers C compiles

echo [4/4] Edition des liens...
"%GDK%\bin\m68k-elf-gcc.exe" -m68000 -T "%GDK%\md.ld" -nostdlib out\src\*.o out\res\resources.o "%GDK%\lib\libmd.a" -lgcc -o out\rom.out
if errorlevel 1 (
    echo ERREUR: Edition des liens a echoue!
    pause
    exit /b 1
)
echo   OK - rom.out cree

echo [5/5] Creation de la ROM binaire...
"%GDK%\bin\m68k-elf-objcopy.exe" -O binary out\rom.out out\rom.bin
if errorlevel 1 (
    echo ERREUR: Creation de la ROM a echoue!
    pause
    exit /b 1
)

copy out\rom.bin galaga.bin >nul 2>&1

echo.
echo ============================================
echo   COMPILATION REUSSIE!
echo ============================================
echo ROM creee: out\rom.bin
echo Copie    : galaga.bin
echo.
echo Testez avec un emulateur Mega Drive:
echo - Gens/GS
echo - Blastem  
echo - RetroArch
echo ============================================
echo.
pause
