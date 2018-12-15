SHELL=/bin/sh

MAINFILE=main
SDIR=src
ODIR=build
_OBJS=main.o symb.o util.o
OBJS=$(patsubst %,$(ODIR)/%,$(_OBJS))
_DEPS=symb.h util.h errors.h
DEPS=$(patsubst %,$(SDIR)/%,$(_DEPS))
CFLAGS=-Wall -g
CC=gcc
ARFLAGS=rs

LIBNAME=out/barcode.a

$(ODIR)/%.o: $(DEPS) $(SDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SDIR)/$*.c -o $@

main: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJS) $(LIBS)

lib: all
	$(AR) $(ARFLAGS) $(LIBNAME) $(OBJS)

all: main

install: lib

.PHONY: clean

clean:
	-$(RM) $(ODIR)/*.o main $(LIBNAME)
