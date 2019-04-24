.PHONY: all lib clean cleanall 

ROOT_DIR = ../..
SRC_DIR  := $(ROOT_DIR)/src
INC_DIR  := $(ROOT_DIR)/include
LIB_DIR  := $(ROOT_DIR)/lib
BIN_DIR  := $(ROOT_DIR)/bin

include $(ROOT_DIR)/Makefile.inc

TARGET := $(LIB_DIR)/libutil.a

HEADERS := $(wildcard $(INC_DIR)/*.h)
SOURCES := $(wildcard *.cpp)
OBJECTS := $(subst cpp,o,$(SOURCES))

CXXFLAGS += -I$(INC_DIR)
LIBS     += -L$(LIB_DIR)

all: lib

$(TARGET): $(OBJECTS)
	@[ -n $(LIB_DIR) ] && mkdir -m 755 -p $(LIB_DIR)
	$(AR) cru $@ $(OBJECTS)
	$(AR) -ts $@
	$(RANLIB) $@

lib: $(TARGET)

clean:
	@rm -f *.o *~ ./#.#

cleanall: clean
	@rm -f $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
