#include "catch_mpi_main.hpp"
//#include "parallel.h"
TEST_CASE("Just test I exist") {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    CHECK(size > 0); CHECK(rank >= 0);
}

/*
TEST_CASE( "Process return positive parameter", "[single-file]" ) {
    int data = 5;
    REQUIRE( race_condition(data) == data );
}
*/
