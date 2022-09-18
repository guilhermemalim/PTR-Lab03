/*
    Implementação de Ref
*/
#pragma once
#include "Matrix.h"

void calc_ref(Matrix* ref, double t);
void *ref_thread(void *args);
