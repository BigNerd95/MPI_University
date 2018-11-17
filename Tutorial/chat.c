// mpicc send_recv.c -o send_recv && mpirun -n 10 ./send_recv

#include <mpi.h>
#include <stdio.h>
#include <string.h>

#define SIZE 100
#define RESP "sei uno bravo"

int main(int argc, char** argv) {
    int world_size = 0, world_rank = 0;
    MPI_Init(NULL, NULL);    
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_size < 2) {
        fprintf(stderr, "Number of process must be at least two, for %s\n", argv[0]); 
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    //////////////////// boilerplate 



    char string[SIZE] = {0};
    int exit = 0;

    if (world_rank == 0) {
        //////// MASTER
        while (!exit){
            fgets(string, SIZE, stdin);
            MPI_Send(string, SIZE, MPI_UNSIGNED_CHAR, 1, 0, MPI_COMM_WORLD);
            exit = !strncmp(string, "exit\n", 5);
            
            MPI_Recv(&string, SIZE, MPI_UNSIGNED_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Master received: %s\n", string);
        }

    } else if (world_rank == 1) {
        //////// SLAVE
        while (!exit){
            MPI_Recv(string, SIZE, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Slave received: %s", string);
            exit = !strncmp(string, "exit\n", 5);

            MPI_Send(RESP, sizeof(RESP), MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
        }
    }



    ///////////////////// boilerplate
    MPI_Finalize();
    return 0;
}