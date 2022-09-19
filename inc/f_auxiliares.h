#ifndef _F_AUXILIARES_H
#define _F_AUXILIARES_H

#include <time.h>

double ts2mili(struct timespec ts);

double lat(struct timespec t1, double t2);

void print_vector(double* v, int s);

void printar_estatisticas(double* arr, int size);

#endif
