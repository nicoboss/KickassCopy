wget64 -N --continue --verbose --trust-server-names --no-check-certificate --referer=http://torrentproject.se/api --user-agent="Mozilla/5.0 (X11; U; Linux i686; de; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5" "http://torrentproject.se/dailydump.txt.gz"
7z.exe -aoa e dailydump.txt.gz
start /wait main.exe
del /P dailydump.txt.gz
del /P dailydump.txt
pause