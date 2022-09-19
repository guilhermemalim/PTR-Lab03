#ifndef _ROBO_H
#define _ROBO_H

#include "Matrix.h"

double *getPeriodoRobo();
double *getJitterRobo();

void calc_xdot(Matrix* Xponto, Matrix* X, Matrix* U);
void calc_x(Matrix* X, Matrix* Xdot, Matrix* Xdot_antigo, double dt);

void calc_y(Matrix* Y, Matrix* X, double Raio);

void *robo_thread(void *args);

#endif
