
@rem    author          Oliver Blaser
@rem    date            01.02.2022
@rem    copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser



rmdir /s /q packed\bestagon-mines_win_x64



xcopy /i /s /e ..\include packed\omw_win\omw\include\

xcopy /i ..\lib\omw-d.lib packed\omw_win\omw\lib\
xcopy /i ..\lib\omw.lib packed\omw_win\omw\lib\
@rem xcopy /i ..\lib64\omw-d.lib packed\omw_win\omw\lib64\
@rem xcopy /i ..\lib64\omw.lib packed\omw_win\omw\lib64\

copy dep_readme.txt packed\omw_win\omw\readme.txt
xcopy ..\license.txt packed\omw_win\omw

cd packed\omw_win
"C:\Program Files\7-Zip\7z.exe" a omw_win_vX.X.X.zip omw\
cd ..\..

move packed\omw_win\omw_win_vX.X.X.zip packed\omw_win_vX.X.X.zip
