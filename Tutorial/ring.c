// mpicc send_recv.c -o send_recv && mpirun -n 10 ./send_recv

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int world_size = 0, world_rank = 0;
    MPI_Init(NULL, NULL);    
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_size < 2) {
        fprintf(stderr, "Number of process must be at least two, for %s\n", argv[0]); 
        MPI_Abort(MPI_COMM_WORLD, 1);
    }


    int token = 0;
    if (world_rank == 0) {
        // Set the token's value if you are process 0
        token = -1; 
    } else {
        MPI_Recv(&token, 1, MPI_INT, world_rank - 1, 0,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received token %d from process %d\n",
                world_rank, token, world_rank - 1);
    }

    // All processes send the token to the next process
    MPI_Send(&token, 1, MPI_INT, (world_rank + 1) % world_size,
            0, MPI_COMM_WORLD);

    // Now process 0 can receive from the last process.
    if (world_rank == 0) {
        MPI_Recv(&token, 1, MPI_INT, world_size - 1, 0,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received token %d from process %d\n",
            world_rank, token, world_size - 1);
    }


    MPI_Finalize();
    return 0;
}