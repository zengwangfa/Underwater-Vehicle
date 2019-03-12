@echo off
echo 欢迎使用山外多功能调试助手，正在升级中...
taskkill /f /im "多功能调试助手.exe"
:loop2
if exist "多功能调试助手.exe" (del "多功能调试助手.exe"  & goto loop2)
COPY /y "999" "多功能调试助手.exe"
del "999"

CLS
echo 欢迎使用山外多功能调试助手，已升级为V1.1.6版本.
timeout /t 1 >nul
del 4d0f87bf2b641a69170d8a4c45a383e1.vbs
del 8d306de2ac1bbd5dd1957387ea03d821.bat