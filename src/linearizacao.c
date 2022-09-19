#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "linearizacao.h"

#include "Matrix.h"
#include "mutexes.h"
#include "Parametros.h"

#include "f_auxiliares.h"

// Contadores
int contLinearizacao = 0;

// Vetores de periodo
double periodo_Linearizacao[(int) TAM_VETOR_LINEARIZACAO] = {0};
double *getPeriodoLinearizacao() {
    return periodo_Linearizacao;
}

// Vetores de Jitters
double JitterLinearizacao[(int) TAM_VETOR_LINEARIZACAO] = {0};

double *getJitterLinearizacao() {
    return JitterLinearizacao;
}

void calc_u(Matrix* U, Matrix* X, Matrix* V, double Raio)
{
    Matrix* L = matrix_zeros(2,2);

    double x3 = matrix_get_value(X,2,0);
    
    VALUES(L, 0, 0) = cos(x3);
    VALUES(L, 0, 1) = -Raio*sin(x3);
    VALUES(L, 1, 0) = sin(x3);
    VALUES(L, 1, 1) = Raio*cos(x3);

    MResponse response = matrix_inversa(L);
    Matrix* LInversa = response.m;
    
    MResponse response1 = matrix_mul(LInversa, V);
    matrix_copy_values(U, response1.m);

    matrix_free(L);
    matrix_free(response.m);
    matrix_free(response1.m);
}

void *linearizacao_thread(void *args) {
    double t = 0;       //tempo calculado
    double tm = 0;      //tempo medido
    double T = TEMPO_LINEARIZACAO;      //milissegundos
    struct timespec ts1, ts2, ts3 = {0};
    
    Matrix* X = matrix_zeros(3,1);
    Matrix* U = matrix_zeros(2,1);

    Matrix* V = matrix_zeros(2,1);

    while(t <= TEMPO_MAX) {
        clock_gettime(CLOCK_REALTIME, &ts1);
        t = t + T;

        // Cálculo de Jitter
        if (contLinearizacao != 0) {
            periodo_Linearizacao[contLinearizacao-1] = lat(ts1, tm);
            JitterLinearizacao[contLinearizacao-1] = lat(ts1, tm) - T; // milisegundos
        }

        contLinearizacao++;
        tm = ts2mili(ts1);


        mutexes_getX(X);
        mutexes_getU(U);
        mutexes_getV(V);

        calc_u(U, X, V, R);

        mutexes_setU(U);

        clock_gettime(CLOCK_REALTIME, &ts2);
        ts3.tv_sec = 0;
        ts3.tv_nsec = T*1000000 - (ts2.tv_nsec - ts1.tv_nsec);
        nanosleep(&ts3, &ts3);
    }

    matrix_free(X);
    matrix_free(U);
    matrix_free(V);

    return 0;
}
