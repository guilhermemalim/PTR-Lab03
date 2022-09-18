/*
    Define a thread Ref
*/
#pragma once
#include <math.h>
#include <time.h>
#include <stdio.h>

#include "ref.h"

#include "Matrix.h"

#include "mutexes.h"

#include "Parametros.h"

#define PI 3.14
#define MPI 3140
void calc_ref(Matrix* ref, double t);

void *ref_thread(void *args);

void calc_ymponto(Matrix* ym_ponto, Matrix* ref, Matrix* Ym);
void calc_ym(Matrix* ym, Matrix* ym_ponto, Matrix* ym_ponto_antigo);

void *modelo_ref_thread(void *args);

void calc_v(Matrix *V, Matrix* ym_ponto, Matrix* Ym, Matrix* Y);
void *controle_thread(void *args);

void calc_u(Matrix* U, Matrix* X, Matrix* V, double R);
void *linearizacao_thread(void *args);

void calc_xdot(Matrix* Xponto, Matrix* X, Matrix* U);

// avaliar processo de integração
void calc_x(Matrix* X, Matrix* Xdot);

void calc_y(Matrix* Y, Matrix* X, double R);

void *robo_thread(void *args);

void *print_thread(void *args);

