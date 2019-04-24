.PHONY: all apps clean cleanall run help

p ?= 1
tag ?= [npany]
help = [help]

ROOT_DIR = ../..
SRC_DIR  := $(ROOT_DIR)/src
INC_DIR  := $(ROOT_DIR)/include
LIB_DIR  := $(ROOT_DIR)/lib
BIN_DIR  := $(ROOT_DIR)/bin

include $(ROOT_DIR)/Makefile.inc

HEADERS := $(wildcard *.h)
SOURCES := $(wildcard *.cpp)
OBJECTS := $(subst cpp,o,$(SOURCES))
TARGETS := $(SOURCES:%.cpp=$(ROOT_DIR)/bin/%.test)

INCLUDES += -I$(INC_DIR)
LIBS     += -L$(LIB_DIR) -lutil -lseq -lparallel 

all: $(TARGETS)

apps: $(TARGETS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(ROOT_DIR)/bin/%.test: %.o $(HEADERS)
	@[ -n $(BIN_DIR) ] && mkdir -m 755 -p $(BIN_DIR) 
	$(LD) -o $@ $< $(LDFLAGS) $(LIBS)
	chmod 755 $@

clean:
	@rm -f *.o *~ ./#.#

cleanall:
	@rm -f $(TARGETS)

run: $(TARGETS)
ifeq ($(tag),$(help))
	@for target in $(TARGETS) ; do \
		echo Running $$target ; \
		$(MPI_DIR)/bin/mpiexec -np 1 $$target --list-tags ; \
		echo ; \
	done
else 
	@for target in $(TARGETS) ; do \
		echo Running $$target ; \
		$(MPI_DIR)/bin/mpiexec -np $(p) --oversubscribe $$target $(tag) ; \
		echo ; \
	done 
endif
