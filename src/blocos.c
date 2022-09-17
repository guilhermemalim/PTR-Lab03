#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <math.h>

#include "Parametros.h"
#include "Matrix.h"
#include "blocos.h"


Matrix* defineRef(double t){
    Matrix* c = matrix_zeros(2,1);

    if(t>=0 && t<2*M_PI ){

        VALUES(c,0,0)=0.5 - 0.5*cos(t);
        VALUES(c,1,0) = 0.5*sin(t);
    

    }else if(t>=2*M_PI && t<4*M_PI){

        VALUES(c,0,0)= -0.5 + 0.5*cos(t);
        VALUES(c,1,0) = -0.5*sin(t);
       

    }else{

        VALUES(c,0,0)= 0;
        VALUES(c,1,0) = 0;
    }

    return c;

}


Matrix* y_m(Matrix* ref, Matrix* Ym){

    Matrix* ym_ponto = matrix_zeros(2,1);
    
    VALUES(ym_ponto, 0, 0) = ALPHA1*(matrix_get_value(ref,0,0)-matrix_get_value(Ym,0,0)); // ymx_ponto = alpha1(X_ref - y_mx)
    VALUES(ym_ponto, 1, 0) = ALPHA2*(matrix_get_value(ref,1,0)-matrix_get_value(Ym,1,0)); // ymy_ponto = alpha2(Y_ref)
    
    return ym_ponto;
}

Matrix* ModeloRefYm(Matrix* ym_ponto, Matrix* ym_pontoAntigo, double t)
{
    Matrix* ym = matrix_zeros(2,1);

    VALUES(ym, 0, 0) = (0.12) * (VALUES(ym_ponto, 0, 0) + VALUES(ym_pontoAntigo, 0, 0)) / 2;
    VALUES(ym, 1, 0) = (0.12) * (VALUES(ym_ponto, 1, 0) + VALUES(ym_pontoAntigo, 1, 0)) / 2;

    return ym;
}


Matrix* ControleBloco(Matrix* ym_ponto, Matrix* Ym, Matrix* Yt)
{
    Matrix* Vt = matrix_zeros(2,1);
    VALUES(Vt, 0, 0) = VALUES(ym_ponto, 0, 0) + ALPHA1 * (VALUES(Ym, 0, 0) - VALUES(Yt, 0, 0));
    VALUES(Vt, 1, 0) = VALUES(ym_ponto, 1, 0) + ALPHA2 * (VALUES(Ym, 1, 0) - VALUES(Yt, 1, 0));

    return Vt;
}

// Bloco de Linearização
Matrix* Linearizacao(Matrix* xt, Matrix* vt, double R)
{
    Matrix* L = matrix_zeros(2,2);
    
    VALUES(L, 0, 0) = cos(VALUES(xt,2,0));
    VALUES(L, 0, 1) = -R*sin (VALUES(xt,2,0));
    VALUES(L, 1, 0) = sin(VALUES(xt, 2, 0));
    VALUES(L, 1, 1) = R * cos(VALUES(xt, 2, 0));

    MResponse response = matrix_inversa(L);
    Matrix* LInvers = response.m;
    
    MResponse response1 = matrix_mul(LInvers,vt);
    Matrix* linearizado = response1.m;
    
    return linearizado;
}

//Bloco robo
Matrix* RoboXtLinha(Matrix* xt, Matrix* ut)
{
    Matrix* Aux = matrix_zeros(3,2);

    /*
     * @brief geração da Matriz que multiplica u(t) no bloco do Robô 
     * 
     * Aux = [cos(x_3) 0;
     *        sin(x_3) 0;  
     *            0    1]
     */

    VALUES(Aux, 0, 0) = cos(VALUES(xt, 2, 0));
    VALUES(Aux, 0, 1) = 0;
    VALUES(Aux, 1, 0) = sin(VALUES(xt, 2, 0));
    VALUES(Aux, 1, 1) = 0;
    VALUES(Aux, 2, 0) = 0;
    VALUES(Aux, 2, 1) = 1;


    MResponse response = matrix_mul(Aux,ut);//retornei a multiplicação
    
    Matrix* xt_linha = response.m;

    return xt_linha;
}

Matrix* RoboXt(Matrix* XtLinha, Matrix* XtLinhaAntigo, double t)
{
    Matrix* Xt = matrix_zeros(3,1);

    VALUES(Xt, 0, 0) = (0.12)*(VALUES(XtLinha,0,0)+VALUES(XtLinhaAntigo,0,0)/2);
    VALUES(Xt, 1, 0) = (0.12)*(VALUES(XtLinha,1,0)+VALUES(XtLinhaAntigo,1,0))/2;
    VALUES(Xt, 2, 0) = (0.12)*(VALUES(XtLinha,2,0)+VALUES(XtLinhaAntigo,2,0))/2;
    
    return Xt;
}

Matrix* RoboYt(Matrix* xt, double R)
{
    Matrix* Aux = matrix_zeros(2,3);
    VALUES(Aux, 0, 0) = 1;
    VALUES(Aux, 1, 1) = 1;
    
    Matrix* Aux2 = matrix_zeros(2, 1);
    VALUES(Aux2, 0, 0) = R*cos(VALUES(xt,2,0));
    VALUES(Aux2, 1, 0) = R*sin(VALUES(xt,2,0));

    MResponse response = matrix_mul(Aux, xt);
    Aux = response.m;
    
    VALUES(Aux,0,0) = VALUES(Aux,0,0) + VALUES(Aux2,0,0);
    VALUES(Aux,1,0) = VALUES(Aux,1,0) + VALUES(Aux2,1,0);
    
    return Aux;
}
