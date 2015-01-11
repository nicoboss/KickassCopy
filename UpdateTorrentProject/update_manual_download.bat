7z.exe -aoa e dailydump.txt.gz
start /wait main.exe
del /P dailydump.txt.gz
del /P dailydump.txt
pause