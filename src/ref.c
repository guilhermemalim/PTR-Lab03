#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "ref.h"

#include "Matrix.h"
#include "mutexes.h"
#include "Parametros.h"

#include "f_auxiliares.h"


// Contadores
int contRef = 0;


// Vetores de periodo
double periodo_Ref[(int) TAM_VETOR_REF] = {0};
double *getPeriodoRef() {
    return periodo_Ref;
}


// Vetores de Jitters
double JitterRef[(int) TAM_VETOR_REF] = {0};
double *getJitterRef() {
    return JitterRef;
}

void calc_ref(Matrix* ref, double t) {
    
    if(t>=0 && t<2*M_PI ){

        VALUES(ref,0,0)=0.5 - 0.5*cos(t);
        VALUES(ref,1,0) = 0.5*sin(t);
    

    }else if(t>=2*M_PI && t<4*M_PI){

        VALUES(ref,0,0)= -0.5 + 0.5*cos(t);
        VALUES(ref,1,0) = -0.5*sin(t);
       

    }else{

        VALUES(ref,0,0)= 0;
        VALUES(ref,1,0) = 0;
    }
}

void *ref_thread(void *args) {
    double t = 0;       //tempo calculado
    double tm = 0;      //tempo medido
    double T = TEMPO_REF;      //milissegundos
    struct timespec ts1, ts2, ts3={0};

    struct timespec ts0 = {0};
    clock_gettime(CLOCK_REALTIME, &ts0);
    
    Matrix *ref = matrix_zeros(2,1);

    while(t <= TEMPO_MAX) {
        clock_gettime(CLOCK_REALTIME, &ts1);
        t = t + T;

        // Cálculo de Jitter
        if (contRef != 0) {
            periodo_Ref[contRef-1] = lat(ts1, tm);
            JitterRef[contRef-1] = lat(ts1, tm) - T; // milisegundos
        }
        
        contRef++;
        tm = ts2mili(ts1);

        // Cálculo de ref e set de mutexes
        mutexes_getRef(ref);

        // printf("tm: %lf\n", tm);
        
        calc_ref(ref, (tm-ts2mili(ts0))/1000.0);

        mutexes_setRef(ref);

        clock_gettime(CLOCK_REALTIME, &ts2);
        ts3.tv_sec = 0;
        ts3.tv_nsec = T*1000000 - (ts2.tv_nsec - ts1.tv_nsec);
        nanosleep(&ts3, &ts3);
    }

    matrix_free(ref);
    return 0;
}
