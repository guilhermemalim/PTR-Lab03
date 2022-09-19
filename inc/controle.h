/*
    Define a thread Ref
*/
#ifndef _CONTROLE_H
#define _CONTROLE_H

#include "Matrix.h"

double *getPeriodoControle();

double *getJitterControle();

void calc_v(Matrix *V, Matrix* ym_ponto, Matrix* Ym, Matrix* Y);
void *controle_thread(void *args);

#endif
