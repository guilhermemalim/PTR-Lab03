#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "robo.h"

#include "Matrix.h"
#include "mutexes.h"
#include "Parametros.h"

#include "f_auxiliares.h"

// Contadores
int contRobo = 0;

// Vetores de periodo
double periodo_Robo[(int) TAM_VETOR_ROBO] = {0};

double *getPeriodoRobo() {
    return periodo_Robo;
}


// Vetores de Jitters
double JitterRobo[(int) TAM_VETOR_ROBO] = {0};

double *getJitterRobo() {
    return JitterRobo;
}

void calc_xdot(Matrix* Xponto, Matrix* X, Matrix* U) {
    Matrix* A = matrix_zeros(3,2);

    /*
     * @brief geração da Matriz que multiplica u(t) no bloco do Robô 
     * 
     * A = [cos(x_3) 0;
     *      sin(x_3) 0;  
     *          0    1]
     */

    double x3 = matrix_get_value(X, 2, 0);

    VALUES(A, 0, 0) = cos(x3);
    VALUES(A, 0, 1) = 0;
    VALUES(A, 1, 0) = sin(x3);
    VALUES(A, 1, 1) = 0;
    VALUES(A, 2, 0) = 0;
    VALUES(A, 2, 1) = 1;

    MResponse response = matrix_mul(A, U);
    
    matrix_copy_values(Xponto, response.m);

    matrix_free(A);
    matrix_free(response.m);
}

// avaliar processo de integração
void calc_x(Matrix* X, Matrix* Xdot, Matrix* Xdot_antigo, double dt)
{
    VALUES(X, 0, 0) = (dt / 2) * (matrix_get_value(Xdot, 0, 0) + matrix_get_value(Xdot_antigo, 0, 0)) ;
    VALUES(X, 1, 0) = (dt / 2) * (matrix_get_value(Xdot, 1, 0) + matrix_get_value(Xdot_antigo, 1, 0)) ;
    VALUES(X, 2, 0) = (dt / 2) * (matrix_get_value(Xdot, 2, 0) + matrix_get_value(Xdot_antigo, 2, 0)) ;
}

void calc_y(Matrix* Y, Matrix* X, double Raio)
{
    Matrix* C = matrix_zeros(2,3);
    VALUES(C, 0, 0) = 1;
    VALUES(C, 1, 1) = 1;
    
    Matrix* D = matrix_zeros(2, 1);
    double x3 = matrix_get_value(X, 2, 0);
    VALUES(D, 0, 0) = Raio*cos(x3);
    VALUES(D, 1, 0) = Raio*sin(x3);

    MResponse response = matrix_mul(C, X);
    Matrix* C_x = response.m;
    
    VALUES(Y,0,0) = matrix_get_value(C_x,0,0) + matrix_get_value(D,0,0);
    VALUES(Y,1,0) = matrix_get_value(C_x,1,0) + matrix_get_value(D,1,0);
    
    matrix_free(C);
    matrix_free(D);
    matrix_free(C_x);
}

void *robo_thread(void *args) {
    double t = 0;       //tempo calculado
    double tm = 0;      //tempo medido
    double T = TEMPO_ROBO;      //milissegundos
    struct timespec ts1, ts2, ts3 = {0};
    
    Matrix* X = matrix_zeros(3,1);
    Matrix* U = matrix_zeros(2,1);

    Matrix* Y = matrix_zeros(2,1);
    Matrix* Xdot = matrix_zeros(3,1);
    Matrix* Xdot_anterior = matrix_zeros(3,1);

    while(t <= TEMPO_MAX) {
        clock_gettime(CLOCK_REALTIME, &ts1);
        t = t + T;

        // Cálculo de Jitter
        if (contRobo != 0) {
            periodo_Robo[contRobo-1] = lat(ts1, tm);
            JitterRobo[contRobo-1] = lat(ts1, tm) - T; // milisegundos
        }

        contRobo++;
        tm = ts2mili(ts1);


        mutexes_getX(X);
        mutexes_getU(U);

        matrix_copy_values(Xdot_anterior, Xdot);

        calc_xdot(Xdot, X, U);
        calc_x(X, Xdot, Xdot_anterior, 0.03);
        calc_y(Y, X, R);

        mutexes_setXdot(Xdot);
        mutexes_setX(X);
        mutexes_setY(Y);

        clock_gettime(CLOCK_REALTIME, &ts2);
        ts3.tv_sec = 0;
        ts3.tv_nsec = T*1000000 - (ts2.tv_nsec - ts1.tv_nsec);
        nanosleep(&ts3, &ts3);
    }

    matrix_free(X);
    matrix_free(U);
    matrix_free(Y);
    matrix_free(Xdot);

    return 0;
}
