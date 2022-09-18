// /*
//     Define a thread Ref
// */

// #include <math.h>
// #include <time.h>

// #include "modelo_ref.h"

// #include "Matrix.h"

// #include "mutexes.h"

// #include "Parametros.h"

// double passo = 0.12;

// void calc_ymponto(Matrix* ym_ponto, Matrix* ref, Matrix* Ym) {
    
//     VALUES(ym_ponto, 0, 0) = ALPHA1*(matrix_get_value(ref,0,0)-matrix_get_value(Ym,0,0)); // ymx_ponto = alpha1(X_ref - y_mx)
//     VALUES(ym_ponto, 1, 0) = ALPHA2*(matrix_get_value(ref,1,0)-matrix_get_value(Ym,1,0)); // ymy_ponto = alpha2(Y_ref) 
// }

// void calc_ym(Matrix* ym, Matrix* ym_ponto, Matrix* ym_ponto_antigo)
// {
//     VALUES(ym, 0, 0) = (passo) * (matrix_get_value(ym_ponto, 0, 0) + matrix_get_value(ym_ponto_antigo, 0, 0)) / 2;
//     VALUES(ym, 1, 0) = (passo) * (matrix_get_value(ym_ponto, 1, 0) + matrix_get_value(ym_ponto_antigo, 1, 0)) / 2;
// }

// void *modelo_ref_thread(void *args) {
//     double t = 0;       //tempo calculado
//     double tm = 0;      //tempo medido
//     double T = TEMPO_MODELO_REF;      //milissegundos
//     struct timespec ts1, ts2, ts3={0};
    
//     Matrix* ref = matrix_zeros(2,1);
//     Matrix* Ym = matrix_zeros(2,1);
//     Matrix* Ymponto = matrix_zeros(2,1);
//     Matrix* Ymponto_anterior = matrix_zeros(2,1);

//     while(t <= TEMPO_MAX) {
//         clock_gettime(CLOCK_REALTIME, &ts1);
//         tm = 1000000 * ts1.tv_nsec - tm;
//         t = t + T;

//         printf("ok2 t: %lf\n", t);

//         mutexes_getRef(ref);
//         mutexes_getYmdot(Ymponto);
//         Ymponto_anterior = matrix_copy(Ymponto);

//         // limpar o anterior e setar
//         calc_ymponto(Ymponto, ref, Ymponto_anterior);
//         mutexes_setYmdot(Ymponto);

//         // limpar o anterior e setar
//         mutexes_getYm(Ym);
//         calc_ym(Ym, Ymponto, Ymponto_anterior);
//         mutexes_setYm(Ym);

//         clock_gettime(CLOCK_REALTIME, &ts2);
//         ts3.tv_sec = 0;
//         ts3.tv_nsec = T*1000000 - (ts2.tv_nsec - ts1.tv_nsec);
//         nanosleep(&ts3, &ts3);
//     }
//     printf("out2\n");

//     matrix_free(ref);
//     matrix_free(Ym);
//     matrix_free(Ymponto);
//     matrix_free(Ymponto_anterior);

//     return 0;
// }