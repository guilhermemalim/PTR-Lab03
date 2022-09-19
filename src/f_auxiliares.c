#include <stdio.h>
#include <math.h>
#include <time.h>

#include "f_auxiliares.h"

double ts2mili(struct timespec ts) {
    return (double) ts.tv_sec*1000.0 + ts.tv_nsec/1000000.0;
}

double lat(struct timespec t1, double t2){
    return ts2mili(t1) - t2;
}

void print_vector(double* v, int s) {
    for (int i = 0; i < s + 1; i++) {
        printf("%.4lf ", v[i]);
    }
    printf("\n");
}

void printar_estatisticas(double* arr, int size) {
    double max = arr[0]; double min  = arr[0];
    
    double media = 0;
    for (int i = 0; i < size; i++) {

        if (arr[i] > max) max = arr[i];
        if (arr[i] < min) min = arr[i];
        
        media += arr[i];
    }
    media /= size;

    double var = 0;
    for (int i = 0; i < size; i++) {
        var += pow(arr[i] - media, 2);
    }
    var /= size;

    double dp = sqrt(var);

    printf("med:%lf,var:%lf,dp:%lf,max:%lf,min:%lf\n", media, var, dp, max, min);
}
