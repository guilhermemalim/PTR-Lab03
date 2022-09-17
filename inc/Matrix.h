#ifndef _MATRIX_H_
#define _MATRIX_H_

#ifndef NaN
#include <float.h>
#define NaN DBL_MAX;
// DBL_MAX é o maior valor possível de um double
#endif 

#ifndef VALUES
#define VALUES(m, i, j) (m->values[i * m->ncols + j])
#endif

#define PROCESSO_SEM_ERRO 0
#define ERRO_NA_MULTIPLICACAO 1
#define MATRIZ_NAO_INVERSIVEL 2
#define MATRIZES_COM_DIM_DIFERENTES 3

typedef struct TMatrix {
	unsigned int nlins;
	unsigned int ncols;
	double      *values;
} Matrix;

typedef struct TMResponse {
    Matrix *m;
    short erro;
} MResponse;

typedef double (*F_aplicavel)(double);

extern Matrix M_NULL;
extern Matrix *MATRIX_NULL;

Matrix* _alloc_matrix(unsigned int nlins, unsigned int ncols);

Matrix* matrix_zeros(unsigned int nlins, unsigned int ncols);
Matrix* matrix_ones(unsigned int nlins, unsigned int ncols);
Matrix* matrix_identity(unsigned int nlins, unsigned int ncols);
Matrix* matrix_copy(Matrix* m);
Matrix* matrix_apply(F_aplicavel f, Matrix* m);

void matrix_free(Matrix *m);
void print_matrix(Matrix* m);
Matrix* matrix_transposta(Matrix* m);

double       matrix_get_value(Matrix* m, int i, int j);
unsigned int matrix_nlins(Matrix* m);
unsigned int matrix_ncols(Matrix* m);
Matrix* create_matrix();
Matrix* create_random_matrix(unsigned int nlins, unsigned int ncols);

// Operações entre matriz-escalar
Matrix* matrix_sum_com_escalar(double c, Matrix* m);
Matrix* matrix_dif_com_escalar(double c, Matrix* m);
Matrix* matrix_mul_com_escalar(double c, Matrix* m);

// Operações entre matrizes
MResponse matrix_sum(Matrix* A, Matrix* B);
MResponse matrix_dif(Matrix* A, Matrix* B);
MResponse matrix_mul(Matrix* A, Matrix* B);

Matrix* matrix_get_cofactor(Matrix *m, int i, int j);
// determinante de uma matriz quadrada
double matrix_det(Matrix *m);

MResponse matrix_inversa(Matrix *m);

#endif
