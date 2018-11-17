/*  Si scriva un programma MPI che effettui in parallelo 
    a) la somma 
    b) il MCD 
    di un insieme di variabili (una per processo): 
    usando solo primitive simmetriche sincrone (send/receive) 
    secondo uno schema ad 
    c) albero
    Si valuti analiticamente le prestazioni aspettate in termini 
    di comunicazioni e di proceda ad una verifica sperimentale. 
    Si abbozzi una analisi dei risultati e delle difficoltà incontrate. 
    
*/

/*

    mpicc reduce_albero.c -o ral -lm
    mpirun -n 8 ./ral

*/

#include <mpi.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char **args)
{
    MPI_Init(NULL, NULL);

    int rank = 0, size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int myval = 1;

    int turn = 2;
    int depth = ceil(log(size) / log(2));

    for (int i = 0; i < depth; i++)
    {
        int whoami = rank % turn;
        int vicino = turn / 2;

        if (whoami == 0) // Riceve
        {
            int source = rank + vicino; // ricevo da

            if (source < size)
            {
                MPI_Status s;
                int recv_val = 0;
                MPI_Recv(&recv_val, 1, MPI_INT, source, i, MPI_COMM_WORLD, &s);

                myval += recv_val;
            }
        }
        else if (whoami == vicino) // Invia
        {
            int dest = rank - vicino; // invio a

            MPI_Send(&myval, 1, MPI_INT, dest, i, MPI_COMM_WORLD);
        }

        turn *= 2;
    }

    if (rank == 0)
        printf("Risultato: %d\n", myval);

    MPI_Finalize();
    return 0;
}

