.PHONY: all apps lib clean cleanall run help

include Makefile.inc

ROOT_DIR = $(shell pwd)
ECHO := echo 

all: lib apps

apps: 
	make -C src apps

lib: 
	make -C src lib

clean:
	make -C src clean

cleanall:
	make -C src cleanall

run:
	make -C src run

help: 
	@$(ECHO) "-- Make variables information --"
	@$(ECHO) "\t COMPILER = $(COMPILER)"
	@$(ECHO) "\t ROOT_DIR = $(ROOT_DIR)"
	@$(ECHO) "\t MPI_DIR  = $(MPI_DIR)"
	@$(ECHO) "-- "
	@$(ECHO) "Description of targets understood by this Makefile"
	@$(ECHO) "\t all      : builds everything, default"
	@$(ECHO) "\t apps     : builds application binaries"
	@$(ECHO) "\t lib 	  : builds libraries"
	@$(ECHO) "\t clean    : delete all object files"
	@$(ECHO) "\t cleanall : delete all object files and binaries"
	@$(ECHO) "\t run      : build and run all tests, using default procs"
	@$(ECHO) "\t run p=n tag=[tag]  : build and run tests with tag, using procs (p) = n" 
