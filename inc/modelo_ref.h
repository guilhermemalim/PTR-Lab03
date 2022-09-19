#ifndef _MODELO_REF_H
#define _MODELO_REF_H

#include "Matrix.h"

double *getPeriodoModeloRef();
double *getJitterModeloRef();

void calc_ymponto(Matrix* ym_ponto, Matrix* ref, Matrix* Ym);

void calc_ym(Matrix* ym, Matrix* ym_ponto, Matrix* ym_ponto_antigo, double dt);

void *modelo_ref_thread(void *args);

#endif

