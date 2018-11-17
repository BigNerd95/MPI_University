/*  Si scriva un programma MPI che effettui in parallelo 
    a) la somma 
    b) il MCD 
    di un insieme di variabili (una per processo) usando 
    la primitiva reduce
    
    Si valuti analiticamente le prestazioni aspettate in 
    termini di comunicazioni e di proceda ad una verifica sperimentale. 
    Si abbozzi una analisi dei risultati e delle difficoltà incontrate.

*/

/*

    mpicc reduce_primitiva.c -o rp
    mpirun -n 8 ./rp

*/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char **args)
{
    MPI_Init(NULL, NULL);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int myval = 1;
    int res = 0;

    MPI_Reduce(
        &myval,         // buffer sorgente
        &res,           // buffer destinazione risultato (root)
        1,              // numero di operandi nel buffer source
        MPI_INT,        // tipo operandi
        MPI_SUM,        // operazione
        0,              // rank processo (root) in cui salvare il risultato
        MPI_COMM_WORLD  // gruppo porcessi di comunicazione
    );

    // il risultato viene salvato nel primo processo
    if (rank == 0)
        printf("Risultato: %d\n", res);

    MPI_Finalize();
    return 0;
}