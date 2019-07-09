setlocal EnableDelayedExpansion
SET INCLUDE_PATH=include
SET CFLAGS=/I%INCLUDE_PATH%
SET SDIR=src
SET ODIR=build
SET CC=cl
SET OBJS=

call vsdevcmd

for %%f in (symb util graphic) do (
  SET OBJS=!OBJS! %ODIR%\%%f.obj
  %CC% %CFLAGS% /c %SDIR%\%%f.c -Fo:%ODIR%\%%f.obj
)

link /dll /out:lib\libbarcode.dll %OBJS%
