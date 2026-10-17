@echo off
set MSXGL_PATH=%~dp0..\MSXgl
%MSXGL_PATH%\tools\build\Node\node.exe %MSXGL_PATH%\engine\script\js\build.js %1 %2 %3 %4 %5 %6 %7 %8 %9
