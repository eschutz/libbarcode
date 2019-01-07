SHELL=/bin/sh

MAINFILE=main
SDIR=src
ODIR=build
_OBJS=main.o symb.o util.o graphic.o
OBJS=$(patsubst %,$(ODIR)/%,$(_OBJS))
_DEPS=symb.h util.h errors.h graphic.h
DEPS=$(patsubst %,$(SDIR)/%,$(_DEPS))
CFLAGS=-Wall -Wextra -g
ifeq ($(OS),Windows_NT)
	CC=bcc32x
else
	CC=clang
endif
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

debug: CFLAGS += -O0

debug: clean main
	docker run -v $(PWD):/home/ valgrind-docker bash -c "cd home; make clean; make main; valgrind --leak-check=yes --read-var-info=yes --track-origins=yes ./main"

.PHONY: clean

clean:
	-$(RM) $(ODIR)/*.o main $(LIBNAME)
