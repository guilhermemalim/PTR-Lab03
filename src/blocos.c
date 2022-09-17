#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <math.h>
#include "Matrix.h"
#include "blocos.h"


Matrix* definedRef(double t, int selec){
    double d = 0; //Seleciona para gerar dados consideradno o instante ou apenas a velocidade angular
    Matrix* c = matriz_iniciada(Ms("INICIADA"),2,1);

    if(selec==0) d = 1;
    else d =t;
    if(t>=0 && t<2*M_PI ){

        VALUES(c,0,0)=0.5 - 0.5*cos(d);
        VALUES(c,1,0) = 0.5*sin(d);
    

    }else if(t>=2*M_PI && t<4*M_PI){

        VALUES(c,0,0)= -0.5 + 0.5*cos(d);
        VALUES(c,1,0) = -0.5*sin(d);
       

    }else{

        VALUES(c,0,0)= 0;
        VALUES(c,1,0) = 0;
    }

    return c;

}


Matrix* y_m(Matrix* ref, Matrix* Ym){

  Matrix* ymLinha = matriz_iniciada(Ms("Ym/"),2,1);
  double aux[2] = { Alpha1*(VALUES(ref,0,0)-VALUES(Ym,0,0)), Alpha2*(VALUES(ref,1,0)-VALUES(Ym,1,0))};
  matrix_putValues(ymLinha, aux);
  return ymLinha;

}

Matrix* ModeloRefYm(Matrix* YmLinha, Matrix* YmLinhaAntigo, double t)
{
    Matrix* ym = matriz_iniciada(Ms("Ym"),2,1);
    double aux[2] = {(t-(t-0.12))*(VALUES(YmLinha,0,0)+VALUES(YmLinhaAntigo,0,0))/2, (t-(t-0.12))*(VALUES(YmLinha,1,0)+VALUES(YmLinhaAntigo,1,0))/2};
    matrix_putValues(ym, aux);
    return ym;
}


Matrix* ControleBloco(Matrix* YmLinha, Matrix* Ym, Matrix* Yt)
{
    Matrix* Vt = matriz_iniciada(Ms("Vt"),2,1);
    double aux[2] = {VALUES(YmLinha,0,0) +Alpha1*(VALUES(Ym,0,0) - VALUES(Yt,0,0)), VALUES(YmLinha,1,0) +Alpha2*(VALUES(Ym,1,0) - VALUES(Yt,1,0))};
    matrix_putValues(Vt,aux);
    return Vt;
}

// Bloco de Linearização
Matrix* Linearizacao(Matrix* xt, Matrix* vt, double R)
{
    Matrix* L=matriz_iniciada(Ms("L"),2,2);
    double LTerms[4] = {cos(VALUES(xt,2,0)), -R*sin (VALUES(xt,2,0)), sin(VALUES(xt,2,0)), R*cos(VALUES(xt,2,0))};
    matrix_putValues(L, LTerms);
    Matrix* LInvers=matrix_invers(L);
    return soma_multi(LInvers,vt,Ms("multi"));
}

//Bloco robo

Matrix* RoboXtLinha(Matrix* xt, Matrix* ut)
{
    Matrix* Aux=matriz_iniciada(Ms("Aux"),3,2);//criei uma 2x2
    double AuxTerms[6] = {cos(VALUES(xt,2,0)), 0, sin(VALUES(xt,2,0)) , 0, 0, 1};
    matrix_putValues(Aux,AuxTerms);//coloquei os valores do vetor na matrix 2x2
    return soma_multi(Aux,ut,Ms("xdot"));//retornei a multiplicação
}

Matrix* RoboXt(Matrix* XtLinha, Matrix* XtLinhaAntigo, double t)
{
    Matrix* Xt = matriz_iniciada(Ms("Xt"),3,1);
    double aux[3] = {(t-(t-0.12))*(VALUES(XtLinha,0,0)+VALUES(XtLinhaAntigo,0,0)/2), (t-(t-0.12))*(VALUES(XtLinha,1,0)+VALUES(XtLinhaAntigo,1,0))/2,
     (t-(t-0.12))*(VALUES(XtLinha,2,0)+VALUES(XtLinhaAntigo,2,0))/2};
    matrix_putValues(Xt, aux);
    return Xt;
}

Matrix* RoboYt(Matrix* xt, double R)
{
    Matrix* Aux=matriz_iniciada(Ms("Aux"),2,3);//criei uma 2x3
    double AuxTerms[6] = {1, 0, 0, 0, 1, 0};
    matrix_putValues(Aux, AuxTerms);
    Matrix* Aux2=matriz_iniciada(Ms("Aux2"), 2, 1);
    double AuxTerms2[2] = {R*cos(VALUES(xt,2,0)),R*sin(VALUES(xt,2,0))};
    matrix_putValues(Aux2, AuxTerms2);
    Aux=soma_multi(Aux, xt,Ms("multi"));
    VALUES(Aux,0,0) = VALUES(Aux,0,0) + VALUES(Aux2,0,0);
    VALUES(Aux,1,0) = VALUES(Aux,1,0) + VALUES(Aux2,1,0);
    return Aux;
}
