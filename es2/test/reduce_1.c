/*
    mpicc reduce_2.c -o r2
    mpirun -n 8 ./r2
*/

#include <mpi.h>
#include <stdio.h>



int main(int argc, char** args){
    MPI_Init(NULL, NULL);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int myval = 1;
    int res = 0;
    MPI_Reduce(&myval, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    printf("Rank: %d, Result: %d\n", rank, res);

    MPI_Finalize();
    return 0;
}