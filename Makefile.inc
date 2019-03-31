MPI+LIBS :=

AR := ar
RANLIN := ranlib

ifeq ($(MPI_DIR),)
    $(error MPI_DIR variable is not defined)
endif

ifeq ($(CATCH_DIR),)
    $(error CATCH_DIR variable is not defined)
endif

MPICC  := $(MPI_DIR)/bin/mpicc
CC     := $(MPI_DIR)/bin/mpicc
MPICXX := $(MPI_DIR)/bin/mpicxx
CXX    := $(MPI_DIR)/bin/mpicxx
LD     := $(MPI_DIR)/bin/mpicxx

INCLUDES += -I./ -I$(CATCH_DIR)/include -I$(MPI_DIR)/include
CFLAGS   += $(INCLUDES)
CXXFLAGS += -g -Wall -std=c++11 $(INCLUDES)
LDFLAGS  += 
LIBS     +=
