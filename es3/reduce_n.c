/*  Si scriva l'implementazione di una reduce O(log2 n) 
    in MPI utilizzando solo comunicazioni simmetriche (1 a 1)
    send/recv per gruppi di processi di size 2^n. 
    Ad ogni turno t, ogni processo i comunica con il vicino i+2^t.

    Es con 8 processi [0-7]:
        Interazione t=0, 2^0=1:
            0) 0 + 1 = 1
            2) 2 + 1 = 3
            4) 4 + 1 = 5
            6) 6 + 1 = 7

        Iterazione t=1, 2^1=2:
            0) 0 + 2 = 2
            4) 4 + 2 = 6

        Iterazione t=2, 2^2=4:
            0) 0 + 4 = 4

    log2(8) = 3 iterazioni
*/

/*

    mpicc reduce_n.c -o rn -lm
    mpirun -n 7 ./rn

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

    int skip = 2;
    int turni = ceil(log(size) / log(SALTO));

    for (int t = 0; t < turni; t++)
    {
        int whoami = rank % skip;
        int vicino = skip / 2;

        if (whoami == 0) // Riceve
        {
            int source = rank + vicino; // ricevo da
            //printf("[Turno: %d] %d <-- %u\n", t, rank, source);

            /* Il problema e' solo nella receive, perche le send
            * vengono effettuate sempre da un processo con rank
            * piu' alto, il quale se e' fuori dalla size
            * non esistera' e dunque non entrera' mail nella send.
            * Per questo NON c'e' bisogno di effettuare controlli 
            * nella send.
            */
            if (source < size)
            {
                MPI_Status s;
                int recv_val = 0;
                MPI_Recv(&recv_val, 1, MPI_INT, source, t, MPI_COMM_WORLD, &s);
                myval += recv_val;
            }
        }
        else if (whoami == vicino) // Invia
        {
            int dest = rank - vicino; // invio a
            //printf("[Turno: %d] %d --> %u\n", t, rank, dest);

            MPI_Send(&myval, 1, MPI_INT, dest, t, MPI_COMM_WORLD);
        }

        //fflush(stdout);
        //MPI_Barrier(MPI_COMM_WORLD); // <-- NON serve, le Recv e' SINCRONA (attende di poter ricevere)
        skip *= SALTO;
    }

    // il risultato viene salvato nel primo processo
    if (rank == 0)
    {
        printf("Risultato: %d\n", myval);
    }

    MPI_Finalize();
    return 0;
}