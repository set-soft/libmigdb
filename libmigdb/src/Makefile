#!/usr/bin/make
PREFIX=/usr

all: libmigdb.a

CFLAGS=-O2 -Wall -gstabs+3 -I.
CXXFLAGS=-O2 -Wall -gstabs+3
LDLIBS=

connect.o: mi_gdb.h

parse.o: mi_gdb.h

cpp_int.o: mi_gdb.h

libmigdb.a: connect.o parse.o prg_control.o misc.o breakpoint.o target_man.o \
	get_free_vt.o data_man.o stack_man.o symbol_query.o thread.o var_obj.o \
	alloc.o error.o cpp_int.o
	ar rcs $@ $^

clean:
	-@rm *.o *.a 2> /dev/null

install:
	install -d -m 755 $(PREFIX)/lib
	install -m 644 libmigdb.a $(PREFIX)/lib
	install -d -m 755 $(PREFIX)/include
	install -m 644 mi_gdb.h $(PREFIX)/include

