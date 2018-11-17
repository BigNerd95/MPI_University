/*  Si scriva un programma MPI che effettui in parallelo 
    a) la somma 
    b) il MCD 
    di un insieme di variabili (una per processo): 
    usando solo primitive simmetriche sincrone (send/receive) 
    secondo uno schema ad 
    b) anello
    Si valuti analiticamente le prestazioni aspettate in termini 
    di comunicazioni e di proceda ad una verifica sperimentale. 
    Si abbozzi una analisi dei risultati e delle difficoltà incontrate. 
    
*/

/*

    mpicc reduce_anelloo.c -o ran -lm
    mpirun -n 8 ./ran

*/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char **args)
{
    MPI_Init(NULL, NULL);

    int rank = 0, size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int myval = 1;
    MPI_Status s;
    int recv_val = 0;

    if (rank == 0){
        MPI_Send(&myval, 1, MPI_INT, rank + 1, /*tag*/ 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_val, 1, MPI_INT, size-1, /*tag*/ 0, MPI_COMM_WORLD, &s);
        myval = recv_val;
    } else {    
        MPI_Recv(&recv_val, 1, MPI_INT, rank - 1, /*tag*/ 0, MPI_COMM_WORLD, &s);
        myval += recv_val;
        MPI_Send(&myval, 1, MPI_INT, (rank + 1)%size, /*tag*/ 0, MPI_COMM_WORLD);
    }

    if (rank == 0)
        printf("Risultato: %d\n", myval);

    MPI_Finalize();
    return 0;
}

