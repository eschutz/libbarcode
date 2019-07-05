SET INCLUDE_PATH=include
SET CFLAGS=-Wall -Wextra -g -I%INCLUDE_PATH%
SET SDIR=src
SET ODIR=build

echo %CFLAGS%

for %%f in (symb util graphic) do (
  bcc32x %CFLAGS% -static -c %SDIR%\%%f.c -o %ODIR%\%%f.o
)

tlib lib\libbarcode /u /a /C +build\symb.o +build\util.o +build\graphic.o
