#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <math.h>

#include "Matrix.h"

Matrix* defineRef(double t);
Matrix* degrau(int i);
Matrix* y_m(Matrix* ref, Matrix* aux_Ref);
Matrix* ModeloRefYm(Matrix* ym_ponto, Matrix* ym_pontoAntigo, double t);
Matrix* ControleBloco(Matrix* ym_ponto, Matrix* Ym, Matrix* Yt);
Matrix* Linearizacao(Matrix* xt, Matrix* vt, double R);
Matrix* RoboXt(Matrix* XtLinha, Matrix* XtLinhaAntigo, double t);
Matrix* RoboYt(Matrix* xt, double R);
Matrix* RoboXtLinha(Matrix* xt, Matrix* ut);

