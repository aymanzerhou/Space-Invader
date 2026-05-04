@echo off
setlocal enabledelayedexpansion

:: Initialize game variables
set /a playerX=40
set /a playerY=22
set /a score=0
set /a alienDirection=1
set /a gameOver=0
set /a won=0

:: Initialize aliens
set alienCount=0
for /l %%r in (1,1,4) do (
    for /l %%c in (1,1,10) do (
        set /a alienX[!alienCount!]=10+%%c*6
        set /a alienY[!alienCount!]=3+%%r*2
        set /a alienActive[!alienCount!]=1
        set /a alienCount+=1
    )
)

:: Initialize bullets
set bulletCount=0

:gameLoop
cls

:: Draw border
echo ###########################################################################
for /l %%i in (1,1,23) do (
    echo #                                                                     #
)
echo ###########################################################################

:: Draw score
echo Score: %score%

:: Draw player
set /a playerDrawX=%playerX%-2
gotoxy %playerDrawX% %playerY%
echo <-^->

:: Draw aliens
set /a activeAliens=0
for /l %%i in (0,1,39) do (
    if !alienActive[%%i]!==1 (
        gotoxy !alienX[%%i]! !alienY[%%i]!
        echo @
        set /a activeAliens+=1
    )
)

:: Draw bullets
for /l %%i in (0,1,%bulletCount%) do (
    if !bulletActive[%%i]!==1 (
        gotoxy !bulletX[%%i]! !bulletY[%%i]!
        echo |
    )
)

:: Check win condition
if %activeAliens%==0 (
    set /a gameOver=1
    set /a won=1
)

:: Check lose condition
for /l %%i in (0,1,39) do (
    if !alienActive[%%i]!==1 (
        if !alienY[%%i]! geq %playerY% (
            set /a gameOver=1
            set /a won=0
        )
    )
)

:: Game over screen
if %gameOver%==1 (
    cls
    if %won%==1 (
        echo YOU WIN!
    ) else (
        echo GAME OVER!
    )
    echo Final Score: %score%
    echo Press R to restart or any other key to exit...
    choice /c:rq /n
    if errorlevel 2 goto end
    goto restart
)

:: Handle input
timeout /t 1 /nobreak >nul
choice /c: lr  /n >nul
if errorlevel 3 (
    :: Space - shoot
    set /a bulletX[%bulletCount%]=%playerX%
    set /a bulletY[%bulletCount%]=%playerY%-1
    set /a bulletActive[%bulletCount%]=1
    set /a bulletCount+=1
) else if errorlevel 2 (
    :: Right arrow
    if %playerX% lss 77 set /a playerX+=1
) else if errorlevel 1 (
    :: Left arrow
    if %playerX% gtr 3 set /a playerX-=1
)

:: Update bullets
for /l %%i in (0,1,%bulletCount%) do (
    if !bulletActive[%%i]!==1 (
        set /a bulletY[%%i]-=1
        if !bulletY[%%i]! lss 1 (
            set /a bulletActive[%%i]=0
        )
    )
)

:: Move aliens
set /a changeDirection=0
for /l %%i in (0,1,39) do (
    if !alienActive[%%i]!==1 (
        if !alienX[%%i]! leq 2 (
            set /a changeDirection=1
        )
        if !alienX[%%i]! geq 77 (
            set /a changeDirection=1
        )
    )
)

if %changeDirection%==1 (
    set /a alienDirection*=-1
    for /l %%i in (0,1,39) do (
        if !alienActive[%%i]!==1 (
            set /a alienY[%%i]+=1
        )
    )
)

for /l %%i in (0,1,39) do (
    if !alienActive[%%i]!==1 (
        set /a alienX[%%i]+=%alienDirection%
    )
)

:: Check collisions
for /l %%i in (0,1,%bulletCount%) do (
    if !bulletActive[%%i]!==1 (
        for /l %%j in (0,1,39) do (
            if !alienActive[%%j]!==1 (
                if !bulletX[%%i]!==!alienX[%%j]! (
                    if !bulletY[%%i]!==!alienY[%%j]! (
                        set /a bulletActive[%%i]=0
                        set /a alienActive[%%j]=0
                        set /a score+=10
                    )
                )
            )
        )
    )
)

goto gameLoop

:restart
set /a playerX=40
set /a playerY=22
set /a score=0
set /a alienDirection=1
set /a gameOver=0
set /a won=0
set /a alienCount=0
set /a bulletCount=0

:: Reset aliens
for /l %%r in (1,1,4) do (
    for /l %%c in (1,1,10) do (
        set /a alienX[!alienCount!]=10+%%c*6
        set /a alienY[!alienCount!]=3+%%r*2
        set /a alienActive[!alienCount!]=1
        set /a alienCount+=1
    )
)

goto gameLoop

:end
echo Thanks for playing!
pause

:gotoxy
setlocal
set /a x=%1-1
set /a y=%2-1
echo exit|cmd /k "prompt $H$nul & for /l %%a in (1,1,!y!) do echo(& for /l %%a in (1,1,!x!) do echo( & echo(!text!"
endlocal
goto :eof
