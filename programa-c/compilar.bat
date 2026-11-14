@echo off
rem Compila e abre o SafeHouse em C.
rem Usa o gcc do PATH ou, se nao achar, o gcc que vem com o Dev-C++.
chcp 65001 >nul
set GCC=gcc
where gcc >nul 2>nul || set GCC="C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\gcc.exe"
%GCC% -Wall -Wextra -std=c99 -o safehouse.exe main.c sensores.c eventos.c util.c
if errorlevel 1 (
  echo.
  echo Erro na compilacao. Verifique se o gcc esta instalado.
  pause
  exit /b 1
)
echo Compilado sem erros. Abrindo o programa...
echo.
safehouse.exe
pause
