// Guilherme Matheus - 21950880
// Odalisio Neto - 21950520

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#include "Parametros.h"
#include "Integral.h"
#include "DString.h"
#include "Matrix.h"

#include "mutexes.h"

#include "threads.h"

int main() {
	mutexes_init();

    
    //Nomeando as Threads
    pthread_t TRef, TModeloRef, TControle, TLinearizacao, TRobo, Print_mostra;

    //Criando as Threads
    pthread_create(&TRef, NULL, ref_thread, NULL);
    pthread_create(&TModeloRef, NULL, modelo_ref_thread, NULL);
    pthread_create(&TControle, NULL, controle_thread, NULL);
    pthread_create(&TLinearizacao, NULL, linearizacao_thread, NULL); 
    pthread_create(&TRobo, NULL, robo_thread, NULL);
    pthread_create(&Print_mostra, NULL, print_thread, NULL);


    //Finalizando as Threads
    pthread_join(TRef, NULL);
    pthread_join(TModeloRef, NULL);
    pthread_join(TControle, NULL);
    pthread_join(TLinearizacao, NULL);
    pthread_join(TRobo, NULL);
    pthread_join(Print_mostra, NULL);

    // printf("fim do programa\n");

	mutexes_destroy();

    return 0;
}


