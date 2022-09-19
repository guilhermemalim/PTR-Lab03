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

#include "ref.h"
#include "modelo_ref.h"
#include "controle.h"
#include "linearizacao.h"
#include "robo.h"
#include "print.h"

#include "f_auxiliares.h"

int main() {
	mutexes_init();

    pthread_t thread_ref, thread_modelo_ref, thread_controle, thread_linearizacao, thread_robo, thread_print;

    pthread_create(&thread_ref, NULL, ref_thread, NULL);
    pthread_create(&thread_modelo_ref, NULL, modelo_ref_thread, NULL);
    pthread_create(&thread_controle, NULL, controle_thread, NULL);
    pthread_create(&thread_linearizacao, NULL, linearizacao_thread, NULL); 
    pthread_create(&thread_robo, NULL, robo_thread, NULL);
    pthread_create(&thread_print, NULL, print_thread, NULL);


    pthread_join(thread_ref, NULL);
    pthread_join(thread_modelo_ref, NULL);
    pthread_join(thread_controle, NULL);
    pthread_join(thread_linearizacao, NULL);
    pthread_join(thread_robo, NULL);
    pthread_join(thread_print, NULL);


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

    printf("Jitter ref:"); printar_estatisticas(JitterRef, (int) TAM_VETOR_REF);
    printf("Jitter modeloRef:"); printar_estatisticas(JitterModeloRef, (int) TAM_VETOR_MODELO_REF);
    printf("Jitter Controle:"); printar_estatisticas(JitterControle, (int) TAM_VETOR_CONTROLE);
    printf("Jitter Linearizacao:"); printar_estatisticas(JitterLinearizacao, (int) TAM_VETOR_LINEARIZACAO);
    printf("Jitter Robo:"); printar_estatisticas(JitterRobo, (int) TAM_VETOR_ROBO);

    printf("\n");

    printf("Periodo ref:"); printar_estatisticas(PeriodoRef, (int) TAM_VETOR_REF);
    printf("Periodo modeloRef:"); printar_estatisticas(PeriodoModeloRef, (int) TAM_VETOR_MODELO_REF);
    printf("Periodo Controle:"); printar_estatisticas(PeriodoControle, (int) TAM_VETOR_CONTROLE);
    printf("Periodo Linearizacao:"); printar_estatisticas(PeriodoLinearizacao, (int) TAM_VETOR_LINEARIZACAO);
    printf("Periodo Robo:"); printar_estatisticas(PeriodoRobo, (int) TAM_VETOR_ROBO);
    

    return 0;
}


