@echo off

set INSTVERS=1_04
set INSTOUT=reddot-%INSTVERS%.exe

cls

echo.
echo ***********************************************************
echo REMOVING OLD FILES
echo ***********************************************************
echo.

call cleaninstaller.bat

echo.
echo ***********************************************************
echo COPYING NEW FILES
echo ***********************************************************
echo.

copy ..\source\Release\reddot.exe .
copy ..\readme.html .

echo.
echo ***********************************************************
echo BUILDING INSTALLER
echo ***********************************************************
echo.

call "c:\program files\inno setup 5\iscc.exe" script.iss

move Output\setup.exe .
ren setup.exe %INSTOUT%

rmdir /q /s Output

echo.
echo ***********************************************************
echo ALL DONE!
echo ***********************************************************
echo.
echo The installer is saved as %INSTOUT%

pause
