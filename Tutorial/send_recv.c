// mpicc send_recv.c -o send_recv && mpirun -n 10 ./send_recv

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int world_size = 0, world_rank = 0;

    MPI_Init(NULL, NULL);    
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


    if (world_rank == 0){
        int send_number = 123;
        for (int dest=1; dest<world_size; dest++){
            send_number++;
            MPI_Send(&send_number, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            printf("Master sent %d to [%d]\n", send_number, dest);
        }
    } else {
        int recv_number = 0;
        MPI_Recv(&recv_number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("[%d] received: %d\n", world_rank, recv_number);
    }

    MPI_Finalize();

}