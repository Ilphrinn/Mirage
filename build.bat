@echo off
echo ===============================
echo [Compilation du projet SDL]
echo ===============================

REM Étape 1 : créer le dossier build s'il n'existe pas
if not exist build (
    mkdir build
)

REM Étape 2 : configuration avec CMake (mise à jour automatique)
cmake -S . -B build

REM Étape 3 : compilation
cmake --build build

REM Pause pour lire les erreurs si besoin
echo.
echo ===============================
echo [Compilation terminee]
echo ===============================
pause
