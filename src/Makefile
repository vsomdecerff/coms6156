.PHONY: all apps lib clean cleanall run help

ROOT_DIR := ..
include $(ROOT_DIR)/Makefile.inc

all: lib apps

apps: 
	make -C tests apps

lib: 
	make -C seq lib
	make -C parallel lib

clean:
	make -C tests clean
	make -C seq clean
	make -C parallel clean

cleanall:
	make -C tests cleanall
	make -C seq cleanall
	make -C parallel cleanall

run:
	make -C tests run

