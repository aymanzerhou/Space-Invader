@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cl /EHsc space_invaders_console.cpp /Fe:SpaceInvaders.exe
if exist SpaceInvaders.exe (
    echo Compilation successful!
    echo Running Space Invaders...
    SpaceInvaders.exe
) else (
    echo Compilation failed!
)
pause
