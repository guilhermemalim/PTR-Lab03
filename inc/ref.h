/*
    Define a thread Ref
*/

#include <math.h>
#include <time.h>

#include "Matrix.h"

#include "mutexes.h"


Matrix* calc_ref(double t) {
    Matrix *ref;
    if(t < 0 && t >= 4*M_PI)
        *ref = matrix_zeros(2,1);
    else if(t >= 0  && t < 2*M_PI) {
        ref->nlins = 2;
        ref->ncols = 1;
        VALUES(ref, 0, 0) = 1/2 - 1/2 * cos(t);
        VALUES(ref, 1, 0) = 1/2 * sin(t);
    } else {
        ref->nlins = 2;
        ref->ncols = 1;
        VALUES(ref, 0, 0) = -1/2 + 1/2 * cos(t);
        VALUES(ref, 1, 0) = -1/2 * sin(t);
    }
    return ref;
}

void *ref_thread(void *) {
    double t = 0;       //tempo calculado
    double tm = 0;      //tempo medido
    double T = 50;      //milissegundos
    struct timespec ts1, ts2, ts3={0};
    
    Matrix *ref;

    while(t <= 13000) {
        clock_gettime(CLOCK_REALTIME, &ts1);
        tm = 1000000 * ts1.tv_nsec - tm;
        t = t + T;
        
        ref = calc_ref(tm);
        mutexes_setRef(ref);

        clock_gettime(CLOCK_REALTIME, &ts2);
        ts3.tv_sec = 0;
        ts3.tv_nsec = T*1_000_000 - (ts2.tv_nsec - ts1.tv_nsec);
        nanosleep(&ts3, &ts3);
    }
}