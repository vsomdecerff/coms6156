#pragma once
#include "catch_mpi_main.hpp"
#include "race_condition.h"
#include "deadlock.h"
#include "mpitest.h"


TEST_CASE("Just test I exist", "[hello][npany][npall][np][mpi]") {

	MPI_Wrap::use_wrapper(true);
	MPI_Wrap::set_threshold(1,1);
		
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Wrap::set_rank(rank);
    CHECK(size > 0); CHECK(rank >= 0);

	MPI_Wrap::write_log("hello.csv");
}


TEST_CASE( "RC: Isend Recv, single value, 2 procs", "[np2][mpi]" ) {
    unsigned int data = 5;
    REQUIRE( race_condition_1(data) == data );
}

TEST_CASE( "RC: Isend Irecv, array, 2 procs", "[np2][mpi]" ) {
    unsigned int data = 5;
    REQUIRE( race_condition_2(data) == data );
}

TEST_CASE( "DL: Send Recv, array", "[npany][npall][np][mpi]" ) {
	MPI_Wrap::use_wrapper(true);
    MPI_Wrap::set_threshold(1,1);
	
	int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Wrap::set_rank(rank);

    int data = 5;
    REQUIRE( deadlock_1(data) == data );

    MPI_Wrap::write_log("DL1.csv");
}

TEST_CASE( "DL: Send Recv, single value, 2 procs", "[np2][mpi]" ) {
    unsigned int data = 5;
    REQUIRE( deadlock_2(data) == data );
}

TEST_CASE( "RC: Recv Bcast Recv, single value, 3 procs", "[np3][mpi]" ) {
    unsigned int data = 5;
    REQUIRE( race_condition_3(data) == data );
}
