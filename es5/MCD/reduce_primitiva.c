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
#include <stdlib.h>

int MCD(int max, int min){

    if (max < min){ // swap if max is less than min
        max = max ^ min;
        min = max ^ min;
        max = max ^ min;
    }

    while (min != 0) {
        int res = max % min;
        max = min;
        min = res;
    }

    return max;
}

// interfaccia per l'operazione personalizzata (controlla il tipo ed esegue l'operazione su tutti gli elementi)
void MCD_mpi(void *in, void *inout, int *len, MPI_Datatype *type){
    if (MPI_INT == *type) {
        for (int i = 0; i < *len; i++)
            ((int *) inout)[i] = MCD(((int *)in)[i], ((int *)inout)[i]);
    } else {
        puts("Type error!");
        exit(1);
    }
}

int main(int argc, char **args)
{
    MPI_Init(NULL, NULL);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int myval = rank*5;
    int res = 0;

    // crea l'operazione personalizzata
    MPI_Op op;
    MPI_Op_create(MCD_mpi, /*commutativa*/ 1, &op);

    MPI_Reduce(
        &myval,         // buffer sorgente
        &res,           // buffer destinazione risultato (root)
        1,              // numero di operandi nel buffer source
        MPI_INT,        // tipo operandi
        op,             // operazione
        0,              // rank processo (root) in cui salvare il risultato
        MPI_COMM_WORLD  // gruppo porcessi di comunicazione
    );

    // libera la memoria occupata dall'operazione personalizzata
    MPI_Op_free(&op);

    // il risultato viene salvato nel primo processo
    if (rank == 0)
        printf("Risultato: %d\n", res);

    MPI_Finalize();
    return 0;
}