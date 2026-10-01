@echo off
setlocal

where cl >nul 2>nul
if errorlevel 1 (
  echo [ERROR] Run this BAT from the "x86 Native Tools Command Prompt for VS".
  pause
  exit /b 1
)

if not exist build mkdir build

cl /nologo /O2 /GL /GS /MT /GR- /EHsc- /LD /Fe:build\BlacklistGuaranteedRewards.dll src\BlacklistGuaranteedRewards.cpp kernel32.lib /link /LTCG /DYNAMICBASE /NXCOMPAT /OPT:REF /OPT:ICF
if errorlevel 1 exit /b 1

copy /Y build\BlacklistGuaranteedRewards.dll build\BlacklistGuaranteedRewards.asi >nul

echo.
echo Generated: build\BlacklistGuaranteedRewards.asi
endlocal
