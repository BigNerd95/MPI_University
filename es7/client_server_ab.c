/*  Si scriva un programma che realizzi un insieme di 
    - n-1 client
    - 1 server. 

    Ogni client:
    - spedisce al server un certo numero di 
        richieste (messaggi contenti un intero).
    - aspetta un intervallo di tempo casuale fra 
        una richiesta e la successiva ([0,1] secondi).
    - dopo la richiesta aspetta una risposta dal server.
    - termina spontaneamente l'esecuzione dopo un
        numero casuale di richieste e risposte ricevute.  
    
    Il server:
    - risponde al client con il quadrato del numero 
        che ha ricevuto nella richiesta. 
    - utilizza una unica receive 
        (con source MPI_ANY_SOURCE).
    
    Si discuta la terminazione del programma. 
    Si provi ad utilizzare sia primitive 
    sincrone, 
    asincroni blocking 
    e non-blocking. 


    ASINCRONA BLOCCANTE
*/

/*

    mpicc client_server_ab.c -o csab
    mpirun -n 8 ./csab

*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

enum tags {message, end};

int main(int argc, char **args)
{
    MPI_Init(NULL, NULL);

    int rank = 0, size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank);
    

    if (rank == 0) // server
    { 
        int exit_count = size - 1; // tiene conto dei client totali, cosi da sapere quando uscire
        printf("[Server] clients: %d \n", exit_count);
        
        while (exit_count != 0) {
            
            // riceve il messaggio da un qualsiasi client
            int recv_val = 0;
            MPI_Status s;
            MPI_Recv(&recv_val, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s);

            switch (s.MPI_TAG){
                case message: // se e' un messaggio, risponde al client con il quadrato
                    recv_val *= recv_val;
                    MPI_Send(&recv_val, 1, MPI_INT, s.MPI_SOURCE, /*tag*/ message, MPI_COMM_WORLD);
                    break;
                case end: // se e' un segnale di uscita, decrementa il numero di client presenti
                    exit_count--;
                    printf("[Server] clients: %d \n", exit_count);
                    break;
                default:
                    puts("Unknown message TAG");
                    break;
            }
        }

        puts("[Server] exiting!");
    }

    else // client
    { 
        // sceglie a caso il numero di richieste da fare
        int requests = rand() % 10 + 1;
        printf("[Client %d] requests: %d\n", rank, requests);

        for (int i=0; i<requests; i++) {
            // invia un numero random al server
            int new_val = rand() % 10 + 1;
            int recv_val = 0;
            MPI_Send(&new_val, 1, MPI_INT, 0, message, MPI_COMM_WORLD);

            // riceve il quadrato dal server
            MPI_Recv(&recv_val, 1, MPI_INT, 0,  message, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("[Client %d] sent: %d, received: %d \n", rank, new_val, recv_val);

            // aspetta un tempo random [0,1] sec
            int micro_sleep = rand() % 1000000;
            usleep(micro_sleep);
        }

        // segnala al server che ha finito
        MPI_Send(NULL, 0, MPI_INT, 0, end, MPI_COMM_WORLD);

    }

    MPI_Finalize();
    return 0;
}
