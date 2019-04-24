#pragma once
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <chrono>
#include "mpitest.h"

int deadlock_1(int data);

int deadlock_2(unsigned int data);
