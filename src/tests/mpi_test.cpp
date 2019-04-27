#pragma once
#include "catch_mpi_main.hpp"
#include "race_condition.h"
#include "deadlock.h"
#include "trivial.h"
#include "mpitest.h"


void test_setup(int* rank, int* mpi_size) {
	MPI_Wrap::use_wrapper(true);
    MPI_Wrap::clear_log();
    MPI_Wrap::set_threshold(10,5);
    MPI_Comm_rank(MPI_COMM_WORLD, rank);
    MPI_Comm_size(MPI_COMM_WORLD, mpi_size);
    MPI_Wrap::set_rank(*rank);
	MPI_Wrap::set_size(*mpi_size);
}

void all_here() {
	MPI_Request req;
    MPI_Status stat;
    MPI_Ibarrier(MPI_COMM_WORLD, &req);
   
    int all_here = false;
    while (!all_here) {
        MPI_Test(&req, &all_here, &stat);
        MPI_Wrap::check_deadlock_message();
    }
}

void test_closeout(std::string test_name, int rank) {
	all_here();
	MPI_Wrap::write_log(test_name + "_" + std::to_string(rank) + ".csv");
    MPI_Barrier(MPI_COMM_WORLD);
}

void check_test_results(int rank, int mpi_size, int desired_value, int ret_value) {
	all_here();

	int all_desired[mpi_size];
	int all_returned[mpi_size];
	MPI_Gather(&desired_value, 1, MPI_INT, &all_desired, 1, MPI_INT, 0, MPI_COMM_WORLD );
	MPI_Gather(&ret_value, 1, MPI_INT, &all_returned, 1, MPI_INT, 0, MPI_COMM_WORLD );
	if (rank == 0) {
        for ( int p = 0; p < mpi_size; p++) {
			if (all_returned[p] != all_desired[p] )	 {
				printf("Undesireable value from process %d", p);
			}
            CHECK( all_returned[p] == all_desired[p] );   

        }
    }
}

TEST_CASE("Just test I exist", "[hello][npany][mpi]") {
    int rank, mpi_size;
    test_setup(&rank, &mpi_size);

    CHECK(mpi_size > 0); CHECK(rank >= 0);

}

TEST_CASE( "RC: Isend Recv, single value, 2 procs", "[np2][mpi][race][race1]" ) {
	int rank, mpi_size;
    test_setup(&rank, &mpi_size);
	REQUIRE(mpi_size == 2);
	
    unsigned int data = 5;
	int desired_value = data;
	int ret_value = race_condition_1(data);

	check_test_results(rank, mpi_size, desired_value, ret_value);
    
	test_closeout("RC1", rank);
}

TEST_CASE( "RC: Isend Irecv, array, 2 procs", "[np2][mpi][race][race2]" ) {
	int rank, mpi_size;
    test_setup(&rank, &mpi_size);
    REQUIRE(mpi_size == 2);

    unsigned int data = 5;
    int desired_value = data;
	int ret_value = race_condition_2(data);

	check_test_results(rank, mpi_size, desired_value, ret_value);
	
	test_closeout("RC2", rank);
}

TEST_CASE( "DL: Send Recv, array", "[npany][mpi][dead][dead1]" ) {
	int rank, mpi_size;
    test_setup(&rank, &mpi_size);

    int data = 5;
	int desired_value = data;
	int ret_value = deadlock_1(data);

	check_test_results(rank, mpi_size, desired_value, ret_value);

	test_closeout("DL1", rank);
}

TEST_CASE( "DL: Send Recv, single value, 2 procs", "[np2][mpi][dead][dead2]" ) {
	int rank, mpi_size;
    test_setup(&rank, &mpi_size);
    REQUIRE(mpi_size == 2);

    unsigned int data = 5;
    int desired_value = data;
	int ret_value = deadlock_2(data);

	check_test_results(rank, mpi_size, desired_value, ret_value);

	test_closeout("DL2", rank);
}

TEST_CASE( "DL: Send, single value, missing recv", "[npany][mpi][dead][dead3]" ) {
    int rank, mpi_size;
    test_setup(&rank, &mpi_size);

    unsigned int data = 5;
    int desired_value = data;
    int ret_value = deadlock_3(data);

    check_test_results(rank, mpi_size, desired_value, ret_value);

    test_closeout("DL3", rank);
}

TEST_CASE( "DL: Recv, single value, missing send", "[npany][mpi][dead][dead4]" ) {
    int rank, mpi_size;
    test_setup(&rank, &mpi_size);

    unsigned int data = 5;
    int desired_value = data;
    int ret_value = deadlock_4(data);

    check_test_results(rank, mpi_size, desired_value, ret_value);

    test_closeout("DL4", rank);
}

TEST_CASE( "RC: Recv Bcast Recv, single value, 3 procs", "[np3][mpi][race][race3]" ) {
	int rank, mpi_size;
    test_setup(&rank, &mpi_size);
    REQUIRE(mpi_size == 3);
	
    unsigned int data = 10;
	
	int desired_value = data;
	int ret_value = race_condition_3(data);

	check_test_results(rank, mpi_size, desired_value, ret_value);

	test_closeout("RC3", rank);
}
