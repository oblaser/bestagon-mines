
@rem    author          Oliver Blaser
@rem    date            02.02.2022
@rem    copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser



setlocal

set /p VERSIONSTR=<dep_vstr.txt
set EXENAME=bestagon-mines
set PLATFORM=x64
set OUTDIRNAME=bestagon-mines_win_%PLATFORM%
set OUTDIR=packed\%OUTDIRNAME%
set ARCHIVE=%EXENAME%_win_x64_v%VERSIONSTR%.zip

rmdir /s /q %OUTDIR%

xcopy /i ..\assets\smiley_exp*.png %OUTDIR%\%EXENAME%\assets\
copy ..\assets\smiley.png %OUTDIR%\%EXENAME%\assets\
copy ..\assets\smiley_hover2.png %OUTDIR%\%EXENAME%\assets\
copy ..\assets\smiley_click.png %OUTDIR%\%EXENAME%\assets\
copy ..\assets\smiley_won.png %OUTDIR%\%EXENAME%\assets\

copy ..\prj\vs\Release-x64\%EXENAME%.exe %OUTDIR%\%EXENAME%\

copy ..\license.txt %OUTDIR%\%EXENAME%\
copy dep_readme.txt %OUTDIR%\%EXENAME%\readme.txt

cd %OUTDIR%
"C:\Program Files\7-Zip\7z.exe" a %ARCHIVE% %EXENAME%\
cd ..\..

del packed\%ARCHIVE%
move %OUTDIR%\%ARCHIVE% packed\%ARCHIVE%

endlocal
