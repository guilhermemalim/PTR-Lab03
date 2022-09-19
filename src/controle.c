#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "controle.h"

#include "Matrix.h"
#include "mutexes.h"
#include "Parametros.h"

#include "f_auxiliares.h"

// Contadores
int contControle = 0;

// Vetores de periodo
double periodo_Controle[(int) TAM_VETOR_CONTROLE] = {0};

double *getPeriodoControle() {
    return periodo_Controle;
}

// Vetores de Jitters
double JitterControle[(int) TAM_VETOR_CONTROLE] = {0};

double *getJitterControle() {
    return JitterControle;
}

void calc_v(Matrix *V, Matrix* ym_ponto, Matrix* Ym, Matrix* Y)
{
    VALUES(V, 0, 0) = matrix_get_value(ym_ponto, 0, 0) + ALPHA1 * (matrix_get_value(Ym, 0, 0) - matrix_get_value(Y, 0, 0));
    VALUES(V, 1, 0) = matrix_get_value(ym_ponto, 1, 0) + ALPHA2 * (matrix_get_value(Ym, 1, 0) - matrix_get_value(Y, 1, 0));
}

void *controle_thread(void *args) {
    double t = 0;       //tempo calculado
    double tm = 0;      //tempo medido
    double T = TEMPO_CONTROLE;      //milissegundos
    struct timespec ts1, ts2, ts3 = {0};
    
    Matrix* Ym = matrix_zeros(2,1);
    Matrix* Ymponto = matrix_zeros(2,1);
    Matrix* Y = matrix_zeros(2,1);

    Matrix* V = matrix_zeros(2,1);

    while(t <= TEMPO_MAX) {
        clock_gettime(CLOCK_REALTIME, &ts1);
        t = t + T;

        // Cálculo de Jitter
        if (contControle != 0) {
            periodo_Controle[contControle - 1] = lat(ts1, tm);
            JitterControle[contControle-1] = lat(ts1, tm) - T; // milisegundos
        }
        
        contControle++;
        tm = ts2mili(ts1);

        
        mutexes_getYmdot(Ymponto);
        mutexes_getYm(Ym);
        mutexes_getY(Y);

        calc_v(V, Ymponto, Ym, Y);

        mutexes_setV(V);

        clock_gettime(CLOCK_REALTIME, &ts2);
        ts3.tv_sec = 0;
        ts3.tv_nsec = T*1000000 - (ts2.tv_nsec - ts1.tv_nsec);
        nanosleep(&ts3, &ts3);
    }

    matrix_free(Ym);
    matrix_free(Ymponto);
    matrix_free(Y);
    matrix_free(V);

    return 0;
}
