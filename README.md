# Unit Testing Framework and Strategies For MPI programs
## COMS 6156 
### Victoria Som de Cerff

#### Goals
- Effective MPI unit test environment
- Reduce false negatives
- Graceful recovery from deadlock 
- Provide useful information for debugging

#### Previous assignments
See docs directory. 


#### Requirements
- MPI Libraries
	- I use openmpi 4.0.0
	- Build instructions will be highly dependent on your machine.  
	Leverage source for details on installation: https://www.open-mpi.org/software/ompi/v4.0/ 
- Catch2
	- I use Catch2 2.7.0
	- The great thing about catch is you dont have to compile it.  
	All you have to do is include the main header: "catch.hpp".  https://github.com/catchorg/Catch2
- Python 3
	- I use python 3.6.5


#### How to build
- Set environment variable MPI_DIR to location of MPI installation. 
- Set environment variable CATCH_DIR to location of Catch2 installation. 
See source.sh for example.

See all make instructions:
```
make help

-- Make variables information --
	 COMPILER = 
	 ROOT_DIR = /Users/vsomdec/workspace/Software_Engineering/project/code/coms6156
	 MPI_DIR  = /Users/vsomdec/workspace/Software_Engineering/project/code/openmpi-4.0.0/install/
-- 
Description of targets understood by this Makefile
	 all      : builds everything, default
	 apps     : builds application binaries
	 lib 	  : builds libraries
	 clean    : delete all object files
	 cleanall : delete all object files and binaries
	 run      : build and run all tests, using default procs
	 run p=n tag=[tag]  : build and run tests with tag, using procs (p) = n
```

#### How to run
Executables are in the bin subdirectory.  If run directly 
> ./bin/mpi_test.test
it will run with one process. 

To run with multiple processes use must run with mpi_exec.  This should be in your MPI installation.  
> $(MPI_DIR)/bin/mpiexec -np 2 --quiet --oversubscribe ./bin/mpi_test.test

You can do this more simply by using make command. 
> make run p=2 tag=[np2]
Tags group together certain tests.  The command above will run the only the tests with the tag [np2] using 2 processes.
