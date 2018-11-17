/*  Si scriva l'implementazione di una reduce O(log n) 
    in MPI utilizzando solo comunicazioni simmetriche send/recv 
    per gruppi di processi di ogni size 
    con pattern di comunicazione inter-allacciata. 
    
    Ad ogni turno, ogni processo nella prima metà 
    comunica con un processo nella seconda metà. 
    Si assuma di avere un operatore associativo e commutativo. 

    Es con 8 processi [0-7]:
        Interazione t=0, size/2 = 8/2 = 4:
            0) 0 + 4 = 4
            1) 1 + 4 = 5
            2) 2 + 4 = 6
            3) 3 + 4 = 7

        Iterazione t=1, size/4 = 8/4 = 2:
            0) 0 + 2 = 2
            1) 1 + 2 = 3

        Iterazione t=2, size/8 = 8/8 = 1:
            0) 0 + 1 = 1

    log2(8) = 3 iterazioni
*/

/*

    mpicc reduce_n_interleaved.c -o rni -lm
    mpirun -n 8 ./rni

*/

#include <mpi.h>
#include <stdio.h>
#include <math.h>

#define SALTO 2

int main(int argc, char **args)
{
    MPI_Init(NULL, NULL);

    int rank = 0, size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int myval = 1;

    int turni = ceil(log(size) / log(2));

    for (int t = 0; t < turni; t++)
    {

        int seconda_meta = ceil((double) size/2);

        if (rank < seconda_meta) // Riceve
        {
            int source = rank + seconda_meta; // ricevo da
            if (source < size)
            {
                // printf("[Turno: %d] %d <-- %d \n", t, rank, source);

                MPI_Status s;
                int recv_val = 0;
                MPI_Recv(&recv_val, 1, MPI_INT, source, t, MPI_COMM_WORLD, &s);
                
                myval += recv_val; // <-- OPERAZIONE
            }
        }
        else if (rank < size) // Invia
        {

            int dest = rank - seconda_meta; // invio a
            //printf("[Turno: %d] %d --> %u\n", t, rank, dest);

            MPI_Send(&myval, 1, MPI_INT, dest, t, MPI_COMM_WORLD);
        }

        //fflush(stdout);
        //MPI_Barrier(MPI_COMM_WORLD); // <-- NON serve, le Recv e' SINCRONA (attende di poter ricevere)

        size = ceil((double) size / 2);
    }

    // il risultato viene salvato nel primo processo
    if (rank == 0)
    {
        printf("Risultato: %d\n", myval);
    }

    MPI_Finalize();
    return 0;
}