
// #include <math.h>
// #include <time.h>

// #include "Matrix.h"

// #include "mutexes.h"

// #include "Parametros.h"

// Matrix* calc_V(Matrix *Ymponto, Matrix *Ym, Matrix *Y) {
//     Matrix* Vt = matrix_zeros(2,1);

//     VALUES(Vt, 0, 0) = matrix_get_value(Ymponto, 0, 0) + ALPHA1 * (matrix_get_value(Ym, 0, 0) - matrix_get_value(Y, 0, 0));
//     VALUES(Vt, 1, 0) = matrix_get_value(Ymponto, 1, 0) + ALPHA2 * (matrix_get_value(Ym, 1, 0) - matrix_get_value(Y, 1, 0));

//     return Vt;
// }

// void *controle_thread() {
//     double t = 0;       //tempo calculado
//     double tm = 0;      //tempo medido
//     double T = TEMPO_CONTROLE;      //milissegundos
//     struct timespec ts1, ts2, ts3={0};
    
//     Matrix *Ymponto = NULL;
//     Matrix *Ym = NULL;
//     Matrix *Y = NULL;
//     Matrix* V = NULL;
    
//     while(t <= TEMPO_MAX) {
//         clock_gettime(CLOCK_REALTIME, &ts1);
//         tm = 1000000 * ts1.tv_nsec - tm;
//         t = t + T;

//         matrix_free(V);
//         bufferV=ControleBloco(bufferYmLinha, bufferYm, bufferY);

//         // Limpar o ref anterior
//         mutexes_getRef(ref);
//         matrix_free(ref);
        
//         ref = calc_ref(tm);

//         mutexes_setRef(ref);

//         clock_gettime(CLOCK_REALTIME, &ts2);
//         ts3.tv_sec = 0;
//         ts3.tv_nsec = T*1000000 - (ts2.tv_nsec - ts1.tv_nsec);
//         nanosleep(&ts3, &ts3);
//     }
// }