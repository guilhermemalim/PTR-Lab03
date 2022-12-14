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

#include "blocos.h"
#include "mutexes.h"

Matrix *bufferV, *bufferU, *bufferY, *bufferX, *XtLinha, *bufferRef, *bufferYm, *bufferYmLinha, *auxBuffer, *auxBuffer2;

double t = 0;
int contRef=0;
int contModeloRef=0;
int contControle=0;
int contLinearizacao=0;
int contRobo=0;
double T = 0.5;

double R = 0.3;

//Semaforos para os Produtores
sem_t ProduzRef, ProduzY, ProduzYm, ProduzYmLinha, ProduzV, ProduzX, ProduzU, Print_Mostra;

//Semaforos Para os Consumidores
sem_t ConsomeRef, ConsomeY, ConsomeYm, ConsomeYmLinha, ConsomeV, ConsomeX, ConsomeU;


//Gerar Ref
void* Ref(void*args)
{
    struct timespec ts1, ts2, ts3={0};
    double dif;
    double tm;
    while(t<=TEMPO_MAX)
    {   
        // start=clock();

        sem_wait(&ProduzRef);
            clock_gettime(CLOCK_REALTIME, &ts1);
            tm = 1000000 * ts1.tv_nsec - tm;

            matrix_free(bufferRef);
            bufferRef=defineRef(t);  //verificar t = t*T ()
            t=t+TEMPO_REF;
            printf("%.4f \n",VALUES(bufferRef,0,0));
            sem_post(&ConsomeRef);
            dif=t - tm;
            //JitterRef[contRef]=TEMPO_REF - dif/1000.0;
            //usleep(TEMPO_REF_MS-dif);
            
            clock_gettime(CLOCK_REALTIME, &ts2);
            ts3.tv_sec = 0;
            ts3.tv_nsec = T*1000000 - (ts2.tv_nsec - ts1.tv_nsec);
            nanosleep(&ts3, &ts3);
        contRef++;
    }
}

void* ModeloRef(void*args)
{
    //clock_t start;
    struct timespec ts1, ts2, ts3={0};
    double dif;
    double tm;
    T = TEMPO_MODELO_REF;
    while(t<=TEMPO_MAX)
    {
        // start=clock();
        sem_wait(&ConsomeRef);
        sem_wait(&ProduzYm);
        sem_wait(&ProduzYmLinha);
            clock_gettime(CLOCK_REALTIME, &ts1);
            tm = 1000000 * ts1.tv_nsec - tm;
            t=t+T;
            auxBuffer = matrix_copy(bufferYmLinha);
            matrix_free(bufferYmLinha);

            bufferYmLinha = y_m(bufferRef,bufferYm);
            bufferYm = ModeloRefYm(bufferYmLinha, auxBuffer, t*T);

        sem_post(&ProduzRef);
        sem_post(&ConsomeYm);
        sem_post(&ConsomeYmLinha);
       
            dif=t - tm;
        //JitterModeloRef[contModeloRef]=TEMPO__MODELO_REF - dif/1000.0;
        //usleep(TEMPO_MODELO_REF_MS - dif);

            clock_gettime(CLOCK_REALTIME, &ts2);
            ts3.tv_sec = 0;
            ts3.tv_nsec = T*1000000 - (ts2.tv_nsec - ts1.tv_nsec);
            nanosleep(&ts3, &ts3);
        contModeloRef++;
    }
}

//Bloco Controle
void* Controle(void*args)
{
    struct timespec ts1, ts2, ts3={0};
    double dif;
    double tm;    
    T = TEMPO_CONTROLE;

    while(t<=TEMPO_MAX)
    {
        // start=clock();
        sem_wait(&ConsomeY);
        sem_wait(&ConsomeYm);
        sem_wait(&ConsomeYmLinha);
        sem_wait(&ProduzV);
            clock_gettime(CLOCK_REALTIME, &ts1);
            tm = 1000000 * ts1.tv_nsec - tm;
            t=t+T;
            

            matrix_free(bufferV);
            bufferV=ControleBloco(bufferYmLinha, bufferYm, bufferY);
        sem_post(&ProduzYm);
        sem_post(&ProduzYmLinha);
        sem_post(&ProduzY);
        sem_post(&ConsomeV);
        dif=t - tm;
        // JitterControle[contControle]=TEMPO_CONTROLE-dif/1000.0;
        // usleep(TEMPO_CONTROLE_MS-dif);
        
            clock_gettime(CLOCK_REALTIME, &ts2);
            ts3.tv_sec = 0;
            ts3.tv_nsec = T*1000000 - (ts2.tv_nsec - ts1.tv_nsec);
            nanosleep(&ts3, &ts3);
        contControle++;
    }
}

void* Linear(void*args)
{
    struct timespec ts1, ts2, ts3={0};
    double dif;
    double tm;
    T = TEMPO_LINEARIZACAO;
    while(t<=TEMPO_MAX)
    {
        //start=clock();
        sem_wait(&ConsomeX);
        sem_wait(&ConsomeV);
        sem_wait(&ProduzU);
            clock_gettime(CLOCK_REALTIME, &ts1);
            tm = 1000000 * ts1.tv_nsec - tm;
            t=t+T;
            
            matrix_free(bufferU);
            bufferU=Linearizacao(bufferX, bufferV, R);
        sem_post(&ProduzX);
        sem_post(&ProduzV);
        sem_post(&ConsomeU);
        dif= t - tm;
        // JitterLinearizacao[contLinearizacao]=TEMPO_LINEARIZACAO -dif/1000.0;
        // usleep(TEMPO_LINEARIZACAO_MS-dif);
            clock_gettime(CLOCK_REALTIME, &ts2);
            ts3.tv_sec = 0;
            ts3.tv_nsec = T*1000000 - (ts2.tv_nsec - ts1.tv_nsec);
            nanosleep(&ts3, &ts3);
        contLinearizacao++;
    }
}

void* Robo(void*args)
{
    struct timespec ts1, ts2, ts3={0};
    double dif;
    double tm;
    T = TEMPO_ROBO;
    while(t<=TEMPO_MAX)
    {
        sem_wait(&ConsomeU);
        sem_wait(&ProduzX);
        sem_wait(&ProduzY);
            clock_gettime(CLOCK_REALTIME, &ts1);
            tm = 1000000 * ts1.tv_nsec - tm;
            t=t+T;

            printf("%.2lf, %lf, %lf, %lf\n", t-0.12 ,matrix_get_value(bufferX,0,0) ,matrix_get_value(bufferX,1,0),matrix_get_value(bufferX,2,0));
            auxBuffer2 = matrix_copy(XtLinha);
            
            matrix_free(XtLinha);
            XtLinha=RoboXtLinha(bufferX, bufferU);
            
            matrix_free(bufferX);
            bufferX=RoboXt(XtLinha, auxBuffer, t*T);
            
            matrix_free(bufferY);
            bufferY = RoboYt(bufferX, R);
        sem_post(&ProduzU);
        sem_post(&ConsomeX);
        sem_post(&ConsomeY);
        dif=t - tm;
        // JitterRobo[contRobo]=TEMPO_ROBO - dif/1000.0;
        // usleep(TEMPO_ROBO_MS-dif);

            clock_gettime(CLOCK_REALTIME, &ts2);
            ts3.tv_sec = 0;
            ts3.tv_nsec = T*1000000 - (ts2.tv_nsec - ts1.tv_nsec);
            nanosleep(&ts3, &ts3);
        contRobo++;
    }

}

int main() {
	mutexes_init();

    //Iniciando semaphoros produtores

    sem_init(&Print_Mostra,0,0);
    sem_init(&ProduzRef, 0,  1);
    sem_init(&ProduzU, 0,  1);
    sem_init(&ProduzV, 0,  1);
    sem_init(&ProduzX, 0,  0);
    sem_init(&ProduzY, 0,  0);
    sem_init(&ProduzYm, 0,  1);
    sem_init(&ProduzYmLinha, 0,  1);

    //Iniciando semaphoros Consumidores
    sem_init(&ConsomeRef, 0,  0);
    sem_init(&ConsomeU, 0,  0);
    sem_init(&ConsomeV, 0,  0);
    sem_init(&ConsomeX, 0,  1);
    sem_init(&ConsomeY, 0,  1);
    sem_init(&ConsomeYm, 0,  0);
    sem_init(&ConsomeYmLinha, 0,  0);

    //iniciando as Matrixes do Buffer que ser??o responsaveis por comunicar os blocos
    double aux[11] = {0,0,0,0,0,0,0,0,0,0,0};
    bufferV = matrix_zeros(2,1);
    bufferU = matrix_zeros(2,1);
    bufferY = matrix_zeros(2,1);
    bufferX = matrix_zeros(3,1);
    bufferRef = matrix_zeros(2,1);
    bufferYm = matrix_zeros(2,1);
    bufferYmLinha = matrix_zeros(2,1);
    XtLinha = matrix_zeros(3 , 1);


    //Nomeando as Threads
    pthread_t TRef, TModeloRef, TControle, TLinearizacao, TRobo, Print_mostra;

    //Criando as Threads
    pthread_create(&TRef, NULL, Ref, NULL);
    pthread_create(&TModeloRef, NULL, ModeloRef, NULL);
    pthread_create(&TControle, NULL, Controle, NULL);
    pthread_create(&TLinearizacao, NULL, Linear, NULL); 
    pthread_create(&TRobo, NULL, Robo, NULL);
   // pthread_create(&Print_mostra, NULL, imprimi, NULL);


    //Finalizando as Threads
    pthread_join(TRef, NULL);
    pthread_join(TModeloRef, NULL);
    pthread_join(TControle, NULL);
    pthread_join(TLinearizacao, NULL);
    pthread_join(TRobo, NULL);

	mutexes_destroy();
    return 0;
}


