// Guilherme Matheus - 21950880
// Odalisio Neto - 21950520

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include "Parametros.h"
#include "Integral.h"
#include "DString.h"
#include "Matrix.h"

#include "mutexes.h"
#include "threads.h"

void print_vector(double* v, int s) {
    for (int i = 0; i < s + 1; i++) {
        printf("%.4lf ", v[i]);
    }
    printf("\n");
}

void printar_estatisticas(double* arr, int size) {
    double max = arr[0]; double min  = arr[0];
    
    double media = 0;
    for (int i = 0; i < size; i++) {

        if (arr[i] > max) max = arr[i];
        if (arr[i] < min) min = arr[i];
        
        media += arr[i];
    }
    media /= size;

    double var = 0;
    for (int i = 0; i < size; i++) {
        var += pow(arr[i] - media, 2);
    }
    var /= size;

    double dp = sqrt(var);

    printf("med:%lf,var:%lf,dp:%lf,max:%lf,min:%lf\n", media, var, dp, max, min);
}

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

    double* PeriodoRef = getPeriodoRef();
    double* PeriodoModeloRef = getPeriodoModeloRef();
    double* PeriodoControle = getPeriodoControle();
    double* PeriodoLinearizacao = getPeriodoLinearizacao();
    double* PeriodoRobo = getPeriodoRobo();

    double* JitterRef = getJitterRef();
    double* JitterModeloRef = getJitterModeloRef();
    double* JitterControle = getJitterControle();
    double* JitterLinearizacao = getJitterLinearizacao();
    double* JitterRobo = getJitterRobo();

    printf("--- Estatisticas ---\n");

    printf("Jitter ref:"); printar_estatisticas(JitterRef, (int) (TEMPO_MAX/TEMPO_REF));
    printf("Jitter modeloRef:"); printar_estatisticas(JitterModeloRef, (int) (TEMPO_MAX/TEMPO_MODELO_REF));
    printf("Jitter Controle:"); printar_estatisticas(JitterControle, (int) (TEMPO_MAX/TEMPO_CONTROLE));
    printf("Jitter Linearizacao:"); printar_estatisticas(JitterLinearizacao, (int) (TEMPO_MAX/TEMPO_LINEARIZACAO));
    printf("Jitter Robo:"); printar_estatisticas(JitterRobo, (int) (TEMPO_MAX/TEMPO_ROBO));

    printf("\n");

    printf("Periodo ref:"); printar_estatisticas(PeriodoRef, (int) (TEMPO_MAX/TEMPO_REF));
    printf("Periodo modeloRef:"); printar_estatisticas(PeriodoModeloRef, (int) (TEMPO_MAX/TEMPO_MODELO_REF));
    printf("Periodo Controle:"); printar_estatisticas(PeriodoControle, (int) (TEMPO_MAX/TEMPO_CONTROLE));
    printf("Periodo Linearizacao:"); printar_estatisticas(PeriodoLinearizacao, (int) (TEMPO_MAX/TEMPO_LINEARIZACAO));
    printf("Periodo Robo:"); printar_estatisticas(PeriodoRobo, (int) (TEMPO_MAX/TEMPO_ROBO));
    

    return 0;
}


