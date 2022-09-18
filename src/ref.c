// /*
//     Define a thread Ref
// */

// #include <math.h>
// #include <time.h>
// #include <stdio.h>

// #include "ref.h"

// #include "Matrix.h"

// #include "mutexes.h"

// #include "Parametros.h"

// #define PI 3.14
// #define MPI 3140


// void calc_ref(Matrix* ref, double t) {
    
//     if(t >= 0  && t <= 2*MPI) {
//         // ref->nlins = 2;
//         // ref->ncols = 1;
//         VALUES(ref, 0, 0) = 1/2 - 1/2*cos(t);
//         VALUES(ref, 1, 0) = 1/2 * sin(t);
//         // printf("aqui\n");
//     } else if (t <= 4*MPI){
//         // ref->nlins = 2;
//         // ref->ncols = 1;
//         VALUES(ref, 0, 0) = -1/2 + 1/2 * cos(t);
//         VALUES(ref, 1, 0) = -1/2 * sin(t);
//     } else {
//         VALUES(ref, 0, 0) = 0;
//         VALUES(ref, 1, 0) = 0;
//     }
// }

// void *ref_thread(void *args) {
//     double t = 0;       //tempo calculado
//     double tm = 0;      //tempo medido
//     double T = TEMPO_REF;      //milissegundos
//     struct timespec ts1, ts2, ts3={0};
    
//     Matrix *ref = matrix_zeros(2,1);
//     Matrix *Ymponto = matrix_zeros(2,1);

//     while(t <= TEMPO_MAX) {
//         clock_gettime(CLOCK_REALTIME, &ts1);
//         tm = 1000000 * ts1.tv_nsec - tm;
//         t = t + T;

//         printf("ok1\n");

//         mutexes_getRef(ref);
//         // matrix_free(ref);
        
//         calc_ref(ref, t);
        
//         mutexes_getYmdot(Ymponto);

//         // printf("%lf, %lf, %lf, %lf, %lf\n", t, matrix_get_value(ref, 0, 0), matrix_get_value(ref, 1, 0), 
//         //     matrix_get_value(Ymponto, 0, 0), matrix_get_value(Ymponto, 1, 0));
//         mutexes_setRef(ref);

//         clock_gettime(CLOCK_REALTIME, &ts2);
//         ts3.tv_sec = 0;
//         ts3.tv_nsec = T*1000000 - (ts2.tv_nsec - ts1.tv_nsec);
//         nanosleep(&ts3, &ts3);
//     }

//     matrix_free(ref);
//     matrix_free(Ymponto);

//     return 0;
// }