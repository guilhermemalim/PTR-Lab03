#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Parametros.h"
#include "Matrix.h"
#include "mutexes.h"

void *print_thread(void *args) {
    double t = 0;       //tempo calculado
    double tm = 0;      //tempo medido
    double T = 100;      //milissegundos
    struct timespec ts1, ts2, ts3 = {0};
    
    Matrix* Ref      = matrix_zeros(2,1);
    Matrix* Y        = matrix_zeros(2,1);
    Matrix* Ym       = matrix_zeros(2,1);
    Matrix* Ymponto  = matrix_zeros(2,1);
    Matrix* V        = matrix_zeros(2,1);
    Matrix* U        = matrix_zeros(2,1);
    Matrix* X        = matrix_zeros(3,1);

    Matrix* Xdot        = matrix_zeros(3,1);


    while(t <= TEMPO_MAX) {
        clock_gettime(CLOCK_REALTIME, &ts1);
        tm = ts1.tv_nsec/1000000 - tm;
        t = t + T;

        mutexes_getRef(Ref);
        mutexes_getYmdot(Ymponto);
        mutexes_getYm(Ym);
        mutexes_getV(V);
        mutexes_getU(U);
        mutexes_getX(X);
        mutexes_getY(Y);

        mutexes_getXdot(Xdot);

        
        printf("%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf, %.4lf, %.4lf, %.4lf\n",
               t,
               matrix_get_value(Ref, 0, 0),     // X_ref
               matrix_get_value(Ref, 1, 0),     // Y_ref
               matrix_get_value(Ymponto, 0, 0), // Ydot_x
               matrix_get_value(Ymponto, 1, 0), // Ydot_x
               matrix_get_value(Ym, 0, 0),      // Ymx
               matrix_get_value(Ym, 1, 0),      // Ymy
               matrix_get_value(V, 0, 0),       // Vx
               matrix_get_value(V, 1, 0),       // Vy
               matrix_get_value(U, 0, 0),       // Ux
               matrix_get_value(U, 1, 0),       // Uy
               matrix_get_value(X, 0, 0),       // X1
               matrix_get_value(X, 1, 0),       // X2
               matrix_get_value(X, 2, 0),       // X3
               matrix_get_value(Y, 0, 0),       // Y1
               matrix_get_value(Y, 1, 0),       // Y2
               matrix_get_value(Xdot, 0, 0),    // Xdot1
               matrix_get_value(Xdot, 1, 0),    // Xdot2
               matrix_get_value(Xdot, 2, 0)     // Xdot3
        );


        clock_gettime(CLOCK_REALTIME, &ts2);
        ts3.tv_sec = 0;
        ts3.tv_nsec = T*1000000 - (ts2.tv_nsec - ts1.tv_nsec);
        nanosleep(&ts3, &ts3);
    }

    matrix_free(Ref);
    matrix_free(Y);
    matrix_free(Ym);
    matrix_free(Ymponto);
    matrix_free(V);
    matrix_free(U);
    matrix_free(X);

    return 0;
}
