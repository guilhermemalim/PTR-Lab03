#include <stdlib.h>
#include <stdio.h>
#include "Matrix.h"

Matrix M_NULL = {.nlins = 0, .ncols = 0, .values = NULL};
Matrix *MATRIX_NULL = &M_NULL;

Matrix* _alloc_matrix(unsigned int nlins, unsigned int ncols) {
    Matrix *m = malloc(sizeof(Matrix));
    
    m->nlins = nlins;
    m->ncols = ncols;
    m->values = malloc(sizeof(double) * nlins*ncols);
    
    return m;
}

Matrix* matrix_zeros(unsigned int nlins, unsigned int ncols) {
    Matrix *m = _alloc_matrix(nlins, ncols);
    
    // setar os valores da matriz = 0
    for (int i = 0 ; i < nlins; i++) {
        for (int j = 0; j < ncols; j++) {
            VALUES(m, i, j) = 0;
        }
    }

    return m;
}

Matrix* matrix_ones(unsigned int nlins, unsigned int ncols) {
    Matrix *m = _alloc_matrix(nlins, ncols);

    // setar os valores da matriz = 1
    for (int i = 0 ; i < nlins; i++) {
        for (int j = 0; j < ncols; j++) {
            VALUES(m, i, j) = 1;
        }
    }

    return m;
}

Matrix* matrix_identity(unsigned int nlins, unsigned int ncols) {
    Matrix *m = matrix_zeros(nlins, ncols);

    for (int i = 0; i < nlins; i++) {
        if (i >= ncols) break;

        VALUES(m, i, i) = 1;
    }

    return m;
}

Matrix* matrix_copy(Matrix* m) { 
    unsigned int nlins = matrix_nlins(m);
    unsigned int ncols = matrix_ncols(m);
    
    Matrix *cp = _alloc_matrix(nlins, ncols);

    for (int i = 0; i < nlins; i++) {
        for (int j = 0; j < ncols; j++) {
            VALUES(cp, i, j) = matrix_get_value(m, i, j);
        }
    }

    return cp;
}

Matrix* matrix_apply(F_aplicavel f, Matrix* m) {
    unsigned int nlins = matrix_nlins(m);
    unsigned int ncols = matrix_ncols(m);
    double v = 0;

    Matrix *applied_m = _alloc_matrix(nlins, ncols);

    for (int i = 0; i < nlins; i++) {
        for (int j = 0; j < ncols; j++) {
            v = f(matrix_get_value(m, i, j));
            VALUES(applied_m, i, j) = v;
        }
    }

    return applied_m;
}

void matrix_free(Matrix *m) {
    free(m->values);
    free(m);
    m = NULL;
}
void print_matrix(Matrix* m) {
    unsigned int nlins = matrix_nlins(m);
    unsigned int ncols = matrix_ncols(m);

    for (int i = 0; i < nlins; i++) {
        for (int j = 0; j < ncols; j++) {
            printf("%lf ", matrix_get_value(m, i, j));
        }
        printf("\n");
    }
}

double matrix_get_value(Matrix* m, int i, int j) {
    if(i >= m->nlins || j >= m->ncols)
		return NaN;

	return m->values[i*m->ncols + j];
}
unsigned int matrix_nlins(Matrix* m) {
    return m->nlins;
}
unsigned int matrix_ncols(Matrix* m) {
    return m->ncols;
}

Matrix* create_matrix() {
    unsigned int nlins = 0, ncols = 0;
    double aux = 0;
    
    printf("Digite o numero de linhas e colunas (Ex: 3 3): ");
    scanf("%d %d\n", &nlins, &ncols);

    Matrix *m = _alloc_matrix(nlins, ncols);

    for (int i = 0; i < nlins; i++) {
        for (int j = 0; j < ncols; j++) {
            printf("Digite o valor da matrix em [%d][%d]: ", i, j);
            scanf("%lf\n", &aux);
            VALUES(m, i, j) = aux;
        }
    }
    
    return m;
}
Matrix* create_random_matrix(unsigned int nlins, unsigned int ncols) {
    int MOD = 15;
    Matrix *m = _alloc_matrix(nlins, ncols);

    for (int i = 0; i < nlins; i++) {
        for (int j = 0; j < ncols; j++) {
            VALUES(m, i, j) = rand() % MOD;
        }
    }
    
    return m;
}

// Operações entre matriz-escalar
Matrix* matrix_sum_com_escalar(double c, Matrix* m) {
    unsigned int nlins = matrix_nlins(m);
    unsigned int ncols = matrix_ncols(m);

    Matrix *m1 = _alloc_matrix(nlins, ncols);
    for (int i = 0; i < nlins; i++) {
        for (int j = 0; j < ncols; j++) {
            VALUES(m1, i, j) = matrix_get_value(m, i, j) + c;
        }
    }

    return m1;
}
Matrix* matrix_dif_com_escalar(double c, Matrix* m) {
    unsigned int nlins = matrix_nlins(m);
    unsigned int ncols = matrix_ncols(m);

    Matrix *m1 = _alloc_matrix(nlins, ncols);
    for (int i = 0; i < nlins; i++) {
        for (int j = 0; j < ncols; j++) {
            VALUES(m1, i, j) = matrix_get_value(m, i, j) - c;
        }
    }

    return m1;
}
Matrix* matrix_mul_com_escalar(double c, Matrix* m) {
    unsigned int nlins = matrix_nlins(m);
    unsigned int ncols = matrix_ncols(m);

    Matrix *m1 = _alloc_matrix(nlins, ncols);
    for (int i = 0; i < nlins; i++) {
        for (int j = 0; j < ncols; j++) {
            VALUES(m1, i, j) = matrix_get_value(m, i, j)*c;
        }
    }

    return m1;
}


// Operações entre matrizes
MResponse matrix_sum(Matrix* A, Matrix* B) {
    unsigned int nlins_A = matrix_nlins(A);
    unsigned int ncols_A = matrix_ncols(A);
    unsigned int nlins_B = matrix_nlins(B);
    unsigned int ncols_B = matrix_ncols(B);

    if ((nlins_A != nlins_B) || (ncols_A != ncols_B)) {
        MResponse response = {.m = MATRIX_NULL, .erro = MATRIZES_COM_DIM_DIFERENTES};
        return response;
    }

    Matrix *m = _alloc_matrix(nlins_A, ncols_B);

    for (int i = 0; i < nlins_A; i++) {
        for (int j = 0; j < ncols_B; j++) {
            VALUES(m, i, j) = matrix_get_value(A, i, j) + matrix_get_value(B, i, j);
        }
    }

    MResponse response = {.m = m, .erro = PROCESSO_SEM_ERRO};

    return response;
}
MResponse matrix_dif(Matrix* A, Matrix* B) {
    unsigned int nlins_A = matrix_nlins(A);
    unsigned int ncols_A = matrix_ncols(A);
    unsigned int nlins_B = matrix_nlins(B);
    unsigned int ncols_B = matrix_ncols(B);

    if ((nlins_A != nlins_B) || (ncols_A != ncols_B)) {
        MResponse response = {.m = MATRIX_NULL, .erro = MATRIZES_COM_DIM_DIFERENTES};
        return response;
    }

    Matrix *m = _alloc_matrix(nlins_A, ncols_B);

    for (int i = 0; i < nlins_A; i++) {
        for (int j = 0; j < ncols_B; j++) {
            VALUES(m, i, j) = matrix_get_value(A, i, j) - matrix_get_value(B, i, j);
        }
    }

    MResponse response = {.m = m, .erro = PROCESSO_SEM_ERRO};

    return response;
}
MResponse matrix_mul(Matrix* A, Matrix* B) {
    unsigned int nlins_A = matrix_nlins(A);
    unsigned int ncols_A = matrix_ncols(A);
    unsigned int nlins_B = matrix_nlins(B);
    unsigned int ncols_B = matrix_ncols(B);

    if (ncols_A != nlins_B) {
        MResponse response = {.m = MATRIX_NULL, .erro = ERRO_NA_MULTIPLICACAO};
        return response;
    }

    Matrix *m = _alloc_matrix(nlins_A, ncols_B);
    double sum = 0;
    // processar multiplicação
    for (int i = 0; i < nlins_A; i++) {
        for (int j = 0; j < ncols_B; j++) {
            sum = 0;

            for(int k = 0; k < ncols_A; k++) {
                sum += matrix_get_value(A, i, k) * matrix_get_value(B, k, j);
            }
            VALUES(m, i, j) = sum;
        }
    }

    MResponse response = {.m = m, .erro = PROCESSO_SEM_ERRO};

    return response;
}

Matrix* matrix_transposta(Matrix* m) {
    unsigned int nlins = matrix_nlins(m);
    unsigned int ncols = matrix_ncols(m);

    Matrix *t = _alloc_matrix(nlins, ncols);
    for (int i = 0; i < nlins; i++) {
        for (int j = 0; j < ncols; j++) {
            VALUES(t, i, j) = matrix_get_value(m, j, i);
        }
    }

    return t;
}

// referência:
// https://www.geeksforgeeks.org/determinant-of-a-matrix/
// Método dos cofatores - Teorema de Laplace
Matrix* matrix_get_cofactor(Matrix *m, int i, int j) {
    unsigned int nlins = matrix_nlins(m);
    unsigned int ncols = matrix_ncols(m);

    Matrix *c = _alloc_matrix(nlins-1, ncols-1);

    int ii = 0, jj = 0;

    for (int row = 0; row < nlins; row++) {
        for (int col = 0; col < ncols; col++) {
            if (row != i && col != j) {
                VALUES(c, ii, jj) = matrix_get_value(m, row, col);
                jj++;

                if (jj == ncols - 1) {
                    jj = 0;
                    ii++;
                }
            }
        }
    }

    return c;
}
double matrix_det(Matrix *m) {
    double det = 0;

    unsigned int nlins = matrix_nlins(m);
    unsigned int ncols = matrix_ncols(m);

    if (nlins != ncols) return NaN;
    // Método definido apenas para matriz quadrada 
 
    //  Caso base: a matrix contém apenas um elemento
    if (nlins == ncols && nlins == 1)
        return matrix_get_value(m, 0, 0);

    Matrix* temp = NULL;
 
    int sign = 1; // sinal do cofator

    for (int f = 0; f < nlins; f++) {
        temp = matrix_get_cofactor(m, 0, f);
        det += sign * matrix_get_value(m, 0, f) * matrix_det(temp);

        matrix_free(temp);  
        sign = -sign;
    }
 
    return det;
}

// Implementação da matriz inversa pela matrix adjunta
// A^-1 = (1/det(A)) * (adj(A))
MResponse matrix_inversa(Matrix *m) {
    unsigned int nlins = matrix_nlins(m);
    unsigned int ncols = matrix_ncols(m);

    double det = matrix_det(m);
    
    // matriz não inversível
    if (det == 0) {
        MResponse response = {.m = MATRIX_NULL, .erro = MATRIZ_NAO_INVERSIVEL};
        return response;
    }
    
    Matrix* m1 = _alloc_matrix(nlins, ncols);
    Matrix *aux = NULL;
    short sign = 1;

    for (int i = 0; i < nlins; i++) {
        for (int j = 0; j < ncols; j++) {
            aux = matrix_get_cofactor(m, i, j);
            VALUES(m1, i, j) = sign * matrix_det(aux);

            sign = -sign;
            matrix_free(aux);
        }
    }

    Matrix *m1_t = matrix_transposta(m1);
    matrix_free(m1);

    Matrix *r = matrix_mul_com_escalar(1/det, m1_t);
    matrix_free(m1_t);


    MResponse response = {.m = r, .erro = PROCESSO_SEM_ERRO};

    return response;
}
