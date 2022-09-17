#include "Integral.h"

double _area_trapezio(double a, double b, double h) {
    return (a+b)*h / 2;
}

// Utiliza o método de soma de trapézio
double integrate(F_integravel f, double a, double b)  {
    double delta = DELTA_0;
    double soma = 0;

    while (a <= b) {
        soma += _area_trapezio(f(a), f(a+delta), delta);
        a += delta;
    }

    return soma;
}
