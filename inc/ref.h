#ifndef _REF_H
#define _REF_H

#include "Matrix.h"

double *getPeriodoRef();
double *getJitterRef();

void calc_ref(Matrix* ref, double t);

void *ref_thread(void *args);

#endif
