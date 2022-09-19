/*
    Define a thread Ref
*/

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "Matrix.h"
#include "mutexes.h"
#include "Parametros.h"


const double R = 0.3;

// Contadores
int contRef = 0;
int contModeloRef = 0;
int contControle = 0;
int contLinearizacao = 0;
int contRobo = 0;

// Vetores de periodo
double periodo_Ref[(int) (TEMPO_MAX/TEMPO_REF)] = {0};
double periodo_ModeloRef[(int) (TEMPO_MAX/TEMPO_MODELO_REF)] = {0};
double periodo_Controle[(int) (TEMPO_MAX/TEMPO_CONTROLE)] = {0};
double periodo_Linearizacao[(int) (TEMPO_MAX/TEMPO_LINEARIZACAO)] = {0};
double periodo_Robo[(int) (TEMPO_MAX/TEMPO_ROBO)] = {0};

double *getPeriodoRef() {
    return periodo_Ref;
}

double *getPeriodoModeloRef() {
    return periodo_ModeloRef;
}

double *getPeriodoControle() {
    return periodo_Controle;
}

double *getPeriodoLinearizacao() {
    return periodo_Linearizacao;
}

double *getPeriodoRobo() {
    return periodo_Robo;
}


// Vetores de Jitters
double JitterRef[(int) (TEMPO_MAX/TEMPO_REF)] = {0};
double JitterModeloRef[(int) (TEMPO_MAX/TEMPO_MODELO_REF)] = {0};
double JitterControle[(int) (TEMPO_MAX/TEMPO_CONTROLE)] = {0};
double JitterLinearizacao[(int) (TEMPO_MAX/TEMPO_LINEARIZACAO)] = {0};
double JitterRobo[(int) (TEMPO_MAX/TEMPO_ROBO)] = {0};

double *getJitterRef() {
    return JitterRef;
}

double *getJitterModeloRef() {
    return JitterModeloRef;
}

double *getJitterControle() {
    return JitterControle;
}

double *getJitterLinearizacao() {
    return JitterLinearizacao;
}

double *getJitterRobo() {
    return JitterRobo;
}


double ts2mili(struct timespec ts) {
    return (double) ts.tv_sec*1000.0 + ts.tv_nsec/1000000.0;
}

double lat(struct timespec t1, double t2){
    return ts2mili(t1) - t2;
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


void calc_ymponto(Matrix* ym_ponto, Matrix* ref, Matrix* Ym) {
    
    VALUES(ym_ponto, 0, 0) = ALPHA1*(matrix_get_value(ref,0,0)-matrix_get_value(Ym,0,0)); // ymx_ponto = alpha1(X_ref - y_mx)
    VALUES(ym_ponto, 1, 0) = ALPHA2*(matrix_get_value(ref,1,0)-matrix_get_value(Ym,1,0)); // ymy_ponto = alpha2(Y_ref) 
}


// aqui eu mudei da integral para isso, verificar resultados e retornar
void calc_ym(Matrix* ym, Matrix* ym_ponto, Matrix* ym_ponto_antigo, double dt)
{
    VALUES(ym, 0, 0) =  (dt) * (matrix_get_value(ym_ponto, 0, 0) + matrix_get_value(ym_ponto_antigo, 0, 0)) / 2;
    VALUES(ym, 1, 0) =  (dt) * (matrix_get_value(ym_ponto, 1, 0) + matrix_get_value(ym_ponto_antigo, 1, 0)) / 2;
    // printf("ym: %lf\n", matrix_get_value(ym, 0, 0));
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
        if (contControle != 0) {
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


void calc_u(Matrix* U, Matrix* X, Matrix* V, double R)
{
    Matrix* L = matrix_zeros(2,2);

    double x3 = matrix_get_value(X,2,0);
    
    VALUES(L, 0, 0) = cos(x3);
    VALUES(L, 0, 1) = -R*sin(x3);
    VALUES(L, 1, 0) = sin(x3);
    VALUES(L, 1, 1) = R*cos(x3);

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

void calc_y(Matrix* Y, Matrix* X, double R)
{
    Matrix* C = matrix_zeros(2,3);
    VALUES(C, 0, 0) = 1;
    VALUES(C, 1, 1) = 1;
    
    Matrix* D = matrix_zeros(2, 1);
    double x3 = matrix_get_value(X, 2, 0);
    VALUES(D, 0, 0) = R*cos(x3);
    VALUES(D, 1, 0) = R*sin(x3);

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


void *print_thread(void *args) {
    double t = 0;       //tempo calculado
    double tm = 0;      //tempo medido
    double T = 300;      //milissegundos
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
        if (contRobo != 0)
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

    // void print_vector(double* v, int s) {
    //     for (int i = 0; i < s + 1; i++) {
    //         printf("%.4lf ", v[i]);
    //     }
    //     printf("\n");
    // }

    // printf("--- Estatisticas ---\n");

    // print_vector(JitterRef, (int) (TEMPO_MAX / TEMPO_REF));
    // print_vector(JitterModeloRef, (int) (TEMPO_MAX / TEMPO_MODELO_REF));
    // print_vector(JitterControle, (int) (TEMPO_MAX / TEMPO_CONTROLE));
    // print_vector(JitterLinearizacao, (int) (TEMPO_MAX / TEMPO_LINEARIZACAO));
    // print_vector(JitterRobo, (int) (TEMPO_MAX / TEMPO_ROBO));

    matrix_free(Ref);
    matrix_free(Y);
    matrix_free(Ym);
    matrix_free(Ymponto);
    matrix_free(V);
    matrix_free(U);
    matrix_free(X);

    return 0;
}
