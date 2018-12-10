#!/usr/bin/make

SHELL = /bin/sh

OBJS = main.o generator.o
CFLAGS = -Wall -g
CC = gcc
ARFLAGS = rs

LIBNAME = barcode
ODIR = build

generator: ${OBJS}
  ${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}

main.o: generator.h
generator.o: generator.h

lib: all
  ${AR} ${ARFLAGS} ${LIBNAME}.a ${OBJS}

all: generator

install: lib

clean:
  -${RM} *.o main ${LIBNAME}.a