@echo off
for /f "delims=" %%i in ('dir /s/b/ad build') do (
rd /s/q "%%~i"
)

for /f "delims=" %%i in ('dir /s/b/ad .vscode') do (
rd /s/q "%%~i"
)

exit

