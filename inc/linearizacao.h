#ifndef _LINEARIZACAO_H
#define _LINEARIZACAO_H

#include "Matrix.h"

double *getPeriodoLinearizacao();
double *getJitterLinearizacao();

void calc_u(Matrix* U, Matrix* X, Matrix* V, double Raio);

void *linearizacao_thread(void *args);

#endif
