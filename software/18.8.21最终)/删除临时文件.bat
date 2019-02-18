::@echo off 

@for /d /r %%c in (Debug,settings,Release,FX15_Debug,FX15_Release,DZ10_Debug,DZ10_Release) do @if exist %%c ( rd /s /q "%%c" & echo     É¾³ýÄ¿Â¼%%c) 

@for /r  %%c in (*.tmp,*.bak ,*.dep,*.sfr ) do del "%%c"
pause