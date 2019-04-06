#include "catch_mpi_main.hpp"
#include "race_condition.h"
#include "deadlock.h"


TEST_CASE("Just test I exist", "[npany][npall][np][mpi]") {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    CHECK(size > 0); CHECK(rank >= 0);
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
    int data = 5;
    REQUIRE( deadlock_1(data) == data );
}

TEST_CASE( "DL: Send Recv, single value, 2 procs", "[np2][mpi]" ) {
    unsigned int data = 5;
    REQUIRE( deadlock_2(data) == data );
}

TEST_CASE( "RC: Recv Bcast Recv, single value, 3 procs", "[np3][mpi]" ) {
    unsigned int data = 5;
    REQUIRE( race_condition_3(data) == data );
}
