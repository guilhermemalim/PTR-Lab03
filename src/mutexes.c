/*
    Implementação dos mutexes e das variáveis
*/

#include <pthread.h>

#include "matrix.h"
#include "mutexes.h"

static Matrix Ref;
static Matrix Y;
static Matrix Ym;
static Matrix Ymdot;
static Matrix V;
static Matrix U;
static Matrix X;

static pthread_mutex_t mutexRef     = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexY       = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexYm      = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexYmdot   = PTHREAD_MUTEX_INITIALIZER;

void mutexes_init() {
    Ref     = matrix_zeros(2,1);
    Y       = matrix_zeros(2,3);
    Ym      = matrix_zeros(2,3);
    Ymdot   = matrix_zeros(2,3);
}

void mutexes_destroy() {
    pthread_mutex_destroy(&mutexRef);
    pthread_mutex_destroy(&mutexY);
    pthread_mutex_destroy(&mutexYm);
    pthread_mutex_destroy(&mutexYmdot);
}

void mutexes_setRef(Matrix ref) {
    pthread_mutex_lock(&mutexRef);
        Ref = ref;
    pthread_mutex_unlock(&mutexRef);
}

void mutexes_getRef(Matrix *ref) {
    if(!ref) return;
    pthread_mutex_lock(&mutexRef);
        *ref = Ref;
    pthread_mutex_unlock(&mutexRef);
}

void mutexes_setY(Matrix y) {
    pthread_mutex_lock(&mutexY);
        Y = y;
    pthread_mutex_unlock(&mutexY);
}

void mutexes_getY(Matrix *y) {
    if(!y) return;
    pthread_mutex_lock(&mutexY);
        *y = Y;
    pthread_mutex_unlock(&mutexY);
}

void mutexes_setYm(Matrix  ym);
void mutexes_getYm(Matrix *ym);
void mutexes_setYmdot(Matrix  ymdot);
void mutexes_getYmdot(Matrix *ymdot);



