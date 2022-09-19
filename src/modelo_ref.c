#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "modelo_ref.h"

#include "Matrix.h"
#include "mutexes.h"
#include "Parametros.h"

#include "f_auxiliares.h"

// Contadores
int contModeloRef = 0;

// Vetores de periodo
double periodo_ModeloRef[(int) TAM_VETOR_MODELO_REF] = {0};

double *getPeriodoModeloRef() {
    return periodo_ModeloRef;
}

// Vetores de Jitters
double JitterModeloRef[(int) TAM_VETOR_MODELO_REF] = {0};


double *getJitterModeloRef() {
    return JitterModeloRef;
}

void calc_ymponto(Matrix* ym_ponto, Matrix* ref, Matrix* Ym) {
    
    VALUES(ym_ponto, 0, 0) = ALPHA1*(matrix_get_value(ref,0,0)-matrix_get_value(Ym,0,0)); // ymx_ponto = alpha1(X_ref - y_mx)
    VALUES(ym_ponto, 1, 0) = ALPHA2*(matrix_get_value(ref,1,0)-matrix_get_value(Ym,1,0)); // ymy_ponto = alpha2(Y_ref) 
}

void calc_ym(Matrix* ym, Matrix* ym_ponto, Matrix* ym_ponto_antigo, double dt)
{
    VALUES(ym, 0, 0) =  (dt) * (matrix_get_value(ym_ponto, 0, 0) + matrix_get_value(ym_ponto_antigo, 0, 0)) / 2;
    VALUES(ym, 1, 0) =  (dt) * (matrix_get_value(ym_ponto, 1, 0) + matrix_get_value(ym_ponto_antigo, 1, 0)) / 2;
}

void *modelo_ref_thread(void *args) {
    double t = 0;       //tempo calculado
    double tm = 0;      //tempo medido
    double T = TEMPO_MODELO_REF;      //milissegundos
    struct timespec ts1, ts2, ts3 = {0};
    
    Matrix* ref = matrix_zeros(2,1);
    Matrix* Ym = matrix_zeros(2,1);
    Matrix* Ymponto = matrix_zeros(2,1);
    Matrix* Ymponto_anterior = matrix_zeros(2,1);

    while(t <= TEMPO_MAX) {
        clock_gettime(CLOCK_REALTIME, &ts1);

        // Cálculo de Jitter
        if (contModeloRef != 0) {
            periodo_ModeloRef[contModeloRef - 1] = lat(ts1, tm);
            JitterModeloRef[contModeloRef-1] = lat(ts1, tm) - T; // milisegundos
        }
        
        contModeloRef++;
        tm = ts2mili(ts1);
        t = t + T;

        // Cálculo de ref e set de mutexes

        mutexes_getRef(ref);
        mutexes_getYmdot(Ymponto);
        mutexes_getYm(Ymponto_anterior);

        // matrix_copy_values(Ymponto_anterior, Ymponto);

        calc_ymponto(Ymponto, ref, Ym);
        calc_ym(Ym, Ymponto, Ymponto_anterior, 0.03);

        mutexes_setYmdot(Ymponto);
        mutexes_setYm(Ym);

        clock_gettime(CLOCK_REALTIME, &ts2);
        ts3.tv_sec = 0;
        ts3.tv_nsec = T*1000000 - (ts2.tv_nsec - ts1.tv_nsec);
        nanosleep(&ts3, &ts3);
    }

    matrix_free(ref);
    matrix_free(Ym);
    matrix_free(Ymponto);
    matrix_free(Ymponto_anterior);

    return 0;
}
