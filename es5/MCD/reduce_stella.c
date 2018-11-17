/*  Si scriva un programma MPI che effettui in parallelo 
    a) la somma 
    b) il MCD 
    di un insieme di variabili (una per processo): 
    usando solo primitive simmetriche sincrone (send/receive) 
    secondo uno schema ad 
    a) stella
    Si valuti analiticamente le prestazioni aspettate in termini 
    di comunicazioni e di proceda ad una verifica sperimentale. 
    Si abbozzi una analisi dei risultati e delle difficoltà incontrate. 
    
*/

/*

    mpicc reduce_stella.c -o rs
    mpirun -n 8 ./rs

*/

#include <mpi.h>
#include <stdio.h>

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

int main(int argc, char **args)
{
    MPI_Init(NULL, NULL);

    int rank = 0, size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int myval = rank * 3;//1;



    if (rank == 0) { // ROOT
        for (int i=0; i < size-1; i++){ // size - 1 perche root NON deve astpettare se stesso
            
            // recv code
            int recv_val = 0;
            MPI_Recv(&recv_val, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            //myval += recv_val;
            myval = MCD(myval, recv_val);
        }
        printf("Risultato: %d \n", myval);


    } else {
        MPI_Send(&myval, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }



    MPI_Finalize();
    return 0;
}