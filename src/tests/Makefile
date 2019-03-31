.PHONY: all apps clean cleanall run help

p ?= 4

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
LIBS     += -L$(LIB_DIR) -lseq -lparallel

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
	@for target in $(TARGETS) ; do \
	echo Running $$target ; \
	$(MPI_DIR)/bin/mpiexec -np $(p) $$target ; \
	echo ; \
	done 

