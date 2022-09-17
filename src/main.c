// Guilherme Matheus - 21950880
// Odalisio Neto - 21950520

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "Parametros.h"
#include "Integral.h"
#include "DString.h"
#include "Matrix.h"

#include "mutexes.h"

Matrix* bufferV, bufferU, bufferY, bufferX, XtLinha, bufferRef, bufferYm, bufferYmLinha, auxBuffer, auxBuffer2;

double t = 0;
int contRef120=0;
int contModeloRef=0;
int contControle=0;
int contLinearizacao=0;
int contRobo=0;
double T = 0.5;

//Semaforos para os Produtores
sem_t ProduzRef, ProduzY, ProduzYm, ProduzYmLinha, ProduzV, ProduzX, ProduzU, Print_Mostra;

//Semaforos Para os Consumidores
sem_t ConsomeRef, ConsomeY, ConsomeYm, ConsomeYmLinha, ConsomeV, ConsomeX, ConsomeU;


//Gerar Ref
void* Ref(void*args)
{
    clock_t start;
    double dif;
    while(t<=TEMPO_MAX)
    {   
        start=clock();
        sem_wait(&ProduzRef);
            matrix_free(bufferRef);
            bufferRef=calc_ref(t*T,1);
            t=t+0.12;
            //printf("%.4f \n",MVAL(bufferRef,0,0));
        sem_post(&ConsomeRef);
        //dif=difftime(clock(), start);
        //JitterRef[contRef]=TEMPO_REF - dif/1000.0;
        usleep(TEMPO_REF_MS-dif);
        contRef++;
    }
}

void* ModeloRef(void*args)
{
    clock_t start;
    double dif;
    while(t<=TEMPO_MAX)
    {
        start=clock();
        sem_wait(&ConsomeRef);
        sem_wait(&ProduzYm);
        sem_wait(&ProduzYmLinha);
            auxBuffer=bufferYmLinha;
            bufferYmLinha=y_m(bufferRef,bufferYm);
            bufferYm=ModeloRefYm(bufferYmLinha, auxBuffer, t*T);

        sem_post(&ProduzRef);
        sem_post(&ConsomeYm);
        sem_post(&ConsomeYmLinha);
       
        // dif=difftime(clock(),start);
        //JitterModeloRef[contModeloRef]=TEMPO__MODELO_REF - dif/1000.0;
        usleep(TEMPO_MODELO_REF_MS - dif);
        contModeloRef++;
    }
}

//Bloco Controle
void* Controle(void*args)
{
    clock_t start;
    double dif;
    while(t<=TEMPO_MAX)
    {
        start=clock();
        sem_wait(&ConsomeY);
        sem_wait(&ConsomeYm);
        sem_wait(&ConsomeYmLinha);
        sem_wait(&ProduzV);
            bufferV=ControleBloco(bufferYmLinha, bufferYm, bufferY);
        sem_post(&ProduzYm);
        sem_post(&ProduzYmLinha);
        sem_post(&ProduzY);
        sem_post(&ConsomeV);
        // dif=difftime(clock(),start);
        // JitterControle[contControle]=TEMPO_CONTROLE-dif/1000.0;
        usleep(TEMPO_CONTROLE_MS-dif);
        contControle++;
    }
}

void* Linear(void*args)
{
    clock_t start;
    double dif;
    while(t<=TEMPO_MAX)
    {
        start=clock();
        sem_wait(&ConsomeX);
        sem_wait(&ConsomeV);
        sem_wait(&ProduzU);
            bufferU=Linearizacao(bufferX, bufferV, R);
        sem_post(&ProduzX);
        sem_post(&ProduzV);
        sem_post(&ConsomeU);
        // dif= difftime(clock(),start);
        // JitterLinearizacao[contLinearizacao]=TEMPO_LINEARIZACAO -dif/1000.0;
        usleep(TEMPO_LINEARIZACAO_MS-dif);
        contLinearizacao++;
    }
}

void* Robo(void*args)
{
    clock_t start;
    double dif;
    while(t<=TEMPO_MAX)
    {
        start=clock();
        sem_wait(&ConsomeU);
        sem_wait(&ProduzX);
        sem_wait(&ProduzY);
            printf("%.2lf, %lf, %lf, %lf\n", t-0.12 ,MVAL(bufferX,0,0) ,MVAL(bufferX,1,0),MVAL(bufferX,2,0));
            auxBuffer2=XtLinha;
            XtLinha=RoboXtLinha(bufferX, bufferU);
            bufferX=RoboXt(XtLinha, auxBuffer, t*T);
            bufferY=RoboYt(bufferX, R);
        sem_post(&ProduzU);
        sem_post(&ConsomeX);
        sem_post(&ConsomeY);
        // dif=difftime(clock(),start);
        // JitterRobo[contRobo]=TEMPO_ROBO - dif/1000.0;
        usleep(TEMPO_ROBO_MS-dif);
        contRobo++;
    }

}

int main() {
	mutexes_init();


	mutexes_destroy();
    return 0;
}

