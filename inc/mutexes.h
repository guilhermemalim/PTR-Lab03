/*
    Define o monitor para acesso às variáveis
*/

#ifndef _MUTEXES_H
#define _MUTEXES_H

#include "Matrix.h"

void mutexes_init();
void mutexes_destroy();
void mutexes_setRef(Matrix  *ref);
void mutexes_getRef(Matrix *ref);
void mutexes_setY(Matrix  *y);
void mutexes_getY(Matrix *y);
void mutexes_setYm(Matrix  *ym);
void mutexes_getYm(Matrix *ym);
void mutexes_setYmdot(Matrix  *ymdot);
void mutexes_getYmdot(Matrix *ymdot);

#endif

