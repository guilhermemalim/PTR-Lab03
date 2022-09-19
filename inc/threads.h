/*
    Define a thread Ref
*/
#pragma once
#include <math.h>
#include <time.h>
#include <stdio.h>
#include "Matrix.h"
#include "mutexes.h"
#include "Parametros.h"

double *getJitterRef();
double *getJitterModeloRef();
double *getJitterControle();
double *getJitterLinearizacao();
double *getJitterRobo();

double *getPeriodoRef();

double *getPeriodoModeloRef();

double *getPeriodoControle();

double *getPeriodoLinearizacao();

double *getPeriodoRobo();



//Ref
void calc_ref(Matrix* ref, double t);
void *ref_thread(void *args);

//Ym
void calc_ymponto(Matrix* ym_ponto, Matrix* ref, Matrix* Ym);
void calc_ym(Matrix* ym, Matrix* ym_ponto, Matrix* ym_ponto_antigo, double dt);

//Bloco ModeloRef
void *modelo_ref_thread(void *args);

//Bloco Controle
void calc_v(Matrix *V, Matrix* ym_ponto, Matrix* Ym, Matrix* Y);
void *controle_thread(void *args);

//Bloco Linearização
void calc_u(Matrix* U, Matrix* X, Matrix* V, double R);
void *linearizacao_thread(void *args);

//Bloco Robo
void calc_xdot(Matrix* Xponto, Matrix* X, Matrix* U);
void calc_x(Matrix* X, Matrix* Xdot, Matrix* Xdot_antigo, double dt);
void calc_y(Matrix* Y, Matrix* X, double R);
void *robo_thread(void *args);

//Saida
void *print_thread(void *args);

