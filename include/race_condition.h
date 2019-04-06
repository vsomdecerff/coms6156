#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <chrono>

unsigned int race_condition_1(unsigned int data);
unsigned int race_condition_2(unsigned int data);
unsigned int race_condition_3(unsigned int data);
