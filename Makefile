SHELL=/bin/sh

MAINFILE=main
MAINOBJ=$(ODIR)/$(MAINFILE).o
SDIR=src
ODIR=build
_OBJS=symb.o util.o graphic.o
OBJS=$(patsubst %,$(ODIR)/%,$(_OBJS))
INCLUDE_PATH=include
_DEPS=barcode/symb.h barcode/util.h barcode/errors.h barcode/graphic.h barcode.h
DEPS=$(patsubst %,$(INCLUDE_PATH)/%,$(_DEPS))
CFLAGS=-Wall -Wextra -g -I$(INCLUDE_PATH)
ifeq ($(OS),Windows_NT)
	CC=bcc32x
else
	CC=clang
endif
ARFLAGS=rs

LIBNAME=lib/libbarcode.a

$(ODIR)/%.o: $(DEPS) $(SDIR)/%.c
	$(CC) $(CFLAGS) -static $(INCLUDES) -c $(SDIR)/$*.c -o $@

main: $(MAINOBJ) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(MAINOBJ) $(OBJS) $(LIBS)

lib: $(OBJS)
	$(AR) $(ARFLAGS) $(LIBNAME) $(OBJS)

all: main

install: lib

debug: CFLAGS += -O0

debug:
	if [ $(shell uname -s) = "Darwin" ]; then\
		docker run -v $(PWD):/home/ valgrind-docker bash -c "cd home; make clean; make main; valgrind --leak-check=yes --read-var-info=yes --track-origins=yes ./main";\
	else\
		clean;\
		main;\
		valgrind --leak-check=yes --read-var-info=yes --track-origins=yes ./main;\
	fi

.PHONY: clean

clean:
	-$(RM) $(ODIR)/*.o main $(LIBNAME)
